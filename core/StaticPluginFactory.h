/**
 * @file StaticPluginFactory.h
 * @brief 静态插件工厂单例类
 * @details 管理所有静态插件的注册和查找
 * @author 唐震 <tangzhen12@otcaix.iscas.ac.cn>
 * @date 2025-06-29
 */

#ifndef FLEET_DATA_MANAGER_CORE_STATIC_PLUGIN_FACTORY_H
#define FLEET_DATA_MANAGER_CORE_STATIC_PLUGIN_FACTORY_H

#include <map>
#include <memory>
#include <string>
#include <vector>

#include "Plugin.h"

namespace Fleet::DataManager::Core {
/**
 * @brief 静态插件工厂单例类
 * @details 管理所有静态插件的注册、查找和元数据存储
 */
class StaticPluginFactory {
  public:
    /// 插件实例创建函数指针类型
    typedef void *(*NewPluginFunction)();

    /// 插件实例销毁函数指针类型
    typedef void (*DeletePluginFunction)(void *);

    /**
     * @brief 根据名称查找静态插件
     * @param[in] pluginName 插件名称
     * @return 找到的插件对象，未找到返回nullptr
     */
    std::shared_ptr<Plugin> FindPlugin(const std::string &pluginName);

    /**
     * @brief 注册静态插件
     * @param[in] pluginName 插件名称
     * @param[in] pluginVersion 插件版本
     * @param[in] pluginDescription 插件描述
     * @param[in] newPluginFunction 插件创建函数
     * @param[in] deletePluginFunction 插件销毁函数
     * @param[in] dependencyList 依赖插件列表
     * @param[in] conflictList 冲突插件列表
     */
    void RegisterPlugin(const std::string &pluginName, const std::string &pluginVersion,
                        const std::string &pluginDescription, NewPluginFunction newPluginFunction,
                        DeletePluginFunction deletePluginFunction,
                        const std::vector<std::string> &dependencyList,
                        const std::vector<std::string> &conflictList);

    /**
     * @brief 获取所有已注册插件的名称
     * @param[out] pluginNames 插件名称列表
     * @return 操作成功返回true
     */
    bool GetAllPluginNames(std::vector<std::string> &pluginNames);

    /**
     * @brief 获取工厂单例实例
     * @return 工厂实例引用
     */
    static StaticPluginFactory &GetInstance();

  private:
    std::map<std::string, std::shared_ptr<Plugin>> pluginMap;

    StaticPluginFactory();

    ~StaticPluginFactory();
};

} // namespace Fleet::DataManager::Core

#endif // FLEET_DATA_MANAGER_CORE_STATIC_PLUGIN_FACTORY_H
