/**
 * @file Plugin.h
 * @brief 插件元数据管理类
 * @details 封装插件的基本信息、依赖关系和生命周期管理
 * @author 唐震 <tangzhen12@otcaix.iscas.ac.cn>
 * @date 2025-06-29
 */

#ifndef FLEET_DATA_MANAGER_CORE_PLUGIN_H
#define FLEET_DATA_MANAGER_CORE_PLUGIN_H

#include <memory>
#include <string>
#include <vector>

#include "PluginBase.h"

namespace Fleet::DataManager::Core {
/**
 * @brief 插件元数据管理类
 * @details 存储插件的基本信息、依赖关系和生命周期管理函数
 */
class Plugin {
  public:
    /// 插件实例创建函数指针类型
    typedef void *(*NewPluginFunction)();

    /// 插件实例销毁函数指针类型
    typedef void (*DeletePluginFunction)(void *);

    /**
     * @brief 默认构造函数
     */
    Plugin();

    /**
     * @brief 虚析构函数
     */
    virtual ~Plugin();

    /**
     * @brief 禁用拷贝构造函数
     */
    Plugin(const Plugin &) = delete;

    /**
     * @brief 禁用赋值操作符
     */
    Plugin &operator=(const Plugin &) = delete;

    /**
     * @brief 获取插件句柄
     * @return 插件句柄指针，静态插件返回nullptr
     */
    void *GetHandle() const;

    /**
     * @brief 设置插件句柄
     * @param[in] handle 插件句柄指针
     */
    void SetHandle(void *handle);

    /**
     * @brief 获取插件文件路径
     * @return 插件文件路径字符串
     */
    const std::string &GetPath() const;

    /**
     * @brief 设置插件文件路径
     * @param[in] path 插件文件路径
     */
    void SetPath(std::string path);

    /**
     * @brief 获取插件名称
     * @return 插件名称字符串
     */
    const std::string &GetName() const;

    /**
     * @brief 设置插件名称
     * @param[in] name 插件名称
     */
    void SetName(std::string name);

    /**
     * @brief 获取插件版本
     * @return 插件版本字符串
     */
    const std::string &GetVersion() const;

    /**
     * @brief 设置插件版本
     * @param[in] version 插件版本
     */
    void SetVersion(std::string version);

    /**
     * @brief 获取插件描述信息
     * @return 插件描述字符串
     */
    const std::string &GetDescription() const;

    /**
     * @brief 设置插件描述信息
     * @param[in] description 插件描述
     */
    void SetDescription(std::string description);

    /**
     * @brief 获取插件实例指针
     * @return 插件实例指针
     */
    PluginBase *GetPlugin();

    /**
     * @brief 设置插件实例指针
     * @param[in] plugin 插件实例指针
     */
    void SetPlugin(PluginBase *plugin);

    /**
     * @brief 创建插件实例
     * @return 新创建的插件实例指针
     */
    void *NewPlugin();

    /**
     * @brief 设置插件实例创建函数
     * @param[in] newPluginFunction 创建函数指针
     */
    void SetNewPluginFunction(NewPluginFunction newPluginFunction);

    /**
     * @brief 销毁插件实例
     */
    void DeletePlugin();

    /**
     * @brief 设置插件实例销毁函数
     * @param[in] deletePluginFunction 销毁函数指针
     */
    void SetDeletePluginFunction(DeletePluginFunction deletePluginFunction);

    /**
     * @brief 获取插件依赖列表
     * @return 依赖插件名称列表
     */
    const std::vector<std::string> &GetDependencyList();

    /**
     * @brief 获取插件冲突列表
     * @return 冲突插件名称列表
     */
    const std::vector<std::string> &GetConflictList();

    /**
     * @brief 解析依赖列表字符串数组
     * @param[in] conflictCount 依赖项数量
     * @param[in] conflictList 依赖项字符串数组
     */
    void ParseDependencyList(int conflictCount, const char *conflictList[]);

    /**
     * @brief 解析冲突列表字符串数组
     * @param[in] conflictCount 冲突项数量
     * @param[in] conflictList 冲突项字符串数组
     */
    void ParseConflictList(int conflictCount, const char *conflictList[]);

    /**
     * @brief 设置插件依赖列表
     * @param[in] dependencyList 依赖插件名称列表
     */
    void SetDependencyList(const std::vector<std::string> &dependencyList);

    /**
     * @brief 设置插件冲突列表
     * @param[in] conflictList 冲突插件名称列表
     */
    void SetConflictList(const std::vector<std::string> &conflictList);

    /**
     * @brief 从另一个插件对象复制元数据
     * @param[in] another 源插件对象
     */
    void From(const std::shared_ptr<Plugin> &another);

  private:
    /// 插件句柄，动态插件为dlopen返回值，静态插件为nullptr
    void *handle;

    /// 插件实例指针
    PluginBase *plugin;

    /// 插件文件路径
    std::string path;

    /// 插件名称
    std::string name;

    /// 插件版本
    std::string version;

    /// 插件描述信息
    std::string description;

    /// 插件依赖列表
    std::vector<std::string> dependencyList;

    /// 插件冲突列表
    std::vector<std::string> conflictList;

    /// 插件实例创建函数指针
    void *(*newPluginFunction)();

    /// 插件实例销毁函数指针
    void (*deletePluginFunction)(void *);
};
} // namespace Fleet::DataManager::Core

#endif // FLEET_DATA_MANAGER_CORE_PLUGIN_H
