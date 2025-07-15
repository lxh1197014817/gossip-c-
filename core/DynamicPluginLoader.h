/**
 * @file DynamicPluginLoader.h
 * @brief 动态插件加载器实现类
 * @details 使用dlopen/dlsym机制加载共享库形式的插件
 * @author 唐震 <tangzhen12@otcaix.iscas.ac.cn>
 * @date 2025-06-29
 */

#ifndef FLEET_DATA_MANAGER_CORE_DYNAMIC_PLUGIN_LOADER_H
#define FLEET_DATA_MANAGER_CORE_DYNAMIC_PLUGIN_LOADER_H

#include "PluginLoader.h"

namespace Fleet {
namespace DataManager {
namespace Core {
/**
 * @brief 动态插件加载器实现类
 * @details 继承自PluginLoader，使用dlopen/dlsym机制加载共享库插件
 */
class DynamicPluginLoader : public PluginLoader {
  public:
    /**
     * @brief 构造动态插件加载器
     * @param[in] pluginContext 插件上下文共享指针
     */
    explicit DynamicPluginLoader(const std::shared_ptr<PluginContext> &pluginContext);

    /**
     * @brief 析构函数
     */
    ~DynamicPluginLoader() override;

    /**
     * @brief 禁用拷贝构造函数
     */
    DynamicPluginLoader(const DynamicPluginLoader &another) = delete;

    /**
     * @brief 禁用赋值操作符
     */
    DynamicPluginLoader &operator=(const DynamicPluginLoader &another) = delete;

    /**
     * @brief 从共享库文件加载插件
     * @details 使用dlopen加载共享库，解析插件元数据和符号
     * @param[in] path 共享库文件路径
     * @param[out] plugin 插件元数据对象
     * @return 加载成功返回true，失败返回false
     */
    bool LoadPlugin(const std::string &path, const std::shared_ptr<Plugin> &plugin) override;

    /**
     * @brief 卸载动态插件
     * @details 使用dlclose关闭共享库句柄
     * @param[in] plugin 待卸载的插件对象
     * @return 卸载成功返回true，失败返回false
     */
    bool UnloadPlugin(const std::shared_ptr<Plugin> &plugin) override;

  private:
    /// 插件上下文对象，用于日志记录
    std::shared_ptr<PluginContext> pluginContext;
};
} // namespace Core
} // namespace DataManager
} // namespace Fleet
#endif // FLEET_DATA_MANAGER_CORE_DYNAMIC_PLUGIN_LOADER_H
