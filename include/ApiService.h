/**
 * @file ApiService.h
 * @brief 外部接口插件服务接口定义
 * @details 提供HTTP API服务，为外部应用提供数据管理器的访问接口
 * @author 唐震 <tangzhen12@otcaix.iscas.ac.cn>
 * @date 2025-07-02
 */

#ifndef FLEET_DATA_MANAGER_API_API_SERVICE_H
#define FLEET_DATA_MANAGER_API_API_SERVICE_H

#include <string>

namespace Fleet::DataManager::Api {
/**
 * @brief 外部API服务接口
 * @details 提供HTTP API服务器功能，允许外部应用通过REST接口访问数据管理器
 * @note 此接口为纯虚接口，由具体的API插件实现
 */
class ApiService {
  public:
    /**
     * @brief 默认构造函数
     */
    ApiService() = default;

    /**
     * @brief 虚析构函数
     */
    virtual ~ApiService() = default;

    /**
     * @brief 禁用拷贝构造函数
     */
    ApiService(const ApiService &) = delete;

    /**
     * @brief 禁用赋值操作符
     */
    ApiService &operator=(const ApiService &) = delete;

    /**
     * @brief 获取API服务端口号
     * @return API服务端口号
     */
    virtual int GetApiPort() = 0;

    /**
     * @brief 在随机端口启动API服务
     * @param[in] address 监听地址
     * @return 启动成功返回true，失败返回false
     */
    virtual bool Start(const std::string &address) = 0;

    /**
     * @brief 在指定端口启动API服务
     * @param[in] address 监听地址
     * @param[in] port 监听端口
     * @return 启动成功返回true，失败返回false
     */
    virtual bool Start(const std::string &address, int port) = 0;

    /**
     * @brief 停止API服务
     * @return 停止成功返回true，失败返回false
     */
    virtual bool Stop() = 0;
};
} // namespace Fleet::DataManager::Api

#endif // FLEET_DATA_MANAGER_API_API_SERVICE_H