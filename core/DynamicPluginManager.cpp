/**
 * @file DynamicPluginManager.cpp
 * @brief 动态插件管理器
 * @details 动态插件管理器
 * @author 唐震 <tangzhen12@otcaix.iscas.ac.cn>
 * @date 2025-06-29
 */

#include <algorithm>
#include <filesystem>

#include "DynamicPluginLoader.h"
#include "DynamicPluginManager.h"
#include "StringTools.h"

namespace Fleet::DataManager::Core {
DynamicPluginManager::DynamicPluginManager(unsigned char *nodeId, const std::string &baseDirectory)
    : PluginManager(nodeId, baseDirectory) {
    this->pluginContext->LogTrace(SOURCE_LOCATION, "调用");
    this->pluginLoader = std::make_shared<DynamicPluginLoader>(this->pluginContext);
    this->pluginContext->LogTrace(SOURCE_LOCATION, "返回");
}

DynamicPluginManager::~DynamicPluginManager() {
    this->pluginContext->LogTrace(SOURCE_LOCATION, "调用");
    this->pluginContext->LogTrace(SOURCE_LOCATION, "返回");
}

std::shared_ptr<PluginLoader> DynamicPluginManager::GetPluginLoader() {
    this->pluginContext->LogTrace(SOURCE_LOCATION, "调用");
    this->pluginContext->LogTrace(SOURCE_LOCATION, "返回");
    return this->pluginLoader;
}

bool DynamicPluginManager::LoadPluginsFromParameter() {
    this->pluginContext->LogTrace(SOURCE_LOCATION, "调用");
    auto pluginDirectoryIter = this->parameters.find("core.pluginDirectory");
    auto pluginsToLoadIter = this->parameters.find("core.pluginsToLoad");

    std::string pluginDirectory;
    std::vector<std::string> pluginsToLoad;
    if (pluginDirectoryIter == this->parameters.end()) {
        this->pluginContext->LogInfo(SOURCE_LOCATION,
                                     "参数 core.pluginDirectory 未设置, 使用当前工作目录");
        pluginDirectory = std::filesystem::current_path();
    } else {
        pluginDirectory = pluginDirectoryIter->second;
    }
    this->pluginContext->LogInfo(SOURCE_LOCATION, "插件目录为: {}",
                                 pluginDirectory);

    if (pluginsToLoadIter != this->parameters.end()) {
        StringSplit(pluginsToLoadIter->second, ' ', pluginsToLoad);
        this->pluginContext->LogInfo(SOURCE_LOCATION, "待加载插件: {}",
                                     pluginsToLoadIter->second);
    } else {
        this->pluginContext->LogInfo(SOURCE_LOCATION,
                                     "参数 core.pluginsToLoad 未设置, 将尝试加载目录中的全部插件");
        this->ProbeAvailablePlugins(pluginDirectory, pluginsToLoad);
    }

    if (!this->LoadPluginsFromDirectory(pluginDirectory, pluginsToLoad)) {
        this->pluginContext->LogError(
            SOURCE_LOCATION,
            "无法加载插件, 请检查参数 core.pluginDirectory 和 core.pluginsToLoad");
        this->pluginContext->LogTrace(SOURCE_LOCATION, "返回");
        return false;
    }
    this->pluginContext->LogTrace(SOURCE_LOCATION, "返回");
    return true;
}

bool DynamicPluginManager::ProbeAvailablePlugins(const std::string &directory,
                                                 std::vector<std::string> &result) {
    this->pluginContext->LogTrace(SOURCE_LOCATION, "调用");
    std::vector<std::string> pluginFileNames;
    if (!this->ListFiles(directory, ".so", pluginFileNames)) {
        this->pluginContext->LogError(SOURCE_LOCATION,
                                      "无法列出目录 {} 中的插件文件", directory);
        this->pluginContext->LogTrace(SOURCE_LOCATION, "返回");
        return false;
    }
    for (const auto &elem : pluginFileNames) {
        auto plugin = std::make_shared<Plugin>();
        if (this->GetPluginLoader()->LoadPlugin(elem, plugin)) {
            result.push_back(plugin->GetName());
            this->pluginContext->LogInfo(SOURCE_LOCATION, "已找到可用插件 {}",
                                         plugin->GetName());
        } else {
            this->pluginContext->LogWarn(SOURCE_LOCATION, "无法加载插件 {}, 已跳过",
                                         elem);
        }
    }
    this->pluginContext->LogTrace(SOURCE_LOCATION, "返回");
    return true;
}

bool DynamicPluginManager::ListFiles(const std::string &directory, const std::string &extension,
                                     std::vector<std::string> &result) {
    this->pluginContext->LogTrace(SOURCE_LOCATION, "调用");
    if (!std::filesystem::exists(directory)) {
        // 路径不存在
        this->pluginContext->LogTrace(SOURCE_LOCATION, "返回");
        return false;
    }
    if (!std::filesystem::is_directory(directory)) {
        // 不是目录
        this->pluginContext->LogTrace(SOURCE_LOCATION, "返回");
        return false;
    }

    for (const auto &entry : std::filesystem::directory_iterator(directory)) {
        if (entry.is_regular_file() && entry.path().extension() == extension) {
            result.push_back(std::string(entry.path()));
        }
    }
    this->pluginContext->LogTrace(SOURCE_LOCATION, "返回");
    return true;
}

bool DynamicPluginManager::LoadPluginsFromWorkingDirectory(
    const std::vector<std::string> &pluginNames) {
    this->pluginContext->LogTrace(SOURCE_LOCATION, "调用");
    std::string currentPath = std::filesystem::current_path();
    this->pluginContext->LogInfo(SOURCE_LOCATION,
                                 "在当前工作目录 {} 中查找可用插件", currentPath);
    bool result = this->LoadPluginsFromDirectory(currentPath, pluginNames);
    this->pluginContext->LogTrace(SOURCE_LOCATION, "返回");
    return result;
}

bool DynamicPluginManager::LoadPluginsFromDirectory(const std::string &path,
                                                    const std::vector<std::string> &pluginNames) {
    this->pluginContext->LogTrace(SOURCE_LOCATION, "调用");
    std::vector<std::string> pluginFileNames;
    std::vector<std::shared_ptr<Plugin>> candidatePlugins;
    if (!this->ListFiles(path, ".so", pluginFileNames)) {
        this->pluginContext->LogError(SOURCE_LOCATION,
                                      "无法列出目录 {} 中的插件文件", path);
        this->pluginContext->LogTrace(SOURCE_LOCATION, "返回");
        return false;
    }

    std::vector<std::string> toLoad;
    toLoad.reserve(pluginNames.size());
    for (const auto &elem : pluginNames) {
        toLoad.push_back(elem);
    }

    this->pluginContext->LogInfo(SOURCE_LOCATION, "在目录 {} 中找到 {} 个可用插件",
                                 path, (int) pluginFileNames.size());
    for (const auto &elem : pluginFileNames) {
        auto plugin = std::make_shared<Plugin>();
        if (this->GetPluginLoader()->LoadPlugin(elem, plugin)) {
            auto iter = std::find(toLoad.begin(), toLoad.end(), plugin->GetName());
            if (iter != toLoad.end()) {
                candidatePlugins.push_back(plugin);
                toLoad.erase(iter);
            } else {
                this->pluginContext->LogInfo(SOURCE_LOCATION,
                                             "插件 {} 不在待加载列表中, 已跳过", elem);
            }
        } else {
            this->pluginContext->LogWarn(SOURCE_LOCATION, "无法加载插件 {}, 已跳过",
                                         elem);
        }
    }

    if (!toLoad.empty()) {
        std::string notFound;
        for (const auto &elem : toLoad) {
            notFound += "[" + elem + "]";
        }
        this->pluginContext->LogError(SOURCE_LOCATION, "未找到需要加载的插件 {}",
                                      notFound);
        return false;
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
        this->pluginContext->LogTrace(SOURCE_LOCATION, "返回");
        return false;
    }

    candidatePlugins.clear();
    this->pluginContext->LogTrace(SOURCE_LOCATION, "返回");
    return true;
}

} // namespace Fleet::DataManager::Core