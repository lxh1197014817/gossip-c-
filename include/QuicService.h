/**
 * @file QuicService.h
 * @brief QUIC协议服务接口定义
 * @details 提供基于QUIC协议的高性能网络通信服务，支持服务端和客户端功能
 * @author 吴嘉俊 <wujiajun@otcaix.iscas.ac.cn>
 * @date 2025-07-02
 */

#ifndef FLEET_DATA_MANAGER_QUIC_QUIC_SERVICE_H
#define FLEET_DATA_MANAGER_QUIC_QUIC_SERVICE_H

#include <string>

namespace Fleet::DataManager::Quic {
/**
 * @brief QUIC服务接口
 * @details 提供QUIC协议的服务端和客户端功能，支持高性能、低延迟的网络通信
 * @note 此接口为纯虚接口，由具体的QUIC插件实现
 */
class QuicService {
  public:
    /**
     * @brief 默认构造函数
     */
    QuicService() = default;

    /**
     * @brief 虚析构函数
     */
    virtual ~QuicService() = default;

    /**
     * @brief 禁用拷贝构造函数
     */
    QuicService(const QuicService &) = delete;

    /**
     * @brief 禁用赋值操作符
     */
    QuicService &operator=(const QuicService &) = delete;

    // ================= 服务端功能接口 =================

    /**
     * @brief 创建QUIC服务器
     * @param[in] addr 监听地址
     * @param[in] port 监听端口
     * @return 服务器对象指针，失败返回nullptr
     */
    virtual void *CreateServer(const std::string &addr, int port) = 0;

    /**
     * @brief 设置数据解析回调函数
     * @param[in] server 服务器对象指针
     * @param[in] parse_data 数据解析函数指针
     * @param[in] context 用户自定义上下文数据
     * @note 解析函数用于处理接收到的数据并生成响应
     */
    virtual void SetParseFunc(void *server,
                              unsigned char *(*parse_data)(void *conn, const unsigned char *data,
                                                           size_t len, size_t &res_len,
                                                           void *context),
                              void *context) = 0;

    /**
     * @brief 设置服务器日志路径
     * @param[in] server 服务器对象指针
     * @param[in] log_path 普通日志文件路径
     * @param[in] qlog_path QLOG格式日志文件路径
     */
    virtual void SetServerLogPath(void *server, const std::string &log_path,
                                  const std::string &qlog_path) = 0;

    /**
     * @brief 启用或禁用服务器日志
     * @param[in] server 服务器对象指针
     * @param[in] enable 是否启用日志
     */
    virtual void SetServerLogEnable(void *server, bool enable) = 0;

    /**
     * @brief 设置服务器证书路径
     * @param[in] server 服务器对象指针
     * @param[in] crt_path 证书文件路径
     * @param[in] key_path 私钥文件路径
     */
    virtual void SetServerCertPath(void *server, const std::string &crt_path,
                                   const std::string &key_path) = 0;

    /**
     * @brief 启动QUIC服务器
     * @param[in] server 服务器对象指针
     * @return 成功返回true，失败返回false
     */
    virtual bool StartServer(void *server) = 0;

    /**
     * @brief 停止QUIC服务器
     * @param[in] server 服务器对象指针
     */
    virtual void StopServer(void *server) = 0;

    /**
     * @brief 销毁QUIC服务器
     * @param[in] server 服务器对象指针
     */
    virtual void DestroyServer(void *server) = 0;

    /**
     * @brief 获取服务器监听端口
     * @param[in] server 服务器对象指针
     * @return 监听端口号
     */
    virtual int GetServerPort(void *server) = 0;

    // ================= 客户端功能接口 =================

    /**
     * @brief 创建QUIC客户端
     * @return 客户端对象指针，失败返回nullptr
     */
    virtual void *CreateClient() = 0;

    /**
     * @brief 设置客户端日志路径
     * @param[in] client 客户端对象指针
     * @param[in] log_path 普通日志文件路径
     * @param[in] qlog_path QLOG格式日志文件路径
     */
    virtual void SetClientLogPath(void *client, const std::string &log_path,
                                  const std::string &qlog_path) = 0;

    /**
     * @brief 启用或禁用客户端日志
     * @param[in] client 客户端对象指针
     * @param[in] enable 是否启用日志
     */
    virtual void SetClientLogEnable(void *client, bool enable) = 0;

    /**
     * @brief 连接到QUIC服务器
     * @param[in] client 客户端对象指针
     * @param[in] addr 服务器地址
     * @param[in] port 服务器端口
     * @return 连接对象指针，失败返回nullptr
     */
    virtual void *ConnectServer(void *client, const std::string &addr, int port) = 0;

    /**
     * @brief 发送数据并接收响应
     * @param[in] client 客户端对象指针
     * @param[in] conn 连接对象指针
     * @param[in] data 发送数据指针
     * @param[in] len 发送数据长度
     * @param[out] res_len 接收数据长度
     * @return 接收数据指针，失败返回nullptr
     * @note 调用方需要释放返回的数据指针
     */
    virtual unsigned char *SendAndRecv(void *client, void *conn, unsigned char *data, size_t len,
                                       size_t &res_len) = 0;

    /**
     * @brief 关闭连接
     * @param[in] client 客户端对象指针
     * @param[in] conn 连接对象指针
     */
    virtual void CloseConn(void *client, void *conn) = 0;

    /**
     * @brief 销毁QUIC客户端
     * @param[in] client 客户端对象指针
     */
    virtual void DestroyClient(void *client) = 0;

    /**
     * @brief 获取连接对端信息
     * @param[in] conn 连接对象指针
     * @param[out] address 对端地址
     * @param[out] port 对端端口
     * @return 获取成功返回true，失败返回false
     */
    virtual bool GetPeerInfo(void *conn, std::string &address, int &port) = 0;
};
} // namespace Fleet::DataManager::Quic

#endif // FLEET_DATA_MANAGER_QUIC_QUIC_SERVICE_H
