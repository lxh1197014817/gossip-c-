// Copyright (c) 2025 Institute of Software, Chinese Academy of Sciences
// Author: Zhen Tang <tangzhen12@otcaix.iscas.ac.cn>
// Affiliation: Institute of Software, Chinese Academy of Sciences

#include <algorithm>
#include <filesystem>

#include "PluginContextImpl.h"
#include "PluginManager.h"

namespace Fleet::DataManager::Core {
PluginManager::PluginManager(uuid_t nodeId, const std::string &baseDirectory) {
    this->pluginContext = std::make_shared<PluginContextImpl>(this, nodeId, baseDirectory);
    this->pluginContext->LogTrace(SOURCE_LOCATION, "调用");
    this->pluginMap.clear();
    this->pluginList.clear();
    this->parameters.clear();
    this->pluginContext->LogTrace(SOURCE_LOCATION, "返回");
}

PluginManager::~PluginManager() {
    this->pluginContext->LogTrace(SOURCE_LOCATION, "调用");
    this->pluginContext->LogTrace(SOURCE_LOCATION, "返回");
}

void PluginManager::SetParameter(const std::string &key, const std::string &value) {
    this->parameters[key] = value;
}

bool PluginManager::LoadAnAvailablePlugin(std::vector<std::shared_ptr<Plugin>> &toLoad) {
    this->pluginContext->LogTrace(SOURCE_LOCATION, "调用");
    int i = 0;
    for (i = 0; i < (int) toLoad.size(); i++) {
        if (this->HasNoConflict(toLoad[i]) && this->AllDependencyLoaded(toLoad[i])) {
            this->pluginContext->LogInfo(SOURCE_LOCATION, "尝试加载插件 {}",
                                         toLoad[i]->GetPath().c_str());
            auto plugin = toLoad[i];
            toLoad.erase(toLoad.begin() + i);
            bool success = this->CheckAndInitializePlugin(plugin);
            this->pluginContext->LogTrace(SOURCE_LOCATION, "返回");
            return success;
        }
    }
    this->pluginContext->LogTrace(SOURCE_LOCATION, "返回");
    return false;
}

bool PluginManager::CheckAndInitializePlugin(const std::shared_ptr<Plugin> &plugin) {
    if (!this->HasNoConflict(plugin)) {
        std::string conflicts;
        for (const auto &elem : plugin->GetConflictList()) {
            conflicts += "[" + elem + "]";
        }
        this->pluginContext->LogError(SOURCE_LOCATION,
                                      "无法加载插件 {}, 请先卸载冲突插件 ({})",
                                      plugin->GetPath().c_str(), conflicts.c_str());
        return false;
    }
    if (!this->AllDependencyLoaded(plugin)) {
        std::string dependency;
        for (const auto &elem : plugin->GetDependencyList()) {
            dependency += "[" + elem + "]";
        }
        this->pluginContext->LogError(SOURCE_LOCATION,
                                      "无法加载插件 {}, 请先加载依赖插件 ({})",
                                      plugin->GetPath().c_str(), dependency.c_str());
        return false;
    }
    this->DoInitializePlugin(plugin);
    return true;
}

bool PluginManager::LoadPlugin(const std::string &path) {
    this->pluginContext->LogInfo(SOURCE_LOCATION, "加载插件 {}", path.c_str());
    auto plugin = std::make_shared<Plugin>();
    if (this->GetPluginLoader()->LoadPlugin(path, plugin)) {
        if (!this->CheckAndInitializePlugin(plugin)) {
            this->GetPluginLoader()->UnloadPlugin(plugin);
            return false;
        }
        return true;
    } else {
        this->pluginContext->LogWarn(SOURCE_LOCATION, "无法加载插件 {}, 已跳过",
                                     path.c_str());
        return false;
    }
}

bool PluginManager::AllDependencyLoaded(const std::shared_ptr<Plugin> &plugin) {
    for (const auto &dependency : plugin->GetDependencyList()) {
        if (this->pluginMap.find(dependency) == this->pluginMap.end()) {
            return false;
        }
    }
    return true;
}

bool PluginManager::HasNoConflict(const std::shared_ptr<Plugin> &plugin) {
    for (const auto &elem : this->pluginList) {
        for (const auto &conflict : plugin->GetConflictList()) {
            if (elem->GetName() == conflict) {
                return false;
            }
        }
    }
    return true;
}

bool PluginManager::HasNoDependency(const std::shared_ptr<Plugin> &plugin) {
    for (const auto &elem : this->pluginList) {
        for (const auto &dependency : elem->GetDependencyList()) {
            if (plugin->GetName() == dependency) {
                return false;
            }
        }
    }
    return true;
}

bool PluginManager::UnloadPlugin(const std::string &pluginName) {
    auto iter = this->pluginMap.find(pluginName);
    if (iter != this->pluginMap.end()) {
        auto plugin = this->pluginMap[pluginName];
        return this->DoDestroyPlugin(plugin);
    } else {
        this->pluginContext->LogError(SOURCE_LOCATION, "未找到插件 {}",
                                      pluginName.c_str());
        return false;
    }
}

bool PluginManager::UnloadAllPlugins() {
    bool success = true;
    std::vector<std::shared_ptr<Plugin>> toRemove;
    toRemove.reserve(this->pluginList.size());
    for (const auto &elem : this->pluginList) {
        toRemove.push_back(elem);
    }

    std::reverse(toRemove.begin(), toRemove.end());
    for (const auto &plugin : toRemove) {
        this->pluginContext->LogInfo(SOURCE_LOCATION, "卸载插件 {} 版本 {} ({})",
                                     plugin->GetName().c_str(), plugin->GetVersion().c_str(),
                                     plugin->GetDescription().c_str());
        success = success && this->DoDestroyPlugin(plugin);
    }

    return success;
}

void PluginManager::DoInitializePlugin(const std::shared_ptr<Plugin> &plugin) {
    PluginBase *pluginPointer = (PluginBase *) plugin->NewPlugin();
    plugin->SetPlugin(pluginPointer);
    auto iter = this->pluginMap.find(plugin->GetName());
    if (iter != this->pluginMap.end()) {
        this->pluginContext->LogInfo(SOURCE_LOCATION,
                                     "插件 {} 已加载, 正在卸载前一个插件实例",
                                     plugin->GetName().c_str());
        auto previous = this->pluginMap[plugin->GetName()];
        this->DoDestroyPlugin(previous);
    }
    this->pluginList.push_back(plugin);
    this->pluginMap[plugin->GetName()] = plugin;
    plugin->GetPlugin()->Initialize(this->GetPluginContext(), this->parameters);
    this->pluginContext->LogInfo(SOURCE_LOCATION, "插件 {} 版本 {} ({}) 已初始化",
                                 plugin->GetName(), plugin->GetVersion(), plugin->GetDescription());
}

void *PluginManager::GetService(const std::string &pluginName) {
    auto iter = this->pluginMap.find(pluginName);
    if (iter != this->pluginMap.end()) {
        auto plugin = this->pluginMap[pluginName];
        return plugin->GetPlugin()->GetService();
    } else {
        this->pluginContext->LogError(SOURCE_LOCATION, "未找到插件 {}", pluginName);
        return nullptr;
    }
}

bool PluginManager::DoDestroyPlugin(const std::shared_ptr<Plugin> &plugin) {
    if (this->HasNoDependency(plugin)) {
        this->pluginContext->LogInfo(SOURCE_LOCATION, "插件 {} 版本 {} ({}) 已销毁",
                                     plugin->GetName(), plugin->GetVersion(),
                                     plugin->GetDescription());
        this->pluginMap.erase(plugin->GetName());
        this->pluginList.erase(
            std::remove(this->pluginList.begin(), this->pluginList.end(), plugin),
            this->pluginList.end());
        plugin->DeletePlugin();
        return this->GetPluginLoader()->UnloadPlugin(plugin);
    } else {
        this->pluginContext->LogError(SOURCE_LOCATION,
                                      "无法卸载插件 {} ({}), 请先卸载所有依赖它的插件",
                                      plugin->GetName(), plugin->GetPath());
        return false;
    }
}

std::shared_ptr<PluginContext> PluginManager::GetPluginContext() const {
    return this->pluginContext;
}

const std::string &PluginManager::GetNodeId() {
    return this->pluginContext->GetNodeId();
}

} // namespace Fleet::DataManager::Core