/**
 * @file Logger.h
 * @brief 日志记录模块
 * @details 基于spdlog实现的日志记录系统，支持控制台和文件输出，提供多级别日志记录功能
 * @author 唐震 <tangzhen12@otcaix.iscas.ac.cn>
 * @date 2025-07-02
 */

#ifndef FLEET_DATA_MANAGER_CORE_LOGGER_H
#define FLEET_DATA_MANAGER_CORE_LOGGER_H

#ifndef BUILD_VERSION
#define BUILD_VERSION "未知"
#endif

#include "spdlog/async.h"
#include "spdlog/logger.h"
#include "spdlog/sinks/rotating_file_sink.h"
#include "spdlog/sinks/stdout_sinks.h"
#include "spdlog/spdlog.h"
#include <memory>

#define SOURCE_LOCATION spdlog::source_loc(__FILE__, __LINE__, __FUNCTION__)

namespace Fleet::DataManager::Core {
/**
 * @brief 日志记录器类
 * @details 基于spdlog库实现的日志记录系统，支持控制台输出和文件轮转，提供多级别日志记录
 * @note 日志级别可通过环境变量FLEET_LOG_LEVEL控制
 */
class Logger {
  public:
    /**
     * @brief 默认构造函数
     * @details 创建控制台日志记录器，输出到标准输出
     */
    Logger() {
        this->logger = spdlog::stdout_logger_mt("Fleet::DataManager");
        this->logger->set_pattern("[%Y-%m-%d %H:%M:%S %l %g:%#][%n][%!] %v");
        // this->logger->set_pattern("[%Y-%m-%d %H:%M:%S %l %g:%#][%n] %v");
        this->ParseLevel();
    }

    /**
     * @brief 构造文件日志记录器
     * @details 创建同时输出到控制台和文件的日志记录器，支持文件轮转
     * @param[in] nodeId 节点标识符，用于日志记录器命名
     * @param[in] fileName 日志文件路径
     * @note 文件大小限制为50MB，保留3个轮转文件
     */
    explicit Logger(const std::string &nodeId, const std::string &fileName) {
        std::string loggerName = "版本 " BUILD_VERSION " 节点 " + nodeId.substr(0, 8);
        std::vector<spdlog::sink_ptr> sinks;
        sinks.push_back(std::make_shared<spdlog::sinks::stdout_sink_mt>());
        sinks.push_back(
            std::make_shared<spdlog::sinks::rotating_file_sink_mt>(fileName, 50 * 1024 * 1024, 3));
        this->logger = std::make_shared<spdlog::logger>(loggerName, begin(sinks), end(sinks));
        this->logger->set_pattern("[%Y-%m-%d %H:%M:%S %l %g:%#][%n][%!] %v");
        // this->logger->set_pattern("[%Y-%m-%d %H:%M:%S %l %g:%#][%n] %v");
        this->ParseLevel();
    }

    /**
     * @brief 虚析构函数
     */
    virtual ~Logger() = default;

    /**
     * @brief 禁用拷贝构造函数
     */
    Logger(const Logger &) = delete;

    /**
     * @brief 禁用赋值操作符
     */
    Logger &operator=(const Logger &) = delete;

    /**
     * @brief 获取控制台日志记录器单例
     * @details 返回全局唯一的控制台日志记录器实例
     * @return 控制台日志记录器引用
     */
    static Logger &ConsoleLogger() {
        static Logger consoleLogger;
        return consoleLogger;
    }

    /**
     * @brief 记录跟踪级别日志
     * @details 记录最详细的程序执行信息，用于深度调试和问题定位
     * @param[in] location 源代码位置信息，包含文件名、行号和函数名
     * @param[in] format 格式化字符串
     * @param[in] args 格式化参数
     */
    template <typename... Args>
    void Trace(const spdlog::source_loc &location, const std::string_view &format, Args &&...args) {
        this->logger->log(location, spdlog::level::trace, fmt::runtime(format),
                          std::forward<Args>(args)...);
    }

    /**
     * @brief 记录调试级别日志
     * @details 记录调试信息，帮助开发者理解程序执行流程和状态
     * @param[in] location 源代码位置信息，包含文件名、行号和函数名
     * @param[in] format 格式化字符串
     * @param[in] args 格式化参数
     */
    template <typename... Args>
    void Debug(const spdlog::source_loc &location, const std::string_view &format, Args &&...args) {
        this->logger->log(location, spdlog::level::debug, fmt::runtime(format),
                          std::forward<Args>(args)...);
    }

    /**
     * @brief 记录信息级别日志
     * @details 记录一般性运行信息，用于了解程序正常运行状态
     * @param[in] location 源代码位置信息，包含文件名、行号和函数名
     * @param[in] format 格式化字符串
     * @param[in] args 格式化参数
     */
    template <typename... Args>
    void Info(const spdlog::source_loc &location, const std::string_view &format, Args &&...args) {
        this->logger->log(location, spdlog::level::info, fmt::runtime(format),
                          std::forward<Args>(args)...);
    }

    /**
     * @brief 记录警告级别日志
     * @details 记录警告信息，表示潜在问题但不影响程序继续执行
     * @param[in] location 源代码位置信息，包含文件名、行号和函数名
     * @param[in] format 格式化字符串
     * @param[in] args 格式化参数
     */
    template <typename... Args>
    void Warn(const spdlog::source_loc &location, const std::string_view &format, Args &&...args) {
        this->logger->log(location, spdlog::level::warn, fmt::runtime(format),
                          std::forward<Args>(args)...);
    }

    /**
     * @brief 记录错误级别日志
     * @details 记录错误信息，表示程序执行中遇到的严重问题
     * @param[in] location 源代码位置信息，包含文件名、行号和函数名
     * @param[in] format 格式化字符串
     * @param[in] args 格式化参数
     */
    template <typename... Args>
    void Error(const spdlog::source_loc &location, const std::string_view &format, Args &&...args) {
        this->logger->log(location, spdlog::level::err, fmt::runtime(format),
                          std::forward<Args>(args)...);
    }

  private:
    /// spdlog日志记录器实例，负责实际的日志输出操作
    std::shared_ptr<spdlog::logger> logger;

    /**
     * @brief 解析日志级别配置
     * @details 从环境变量FLEET_LOG_LEVEL读取日志级别配置，默认为ERROR级别
     * @note 支持的日志级别：TRACE, DEBUG, INFO, WARN, ERROR
     */
    void ParseLevel() {
        char *level = getenv("FLEET_LOG_LEVEL");
        this->logger->set_level(spdlog::level::err);
        if (level != nullptr) {
            auto logLevel = spdlog::level::from_str(level);
            if (logLevel != spdlog::level::off) {
                this->logger->set_level(logLevel);
                this->Info(SOURCE_LOCATION,
                           "已从环境变量 FLEET_LOG_LEVEL 中解析日志级别, 设置为: {}", level);
            } else {
                this->Error(SOURCE_LOCATION,
                            "无法从环境变量 FLEET_LOG_LEVEL 中解析日志级别, 传入的值为: {}, "
                            "可接受的值为: TRACE, DEBUG, INFO, WARN, ERROR",
                            level);
            }
        }
    }
};
} // namespace Fleet::DataManager::Core
#endif // FLEET_DATA_MANAGER_CORE_LOGGER_H
