// Copyright (c) 2025 Institute of Software, Chinese Academy of Sciences
// Author: Zhen Tang <tangzhen12@otcaix.iscas.ac.cn>
// Affiliation: Institute of Software, Chinese Academy of Sciences

#include <algorithm>

#include "PluginContextImpl.h"
#include "StaticPluginFactory.h"
#include "StaticPluginLoader.h"
#include "StaticPluginManager.h"
#include "StringTools.h"

namespace Fleet::DataManager::Core {

StaticPluginManager::StaticPluginManager(uuid_t nodeId, const std::string &baseDirectory)
    : PluginManager(nodeId, baseDirectory) {
    this->pluginLoader = std::make_shared<StaticPluginLoader>(this->pluginContext);
}

StaticPluginManager::~StaticPluginManager() {
    this->pluginContext->LogTrace(SOURCE_LOCATION, "调用");
    this->pluginContext->LogTrace(SOURCE_LOCATION, "返回");
}

std::shared_ptr<PluginLoader> StaticPluginManager::GetPluginLoader() {
    return this->pluginLoader;
}

bool StaticPluginManager::LoadPluginsFromParameter() {
    auto pluginsToLoadIter = this->parameters.find("core.pluginsToLoad");
    if (pluginsToLoadIter != parameters.end()) {
        std::vector<std::string> pluginsToLoad;
        StringSplit(pluginsToLoadIter->second, ' ', pluginsToLoad);
        this->pluginContext->LogInfo(SOURCE_LOCATION, "待加载插件: {}",
                                     pluginsToLoadIter->second);
        return this->LoadPlugins(pluginsToLoad);
    } else {
        // Load all plugins
        this->pluginContext->LogInfo(SOURCE_LOCATION,
                                     "参数 core.pluginsToLoad 未设置, 将加载所有可用插件");
        std::vector<std::string> pluginNames;
        StaticPluginFactory::GetInstance().GetAllPluginNames(pluginNames);
        return this->LoadPlugins(pluginNames);
    }
}

bool StaticPluginManager::LoadPlugins(const std::vector<std::string> &pluginNames) {
    std::vector<std::shared_ptr<Plugin>> candidatePlugins;
    std::vector<std::string> notFoundList;
    for (const auto &elem : pluginNames) {
        auto plugin = StaticPluginFactory::GetInstance().FindPlugin(elem);
        if (plugin == nullptr) {
            notFoundList.push_back(elem);
        } else {
            auto newInstance = std::make_shared<Plugin>();
            newInstance->From(plugin);
            candidatePlugins.push_back(newInstance);
        }
    }

    if (!notFoundList.empty()) {
        std::string notFound;
        for (const auto &elem : notFoundList) {
            notFound += "[" + elem + "]";
        }
        this->pluginContext->LogError(SOURCE_LOCATION, "未找到需要加载的插件 {}",
                                      notFound.c_str());
    }

    while (this->LoadAnAvailablePlugin(candidatePlugins)) {
    }

    if (!candidatePlugins.empty()) {
        this->pluginContext->LogError(SOURCE_LOCATION,
                                      "以下插件不满足依赖关系, 即将卸载");
        for (const auto &elem : candidatePlugins) {
            this->pluginContext->LogError(SOURCE_LOCATION, "正在卸载插件 {}",
                                          elem->GetPath());
            this->GetPluginLoader()->UnloadPlugin(elem);
        }
        return false;
    }

    candidatePlugins.clear();
    return true;
}
bool StaticPluginManager::LoadPluginsNocheck(const std::vector<std::string> &pluginNames) {
    std::vector<std::shared_ptr<Plugin>> candidatePlugins;
    std::vector<std::string> notFoundList;
    for (const auto &elem : pluginNames) {
        auto plugin = StaticPluginFactory::GetInstance().FindPlugin(elem);
        if (plugin == nullptr) {
            notFoundList.push_back(elem);
        } else {
            auto newInstance = std::make_shared<Plugin>();
            newInstance->From(plugin);
            candidatePlugins.push_back(newInstance);
        }
    }

    for (auto plugin: candidatePlugins) {
        DoInitializePlugin(plugin);
    }
    return true;
}
} // namespace Fleet::DataManager::Core