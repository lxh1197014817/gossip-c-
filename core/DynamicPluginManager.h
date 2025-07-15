/**
 * @file DynamicPluginManager.h
 * @brief 动态插件管理器实现类
 * @details 支持运行时动态加载共享库形式的插件
 * @author 唐震 <tangzhen12@otcaix.iscas.ac.cn>
 * @date 2025-06-29
 */

#ifndef FLEET_DATA_MANAGER_CORE_DYNAMIC_PLUGIN_MANAGER_H
#define FLEET_DATA_MANAGER_CORE_DYNAMIC_PLUGIN_MANAGER_H

#include "Plugin.h"
#include "PluginLoader.h"
#include "PluginManager.h"
#include <map>
#include <memory>
#include <vector>

namespace Fleet::DataManager::Core {
/**
 * @brief 动态插件管理器实现类
 * @details 继承自PluginManager，专门处理共享库形式的动态插件加载
 */
class DynamicPluginManager : public PluginManager {
  public:
    /**
     * @brief 构造动态插件管理器
     * @param[in] nodeId 节点唯一标识符
     * @param[in] baseDirectory 数据存储根目录路径
     */
    DynamicPluginManager(uuid_t nodeId, const std::string &baseDirectory);

    /**
     * @brief 析构函数，清理动态插件加载器
     */
    ~DynamicPluginManager() override;

    /**
     * @brief 禁用拷贝构造函数
     */
    DynamicPluginManager(const DynamicPluginManager &) = delete;

    /**
     * @brief 禁用赋值操作符
     */
    DynamicPluginManager &operator=(const DynamicPluginManager &) = delete;

    /**
     * @brief 从指定目录加载指定名称的插件
     * @param[in] path 插件目录路径
     * @param[in] pluginNames 待加载的插件名称列表
     * @return 全部加载成功返回true，否则返回false
     */
    bool LoadPluginsFromDirectory(const std::string &path,
                                  const std::vector<std::string> &pluginNames);

    /**
     * @brief 从当前工作目录加载指定名称的插件
     * @param[in] pluginNames 待加载的插件名称列表
     * @return 全部加载成功返回true，否则返回false
     */
    bool LoadPluginsFromWorkingDirectory(const std::vector<std::string> &pluginNames);

    /**
     * @brief 根据参数配置加载插件
     * @details 解析core.pluginDirectory和core.pluginsToLoad参数
     * @return 加载成功返回true，失败返回false
     */
    bool LoadPluginsFromParameter() override;

  private:

    /// 动态插件加载器实例，负责共享库的加载和卸载
    std::shared_ptr<PluginLoader> pluginLoader;

    /**
     * @brief 获取插件加载器实例
     * @return 动态插件加载器指针
     */
    std::shared_ptr<PluginLoader> GetPluginLoader() override;

    /**
     * @brief 列出指定目录下特定扩展名的文件
     * @param[in] directory 目录路径
     * @param[in] extension 文件扩展名，如".so"
     * @param[out] result 找到的文件路径列表
     * @return 操作成功返回true，失败返回false
     */
    bool ListFiles(const std::string &directory, const std::string &extension,
                   std::vector<std::string> &result);

    /**
     * @brief 探测目录中所有可用的插件
     * @param[in] directory 插件目录路径
     * @param[out] result 发现的插件名称列表
     * @return 探测成功返回true，失败返回false
     */
    bool ProbeAvailablePlugins(const std::string &directory, std::vector<std::string> &result);
};
} // namespace Fleet::DataManager::Core
#endif // FLEET_DATA_MANAGER_CORE_DYNAMIC_PLUGIN_MANAGER_H
