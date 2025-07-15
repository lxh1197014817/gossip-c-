/**
 * @file PluginContextImpl.h
 * @brief 插件上下文接口实现类
 * @details 提供插件运行环境和资源访问的具体实现
 * @author 唐震 <tangzhen12@otcaix.iscas.ac.cn>
 * @date 2025-06-29
 */

#ifndef FLEET_DATA_MANAGER_CORE_DYNAMIC_PLUGIN_CONTEXT_H
#define FLEET_DATA_MANAGER_CORE_DYNAMIC_PLUGIN_CONTEXT_H
#include "PluginBase.h"
#include "PluginManager.h"

namespace Fleet::DataManager::Core {
/**
 * @brief 插件上下文接口实现类
 * @details 继承自PluginContext，提供目录管理、服务访问和文件锁机制
 */
class PluginContextImpl : public PluginContext {
  public:
    /**
     * @brief 构造插件上下文实现
     * @param[in] pluginManager 插件管理器指针
     * @param[in] nodeId 节点UUID，nullptr时自动生成
     * @param[in] baseDirectory 基础目录路径
     */
    PluginContextImpl(PluginManager *pluginManager, uuid_t nodeId,
                      const std::string &baseDirectory);

    /**
     * @brief 析构函数，释放文件锁和清理资源
     */
    ~PluginContextImpl() override;

    /**
     * @brief 禁用拷贝构造函数
     */
    PluginContextImpl(const PluginContextImpl &) = delete;

    /**
     * @brief 禁用赋值操作符
     */
    PluginContextImpl &operator=(const PluginContextImpl &) = delete;

    /**
     * @brief 获取节点标识符
     * @return 节点UUID字符串
     */
    const std::string &GetNodeId() override;

    /**
     * @brief 获取指定插件的服务接口
     * @param[in] pluginName 插件名称
     * @return 服务接口指针
     */
    void *GetService(const std::string &pluginName) override;

    /**
     * @brief 获取基础目录路径
     * @return 基础目录路径字符串
     */
    const std::string &GetBaseDirectory() override;

    /**
     * @brief 获取数据目录路径
     * @return 数据目录路径字符串
     */
    const std::string &GetDataDirectory() override;

    /**
     * @brief 获取日志目录路径
     * @return 日志目录路径字符串
     */
    const std::string &GetLogDirectory() override;

    /**
     * @brief 获取数据库目录路径
     * @return 数据库目录路径字符串
     */
    const std::string &GetDatabaseDirectory() override;

  private:
    /// 插件管理器指针，用于服务查找
    PluginManager *pluginManager;

    /// 节点唯一标识符字符串
    std::string nodeId;

    /// 基础目录路径
    std::string baseDirectory;

    /// 数据存储目录路径
    std::string dataDirectory;

    /// 日志文件目录路径
    std::string logDirectory;

    /// 数据库文件目录路径
    std::string databaseDirectory;

    /// 文件锁句柄
    int lockHandle;

    /// 文件锁状态标志
    bool locked;
};
} // namespace Fleet::DataManager::Core
#endif // FLEET_DATA_MANAGER_CORE_DYNAMIC_PLUGIN_CONTEXT_H
