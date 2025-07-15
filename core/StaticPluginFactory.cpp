// Copyright (c) 2025 Institute of Software, Chinese Academy of Sciences
// Author: Zhen Tang <tangzhen12@otcaix.iscas.ac.cn>
// Affiliation: Institute of Software, Chinese Academy of Sciences

#include "StaticPluginFactory.h"
#include "Logger.h"

namespace Fleet::DataManager::Core {
StaticPluginFactory::StaticPluginFactory() {
    this->pluginMap.clear();
}

StaticPluginFactory::~StaticPluginFactory() {
}

StaticPluginFactory &StaticPluginFactory::GetInstance() {
    static StaticPluginFactory factory;
    return factory;
}

std::shared_ptr<Plugin> StaticPluginFactory::FindPlugin(const std::string &pluginName) {
    auto iter = this->pluginMap.find(pluginName);
    if (iter == this->pluginMap.end()) {
        Logger::ConsoleLogger().Error(SOURCE_LOCATION, "未找到插件 {}",
                                      pluginName.c_str());
        return nullptr;
    } else {
        return iter->second;
    }
}

bool StaticPluginFactory::GetAllPluginNames(std::vector<std::string> &pluginNames) {
    pluginNames.clear();
    pluginNames.reserve(this->pluginMap.size());
    for (const auto &elem : this->pluginMap) {
        pluginNames.push_back(elem.first);
    }
    return true;
}

void StaticPluginFactory::RegisterPlugin(const std::string &pluginName,
                                         const std::string &pluginVersion,
                                         const std::string &pluginDescription,
                                         NewPluginFunction newPluginFunction,
                                         DeletePluginFunction deletePluginFunction,
                                         const std::vector<std::string> &dependencyList,
                                         const std::vector<std::string> &conflictList) {
    auto iter = this->pluginMap.find(pluginName);
    if (iter != this->pluginMap.end()) {
        Logger::ConsoleLogger().Error(SOURCE_LOCATION, "插件 {} 已注册, 即将替换",
                                      pluginName);
    }
    std::shared_ptr<Plugin> plugin = std::make_shared<Plugin>();
    plugin->SetHandle(nullptr);
    plugin->SetPath("");
    plugin->SetName(pluginName);
    plugin->SetVersion(pluginVersion);
    plugin->SetDescription(pluginDescription);
    plugin->SetNewPluginFunction(newPluginFunction);
    plugin->SetDeletePluginFunction(deletePluginFunction);
    plugin->SetDependencyList(dependencyList);
    plugin->SetConflictList(conflictList);
    this->pluginMap[pluginName] = plugin;
}
} // namespace Fleet::DataManager::Core