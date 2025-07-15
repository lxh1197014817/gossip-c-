// Copyright (c) 2025 Institute of Software, Chinese Academy of Sciences
// Author: Zhen Tang <tangzhen12@otcaix.iscas.ac.cn>
// Affiliation: Institute of Software, Chinese Academy of Sciences

#include "Logger.h"
#include "StaticPluginBase.h"
#include "StaticPluginFactory.h"
#include <string>
#include <vector>

namespace Fleet::DataManager::Core {
RegisterStaticPlugin::RegisterStaticPlugin(const std::string &pluginName,
                                           const std::string &pluginVersion,
                                           const std::string &pluginDescription,
                                           NewPluginFunction newPluginFunction,
                                           DeletePluginFunction deletePluginFunction,
                                           const std::vector<std::string> &dependencyList,
                                           const std::vector<std::string> &conflictList) {
    Fleet::DataManager::Core::Logger::ConsoleLogger().Info(SOURCE_LOCATION,
                                                           "注册静态插件 {} {} ({})", pluginName,
                                                           pluginVersion, pluginDescription);
    Fleet::DataManager::Core::StaticPluginFactory::GetInstance().RegisterPlugin(
        pluginName, pluginVersion, pluginDescription, newPluginFunction, deletePluginFunction,
        dependencyList, conflictList);
}
} // namespace Fleet::DataManager::Core