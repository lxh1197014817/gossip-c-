/**
 * @file StaticPluginBase.h
 * @brief 静态插件契约接口定义
 * @details 定义静态链接插件的注册机制，通过构造函数自动注册插件到插件管理器
 * @author 唐震 <tangzhen12@otcaix.iscas.ac.cn>
 * @date 2025-07-02
 */

#ifndef FLEET_DATA_MANAGER_CORE_STATIC_PLUGIN_BASE_H
#define FLEET_DATA_MANAGER_CORE_STATIC_PLUGIN_BASE_H

#include <string>
#include <vector>

namespace Fleet::DataManager::Core {
/**
 * @brief 静态插件注册类
 * @details 用于静态链接插件的自动注册，通过全局对象的构造函数在程序启动时注册插件
 * @note 静态插件应创建此类的全局实例以完成自动注册
 */
class RegisterStaticPlugin {
  public:
    /**
     * @brief 插件创建函数类型定义
     * @return 插件对象指针
     */
    typedef void *(*NewPluginFunction)();

    /**
     * @brief 插件销毁函数类型定义
     * @param[in] plugin 插件对象指针
     */
    typedef void (*DeletePluginFunction)(void *);

    /**
     * @brief 注册静态插件
     * @details 构造函数会自动将插件信息注册到静态插件管理器
     * @param[in] pluginName 插件名称
     * @param[in] pluginVersion 插件版本
     * @param[in] pluginDescription 插件描述
     * @param[in] newPluginFunction 插件创建函数指针
     * @param[in] deletePluginFunction 插件销毁函数指针
     * @param[in] dependencyList 依赖插件名称列表
     * @param[in] conflictList 冲突插件名称列表
     */
    RegisterStaticPlugin(const std::string &pluginName, const std::string &pluginVersion,
                         const std::string &pluginDescription, NewPluginFunction newPluginFunction,
                         DeletePluginFunction deletePluginFunction,
                         const std::vector<std::string> &dependencyList,
                         const std::vector<std::string> &conflictList);
};
} // namespace Fleet::DataManager::Core

#endif // FLEET_DATA_MANAGER_CORE_STATIC_PLUGIN_BASE_H
