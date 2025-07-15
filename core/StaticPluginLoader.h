/**
 * @file StaticPluginLoader.h
 * @brief 静态插件加载器实现类
 * @details 从静态插件工厂获取编译时链接的插件
 * @author 唐震 <tangzhen12@otcaix.iscas.ac.cn>
 * @date 2025-06-29
 */

#ifndef FLEET_DATA_MANAGER_CORE_STATIC_PLUGIN_LOADER_H
#define FLEET_DATA_MANAGER_CORE_STATIC_PLUGIN_LOADER_H

#include "PluginLoader.h"

namespace Fleet::DataManager::Core {
/**
 * @brief 静态插件加载器实现类
 * @details 继承自PluginLoader，从静态插件工厂获取已注册的插件
 */
class StaticPluginLoader : public PluginLoader {
  public:
    /**
     * @brief 构造静态插件加载器
     * @param[in] pluginContext 插件上下文共享指针
     */
    explicit StaticPluginLoader(const std::shared_ptr<PluginContext> &pluginContext);

    /**
     * @brief 析构函数
     */
    ~StaticPluginLoader() override;

    /**
     * @brief 禁用拷贝构造函数
     */
    StaticPluginLoader(const StaticPluginLoader &) = delete;

    /**
     * @brief 禁用赋值操作符
     */
    StaticPluginLoader &operator=(const StaticPluginLoader &) = delete;

    /**
     * @brief 从静态插件工厂加载插件
     * @details 根据插件名称从工厂获取已注册的插件元数据
     * @param[in] path 插件名称，静态插件不使用文件路径
     * @param[out] plugin 插件元数据对象
     * @return 找到插件返回true，未找到返回false
     */
    bool LoadPlugin(const std::string &path, const std::shared_ptr<Plugin> &plugin) override;

    /**
     * @brief 卸载静态插件
     * @details 静态插件无需卸载操作，直接返回成功
     * @param[in] plugin 插件对象
     * @return 始终返回true
     */
    bool UnloadPlugin(const std::shared_ptr<Plugin> &plugin) override;

  private:
    /// 插件上下文对象，用于日志记录
    std::shared_ptr<PluginContext> pluginContext;
};
} // namespace Fleet::DataManager::Core

#endif // FLEET_DATA_MANAGER_CORE_STATIC_PLUGIN_LOADER_H
