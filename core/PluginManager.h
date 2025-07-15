/**
 * @file PluginManager.h
 * @brief 插件管理器抽象基类
 * @details 提供插件生命周期管理和依赖解析的核心框架
 * @author 唐震 <tangzhen12@otcaix.iscas.ac.cn>
 * @date 2025-06-29
 */

#ifndef FLEET_DATA_MANAGER_CORE_PLUGIN_MANAGER_H
#define FLEET_DATA_MANAGER_CORE_PLUGIN_MANAGER_H

#include "PluginBase.h"
#include "PluginLoader.h"
#include <map>
#include <string>
#include <uuid/uuid.h>
#include <vector>

namespace Fleet::DataManager::Core {
/**
 * @brief 插件管理器抽象基类
 * @details 负责插件的加载、卸载、依赖管理和生命周期控制
 */
class PluginManager {
  public:
    /**
     * @brief 构造插件管理器
     * @param[in] nodeId 节点唯一标识符
     * @param[in] baseDirectory 数据存储根目录路径
     */
    PluginManager(uuid_t nodeId, const std::string &baseDirectory);

    /**
     * @brief 析构函数，清理所有已加载插件
     */
    virtual ~PluginManager();

    /**
     * @brief 禁用拷贝构造函数
     */
    PluginManager(const PluginManager &) = delete;

    /**
     * @brief 禁用赋值操作符
     */
    PluginManager &operator=(const PluginManager &) = delete;

    /**
     * @brief 从指定路径加载单个插件
     * @param[in] path 插件文件路径
     * @return 加载成功返回true，失败返回false
     */
    bool LoadPlugin(const std::string &path);

    /**
     * @brief 卸载指定名称的插件
     * @param[in] pluginName 插件名称
     * @return 卸载成功返回true，失败返回false
     */
    bool UnloadPlugin(const std::string &pluginName);

    /**
     * @brief 卸载所有已加载的插件
     * @return 全部卸载成功返回true，否则返回false
     */
    bool UnloadAllPlugins();

    /**
     * @brief 获取节点标识符
     * @return 节点ID字符串
     */
    const std::string &GetNodeId();

    /**
     * @brief 获取指定插件提供的服务接口
     * @param[in] pluginName 插件名称
     * @return 服务接口指针，未找到返回nullptr
     */
    void *GetService(const std::string &pluginName);

    /**
     * @brief 获取插件上下文对象
     * @return 插件上下文共享指针
     */
    std::shared_ptr<PluginContext> GetPluginContext() const;

    /**
     * @brief 设置插件管理器参数
     * @param[in] key 参数键名
     * @param[in] value 参数值
     */
    void SetParameter(const std::string &key, const std::string &value);

    /**
     * @brief 从参数配置加载插件，由子类实现具体策略
     * @return 加载成功返回true，失败返回false
     */
    virtual bool LoadPluginsFromParameter() = 0;

    /**
     * @brief 执行插件初始化操作
     * @param[in] plugin 待初始化的插件对象
     */
    void DoInitializePlugin(const std::shared_ptr<Plugin> &plugin);

  protected:
    /// 插件管理器配置参数映射表
    std::map<std::string, std::string> parameters;

    /**
     * @brief 从待加载列表中选择一个可用插件进行加载
     * @param[in,out] toLoad 待加载插件列表，成功加载的插件会被移除
     * @return 成功加载一个插件返回true，无可用插件返回false
     */
    bool LoadAnAvailablePlugin(std::vector<std::shared_ptr<Plugin>> &toLoad);

    /// 插件上下文对象，提供插件运行环境
    std::shared_ptr<PluginContext> pluginContext;

  private:
    /// 已加载插件的有序列表，按加载顺序排列
    std::vector<std::shared_ptr<Plugin>> pluginList;

    /// 插件名称到插件对象的映射表，用于快速查找
    std::map<std::string, std::shared_ptr<Plugin>> pluginMap;

    

    /**
     * @brief 执行插件销毁操作
     * @param[in] plugin 待销毁的插件对象
     * @return 销毁成功返回true，失败返回false
     */
    bool DoDestroyPlugin(const std::shared_ptr<Plugin> &plugin);

    /**
     * @brief 检查插件是否与已加载插件存在冲突
     * @param[in] plugin 待检查的插件对象
     * @return 无冲突返回true，存在冲突返回false
     */
    bool HasNoConflict(const std::shared_ptr<Plugin> &plugin);

    /**
     * @brief 检查是否有其他插件依赖于指定插件
     * @param[in] plugin 待检查的插件对象
     * @return 无依赖返回true，存在依赖返回false
     */
    bool HasNoDependency(const std::shared_ptr<Plugin> &plugin);

    /**
     * @brief 检查插件的所有依赖是否已加载
     * @param[in] plugin 待检查的插件对象
     * @return 依赖全部满足返回true，否则返回false
     */
    bool AllDependencyLoaded(const std::shared_ptr<Plugin> &plugin);

    /**
     * @brief 检查插件依赖关系并执行初始化
     * @param[in] plugin 待处理的插件对象
     * @return 检查通过并初始化成功返回true，否则返回false
     */
    bool CheckAndInitializePlugin(const std::shared_ptr<Plugin> &plugin);

    /**
     * @brief 获取插件加载器实例，由子类实现具体类型
     * @return 插件加载器指针
     */
    virtual std::shared_ptr<PluginLoader> GetPluginLoader() = 0;
};
} // namespace Fleet::DataManager::Core
#endif // FLEET_DATA_MANAGER_CORE_PLUGIN_MANAGER_H
