/**
 * @file PluginContext.h
 * @brief 插件上下文接口定义
 * @details 为插件提供运行时环境和服务访问能力，包括目录管理、服务获取和日志记录功能
 * @author 唐震 <tangzhen12@otcaix.iscas.ac.cn>
 * @date 2025-07-02
 */

#ifndef FLEET_DATA_MANAGER_CORE_PLUGIN_CONTEXT_H
#define FLEET_DATA_MANAGER_CORE_PLUGIN_CONTEXT_H

#include "Logger.h"
#include <memory>
#include <string>

namespace Fleet::DataManager::Core {

/**
 * @brief 插件上下文接口
 * @details 为插件提供运行时环境和资源访问能力，包括节点信息、目录管理、服务发现和日志记录
 * @note 此接口由插件管理器实现，插件通过此接口访问系统资源
 * @see PluginContextImpl
 */
class PluginContext {
  public:
    /**
     * @brief 默认构造函数
     */
    PluginContext() = default;

    /**
     * @brief 虚析构函数
     */
    virtual ~PluginContext() = default;

    /**
     * @brief 禁用拷贝构造函数
     */
    PluginContext(const PluginContext &) = delete;

    /**
     * @brief 禁用赋值操作符
     */
    PluginContext &operator=(const PluginContext &) = delete;

    /**
     * @brief 获取节点标识符
     * @details 返回当前节点的UUID字符串，用于节点间通信和标识
     * @return 节点UUID字符串引用
     */
    virtual const std::string &GetNodeId() = 0;

    /**
     * @brief 获取其他插件服务
     * @details 通过插件名称获取其他插件提供的服务实例
     * @param[in] pluginName 插件名称
     * @return 服务对象指针，未找到返回nullptr
     * @note 调用方需要将返回指针转换为对应的服务接口类型
     */
    virtual void *GetService(const std::string &pluginName) = 0;

    /**
     * @brief 获取基础目录路径
     * @details 返回插件系统的根目录路径
     * @return 基础目录路径字符串引用
     */
    virtual const std::string &GetBaseDirectory() = 0;

    /**
     * @brief 获取数据目录路径
     * @details 返回插件数据存储目录路径
     * @return 数据目录路径字符串引用
     */
    virtual const std::string &GetDataDirectory() = 0;

    /**
     * @brief 获取日志目录路径
     * @details 返回日志文件存储目录路径
     * @return 日志目录路径字符串引用
     */
    virtual const std::string &GetLogDirectory() = 0;

    /**
     * @brief 获取数据库目录路径
     * @details 返回数据库文件存储目录路径
     * @return 数据库目录路径字符串引用
     */
    virtual const std::string &GetDatabaseDirectory() = 0;

    /**
     * @brief 记录跟踪级别日志
     * @details 记录详细的程序执行跟踪信息，用于调试和问题定位
     * @param[in] location 源代码位置信息
     * @param[in] format 格式化字符串
     * @param[in] args 格式化参数
     */
    template <typename... Args>
    void LogTrace(const spdlog::source_loc &location, const std::string_view &format,
                  Args &&...args) {
        this->logger->Trace(location, format, std::forward<Args>(args)...);
    }

    /**
     * @brief 记录调试级别日志
     * @details 记录调试信息，帮助开发者理解程序执行流程
     * @param[in] location 源代码位置信息
     * @param[in] format 格式化字符串
     * @param[in] args 格式化参数
     */
    template <typename... Args>
    void LogDebug(const spdlog::source_loc &location, const std::string_view &format,
                  Args &&...args) {
        this->logger->Debug(location, format, std::forward<Args>(args)...);
    }

    /**
     * @brief 记录信息级别日志
     * @details 记录一般性信息，用于了解程序运行状态
     * @param[in] location 源代码位置信息
     * @param[in] format 格式化字符串
     * @param[in] args 格式化参数
     */
    template <typename... Args>
    void LogInfo(const spdlog::source_loc &location, const std::string_view &format,
                 Args &&...args) {
        this->logger->Info(location, format, std::forward<Args>(args)...);
    }

    /**
     * @brief 记录警告级别日志
     * @details 记录警告信息，表示潜在问题但不影响程序继续执行
     * @param[in] location 源代码位置信息
     * @param[in] format 格式化字符串
     * @param[in] args 格式化参数
     */
    template <typename... Args>
    void LogWarn(const spdlog::source_loc &location, const std::string_view &format,
                 Args &&...args) {
        this->logger->Warn(location, format, std::forward<Args>(args)...);
    }

    /**
     * @brief 记录错误级别日志
     * @details 记录错误信息，表示程序执行中遇到的严重问题
     * @param[in] location 源代码位置信息
     * @param[in] format 格式化字符串
     * @param[in] args 格式化参数
     */
    template <typename... Args>
    void LogError(const spdlog::source_loc &location, const std::string_view &format,
                  Args &&...args) {
        this->logger->Error(location, format, std::forward<Args>(args)...);
    }

  protected:
    /// 日志记录器实例，用于输出各级别日志信息
    std::shared_ptr<Logger> logger;
};
} // namespace Fleet::DataManager::Core
#endif // FLEET_DATA_MANAGER_CORE_PLUGIN_CONTEXT_H
