/**
 * @file PluginBase.h
 * @brief 插件契约接口基类
 * @details 定义所有插件必须实现的基础接口，包括初始化和服务获取功能
 * @author 唐震 <tangzhen12@otcaix.iscas.ac.cn>
 * @date 2025-07-02
 */

#ifndef FLEET_DATA_MANAGER_CORE_PLUGIN_BASE_H
#define FLEET_DATA_MANAGER_CORE_PLUGIN_BASE_H

#include <map>
#include <string>

#include "PluginContext.h"

namespace Fleet::DataManager::Core {
/**
 * @brief 插件契约接口基类
 * @details 所有插件必须继承此抽象基类，实现初始化和服务提供功能，确保插件系统的统一性和可扩展性
 * @note 此类为纯虚基类，不能直接实例化
 * @see DynamicPluginBase StaticPluginBase
 */
class PluginBase {
  public:
    /**
     * @brief 默认构造函数
     */
    PluginBase() = default;

    /**
     * @brief 虚析构函数
     * @details 确保派生类对象能够正确析构
     */
    virtual ~PluginBase() = default;

    /**
     * @brief 禁用拷贝构造函数
     */
    PluginBase(const PluginBase &) = delete;

    /**
     * @brief 禁用赋值操作符
     */
    PluginBase &operator=(const PluginBase &) = delete;

    /**
     * @brief 初始化插件
     * @details 插件加载后调用此方法进行初始化，传入插件上下文和配置参数
     * @param[in] context 插件上下文，提供日志、目录管理和服务访问功能
     * @param[in] parameters 插件配置参数键值对
     * @return 初始化成功返回true，失败返回false
     * @note 初始化失败的插件将不会被加载到系统中
     */
    virtual bool Initialize(const std::shared_ptr<PluginContext> &context,
                            const std::map<std::string, std::string> &parameters) = 0;

    /**
     * @brief 获取插件服务对象
     * @details 返回插件提供的具体服务实例，调用方需要转换为对应的服务接口类型
     * @return 服务对象指针，失败返回nullptr
     * @note 返回的指针生命周期由插件管理，调用方不应释放
     */
    virtual void *GetService() = 0;
};
} // namespace Fleet::DataManager::Core

#endif // FLEET_DATA_MANAGER_CORE_PLUGIN_BASE_H
