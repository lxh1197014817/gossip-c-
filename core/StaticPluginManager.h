/**
 * @file StaticPluginManager.h
 * @brief 静态插件管理器实现类
 * @details 支持编译时静态链接的插件管理机制
 * @author 唐震 <tangzhen12@otcaix.iscas.ac.cn>
 * @date 2025-06-29
 */

#ifndef FLEET_DATA_MANAGER_CORE_STATIC_PLUGIN_MANAGER_H
#define FLEET_DATA_MANAGER_CORE_STATIC_PLUGIN_MANAGER_H

#include "PluginBase.h"
#include "PluginManager.h"
#include <map>
#include <memory>
#include <vector>

namespace Fleet::DataManager::Core {
/**
 * @brief 静态插件管理器实现类
 * @details 继承自PluginManager，专门处理编译时静态链接的插件
 */
class StaticPluginManager : public PluginManager {
  public:
    /**
     * @brief 构造静态插件管理器
     * @param[in] nodeId 节点唯一标识符
     * @param[in] baseDirectory 数据存储根目录路径
     */
    StaticPluginManager(uuid_t nodeId, const std::string &baseDirectory);

    /**
     * @brief 析构函数，清理静态插件加载器
     */
    ~StaticPluginManager() override;

    /**
     * @brief 禁用拷贝构造函数
     */
    StaticPluginManager(const StaticPluginManager &) = delete;

    /**
     * @brief 禁用赋值操作符
     */
    StaticPluginManager &operator=(const StaticPluginManager &) = delete;

    /**
     * @brief 加载指定名称的静态插件列表
     * @param[in] pluginNames 待加载的插件名称列表
     * @return 全部加载成功返回true，否则返回false
     */
    bool LoadPlugins(const std::vector<std::string> &pluginNames);

    /**
     * @brief 根据参数配置加载静态插件
     * @details 解析core.pluginsToLoad参数，未设置时加载所有已注册插件
     * @return 加载成功返回true，失败返回false
     */
    bool LoadPluginsFromParameter() override;

    //for test
    bool LoadPluginsNocheck(const std::vector<std::string> &pluginNames);
  private:
    /// 静态插件加载器实例，负责从工厂获取插件
    std::shared_ptr<PluginLoader> pluginLoader;

    /**
     * @brief 获取插件加载器实例
     * @return 静态插件加载器指针
     */
    std::shared_ptr<PluginLoader> GetPluginLoader() override;
};
} // namespace Fleet::DataManager::Core
#endif // FLEET_DATA_MANAGER_CORE_STATIC_PLUGIN_MANAGER_H
