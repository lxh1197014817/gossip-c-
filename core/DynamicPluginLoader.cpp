/**
* @file DynamicPluginLoader.cpp
* @brief 动态插件加载器
* @details 动态插件加载器
* @author 唐震 <tangzhen12@otcaix.iscas.ac.cn>
* @date 2025-06-29
*/

#include "DynamicPluginLoader.h"
#include <dlfcn.h>

namespace Fleet::DataManager::Core {
DynamicPluginLoader::DynamicPluginLoader(const std::shared_ptr<PluginContext> &pluginContext) {
    this->pluginContext = pluginContext;
    this->pluginContext->LogTrace(SOURCE_LOCATION, "调用");
    this->pluginContext->LogTrace(SOURCE_LOCATION, "返回");
}

DynamicPluginLoader::~DynamicPluginLoader() {
    this->pluginContext->LogTrace(SOURCE_LOCATION, "调用");
    this->pluginContext->LogTrace(SOURCE_LOCATION, "返回");
}

bool DynamicPluginLoader::LoadPlugin(const std::string &path,
                                     const std::shared_ptr<Plugin> &plugin) {
    this->pluginContext->LogTrace(SOURCE_LOCATION, "调用");

    char *error = nullptr;

    void *handle = dlopen(path.c_str(), RTLD_NOW | RTLD_LOCAL);
    if ((error = dlerror()) != nullptr) {
        this->pluginContext->LogError(SOURCE_LOCATION, "无法加载插件文件 {} ({})",
                                      path, error);
        this->pluginContext->LogTrace(SOURCE_LOCATION, "返回");
        return false;
    }

    plugin->SetHandle(handle);
    plugin->SetPath(path);

    void *newPluginFunction = dlsym(handle, "NewPlugin");
    if ((error = dlerror()) != nullptr) {
        this->pluginContext->LogError(SOURCE_LOCATION,
                                      "在插件文件 {} 中无法找到符号 \"{}\" ({})", path, "NewPlugin",
                                      error);
        this->pluginContext->LogTrace(SOURCE_LOCATION, "返回");
        return false;
    }
    plugin->SetNewPluginFunction((Plugin::NewPluginFunction) newPluginFunction);

    void *deletePluginFunction = dlsym(handle, "DeletePlugin");
    if ((error = dlerror()) != nullptr) {
        this->pluginContext->LogError(SOURCE_LOCATION,
                                      "在插件文件 {} 中无法找到符号 \"{}\" ({})", path,
                                      "DeletePlugin", error);
        this->pluginContext->LogTrace(SOURCE_LOCATION, "返回");
        return false;
    }
    plugin->SetDeletePluginFunction((Plugin::DeletePluginFunction) deletePluginFunction);

    void *pluginName = dlsym(handle, "PluginName");
    if ((error = dlerror()) != nullptr) {
        this->pluginContext->LogError(SOURCE_LOCATION,
                                      "在插件文件 {} 中无法找到符号 \"{}\" ({})", path,
                                      "PluginName", error);
        this->pluginContext->LogTrace(SOURCE_LOCATION, "返回");
        return false;
    }
    plugin->SetName(std::string(*((const char **) pluginName)));

    void *pluginVersion = dlsym(handle, "PluginVersion");
    if ((error = dlerror()) != nullptr) {
        this->pluginContext->LogError(SOURCE_LOCATION,
                                      "在插件文件 {} 中无法找到符号 \"{}\" ({})", path,
                                      "PluginVersion", error);
        this->pluginContext->LogTrace(SOURCE_LOCATION, "返回");
        return false;
    }
    plugin->SetVersion(std::string(*((const char **) pluginVersion)));

    void *pluginDescription = dlsym(handle, "PluginDescription");
    if ((error = dlerror()) != nullptr) {
        this->pluginContext->LogError(SOURCE_LOCATION,
                                      "在插件文件 {} 中无法找到符号 \"{}\" ({})", path,
                                      "PluginDescription", error);
        this->pluginContext->LogTrace(SOURCE_LOCATION, "返回");
        return false;
    }
    plugin->SetDescription(std::string(*((const char **) pluginDescription)));

    int *dependencyCount = (int *) dlsym(handle, "DependencyCount");
    if ((error = dlerror()) != nullptr) {
        this->pluginContext->LogError(SOURCE_LOCATION,
                                      "在插件文件 {} 中无法找到符号 \"{}\" ({})", path,
                                      "DependencyCount", error);
        this->pluginContext->LogTrace(SOURCE_LOCATION, "返回");
        return false;
    }
    const char **dependencyList = (const char **) dlsym(handle, "DependencyList");
    if ((error = dlerror()) != nullptr) {
        this->pluginContext->LogError(SOURCE_LOCATION,
                                      "在插件文件 {} 中无法找到符号 \"{}\" ({})", path,
                                      "DependencyList", error);
        this->pluginContext->LogTrace(SOURCE_LOCATION, "返回");
        return false;
    }
    plugin->ParseDependencyList(*dependencyCount, dependencyList);

    int *conflictCount = (int *) dlsym(handle, "ConflictCount");
    if ((error = dlerror()) != nullptr) {
        this->pluginContext->LogError(SOURCE_LOCATION,
                                      "在插件文件 {} 中无法找到符号 \"{}\" ({})", path,
                                      "ConflictCount", error);
        this->pluginContext->LogTrace(SOURCE_LOCATION, "返回");
        return false;
    }
    const char **conflictList = (const char **) dlsym(handle, "ConflictList");
    if ((error = dlerror()) != nullptr) {
        this->pluginContext->LogError(SOURCE_LOCATION,
                                      "在插件文件 {} 中无法找到符号 \"{}\" ({})", path,
                                      "ConflictList", error);
        this->pluginContext->LogTrace(SOURCE_LOCATION, "返回");
        return false;
    }
    plugin->ParseConflictList(*conflictCount, conflictList);

    auto dependency = plugin->GetDependencyList();
    auto conflict = plugin->GetConflictList();
    for (const auto &elem : dependency) {
        this->pluginContext->LogDebug(SOURCE_LOCATION, "插件 {} 依赖插件 {}",
                                      plugin->GetPath(), elem);
    }
    for (const auto &elem : conflict) {
        this->pluginContext->LogDebug(SOURCE_LOCATION, "插件 {} 与插件 {} 冲突",
                                      plugin->GetPath(), elem);
    }

    this->pluginContext->LogInfo(SOURCE_LOCATION,
                                 "已读取插件元数据: 插件名称: {}; 版本: {}; 描述: {}; 文件名: {}",
                                 plugin->GetName(), plugin->GetVersion(), plugin->GetDescription(),
                                 plugin->GetPath());

    this->pluginContext->LogTrace(SOURCE_LOCATION, "返回");
    return true;
}

bool DynamicPluginLoader::UnloadPlugin(const std::shared_ptr<Plugin> &plugin) {
    this->pluginContext->LogTrace(SOURCE_LOCATION, "调用");
    char *error = nullptr;
    if (plugin->GetHandle() == nullptr) {
        return false;
    }
    dlclose(plugin->GetHandle());
    if ((error = dlerror()) != nullptr) {
        this->pluginContext->LogError(SOURCE_LOCATION, "无法卸载插件 {} ({})",
                                      plugin->GetPath(), error);
        this->pluginContext->LogTrace(SOURCE_LOCATION, "返回");
        return false;
    } else {
        this->pluginContext->LogInfo(SOURCE_LOCATION, "已卸载插件 {}",
                                     plugin->GetPath());
        this->pluginContext->LogTrace(SOURCE_LOCATION, "返回");
        return true;
    }
}
} // namespace Fleet::DataManager::Core