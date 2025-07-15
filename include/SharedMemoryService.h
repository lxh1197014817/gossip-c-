// Copyright (c) 2025 Institute of Software, Chinese Academy of Sciences
// Author: Zhen Tang <tangzhen12@otcaix.iscas.ac.cn>
// Affiliation: Institute of Software, Chinese Academy of Sciences

/**
 * @file SharedMemoryService.h
 * @brief 共享内存服务接口定义
 * @details 提供基于共享内存的高性能进程间通信服务，支持消息传递和数据共享
 * @author 唐震 <tangzhen12@otcaix.iscas.ac.cn>
 * @date 2025-07-02
 */

#ifndef FLEET_DATA_MANAGER_SHARED_MEMORY_SERVICE_H
#define FLEET_DATA_MANAGER_SHARED_MEMORY_SERVICE_H

#include <string>
#include <uuid/uuid.h>

namespace Fleet::DataManager::SharedMemory {

/**
 * @brief 共享内存消息回调函数类型定义
 * @param[in] from 发送方UUID
 * @param[in] data 消息数据内容
 * @param[in] size 消息数据大小
 * @param[in] closure 用户自定义上下文数据
 */
typedef void (*Callback)(uuid_t from, const char *data, int size, void *closure);

/**
 * @brief 共享内存服务接口
 * @details 提供基于共享内存的进程间通信功能，支持高性能的消息传递和数据共享
 * @note 此接口为纯虚接口，由具体的共享内存插件实现
 */
class SharedMemoryService {
  public:
    /**
     * @brief 默认构造函数
     */
    SharedMemoryService() = default;

    /**
     * @brief 虚析构函数
     */
    virtual ~SharedMemoryService() = default;

    /**
     * @brief 禁用拷贝构造函数
     */
    SharedMemoryService(const SharedMemoryService &) = delete;

    /**
     * @brief 禁用赋值操作符
     */
    SharedMemoryService &operator=(const SharedMemoryService &) = delete;

    /**
     * @brief 创建共享内存实例
     * @param[in] uuid 共享内存区域的唯一标识符
     * @return 共享内存实例指针，失败返回nullptr
     */
    virtual void *Create(uuid_t uuid) = 0;

    /**
     * @brief 打开已存在的共享内存实例
     * @param[in] uuid 共享内存区域的唯一标识符
     * @return 共享内存实例指针，失败返回nullptr
     */
    virtual void *Open(uuid_t uuid) = 0;

    /**
     * @brief 关闭共享内存实例
     * @param[in] instance 共享内存实例指针
     * @return 关闭成功返回true，失败返回false
     */
    virtual bool Close(void *instance) = 0;

    /**
     * @brief 销毁共享内存实例
     * @param[in] instance 共享内存实例指针
     * @return 销毁成功返回true，失败返回false
     * @note 销毁操作会释放共享内存资源
     */
    virtual bool Destroy(void *instance) = 0;

    /**
     * @brief 启动共享内存通信服务
     * @param[in] uuid 服务的唯一标识符
     * @return 服务实例指针，失败返回nullptr
     */
    virtual void *Start(uuid_t uuid) = 0;

    /**
     * @brief 停止共享内存通信服务
     * @param[in] instance 服务实例指针
     * @return 停止成功返回true，失败返回false
     */
    virtual bool Stop(void *instance) = 0;

    /**
     * @brief 设置消息接收回调函数
     * @param[in] instance 共享内存实例指针
     * @param[in] callback 消息接收回调函数
     * @param[in] closure 用户自定义上下文数据
     * @return 设置成功返回true，失败返回false
     */
    virtual bool ReceiveFrom(void *instance, Callback callback, void *closure) = 0;

    /**
     * @brief 向目标实例发送消息
     * @param[in] myUuid 发送方UUID
     * @param[in] targetInstance 目标实例指针
     * @param[in] data 消息数据内容
     * @param[in] length 消息数据长度
     * @return 发送成功返回true，失败返回false
     */
    virtual bool SendTo(uuid_t myUuid, void *targetInstance, const char *data, int length) = 0;
};
} // namespace Fleet::DataManager::SharedMemory

#endif //FLEET_DATA_MANAGER_SHARED_MEMORY_SERVICE_H
