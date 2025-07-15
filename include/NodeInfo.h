// Copyright (c) 2025 Institute of Software, Chinese Academy of Sciences
// Author: Zhen Tang <tangzhen12@otcaix.iscas.ac.cn>
// Affiliation: Institute of Software, Chinese Academy of Sciences

/**
 * @file NodeInfo.h
 * @brief 网络节点信息管理
 * @details 提供分布式网络中节点的标识和地址信息管理功能
 * @author 唐震 <tangzhen12@otcaix.iscas.ac.cn>
 * @date 2025-07-02
 */

#ifndef FLEET_DATA_MANAGER_MESSAGING_NODE_INFO_H
#define FLEET_DATA_MANAGER_MESSAGING_NODE_INFO_H

#include <string>

namespace Fleet::DataManager::Messaging {

/**
 * @brief 节点信息实体类
 * @details 用于板卡管理功能，存储节点的UUID和IP地址信息，支持动态地址更新
 */
class NodeInfo {
  public:
    /**
     * @brief 构造函数
     * @param uuid 节点UUID
     * @param ipAddress 节点IP地址
     */
    NodeInfo(const std::string &uuid, const std::string &ipAddress)
        : uuid(uuid), ipAddress(ipAddress) {
    }

    /**
     * @brief 析构函数
     */
    virtual ~NodeInfo() = default;

    /**
     * @brief 禁用拷贝构造函数
     */
    NodeInfo(const NodeInfo &) = delete;

    /**
     * @brief 禁用赋值操作符
     */
    NodeInfo &operator=(const NodeInfo &) = delete;

    /**
     * @brief 获取节点UUID
     * @return 节点UUID
     */
    const std::string &GetUuid() const {
        return this->uuid;
    }

    /**
     * @brief 获取节点IP地址
     * @return 节点IP地址
     */
    const std::string &GetIpAddress() const {
        return this->ipAddress;
    }

    /**
     * @brief 更新节点IP地址
     * @param newIpAddress 新的IP地址
     */
    void UpdateAddress(const std::string &newIpAddress) {
        this->ipAddress = newIpAddress;
    }

  private:
    /// 节点唯一标识符，用于在分布式网络中标识节点
    std::string uuid;
    /// 节点IP地址，用于网络通信
    std::string ipAddress;
};

} // namespace Fleet::DataManager::Messaging

#endif // FLEET_DATA_MANAGER_MESSAGING_NODE_INFO_H
