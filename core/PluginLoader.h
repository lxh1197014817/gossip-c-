/**
 * @file PluginLoader.h
 * @brief 插件加载器抽象接口
 * @details 定义插件加载和卸载的统一接口规范
 * @author 唐震 <tangzhen12@otcaix.iscas.ac.cn>
 * @date 2025-06-29
 */

#ifndef FLEET_DATA_MANAGER_CORE_PLUGIN_LOADER_H
#define FLEET_DATA_MANAGER_CORE_PLUGIN_LOADER_H

#include <memory>
#include <string>

#include "Plugin.h"

namespace Fleet::DataManager::Core {
/**
 * @brief 插件加载器抽象接口类
 * @details 定义插件加载和卸载的统一接口，由具体实现类继承
 */
class PluginLoader {
  public:
    /**
     * @brief 默认构造函数
     */
    PluginLoader() = default;

    /**
     * @brief 虚析构函数
     */
    virtual ~PluginLoader() = default;

    /**
     * @brief 禁用拷贝构造函数
     */
    PluginLoader(const PluginLoader &) = delete;

    /**
     * @brief 禁用赋值操作符
     */
    PluginLoader &operator=(const PluginLoader &) = delete;

    /**
     * @brief 加载插件并填充元数据
     * @param[in] path 插件路径或名称
     * @param[out] plugin 插件元数据对象
     * @return 加载成功返回true，失败返回false
     */
    virtual bool LoadPlugin(const std::string &path, const std::shared_ptr<Plugin> &plugin) = 0;

    /**
     * @brief 卸载指定插件
     * @param[in] plugin 待卸载的插件对象
     * @return 卸载成功返回true，失败返回false
     */
    virtual bool UnloadPlugin(const std::shared_ptr<Plugin> &plugin) = 0;
};
} // namespace Fleet::DataManager::Core

#endif // FLEET_DATA_MANAGER_CORE_PLUGIN_LOADER_H
