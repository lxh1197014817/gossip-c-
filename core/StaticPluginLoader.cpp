// Copyright (c) 2025 Institute of Software, Chinese Academy of Sciences
// Author: Zhen Tang <tangzhen12@otcaix.iscas.ac.cn>
// Affiliation: Institute of Software, Chinese Academy of Sciences

#include "StaticPluginLoader.h"
#include "StaticPluginFactory.h"

namespace Fleet::DataManager::Core {
StaticPluginLoader::StaticPluginLoader(const std::shared_ptr<PluginContext> &pluginContext) {
    this->pluginContext = pluginContext;
}

StaticPluginLoader::~StaticPluginLoader() {
}

bool StaticPluginLoader::LoadPlugin(const std::string &path,
                                    const std::shared_ptr<Plugin> &plugin) {
    // 此处路径即为插件名
    auto result = StaticPluginFactory::GetInstance().FindPlugin(path);
    if (plugin == nullptr) {
        this->pluginContext->LogError(SOURCE_LOCATION, "未找到静态插件 {}",
                                      path.c_str());
        return false;
    }
    plugin->From(result);
    this->pluginContext->LogInfo(SOURCE_LOCATION, "已找到静态插件 {}",
                                 plugin->GetPath().c_str());
    return true;
}

bool StaticPluginLoader::UnloadPlugin(const std::shared_ptr<Plugin> &plugin) {
    // 无需卸载静态插件
    return true;
}
} // namespace Fleet::DataManager::Core