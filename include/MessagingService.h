/**
 * @file MessagingService.h
 * @brief 消息协同插件服务接口定义
 * @details 提供多协议消息传输、发布订阅、节点管理等分布式通信功能
 * @author 唐震 <tangzhen12@otcaix.iscas.ac.cn>
 * @date 2025-07-02
 */

#ifndef FLEET_DATA_MANAGER_MESSAGING_SERVICE_H
#define FLEET_DATA_MANAGER_MESSAGING_SERVICE_H

#include "NodeInfo.h"
#include <memory>
#include <string>
#include <vector>

namespace Fleet::DataManager::Messaging {
/**
 * @brief 消息回调函数类型定义
 * @param[in] topic 消息主题
 * @param[in] uuid 消息唯一标识符
 * @param[in] size 消息数据大小
 * @param[in] data 消息数据内容
 * @param[in] closure 用户自定义上下文数据
 */
typedef void (*MessageCallback)(const char *topic, const char *uuid, int size, const char *data,
                                void *closure);

/**
 * @brief 消息协同服务接口
 * @details 提供TCP、UDP、QUIC和共享内存等多种传输协议的消息通信服务，支持发布订阅模式和节点管理
 * @note 此接口为纯虚接口，由具体的消息插件实现
 */
class MessagingService {
  public:
    /**
     * @brief 默认构造函数
     */
    MessagingService() = default;

    /**
     * @brief 虚析构函数
     */
    virtual ~MessagingService() = default;

    /**
     * @brief 禁用拷贝构造函数
     */
    MessagingService(const MessagingService &) = delete;

    /**
     * @brief 禁用赋值操作符
     */
    MessagingService &operator=(const MessagingService &) = delete;

    /**
     * @brief 启动TCP消息服务
     * @param[in] address 监听地址
     * @param[in] port 监听端口
     * @return 启动成功返回true，失败返回false
     */
    virtual bool StartTcp(const std::string &address, int port) = 0;

    /**
     * @brief 获取TCP服务端口号
     * @return TCP服务端口号
     */
    virtual int GetTcpPort() = 0;

    /**
     * @brief 停止TCP消息服务
     * @return 停止成功返回true，失败返回false
     */
    virtual bool StopTcp() = 0;

    /**
     * @brief 启动UDP消息服务
     * @param[in] address 监听地址
     * @param[in] port 监听端口
     * @return 启动成功返回true，失败返回false
     */
    virtual bool StartUdp(const std::string &address, int port) = 0;

    /**
     * @brief 获取UDP服务端口号
     * @return UDP服务端口号
     */
    virtual int GetUdpPort() = 0;

    /**
     * @brief 停止UDP消息服务
     * @return 停止成功返回true，失败返回false
     */
    virtual bool StopUdp() = 0;

    /**
     * @brief 启动QUIC消息服务
     * @param[in] address 监听地址
     * @param[in] port 监听端口
     * @return 启动成功返回true，失败返回false
     */
    virtual bool StartXQuic(const std::string &address, int port) = 0;

    /**
     * @brief 获取QUIC服务端口号
     * @return QUIC服务端口号
     */
    virtual int GetXQuicPort() = 0;

    /**
     * @brief 停止QUIC消息服务
     * @return 停止成功返回true，失败返回false
     */
    virtual bool StopXQuic() = 0;

    /**
     * @brief 启动共享内存消息服务
     * @return 启动成功返回true，失败返回false
     */
    virtual bool StartSharedMemory() = 0;

    /**
     * @brief 停止共享内存消息服务
     * @return 停止成功返回true，失败返回false
     */
    virtual bool StopSharedMemory() = 0;
    /**
     * @brief 加入消息网络
     * @param[in] id 节点标识符
     * @param[in] address 节点地址
     * @param[in] port 节点端口
     * @return 加入成功返回true，失败返回false
     */
    virtual bool Join(const std::string &id, const std::string &address, int port) = 0;

    /**
     * @brief 离开消息网络
     * @param[in] id 节点标识符
     * @return 离开成功返回true，失败返回false
     */
    virtual bool Leave(const std::string &id) = 0;

    /**
     * @brief 发布消息到指定主题
     * @param[in] topic 消息主题
     * @param[in] size 消息数据大小
     * @param[in] data 消息数据内容
     * @return 发布成功返回true，失败返回false
     */
    virtual bool Publish(const std::string &topic, int size, const char *data) = 0;

    /**
     * @brief 订阅指定主题的消息
     * @param[in] topic 消息主题
     * @param[in] messageCallback 消息回调函数
     * @param[in] closure 用户自定义上下文数据
     * @return 订阅成功返回true，失败返回false
     */
    virtual bool Subscribe(const std::string &topic, MessageCallback messageCallback,
                           void *closure) = 0;

    /**
     * @brief 取消订阅指定主题
     * @param[in] topic 消息主题
     * @param[in] messageCallback 消息回调函数
     * @return 取消成功返回true，失败返回false
     */
    virtual bool Unsubscribe(const std::string &topic, MessageCallback messageCallback) = 0;

    /**
     * @brief 发送请求消息并等待响应
     * @param[in] topic 请求主题
     * @param[in] requestSize 请求数据大小
     * @param[in] requestData 请求数据内容
     * @param[in] responseCallback 响应回调函数
     * @param[in] closure 用户自定义上下文数据
     * @return 发送成功返回true，失败返回false
     */
    virtual bool Request(const std::string &topic, int requestSize, const char *requestData,
                         MessageCallback responseCallback, void *closure) = 0;

    /**
     * @brief 回复请求消息
     * @param[in] uuid 请求消息的唯一标识符
     * @param[in] responseSize 响应数据大小
     * @param[in] responseData 响应数据内容
     * @return 回复成功返回true，失败返回false
     */
    virtual bool Reply(const std::string &uuid, int responseSize, const char *responseData) = 0;

    /**
     * @brief 取消指定的请求
     * @details 清理pendingRequests中的超时请求，取消相关订阅，释放资源
     * @param[in] uuid 请求消息的唯一标识符
     * @return 取消成功返回true，未找到请求或取消失败返回false
     */
    virtual bool CancelRequest(const std::string &uuid) = 0;

    /**
     * @brief 存储主题信息
     * @param[in] topic 主题名称
     * @return 存储成功返回true，失败返回false
     */
    virtual bool StoreTopic(const std::string &topic) = 0;

    /**
     * @brief 查询主题是否存在
     * @param[in] topic 主题名称
     * @return 存在返回true，不存在返回false
     */
    virtual bool QueryTopic(const std::string &topic) = 0;

    /**
     * @brief 删除主题
     * @param[in] topic 主题名称
     * @return 删除成功返回true，失败返回false
     */
    virtual bool DeleteTopic(const std::string &topic) = 0;

    /**
     * @brief 创建网络节点
     * @param[in] uuid 节点UUID
     * @param[in] ipAddress 节点IP地址
     * @return 创建成功返回true，失败返回false
     */
    virtual bool CreateNode(const std::string &uuid, const std::string &ipAddress) = 0;

    /**
     * @brief 更新节点信息
     * @param[in] uuid 节点UUID
     * @param[in] newIpAddress 新的IP地址
     * @return 更新成功返回true，失败返回false
     */
    virtual bool UpdateNode(const std::string &uuid, const std::string &newIpAddress) = 0;

    /**
     * @brief 删除网络节点
     * @param[in] uuid 节点UUID
     * @return 删除成功返回true，失败返回false
     */
    virtual bool RemoveNode(const std::string &uuid) = 0;

    /**
     * @brief 根据UUID获取节点信息
     * @param[in] uuid 节点UUID
     * @return 节点信息对象指针，未找到返回nullptr
     */
    virtual std::shared_ptr<NodeInfo> GetNodeByUuid(const std::string &uuid) = 0;

    /**
     * @brief 根据IP地址获取节点信息
     * @param[in] ipAddress 节点IP地址
     * @return 节点信息对象指针，未找到返回nullptr
     */
    virtual std::shared_ptr<NodeInfo> GetNodeByIpAddress(const std::string &ipAddress) = 0;

    /**
     * @brief 获取所有节点信息列表
     * @return 节点信息对象指针列表
     */
    virtual std::vector<std::shared_ptr<NodeInfo>> GetAllNodes() = 0;

    /**
     * @brief 检查节点是否存在
     * @param[in] uuid 节点UUID
     * @return 存在返回true，不存在返回false
     */
    virtual bool NodeExists(const std::string &uuid) = 0;

    /**
     * @brief 获取配置值
     * @details 从分布式键值存储系统中获取指定键的配置值，使用LWW算法确保最终一致性
     * @param[in] key 配置键
     * @param[out] value 配置值指针，函数会分配内存存储配置值
     * @return 配置值长度，< 0 表示错误（键不存在或已被删除）
     * @note 该函数会收集所有相关的ConfigOperation消息，使用HLC时间戳进行LWW选择
     */
    virtual int GetConfig(const std::string &key, char **value) = 0;

    /**
     * @brief 设置配置值
     * @details 在分布式键值存储系统中设置指定键的配置值，自动分配HLC时间戳并广播到集群
     * @param[in] key 配置键
     * @param[in] value 配置值
     * @param[in] length 配置值长度
     * @return 设置成功返回true，失败返回false
     * @note 该函数会创建ConfigOperation消息并通过MessagingService发布到集群
     */
    virtual bool PutConfig(const std::string &key, const std::string &value, int length) = 0;

    /**
     * @brief 删除配置
     * @details 从分布式键值存储系统中删除指定键的配置，自动分配HLC时间戳并广播到集群
     * @param[in] key 配置键
     * @return 删除成功返回true，失败返回false
     * @note 该函数会创建DELETE类型的ConfigOperation消息并发布到集群
     */
    virtual bool RemoveConfig(const std::string &key) = 0;

    /**
     * @brief 设置HLC时钟偏差（用于测试）
     * @details 设置节点的HLC时钟偏差，用于模拟分布式环境中的时钟不同步
     * @param[in] offsetMs 时钟偏差（毫秒），正数表示时钟快，负数表示时钟慢
     * @return 设置成功返回true，失败返回false
     * @note 仅用于测试目的，生产环境不应使用
     */
    virtual bool SetClockOffset(int64_t offsetMs) = 0;

    /**
     * @brief 获取当前HLC时钟偏差
     * @return 当前时钟偏差（毫秒）
     */
    virtual int64_t GetClockOffset() = 0;
};
} // namespace Fleet::DataManager::Messaging
#endif // FLEET_DATA_MANAGER_MESSAGING_SERVICE_H