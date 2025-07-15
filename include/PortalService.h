// Copyright (c) 2025 Institute of Software, Chinese Academy of Sciences
// Author: Zhen Tang <tangzhen12@otcaix.iscas.ac.cn>
// Affiliation: Institute of Software, Chinese Academy of Sciences

/**
 * @file PortalService.h
 * @brief 门户服务接口定义
 * @details 提供数据管理、对象存储、消息传输、任务调度等综合性门户服务功能
 * @author 唐震 <tangzhen12@otcaix.iscas.ac.cn>
 * @date 2025-07-02
 */

#ifndef FLEET_DATA_MANAGER_PORTAL_PORTAL_SERVICE_H
#define FLEET_DATA_MANAGER_PORTAL_PORTAL_SERVICE_H

#include "MessagingService.h"
#include <string>
#include <vector>

namespace Fleet::DataManager::Portal {
/**
 * @brief 门户服务接口
 * @details 提供完整的数据管理门户功能，包括数据库操作、数据传输、对象存储、消息总线和任务管理
 * @note 此接口为纯虚接口，由具体的门户插件实现
 */
class PortalService {
  public:
    /**
     * @brief 默认构造函数
     */
    PortalService() = default;

    /**
     * @brief 虚析构函数
     */
    virtual ~PortalService() = default;

    /**
     * @brief 禁用拷贝构造函数
     */
    PortalService(const PortalService &) = delete;

    /**
     * @brief 禁用赋值操作符
     */
    PortalService &operator=(const PortalService &) = delete;

    // ================= 分布式数据库操作 =================

    /**
     * @brief 在多个节点执行SQLite语句
     * @param[in] nodeIdList 目标节点ID列表
     * @param[in] connectionString 数据库连接字符串
     * @param[in] sql SQL语句
     * @return 执行成功返回true，失败返回false
     */
    virtual bool SqliteExecute(const std::vector<std::string> &nodeIdList,
                               const std::string &connectionString, const std::string &sql) = 0;

    /**
     * @brief 在多个节点执行参数化SQLite语句
     * @param[in] nodeIdList 目标节点ID列表
     * @param[in] connectionString 数据库连接字符串
     * @param[in] sql 参数化SQL语句
     * @param[in] types 参数类型列表
     * @param[in] values 参数值列表
     * @return 执行成功返回true，失败返回false
     */
    virtual bool SqliteExecuteParameterized(const std::vector<std::string> &nodeIdList,
                                            const std::string &connectionString,
                                            const std::string &sql,
                                            const std::vector<std::string> &types,
                                            const std::vector<std::string> &values) = 0;

    /**
     * @brief 在多个节点执行PostgreSQL语句
     * @param[in] nodeIdList 目标节点ID列表
     * @param[in] connectionString 数据库连接字符串
     * @param[in] sql SQL语句
     * @return 执行成功返回true，失败返回false
     */
    virtual bool PostgreSqlExecute(const std::vector<std::string> &nodeIdList,
                                   const std::string &connectionString, const std::string &sql) = 0;

    /**
     * @brief 在多个节点执行参数化PostgreSQL语句
     * @param[in] nodeIdList 目标节点ID列表
     * @param[in] connectionString 数据库连接字符串
     * @param[in] sql 参数化SQL语句
     * @param[in] types 参数类型列表
     * @param[in] values 参数值列表
     * @return 执行成功返回true，失败返回false
     */
    virtual bool PostgreSqlExecuteParameterized(const std::vector<std::string> &nodeIdList,
                                                const std::string &connectionString,
                                                const std::string &sql,
                                                const std::vector<std::string> &types,
                                                const std::vector<std::string> &values) = 0;

    // ================= 数据传输操作 =================

    /**
     * @brief 上传数据
     * @details 收到数据保存至特定位置
     * @param [in] dataType 数据类型
     * @param [in] name 数据唯一标识符
     * @param [in] to 应用名称
     * @param [in] data 数据
     * @param [in] length 数据长度
     * @return true: 成功. false: 失败
     * @note 除data外，其他输入不能包含'|'或'\n'
     */
    virtual bool UploadData(const char *dataType, const char *name, const char *to,
                            const char *data, uint64_t length) = 0;
    /**
     * @brief 上传安全数据至自动下传区域
     * @param[in] key 数据唯一标识符
     * @param[in] data 数据
     * @param[in] length 数据长度
     * @return 指示操作是否成功
     */
    virtual bool UploadSecurityDataToAutoDownloadArea(const char *key, const char *data,
                                                      uint64_t length) = 0;
    /**
     * @brief 上传分片数据
     * @param[in] dataType 数据类型
     * @param[in] key 数据唯一标识符
     * @param[in] owner 数据所有者
     * @param[in] currentSegmentId 当前分片编号（从1开始）
     * @param[in] totalSegmentCount 分片总数
     * @param[in] totalSize 数据总大小
     * @param[in] segmentSize 当前分片大小
     * @param[in] segmentData 当前分片数据
     * @return 指示操作是否成功
     */
    virtual bool UploadSegmentData(const char *dataType, const char *key, const char *owner,
                                   uint64_t currentSegmentId, uint64_t totalSegmentCount,
                                   uint64_t totalSize, uint64_t segmentSize,
                                   const char *segmentData) = 0;
    /**
     * @brief 下载数据
     * @details 返回最新匹配数据
     * @param [in] dataType 数据类型
     * @param [in] name 数据唯一标识符
     * @param [in] from 数据来源
     * @param [out] data 数据
     * @return 数据长度
     * @note 函数会自行为data分配内存，请传入空指针，调用后需要释放data
     * @note 若本地无数据，会向其他节点请求数据
     */
    virtual int DownloadData(const char *dataType, const char *name, const char *from,
                             char *&data) = 0;
    /**
     * @brief 查询数据
     * @details 返回匹配数据列表
     * @param [in] dataType 数据类型
     * @param [in] name 数据唯一标识符
     * @param [out] data 数据列表
     * @return 数据长度
     * @note 函数会自行为data分配内存，请传入空指针，调用后需要释放data
     * @note Data format: dataName|node|dataType|dataOwner|dataSize|available|operationType|operationDetail|operationTime\n
     * @note 查询为模糊查询，查询范围包括本地表和全局表，name 为 keyword
     */
    virtual int QueryData(const char *dataType, const char *name, char *&data) = 0;
    /**
     * @brief 同步数据
     * @details 本地数据发到{node}
     * @param [in] dataType 数据类型
     * @param [in] name 数据唯一标识符
     * @param [in] dataOwner 数据所有者
     * @param [in] to 目标节点
     * @return true if success
     */
    virtual bool SyncData(const char *dataType, const char *name, const char *dataOwner,
                          const char *to) = 0;
    /**
     * @brief 备份数据
     * @details 将本地数据打包，添加版本，发送到{node}
     * @param [in] dataType 数据类型
     * @param [in] name 数据唯一标识符
     * @param [in] dataOwner 数据所有者
     * @param [in] version 备份版本
     * @param [in] to 目标节点
     * @return true if success
     */
    virtual bool BackupData(const char *dataType, const char *name, const char *dataOwner,
                            const char *version, const char *to) = 0;
    /**
     * @brief 恢复数据
     * @details 从{node}获取指定版本的备份并恢复至本地
     * @param [in] dataType 数据类型
     * @param [in] from 备份所在节点
     * @param [in] name 数据唯一标识符
     * @param [in] dataOwner Data owner(software or hardware module ....)
     * @param [in] version 备份版本
     * @return true if success
     */
    virtual bool RecoverData(const char *dataType, const char *name, const char *dataOwner,
                             const char *version, const char *from) = 0;
    /**
     * @brief Receive telemetry
     * @details 读取本地的遥测数据，发送至对应topic
     * @param [in] from Data owner(software or hardware module ....)
     * @return true if success
     * @note 遥测数据发送后会被清空
     */
    virtual bool ReceiveTelemetry(const char *from) = 0;
    /**
     * @brief Report telemetry
     * @details 接收遥测数据，存储至本地
     * @param [in] from Data owner(software or hardware module ....)
     * @param [in] data Original data
     * @param [in] length Data length
     * @return true if success
     * @note 遥测数据的数据名及数据类型固定，因此只在本地存储一份，使用完毕后会被清空
     */
    virtual bool ReportTelemetry(const char *from, const char *data, uint64_t length) = 0;
    /**
     * @brief Send remote control
     * @details 发送远程控制指令至目标软件或硬件模块
     * @param [in] to Data owner(software or hardware module ....)
     * @param [in] data Original data
     * @param [in] length Data length
     * @return true if success
     */
    virtual bool SendRemoteControl(const char *to, const char *data, uint64_t length) = 0;
    /**
     * @brief Observation order
     * @details 接收观测单数据，存储至本地
     * @param [in] uuid Order uuid
     * @param [in] data Original data
     * @param [in] length Data length
     * @return true if success
     */
    virtual bool ObservationOrder(const char *uuid, const char *data, uint64_t length) = 0;
    /**
     * @brief Processing order
     * @details 接收处理单数据，存储至本地
     * @param [in] uuid Order uuid
     * @param [in] data Original data
     * @param [in] length Data length
     * @return true if success
     */
    virtual bool ProcessingOrder(const char *uuid, const char *data, uint64_t length) = 0;
    /**
     * @brief Dispatching order
     * @details 接收调度单数据，存储至本地
     * @param [in] uuid Order uuid
     * @param [in] data Original data
     * @param [in] length Data length
     * @return true if success
     */
    virtual bool DispatchingOrder(const char *uuid, const char *data, uint64_t length) = 0;
    /**
     * @brief Order status
     * @details 查询任务单状态
     * @param [in] uuid Order uuid
     * @param [out] data Order status
     * @return size > 0, < 0 means error
     * @note 函数会自行为data分配内存，请传入空指针，调用后需要释放data
     */
    virtual int OrderStatus(const char *uuid, char *&data) = 0;
    /**
     * @brief Order result
     * @details 查询任务单结果
     * @param [in] uuid Order uuid
     * @param [out] data Order result
     * @return size > 0, < 0 means error
     * @note 函数会自行为data分配内存，请传入空指针，调用后需要释放data
     */
    virtual int OrderResult(const char *uuid, char *&data) = 0;
    virtual void InitializeDataService() = 0;

    /**
     * @brief  产品数据入库
     
     * @param[in]   name       产品唯一标识符
     * @param[in]   dataOwner  应用名称
     * @param[in]   data       数据
     * @param[in]   length     数据长度
     * @return    true: 成功. false: 失败
     */
    virtual bool ProductDataStore(const char *name, const char *dataOwner, const char *data,
                                  uint64_t length) = 0;

     /**
     * @brief  产品映射关系解析
     
     * @param[in]   data    json 文件内容
     * @param[in]   length    json 长度
     * @param[in]   dataOwner   应用名称
     * @return      true: 成功. false: 失败
     * @note       解析结果可以通过QueryProductFilename接口查询
     */
    // json 文件暂时默认为 Key:value的string格式
    virtual bool ProductAnalysis(const char *data, int length, const char *dataOwner) = 0;
  
    /**
     * @brief     查询产品映射关系
     * @param[in]   ProductName    产品唯一标识符
     * @param[in]   dataOwner      应用名称
     * @param[out]  fileName       对应的文件名称
     * @return    文件名称长度
     * @note     函数会自行为fileName分配内存，请传入空指针，调用后需要释放fileName
     */
    virtual int QueryProductFilename(const char *ProductName, const char *dataOwner,
                                     char *&fileName) = 0;
    /**
     * @brief Upload object
     * @param[in] name Data name
     * @param[in] dataOwner Data owner(software or hardware module ....)
     * @param[in] data Original data
     * @param[in] dataLength Data length
     * @param[in] metadata object metadata data (json format)
     * @param[in] metadataLength object metadata length
     * @return true if success
     */
    virtual bool UploadObject(const char *name, const char *dataOwner, const char *data,
                              uint64_t dataLength, const char *metadata,
                              uint64_t metadataLength) = 0;
    /**
     * @brief sync object
     * @param [in] name Data name
     * @param [in] dataOwner Data owner(software or hardware module ....)
     * @param [in] to  the node  send object to
     * @return true if success
     */
    virtual bool SyncObject(const char *name, const char *dataOwner, const char *to) = 0;
    /**
     * @brief Download object
     * @param [in] name object name
     * @param [in] dataOwner  object owner(software or hardware module ....)
     * @param [out] data object 
     * @return size > 0, < 0 means error
     * @note 函数会自行为data分配内存，请传入空指针，调用后需要释放data
     */
    virtual int DownloadObject(const char *name, const char *dataOwner, char *&data) = 0;
    /**
     * @brief 上传分片对象
     * @param[in] key 数据唯一标识符
     * @param[in] owner 数据所有者
     * @param[in] currentSegmentId 当前分片编号（从1开始）
     * @param[in] totalSegmentCount 分片总数
     * @param[in] totalSize 数据总大小
     * @param[in] segmentSize 当前分片大小
     * @param[in] segmentData 当前分片数据
     * @return 指示操作是否成功
     */
    virtual bool UploadSegmentObject(const char *key, const char *owner, uint64_t currentSegmentId,
                                     uint64_t totalSegmentCount, uint64_t totalSize,
                                     uint64_t segmentSize, const char *segmentData) = 0;
    /**
     * @brief     根据元数据查找对象数据
     * @param[in]   dataOwner  对象所有者
     * @param[in]   timeStamp  对象元数据时间戳字段
     * @param[in]   taskId     对象元数据任务id字段
     * @param[in]   status     对象状态id字段
     * @param[out]   data      对象数据
     * @return size > 0, < 0 means error
     * @note 函数会自行为data分配内存，请传入空指针，调用后需要释放data   
     */
    virtual int QueryObjectUseMeta(const char *dataOwner, const char *timeStamp, const char *taskId,
                                   const char *status, char *&data) = 0;
    /**
     * @brief     Get the Object Metadata object
     * @details   获取对象指定元数据字段的值
     * @param[in]   name       对象名字
     * @param[in]   dataOwner  对象所有者
     * @param[in]   metaName   对象元数据字段名称
     * @param[out]  metaValue  对象元数据字段请求对应的值
     * @return size > 0, < 0 means error
     * @note 函数会自行为metaValue分配内存，请传入空指针，调用后需要释放metaValue  
     */
    virtual int GetObjectMetadata(const char *name, const char *dataOwner, const char *metaName,
                                  char *&metaValue) = 0;
    /**
     * @brief     Set the Object Metadata object
     * @details   设置对象指定元数据字段的值
     * @param[in]   name       对象名字
     * @param[in]   dataOwner  对象所有者
     * @param[in]   metaName   对象元数据字段名称
     * @param[in]   metaValue  对象元数据字段对应的值
     * @return  true: 成功. false: 失败
     */
    virtual bool SetObjectMetadata(const char *name, const char *dataOwner, const char *metaName,
                                   const char *metaValue) = 0;
    /**
     * @brief     Set the Object Metadata object
     * @details   删除对象指定元数据字段的值
     * @param[in]   name       对象名字
     * @param[in]   dataOwner  对象所有者
     * @param[in]   metaName   对象元数据字段名称
     * @return   true: 成功. false: 失败
     */
    virtual bool DeleteObjectMetadata(const char *name, const char *dataOwner,
                                      const char *metaName) = 0;
    /**
     * @brief     Update the Object Metadata object
     * @details   更新对象指定元数据字段的值
     * @param[in]   name       对象名字
     * @param[in]   dataOwner  对象所有者
     * @param[in]   metaName   对象元数据字段名称
     * @param[in]   metaValue  对象元数据字段对应的值
     * @return   true: 成功. false: 失败
     */
    virtual bool UpdateObjectMetadata(const char *name, const char *dataOwner, const char *metaName,
                                      const char *metaValue) = 0;

    /**
     * @brief 星内消息总线封装:发布断点续传消息
     * @details 自动处理大消息的分片传输和断点续传
     * @param[in] topic 消息主题
     * @param[in] data 消息数据
     * @param[in] length 数据长度
     * @return 指示操作是否成功
     */
    virtual bool PublishResumableMessage(const char *topic, const char *data, uint64_t length) = 0;

    /**
     * @brief 星内消息总线封装:订阅断点续传消息
     * @details 自动处理消息分片的接收和合并
     * @param[in] topic 消息主题
     * @param[in] callback 回调函数
     * @return 指示操作是否成功
     */
    virtual bool
    SubscribeResumableMessage(const char *topic,
                              Fleet::DataManager::Messaging::MessageCallback callback) = 0;

    /**
     * @brief 星内消息总线封装:发布可靠消息（失效重传）
     * @details 自动处理消息重传，确保可靠传输
     * @param[in] topic 消息主题
     * @param[in] data 消息数据
     * @param[in] length 数据长度
     * @return 指示操作是否成功
     */
    virtual bool PublishReliableMessage(const char *topic, const char *data, uint64_t length) = 0;

    /**
     * @brief 星内消息总线封装:订阅可靠消息
     * @details 订阅具有重传机制的可靠消息
     * @param[in] topic 消息主题
     * @param[in] callback 回调函数
     * @return 指示操作是否成功
     */
    virtual bool
    SubscribeReliableMessage(const char *topic,
                             Fleet::DataManager::Messaging::MessageCallback callback) = 0;

    /**
     * @brief 星内消息总线封装:发送控制命令
     * @details 专门用于节点间控制命令的传输
     * @param[in] targetNode 目标节点
     * @param[in] command 控制命令数据
     * @param[in] length 命令长度
     * @return 指示操作是否成功
     */
    virtual bool SendControlCommand(const char *targetNode, const char *command,
                                    uint64_t length) = 0;

    /**
     * @brief 星内消息总线封装:订阅控制命令
     * @details 订阅来自其他节点的控制命令
     * @param[in] callback 回调函数
     * @return 指示操作是否成功
     */
    virtual bool
    SubscribeControlCommand(Fleet::DataManager::Messaging::MessageCallback callback) = 0;

    /**
     * @brief 星内消息总线封装:发送状态数据
     * @details 专门用于节点状态信息的传输
     * @param[in] targetNode 目标节点
     * @param[in] statusData 状态数据
     * @param[in] length 数据长度
     * @return 指示操作是否成功
     */
    virtual bool SendStatusData(const char *targetNode, const char *statusData,
                                uint64_t length) = 0;

    /**
     * @brief 星内消息总线封装:订阅状态数据
     * @details 订阅来自其他节点的状态信息
     * @param[in] callback 回调函数
     * @return 指示操作是否成功
     */
    virtual bool SubscribeStatusData(Fleet::DataManager::Messaging::MessageCallback callback) = 0;

    /**
     * @brief 星间消息总线封装:发送任务规划信息
     * @details 专门用于任务规划信息的传输
     * @param[in] targetNode 目标节点
     * @param[in] planningData 任务规划数据
     * @param[in] length 数据长度
     * @return 指示操作是否成功
     */
    virtual bool SendTaskPlanningInfo(const char *targetNode, const char *planningData,
                                      uint64_t length) = 0;

    /**
     * @brief 星间消息总线封装:订阅任务规划信息
     * @details 订阅来自其他节点的任务规划信息
     * @param[in] callback 回调函数
     * @return 指示操作是否成功
     */
    virtual bool
    SubscribeTaskPlanningInfo(Fleet::DataManager::Messaging::MessageCallback callback) = 0;

    /**
     * @brief 星间消息总线封装:发送调度信息
     * @details 专门用于调度信息的传输
     * @param[in] targetNode 目标节点
     * @param[in] schedulingData 调度信息数据
     * @param[in] length 数据长度
     * @return 指示操作是否成功
     */
    virtual bool SendSchedulingInfo(const char *targetNode, const char *schedulingData,
                                    uint64_t length) = 0;

    /**
     * @brief 星间消息总线封装:订阅调度信息
     * @details 订阅来自其他节点的调度信息
     * @param[in] callback 回调函数
     * @return 指示操作是否成功
     */
    virtual bool
    SubscribeSchedulingInfo(Fleet::DataManager::Messaging::MessageCallback callback) = 0;

    /**
     * @brief 星间消息总线封装:发送资源状态信息
     * @details 专门用于资源状态信息的传输
     * @param[in] targetNode 目标节点
     * @param[in] resourceData 资源状态数据
     * @param[in] length 数据长度
     * @return 指示操作是否成功
     */
    virtual bool SendResourceStatus(const char *targetNode, const char *resourceData,
                                    uint64_t length) = 0;

    /**
     * @brief 星间消息总线封装:订阅资源状态信息
     * @details 订阅来自其他节点的资源状态信息
     * @param[in] callback 回调函数
     * @return 指示操作是否成功
     */
    virtual bool
    SubscribeResourceStatus(Fleet::DataManager::Messaging::MessageCallback callback) = 0;

    /**
     * @brief 发布指令
     * @details 向指定主题发布指令消息，基于DoPublish方法封装
     * @param [in] topic 指令主题
     * @param [in] command 指令数据
     * @param [in] length 指令数据长度
     * @return true if success
     */
    virtual bool PublishCommand(const char *topic, const char *command, int length) = 0;

    /**
     * @brief 订阅指令
     * @details 订阅指定主题的指令消息，基于DoSubscribe方法封装
     * @param [in] topic 指令主题
     * @param [in] callback 消息回调函数
     * @return true if success
     */
    virtual bool SubscribeCommand(const char *topic, Messaging::MessageCallback callback) = 0;

    /**
     * @brief 请求指令
     * @details 向指定主题发送指令请求消息，基于MessagingService的Request方法封装
     * @param [in] topic 请求主题
     * @param [in] request 请求数据
     * @param [in] length 请求数据长度
     * @param [in] responseCallback 响应回调函数
     * @param [in] closure 回调函数上下文
     * @return true if success
     */
    virtual bool RequestCommand(const char *topic, const char *request, int length,
                                Messaging::MessageCallback responseCallback, void *closure) = 0;

    /**
     * @brief 回复指令
     * @details 向指定UUID发送指令回复消息，基于MessagingService的Reply方法封装
     * @param [in] uuid 请求消息的UUID
     * @param [in] reply 回复数据
     * @param [in] length 回复数据长度
     * @return true if success
     */
    virtual bool ReplyCommand(const char *uuid, const char *reply, int length) = 0;

    /**
     * @brief 发布状态信息
     * @details 向指定主题发布状态信息，基于DoPublish方法封装
     * @param [in] topic 状态信息主题
     * @param [in] statusInfo 状态信息数据
     * @param [in] length 状态信息数据长度
     * @return true if success
     */
    virtual bool PublishStatusInfo(const char *topic, const char *statusInfo, int length) = 0;

    /**
     * @brief 订阅状态信息
     * @details 订阅指定主题的状态信息，基于DoSubscribe方法封装
     * @param [in] topic 状态信息主题
     * @param [in] callback 消息回调函数
     * @return true if success
     */
    virtual bool SubscribeStatusInfo(const char *topic, Messaging::MessageCallback callback) = 0;

    /**
     * @brief 请求状态信息
     * @details 向指定主题发送状态信息请求消息，基于MessagingService的Request方法封装
     * @param [in] topic 请求主题
     * @param [in] request 请求数据
     * @param [in] length 请求数据长度
     * @param [in] responseCallback 响应回调函数
     * @param [in] closure 回调函数上下文
     * @return true if success
     */
    virtual bool RequestStatusInfo(const char *topic, const char *request, int length,
                                   Messaging::MessageCallback responseCallback, void *closure) = 0;

    /**
     * @brief 回复状态信息
     * @details 向指定UUID发送状态信息回复消息，基于MessagingService的Reply方法封装
     * @param [in] uuid 请求消息的UUID
     * @param [in] reply 回复数据
     * @param [in] length 回复数据长度
     * @return true if success
     */
    virtual bool ReplyStatusInfo(const char *uuid, const char *reply, int length) = 0;

    /**
     * @brief 发布小文件
     * @details 向指定主题发布小文件数据，基于DoPublish方法封装
     * @param [in] topic 小文件主题
     * @param [in] fileData 文件数据
     * @param [in] length 文件数据长度
     * @return true if success
     */
    virtual bool PublishSmallFile(const char *topic, const char *fileData, int length) = 0;

    /**
     * @brief 订阅小文件
     * @details 订阅指定主题的小文件，基于DoSubscribe方法封装
     * @param [in] topic 小文件主题
     * @param [in] callback 消息回调函数
     * @return true if success
     */
    virtual bool SubscribeSmallFile(const char *topic, Messaging::MessageCallback callback) = 0;

    /**
     * @brief 请求小文件
     * @details 向指定主题发送小文件请求消息，基于MessagingService的Request方法封装
     * @param [in] topic 请求主题
     * @param [in] request 请求数据
     * @param [in] length 请求数据长度
     * @param [in] responseCallback 响应回调函数
     * @param [in] closure 回调函数上下文
     * @return true if success
     */
    virtual bool RequestSmallFile(const char *topic, const char *request, int length,
                                  Messaging::MessageCallback responseCallback, void *closure) = 0;

    /**
     * @brief 回复小文件
     * @details 向指定UUID发送小文件回复消息，基于MessagingService的Reply方法封装
     * @param [in] uuid 请求消息的UUID
     * @param [in] reply 回复数据
     * @param [in] length 回复数据长度
     * @return true if success
     */
    virtual bool ReplySmallFile(const char *uuid, const char *reply, int length) = 0;
};
} // namespace Fleet::DataManager::Portal
#endif // FLEET_DATA_MANAGER_PORTAL_PORTAL_SERVICE_H