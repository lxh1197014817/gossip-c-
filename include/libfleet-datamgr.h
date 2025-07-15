/**
 * @file libfleet-datamgr.h
 * @brief Fleet数据管理器C接口定义
 * @details 提供完整的C语言接口，供Rust和其他语言调用，包括插件管理、消息传输、数据存储等功能
 * @author 唐震 <tangzhen12@otcaix.iscas.ac.cn>
 * @date 2025-07-02
 */

#ifndef LIBFLEET_DATAMGR_H
#define LIBFLEET_DATAMGR_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

/**
 * @brief 使用随机UUID作为节点唯一标识符，并使用默认存储路径，创建插件管理器
 * @return 插件管理器实例指针
 */
void *NewPluginManager();

/**
 * @brief 使用指定UUID作为节点唯一标识符创建插件管理器
 * @param[in] id 节点唯一标识符，一个UUID字符串
 * @return 插件管理器实例指针
 */
void *NewPluginManagerByUuid(const char *id);

/**
 * @brief 使用指定数据存储路径创建插件管理器
 * @param[in] directory 数据存储路径
 * @return 插件管理器实例指针
 */
void *NewPluginManagerByDirectory(const char *directory);

/**
 * @brief 使用指定UUID作为节点唯一标识符，并使用指定数据存储路径创建插件管理器
 * @param[in] id 节点唯一标识符，一个UUID字符串
 * @param[in] directory 数据存储路径
 * @return 插件管理器实例指针
 */
void *NewPluginManagerByUuidAndDirectory(const char *id, const char *directory);

/**
 * @brief 销毁插件管理器
 * @param[in] pluginManager 插件管理器实例指针
 */
void DeletePluginManager(void *pluginManager);

/**
 * @brief 获取节点唯一标识符
 * @param[in] pluginManager 插件管理器实例指针
 * @return 节点唯一标识符，一个UUID字符串
 */
const char *GetNodeId(void *pluginManager);

/**
 * @brief 设置参数
 * @param[in] pluginManager 插件管理器实例指针
 * @param[in] key 键
 * @param[in] value 值
 */
void SetParameter(void *pluginManager, const char *key, const char *value);

/**
 * @brief 根据参数设置加载插件
 * @param[in] pluginManager 插件管理器实例指针
 */
void LoadPlugins(void *pluginManager);

/**
 * @brief 卸载所有插件
 * @param[in] pluginManager 插件管理器实例指针
 */
void UnloadPlugins(void *pluginManager);

/**
 * @brief 在随机端口上启动API Server
 * @param[in] pluginManager 插件管理器实例指针
 * @param[in] address 绑定地址
 * @return 指示操作是否成功
 */
int StartApiServer(void *pluginManager, const char *address);

/**
 * @brief 在指定端口上启动API Server
 * @param[in] pluginManager 插件管理器实例指针
 * @param[in] address 绑定地址
 * @param[in] port 端口
 * @return 指示操作是否成功
 */
int StartApiServerByPort(void *pluginManager, const char *address, int port);

/**
 * @brief 关闭API Server
 * @param[in] pluginManager 插件管理器实例指针
 * @return 指示操作是否成功
 */
int StopApiServer(void *pluginManager);

/**
 * @brief 获取API Server的监听端口号
 * @param[in] pluginManager 插件管理器实例指针
 * @return API Server的监听端口号
 */
int GetApiPort(void *pluginManager);

/**
 * @brief 消息处理回调函数
 * @param[in] topic 主题
 * @param[in] uuid 消息唯一标识符
 * @param[in] size 消息长度
 * @param[in] data 消息内容
 * @param[in] closure 上下文数据
 */
typedef void (*MessageCallback)(const char *topic, const char *uuid, int size, const char *data,
                                void *closure);

/**
 * @brief 在随机端口使用TCP协议启动消息协同服务
 * @param[in] pluginManager 插件管理器实例指针
 * @param[in] address 需要监听的本地IP地址
 */
void StartTcp(void *pluginManager, const char *address);

/**
 * @brief 在指定端口使用TCP协议启动消息协同服务
 * @param[in] pluginManager 插件管理器实例指针
 * @param[in] address 需要监听的本地IP地址
 * @param[in] port 端口
 */
void StartTcpByPort(void *pluginManager, const char *address, int port);

/**
 * @brief 停止使用TCP协议启动的消息协同服务
 * @param[in] pluginManager 插件管理器实例指针
 */
void StopTcp(void *pluginManager);

/**
 * @brief 获取TCP协议端口号
 * @param[in] pluginManager 插件管理器实例指针
 * @return TCP协议端口号
 */
int GetTcpPort(void *pluginManager);

/**
 * @brief 在随机端口使用UDP协议启动消息协同服务
 * @param[in] pluginManager 插件管理器实例指针
 * @param[in] address 需要监听的本地IP地址
 */
void StartUdp(void *pluginManager, const char *address);

/**
 * @brief 在指定端口使用UDP协议启动消息协同服务
 * @param[in] pluginManager 插件管理器实例指针
 * @param[in] address 需要监听的本地IP地址
 * @param[in] port 端口
 */
void StartUdpByPort(void *pluginManager, const char *address, int port);

/**
 * @brief 停止使用UDP协议启动的消息协同服务
 * @param[in] pluginManager 插件管理器实例指针
 */
void StopUdp(void *pluginManager);

/**
 * @brief 获取UDP协议端口号
 * @param[in] pluginManager 插件管理器实例指针
 * @return UDP协议端口号
 */
int GetUdpPort(void *pluginManager);

/**
 * @brief 在随机端口使用QUIC协议启动消息协同服务
 * @param[in] pluginManager 插件管理器实例指针
 * @param[in] address 需要监听的本地IP地址
 * @return 指示操作是否成功
 */
int StartXQuic(void *pluginManager, const char *address);

/**
 * @brief 在指定端口使用QUIC协议启动消息协同服务
 * @param[in] pluginManager 插件管理器实例指针
 * @param[in] address 需要监听的本地IP地址
 * @param[in] port 监听端口号
 * @return 指示操作是否成功
 */
int StartXQuicByPort(void *pluginManager, const char *address, int port);

/**
 * @brief 停止使用QUIC协议启动的消息协同服务
 * @param[in] pluginManager 插件管理器实例指针
 */
void StopXQuic(void *pluginManager);

/**
 * @brief 获取QUIC协议端口号
 * @param pluginManager 插件管理器实例指针
 * @return QUIC协议端口号
 */
int GetXQuicPort(void *pluginManager);

/**
 * @brief 使用共享内存传输方式启动消息协同服务
 * @param[in] pluginManager 插件管理器实例指针
 */
void StartSharedMemory(void *pluginManager);

/**
 * @brief 停止使用共享内存传输方式启动的消息协同服务
 * @param[in] pluginManager 插件管理器实例指针
 */
void StopSharedMemory(void *pluginManager);

/**
 * @brief 在网络拓扑中建立当前节点到目标节点的边
 * @param[in] pluginManager 插件管理器实例指针
 * @param[in] id 节点唯一标识符，一个UUID字符串
 * @param[in] address 节点IP地址
 * @param[in] port 节点端口号
 */
void Join(void *pluginManager, const char *id, const char *address, int port);

/**
 * @brief 在网络拓扑中删除当前节点到目标节点的边
 * @param pluginManager 插件管理器实例指针
 * @param id 节点唯一标识符，一个UUID字符串
 */
void Leave(void *pluginManager, const char *id);

/**
 * @brief 向指定主题发布消息
 * @param[in] pluginManager 插件管理器实例指针
 * @param[in] topic 主题
 * @param[in] size 消息长度
 * @param[in] data 消息
 * @return 指示操作是否成功
 */
int Publish(void *pluginManager, const char *topic, int size, const char *data);

/**
 * @brief 订阅指定主题
 * @param[in] pluginManager 插件管理器实例指针
 * @param[in] topic 主题
 * @param[in] messageCallback 消息处理回调函数
 * @param[in] closure 传递给消息处理回调函数的上下文信息
 * @return 指示操作是否成功
 */
int Subscribe(void *pluginManager, const char *topic, MessageCallback messageCallback,
              void *closure);

/**
 * @brief 取消订阅指定主题
 * @param[in] pluginManager 插件管理器实例指针
 * @param[in] topic 主题
 * @param[in] messageCallback 消息处理回调函数
 * @return 指示操作是否成功
 */
int Unsubscribe(void *pluginManager, const char *topic, MessageCallback messageCallback);

/**
 * @brief 向指定主题发送请求并等待响应
 * @param[in] pluginManager 插件管理器实例指针
 * @param[in] topic 主题
 * @param[in] requestSize 请求长度
 * @param[in] requestData 请求数据
 * @param[in] responseCallback 消息处理回调函数，收到响应时会被调用
 * @param[in] closure 传递给消息处理回调函数的上下文信息
 * @return 指示操作是否成功
 */
int Request(void *pluginManager, const char *topic, int requestSize, const char *requestData,
            MessageCallback responseCallback, void *closure);

/**
 * @brief 向指定请求回复响应数据
 * @param[in] pluginManager 插件管理器实例指针
 * @param[in] uuid 消息唯一标识符
 * @param[in] responseSize 响应长度
 * @param[in] responseData 响应数据
 * @return 指示操作是否成功
 */
int Reply(void *pluginManager, const char *uuid, int responseSize, const char *responseData);

/**
 * @brief 取消指定的请求
 * @param[in] pluginManager 插件管理器实例指针
 * @param[in] uuid 请求消息的唯一标识符
 * @return 1表示成功，0表示失败
 */
int CancelRequest(void *pluginManager, const char *uuid);

/**
 * @brief 数据库查询结果
 */
struct DbResponse {
    /**
     * @brief 行数
     */
    int RowCount;
    /**
     * @brief 列数
     */
    int ColumnCount;
    /**
     * @brief 列名
     */
    char **ColumnNames;
    /**
     * @brief 数据
     */
    char ***Data;
};

/**
 * @brief 创建数据库查询结果
 * @return 空的数据库查询结果
 */
struct DbResponse *CreateDbResponse();

/**
 * @brief 回收数据库查询结果
 * @param[in] response 数据库查询结果
 */
void FreeDbResponse(struct DbResponse *response);

/**
 * @brief 在SQLite数据库上执行返回结果数据的SQL语句
 * @param[in] pluginManager 插件管理器实例指针
 * @param[in] path 数据库文件路径
 * @param[in] sql SQL语句
 * @param[out] response 返回结果
 * @return 指示操作是否成功
 */
int SqliteExecuteResult(void *pluginManager, const char *path, const char *sql,
                        struct DbResponse *response);

/**
 * @brief 在SQLite数据库上执行无需返回结果数据的SQL语句
 * @param[in] pluginManager 插件管理器实例指针
 * @param[in] path 数据库文件路径
 * @param[in] sql SQL语句
 * @return 指示操作是否成功
 */
int SqliteExecuteNoResult(void *pluginManager, const char *path, const char *sql);

/**
 * @brief 在SQLite数据库上执行返回结果数据的参数化SQL语句
 * @param[in] pluginManager 插件管理器实例指针
 * @param[in] path 数据库文件路径
 * @param[in] sql SQL语句
 * @param[in] parameterCount 参数数量
 * @param[in] parameterTypes 参数类型
 * @param[in] parameterValues 参数值
 * @param[out] response 返回结果
 * @return 指示操作是否成功
 */
int SqliteExecuteParameterizedResult(void *pluginManager, const char *path, const char *sql,
                                     int parameterCount, const char **parameterTypes,
                                     const char **parameterValues, struct DbResponse *response);

/**
 * @brief 在SQLite数据库上执行无需返回结果数据的参数化SQL语句
 * @param[in] pluginManager 插件管理器实例指针
 * @param[in] path 数据库文件路径
 * @param[in] sql SQL语句
 * @param[in] parameterCount 参数数量
 * @param[in] parameterTypes 参数类型
 * @param[in] parameterValues 参数值
 * @return 指示操作是否成功
 */
int SqliteExecuteParameterizedNoResult(void *pluginManager, const char *path, const char *sql,
                                       int parameterCount, const char **parameterTypes,
                                       const char **parameterValues);

/**
 * @brief 在PostgreSQL数据库上执行返回结果数据的SQL语句
 * @param[in] pluginManager 插件管理器实例指针
 * @param[in] connectionString 数据库连接字符串
 * @param[in] sql SQL语句
 * @param[out] response 返回结果
 * @return 指示操作是否成功
 */
int PostgreSqlExecuteResult(void *pluginManager, const char *connectionString, const char *sql,
                            struct DbResponse *response);

/**
 * @brief 在PostgreSQL数据库上执行无需返回结果数据的SQL语句
 * @param[in] pluginManager 插件管理器实例指针
 * @param[in] connectionString 数据库连接字符串
 * @param[in] sql SQL语句
 * @return 指示操作是否成功
 */
int PostgreSqlExecuteNoResult(void *pluginManager, const char *connectionString, const char *sql);

/**
 * @brief 在PostgreSQL数据库上执行返回结果数据的SQL语句
 * @param[in] pluginManager 插件管理器实例指针
 * @param[in] connectionString 数据库连接字符串
 * @param[in] sql SQL语句
 * @param[in] parameterCount 参数数量
 * @param[in] parameterTypes 参数类型
 * @param[in] parameterValues 参数值
 * @param[out] response 返回结果
 * @return 指示操作是否成功
 */
int PostgreSqlExecuteParameterizedResult(void *pluginManager, const char *connectionString,
                                         const char *sql, int parameterCount,
                                         const char **parameterTypes, const char **parameterValues,
                                         struct DbResponse *response);

/**
 * @brief 在PostgreSQL数据库上执行无需返回结果数据的SQL语句
 * @param[in] pluginManager 插件管理器实例指针
 * @param[in] connectionString 数据库连接字符串
 * @param[in] sql SQL语句
 * @param[in] parameterCount 参数数量
 * @param[in] parameterTypes 参数类型
 * @param[in] parameterValues 参数值
 * @return 指示操作是否成功
 */
int PostgreSqlExecuteParameterizedNoResult(void *pluginManager, const char *connectionString,
                                           const char *sql, int parameterCount,
                                           const char **parameterTypes,
                                           const char **parameterValues);

/**
 * @brief 元数据
 */
struct DataInfo {
    /**
     * @brief 数据大小
     */
    int size;
    /**
     * @brief 更新时间
     */
    int lastUpdate;
    /**
     * @brief 存储路径数目
     */
    int pathCount;
    /**
     * @brief 存储路径
     */
    char **paths;
};

/**
 * @brief 获取元数据
 * @details 注意使用 FreeDataInfo 回收数据
 * @param[in] pluginManager 插件管理器实例指针
 * @param[in] name 数据唯一标识符
 * @param[in] dataType 数据类型
 * @param[in] owner 数据使用方
 * @return 元数据
 */
struct DataInfo *GetDataInfo(void *pluginManager, const char *name, const char *dataType,
                             const char *owner);

/**
 * @brief 获取指定版本数据的元数据
 * @details 注意使用 FreeDataInfo 回收数据
 * @param[in] pluginManager 插件管理器实例指针
 * @param[in] name 数据唯一标识符
 * @param[in] dataType 数据类型
 * @param[in] owner 数据使用方
 * @param[in] version 数据版本
 * @return 元数据
 */
struct DataInfo *GetDataInfoWithVersion(void *pluginManager, const char *name, const char *dataType,
                                        const char *owner, const char *version);

/**
 * @brief 回收DataInfo数据结构
 * @param[in] dataInfo
 */
void FreeDataInfo(struct DataInfo *dataInfo);

/**
 * @brief 存储介质
 */
struct Device {
    /**
     * @brief 名称
     */
    char *name;
    /**
     * @brief 描述
     */
    char *description;
    /**
     * @brief 设备文件
     */
    char *deviceFile;
    /**
     * @brief 文件系统中的路径
     */
    char *directory;
};

/**
 * @brief 存储介质列表
 */
struct DeviceList {
    /**
     * @brief 元数数量
     */
    unsigned long count;
    /**
     * @brief 存储介质列表
     */
    struct Device **devices;
};

/**
 * @brief 回收存储介质结构体
 * @param[in] device 存储介质
 */
void FreeDevice(struct Device *device);

/**
 * @brief 回收存储介质列表结构体
 * @param[in] deviceList 存储介质列表
 */
void FreeDeviceList(struct DeviceList *deviceList);

/**
 * @brief 创建存储介质
 * @param[in] pluginManager 插件管理器实例指针
 * @param[in] name 名称
 * @param[in] description 描述
 * @param[in] deviceFile 设备文件
 * @param[in] directory 文件系统中的路径
 * @return 指示操作是否成功
 * @details
 * 外部接口说明如下：
 * - 封装后的URL: /Device
 * - 请求HTTP Method：POST
 * - 请求参数通过HTTP Header传递：
 *   - x-cloud-name：名称
 *   - x-cloud-description：描述
 *   - x-cloud-device-file：设备文件
 *   - x-cloud-directory：文件系统中的路径
 * - 响应状态码：
 *   - 200 OK：返回值为true
 *   - 500 Internal Server Error：返回值为false
 */
int CreateDevice(void *pluginManager, const char *name, const char *description,
                 const char *deviceFile, const char *directory);

/**
 * @brief 更新存储介质
 * @param[in] pluginManager 插件管理器实例指针
 * @param[in] name 名称
 * @param[in] newDescription 新的描述
 * @param[in] newDeviceFile 新的设备文件
 * @param[in] newDirectory 新的路径
 * @return 指示操作是否成功
 * @details
 * 外部接口说明如下：
 * - 封装后的URL: /Device
 * - 请求HTTP Method：PUT
 * - 请求参数通过HTTP Header传递：
 *   - x-cloud-name：名称
 *   - x-cloud-new-description：新的描述
 *   - x-cloud-new-device-file：新的设备文件
 *   - x-cloud-new-directory：新的的路径
 * - 响应状态码：
 *   - 200 OK：返回值为true
 *   - 500 Internal Server Error：返回值为false
 */
int UpdateDevice(void *pluginManager, const char *name, const char *newDescription,
                 const char *newDeviceFile, const char *newDirectory);

/**
 * @brief 删除存储介质
 * @param[in] pluginManager 插件管理器实例指针
 * @param[in] name 名称
 * @return 指示操作是否成功
 * @details
 * 外部接口说明如下：
 * - 封装后的URL: /Device
 * - 请求HTTP Method：DELETE
 * - 请求参数通过HTTP Header传递：
 *   - x-cloud-name：名称
 * - 响应状态码：
 *   - 200 OK：返回值为true
 *   - 500 Internal Server Error：返回值为false
 */
int RemoveDevice(void *pluginManager, const char *name);

/**
 * @brief 获取存储介质
 * @param[in] pluginManager 插件管理器实例指针
 * @param[in] name 名称
 * @return 存储介质
 * @details
 * 外部接口说明如下：
 * - 封装后的URL: /Device
 * - 请求HTTP Method：GET
 * - 请求参数通过HTTP Header传递：
 *   - x-cloud-name：名称
 * - 响应状态码：
 *   - 200 OK：返回值不为nullptr
 *   - 404 Not Found：返回值为nullptr
 * - 响应体：application/json
 *   - name：名称
 *   - description：描述
 *   - deviceFile：设备文件
 *   - directory：文件系统中的路径
 */
struct Device *GetDeviceByName(void *pluginManager, const char *name);

/**
 * @brief 获取默认存储介质
 * @param[in] pluginManager 插件管理器实例指针
 * @return 默认存储介质
 * @details
 * 外部接口说明如下：
 * - 封装后的URL: /Device
 * - 请求HTTP Method：GET
 * - 请求参数通过HTTP Header传递：
 *   - x-cloud-name：名称，固定为default
 * - 响应状态码：
 *   - 200 OK：返回值不为nullptr
 *   - 404 Not Found：返回值为nullptr
 * - 响应体：application/json
 *   - name：名称
 *   - description：描述
 *   - deviceFile：设备文件
 *   - directory：文件系统中的路径
 */
struct Device *GetDefaultDevice(void *pluginManager);

/**
 * @brief 获取所有存储介质
 * @param[in] pluginManager 插件管理器实例指针
 * @return 存储介质列表
 * @details
 * 外部接口说明如下：
 * - 封装后的URL: /Device
 *   - 请求HTTP Method：GET
 * - 响应状态码：
 *   - 200 OK：返回值不为nullptr
 *   - 500 Internal Server Error：返回值为nullptr
 * - 响应体：application/json，为Device列表，其中每个元素如下
 *   - name：名称
 *   - description：描述
 *   - deviceFile：设备文件
 *   - directory：文件系统中的路径
 */
struct DeviceList *GetAllDevices(void *pluginManager);

/**
 * @brief 存储策略
 */
struct Strategy {
    /**
     * @brief 名称
     */
    char *name;
    /**
     * @brief 描述
     */
    char *description;
    /**
     * @brief 存储路径数目
     */
    unsigned long locationCount;
    /**
     * @brief 每个存储路径对应的存储介质名称
     */
    char **locationDeviceNames;
    /**
     * @brief 每个存储路径对应的相对路径
     */
    char **locationRelativePaths;
    /**
     * @brief 容错纠错算法
     */
    char *errorCorrectingAlgorithm;
    /**
     * @brief 完整性校验算法
     */
    char *integrityCheckAlgorithm;
    /**
     * @brief 数据保存时长
     */
    unsigned long lifeTimeInSecond;
};

/**
 * @brief 存储策略列表
 */
struct StrategyList {
    /**
     * @brief 元素数量
     */
    unsigned long count;
    /**
     * @brief 存储策略列表
     */
    struct Strategy **strategies;
};

/**
 * @brief 回收存储策略结构体
 * @param[in] strategy 存储策略
 */
void FreeStrategy(struct Strategy *strategy);

/**
 * @brief 回收存储策略列表
 * @param[in] strategyList 存储策略列表
 */
void FreeStrategyList(struct StrategyList *strategyList);

/**
 * @brief 回收字符串
 * @param string 字符串
 */
void FreeString(const char *string);

/**
 * @brief 创建存储策略
 * @param[in] pluginManager 插件管理器实例指针
 * @param[in] name 名称
 * @param[in] description 描述
 * @param[in] locationCount 存储路径数目
 * @param[in] locationDeviceNames 每个存储路径对应的存储介质名称
 * @param[in] locationRelativePaths 每个存储路径对应的相对路径
 * @param[in] errorCorrectingAlgorithm 容错纠错算法
 * @param[in] integrityCheckAlgorithm 完整性校验算法
 * @param[in] lifeTimeInSecond 数据保存时长
 * @return 指示操作是否成功
 * @details
 * 外部接口说明如下：
 * - 封装后的URL: /Strategy
 * - 请求HTTP Method：POST
 * - 请求参数通过HTTP Header传递：
 *   - x-cloud-name：名称
 *   - x-cloud-description：描述
 *   - x-cloud-locations：存储路径，使用分号分隔，每个存储路径的格式为deviceName=xxx,relativePath=xxx
 *   - x-cloud-error-correcting-algorithm：容错纠错算法
 *   - x-cloud-integrity-check-algorithm：完整性校验算法
 *   - x-cloud-life-time-in-second：数据保存时长
 * - 响应状态码：
 *   - 200 OK：返回值为true
 *   - 500 Internal Server Error：返回值为false
 */
int CreateStrategy(void *pluginManager, const char *name, const char *description,
                   int locationCount, const char **locationDeviceNames,
                   const char **locationRelativePaths, const char *errorCorrectingAlgorithm,
                   const char *integrityCheckAlgorithm, unsigned long lifeTimeInSecond);

/**
 * @brief 更新存储策略
 * @param[in] pluginManager 插件管理器实例指针
 * @param[in] name 名称
 * @param[in] newDescription 描述
 * @param[in] newLocationCount 存储路径数目
 * @param[in] newLocationDeviceNames 每个存储路径对应的存储介质名称
 * @param[in] newLocationRelativePaths 每个存储路径对应的相对路径
 * @param[in] newErrorCorrectingAlgorithm 容错纠错算法
 * @param[in] newIntegrityCheckAlgorithm 完整性校验算法
 * @param[in] newLifeTimeInSecond 数据保存时长
 * @return 指示操作是否成功
 * @details
 * 外部接口说明如下：
 * - 封装后的URL: /Strategy
 * - 请求HTTP Method：PUT
 * - 请求参数通过HTTP Header传递：
 *   - x-cloud-name：名称
 *   - x-cloud-new-description：新的描述
 *   - x-cloud-new-locations：新的存储路径，使用分号分隔，每个存储路径的格式为deviceName=xxx,relativePath=xxx
 *   - x-cloud-new-error-correcting-algorithm：新的容错纠错算法
 *   - x-cloud-new-integrity-check-algorithm：新的完整性校验算法
 *   - x-cloud-new-life-time-in-second：新的数据保存时长
 * - 响应状态码：
 *   - 200 OK：返回值为true
 *   - 500 Internal Server Error：返回值为false
 */
int UpdateStrategy(void *pluginManager, const char *name, const char *newDescription,
                   int newLocationCount, const char **newLocationDeviceNames,
                   const char **newLocationRelativePaths, const char *newErrorCorrectingAlgorithm,
                   const char *newIntegrityCheckAlgorithm, unsigned long newLifeTimeInSecond);

/**
 * @brief 删除存储策略
 * @param[in] pluginManager 插件管理器实例指针
 * @param[in] name 名称
 * @return 指示操作是否成功
 * @details
 * 外部接口说明如下：
 * - 封装后的URL: /Strategy
 * - 请求HTTP Method：DELETE
 * - 请求参数通过HTTP Header传递：
 *   - x-cloud-name：名称
 * - 响应状态码：
 *   - 200 OK：返回值为true
 *   - 500 Internal Server Error：返回值为false
 */
int RemoveStrategy(void *pluginManager, const char *name);

/**
 * @brief 获取存储策略
 * @param[in] pluginManager 插件管理器实例指针
 * @param[in] name 名称
 * @return 存储策略
 * @details
 * 外部接口说明如下：
 * - 封装后的URL: /Strategy
 * - 请求HTTP Method：GET
 * - 请求参数通过HTTP Header传递：
 *   - x-cloud-name：名称
 * - 响应状态码：
 *   - 200 OK：返回值不为nullptr
 *   - 404 Not Found：返回值为nullptr
 * - 响应体：application/json
 *   - name：名称
 *   - description：描述
 *   - location：存储路径，一个列表，其中每个元素包含如下字段
 *     - deviceName：设备名称
 *     - relativePath：相对路径
 *   - errorCorrectingAlgorithm：容错纠错算法
 *   - integrityCheckAlgorithm：完整性校验算法
 *   - lifeTimeInSecond：数据保存时长
 */
struct Strategy *GetStrategyByName(void *pluginManager, const char *name);

/**
 * @brief 获取默认存储策略
 * @param[in] pluginManager 插件管理器实例指针
 * @return 默认存储策略
 * @details
 * 外部接口说明如下：
 * - 封装后的URL: /Strategy
 * - 请求HTTP Method：GET
 * - 请求参数通过HTTP Header传递：
 *   - x-cloud-name：名称，固定为default
 * - 响应状态码：
 *   - 200 OK：返回值不为nullptr
 *   - 404 Not Found：返回值为nullptr
 * - 响应体：application/json
 *   - name：名称
 *   - description：描述
 *   - location：存储路径，一个列表，其中每个元素包含如下字段
 *     - deviceName：设备名称
 *     - relativePath：相对路径
 *   - errorCorrectingAlgorithm：容错纠错算法
 *   - integrityCheckAlgorithm：完整性校验算法
 *   - lifeTimeInSecond：数据保存时长
 */
struct Strategy *GetDefaultStrategy(void *pluginManager);

/**
 * @brief 获取所有存储策略
 * @param[in] pluginManager 插件管理器实例指针
 * @return 存储策略列表
 * @details
 * 外部接口说明如下：
 * - 封装后的URL: /Strategy
 *   - 请求HTTP Method：GET
 * - 响应状态码：
 *   - 200 OK：返回值不为nullptr
 *   - 500 Internal Server Error：返回值为nullptr
 * - 响应体：application/json，为Strategy列表，其中每个元素如下
 *   - name：名称
 *   - description：描述
 *   - location：存储路径，一个列表，其中每个元素包含如下字段
 *     - deviceName：设备名称
 *     - relativePath：相对路径
 *   - errorCorrectingAlgorithm：容错纠错算法
 *   - integrityCheckAlgorithm：完整性校验算法
 *   - lifeTimeInSecond：数据保存时长
 */
struct StrategyList *GetAllStrategies(void *pluginManager);

/**
 * @brief 配置存储策略
 * @param[in] pluginManager 插件管理器实例指针
 * @param[in] application 应用名称
 * @param[in] dataType 数据类型
 * @param[in] strategyName 存储策略名称
 * @return 指示操作是否成功
 * @details
 * 外部接口说明如下：
 * - 封装后的URL: /Profile
 * - 请求HTTP Method：POST
 * - 请求参数通过HTTP Header传递：
 *   - x-cloud-application：应用名称
 *   - x-cloud-data-type：数据类型
 *   - x-cloud-strategy-name：存储策略名称
 * - 响应状态码：
 *   - 200 OK：返回值为true
 *   - 500 Internal Server Error：返回值为false
 */
int CreateProfile(void *pluginManager, const char *application, const char *dataType,
                  const char *strategyName);

/**
 * @brief 更新存储策略配置
 * @param[in] pluginManager 插件管理器实例指针
 * @param[in] application 应用名称
 * @param[in] dataType 数据类型
 * @param[in] newStrategyName 新的存储策略名称
 * @return 指示操作是否成功
 * @details
 * 外部接口说明如下：
 * - 封装后的URL: /Profile
 * - 请求HTTP Method：PUT
 * - 请求参数通过HTTP Header传递：
 *   - x-cloud-application：应用名称
 *   - x-cloud-data-type：数据类型
 *   - x-cloud-new-strategy-name：新的存储策略名称
 * - 响应状态码：
 *   - 200 OK：返回值为true
 *   - 500 Internal Server Error：返回值为false
 */
int UpdateProfile(void *pluginManager, const char *application, const char *dataType,
                  const char *newStrategyName);

/**
 * @brief 删除存储策略配置
 * @param[in] pluginManager 插件管理器实例指针
 * @param[in] application 应用
 * @param[in] dataType 数据类型
 * @return 指示操作是否成功
 * @details
 * 外部接口说明如下：
 * - 封装后的URL: /Profile
 * - 请求HTTP Method：DELETE
 * - 请求参数通过HTTP Header传递：
 *   - x-cloud-application：应用名称
 *   - x-cloud-data-type：数据类型
 * - 响应状态码：
 *   - 200 OK：返回值为true
 *   - 500 Internal Server Error：返回值为false
 */
int RemoveProfile(void *pluginManager, const char *application, const char *dataType);

/**
 * @brief 获取存储策略配置
 * @param[in] pluginManager 插件管理器实例指针
 * @param[in] application 应用
 * @param[in] dataType 数据类型
 * @return 一个字符串，关联到特定应用和数据类型的存储策略名称
 * @details
 * 外部接口说明如下：
 * - 封装后的URL: /Profile
 * - 请求HTTP Method：GET
 * - 请求参数通过HTTP Header传递：
 *   - x-cloud-application：应用名称
 *   - x-cloud-data-type：数据类型
 * - 响应状态码：
 *   - 200 OK：返回值不为nullptr
 *   - 404 Not Found：返回值为nullptr
 * - 响应体：application/json，一个字符串，关联到特定应用和数据类型的存储策略名称
 */
char *GetProfile(void *pluginManager, const char *application, const char *dataType);

/**
 * @brief 为应用的所有数据类型设置存储策略
 * @param[in] pluginManager 插件管理器实例指针
 * @param[in] application 应用名称
 * @param[in] strategyName 存储策略名称
 * @return 指示操作是否成功
 * @details
 * 外部接口说明如下：
 * - 封装后的URL: /Profile
 * - 请求HTTP Method：POST
 * - 请求参数通过HTTP Header传递：
 *   - x-cloud-application：应用名称
 *   - x-cloud-strategy-name：存储策略名称
 * - 响应状态码：
 *   - 200 OK：返回值为true
 *   - 500 Internal Server Error：返回值为false
 */
int CreateProfileForApplication(void *pluginManager, const char *application,
                                const char *strategyName);

/**
 * @brief 更新应用的存储策略配置
 * @param[in] pluginManager 插件管理器实例指针
 * @param[in] application 应用名称
 * @param[in] newStrategyName 存储策略名称
 * @return 指示操作是否成功
 * @details
 * 外部接口说明如下：
 * - 封装后的URL: /Profile
 * - 请求HTTP Method：PUT
 * - 请求参数通过HTTP Header传递：
 *   - x-cloud-application：应用名称
 *   - x-cloud-new-strategy-name：新的存储策略名称
 * - 响应状态码：
 *   - 200 OK：返回值为true
 *   - 500 Internal Server Error：返回值为false
 */
int UpdateProfileForApplication(void *pluginManager, const char *application,
                                const char *newStrategyName);

/**
 * @brief 删除应用的存储策略配置
 * @param[in] pluginManager 插件管理器实例指针
 * @param[in] application 应用
 * @return 指示操作是否成功
 * @details
 * 外部接口说明如下：
 * - 封装后的URL: /Profile
 * - 请求HTTP Method：DELETE
 * - 请求参数通过HTTP Header传递：
 *   - x-cloud-application：应用名称
 * - 响应状态码：
 *   - 200 OK：返回值为true
 *   - 500 Internal Server Error：返回值为false
 */
int RemoveProfileForApplication(void *pluginManager, const char *application);

/**
 * @brief 获取应用的存储策略配置
 * @param[in] pluginManager 插件管理器实例指针
 * @param[in] application 应用
 * @return 一个字符串，关联到特定应用的存储策略名称
 * @details
 * 外部接口说明如下：
 * - 封装后的URL: /Profile
 * - 请求HTTP Method：GET
 * - 请求参数通过HTTP Header传递：
 *   - x-cloud-application：应用名称
 * - 响应状态码：
 *   - 200 OK：返回值不为nullptr
 *   - 404 Not Found：返回值为nullptr
 * - 响应体：application/json，一个字符串，关联到特定应用和数据类型的存储策略名称
 */
char *GetProfileForApplication(void *pluginManager, const char *application);

/**
 * @brief 为特定数据类型设置存储策略
 * @param[in] pluginManager 插件管理器实例指针
 * @param[in] dataType 数据类型
 * @param[in] strategyName 存储策略名称
 * @return 指示操作是否成功
 * @details
 * 外部接口说明如下：
 * - 封装后的URL: /Profile
 * - 请求HTTP Method：POST
 * - 请求参数通过HTTP Header传递：
 *   - x-cloud-data-type：数据类型
 *   - x-cloud-strategy-name：存储策略名称
 * - 响应状态码：
 *   - 200 OK：返回值为true
 *   - 500 Internal Server Error：返回值为false
 */
int CreateProfileForDataType(void *pluginManager, const char *dataType, const char *strategyName);

/**
 * @brief 更新特定数据类型的存储策略配置
 * @param[in] pluginManager 插件管理器实例指针
 * @param[in] dataType 数据类型
 * @param[in] newStrategyName 存储策略名称
 * @return 指示操作是否成功
 * @details
 * 外部接口说明如下：
 * - 封装后的URL: /Profile
 * - 请求HTTP Method：PUT
 * - 请求参数通过HTTP Header传递：
 *   - x-cloud-data-type：数据类型
 *   - x-cloud-new-strategy-name：新的存储策略名称
 * - 响应状态码：
 *   - 200 OK：返回值为true
 *   - 500 Internal Server Error：返回值为false
 */
int UpdateProfileForDataType(void *pluginManager, const char *dataType,
                             const char *newStrategyName);

/**
 * @brief 删除特定数据类型的存储策略配置
 * @param[in] pluginManager 插件管理器实例指针
 * @param[in] dataType 数据类型
 * @return 指示操作是否成功
 * @details
 * 外部接口说明如下：
 * - 封装后的URL: /Profile
 * - 请求HTTP Method：DELETE
 * - 请求参数通过HTTP Header传递：
 *   - x-cloud-data-type：数据类型
 * - 响应状态码：
 *   - 200 OK：返回值为true
 *   - 500 Internal Server Error：返回值为false
 */
int RemoveProfileForDataType(void *pluginManager, const char *dataType);

/**
 * @brief 获取特定数据类型的存储策略配置
 * @param[in] pluginManager 插件管理器实例指针
 * @param[in] dataType 数据类型
 * @return 一个字符串，关联到特定数据类型的存储策略名称
 * @details
 * 外部接口说明如下：
 * - 封装后的URL: /Profile
 * - 请求HTTP Method：GET
 * - 请求参数通过HTTP Header传递：
 *   - x-cloud-data-type：数据类型
 * - 响应状态码：
 *   - 200 OK：返回值不为nullptr
 *   - 404 Not Found：返回值为nullptr
 * - 响应体：application/json，一个字符串，关联到特定应用和数据类型的存储策略名称
 */
char *GetProfileForDataType(void *pluginManager, const char *dataType);

/**
 * @brief 查找存储策略配置
 * @details 优先检索“应用+数据类型”的配置信息，之后检索特定应用的配置信息，之后检索特定数据类型的配置信息，都未找到时返回默认存储策略
 * @param[in] pluginManager 插件管理器实例指针
 * @param[in] application 应用
 * @param[in] dataType 数据类型
 * @return 一个字符串，存储策略名称
 * @details
 * 外部接口说明如下：
 * - 封装后的URL: /Profile/Find
 * - 请求HTTP Method：GET
 * - 请求参数通过HTTP Header传递：
 *   - x-cloud-application：应用名称
 *   - x-cloud-data-type：数据类型
 * - 响应状态码：
 *   - 200 OK：返回值不为nullptr
 *   - 404 Not Found：返回值为nullptr
 * - 响应体：application/json，一个字符串，关联到特定应用和数据类型的存储策略名称
 */
char *FindProfile(void *pluginManager, const char *application, const char *dataType);

/**
 * @brief 数据块
 */
struct DataBlock {
    /**
     * @brief 数据大小
     */
    unsigned long size;
    /**
     * @brief 数据内容
     */
    char *data;
};

/**
 * @brief 回收数据块
 * @param dataBlock 数据块
 */
void FreeDataBlock(struct DataBlock *dataBlock);

/**
 * @brief 读取数据
 * @param[in] pluginManager 插件管理器实例指针
 * @param[in] application 应用
 * @param[in] dataType 数据类型
 * @param[in] name 数据唯一标识符
 * @return 数据块
 */
struct DataBlock *ReadData(void *pluginManager, const char *application, const char *dataType,
                           const char *name);

/**
 * @brief 读取指定版本的数据
 * @param[in] pluginManager 插件管理器实例指针
 * @param[in] application 应用
 * @param[in] dataType 数据类型
 * @param[in] name 数据唯一标识符
 * @param[in] version 版本
 * @return 数据块
 */
struct DataBlock *ReadDataWithVersion(void *pluginManager, const char *application,
                                      const char *dataType, const char *name, const char *version);

/**
 * @brief 写入数据
 * @param[in] pluginManager 插件管理器实例指针
 * @param[in] application 应用
 * @param[in] dataType 数据类型
 * @param[in] name 数据唯一标识符
 * @param[in] size 数据大小
 * @param[in] data 数据内容
 * @return 指示操作是否成功
 */
int WriteData(void *pluginManager, const char *application, const char *dataType, const char *name,
              unsigned long size, const char *data);

/**
 * @brief 写入指定版本的数据
 * @param[in] pluginManager 插件管理器实例指针
 * @param[in] application 应用
 * @param[in] dataType 数据类型
 * @param[in] name 数据唯一标识符
 * @param[in] version 版本
 * @param[in] size 数据大小
 * @param[in] data 数据内容
 * @return 指示操作是否成功
 */
int WriteDataWithVersion(void *pluginManager, const char *application, const char *dataType,
                         const char *name, const char *version, unsigned long size,
                         const char *data);

/**
 * @brief 删除数据
 * @param[in] pluginManager 插件管理器实例指针
 * @param[in] application 应用
 * @param[in] dataType 数据类型
 * @param[in] name 数据唯一标识符
 * @return 指示操作是否成功
 */
int RemoveData(void *pluginManager, const char *application, const char *dataType,
               const char *name);

/**
 * @brief 删除指定版本的数据
 * @param[in] pluginManager 插件管理器实例指针
 * @param[in] application 应用
 * @param[in] dataType 数据类型
 * @param[in] name 数据唯一标识符
 * @param[in] version 版本
 * @return 指示操作是否成功
 */
int RemoveDataWithVersion(void *pluginManager, const char *application, const char *dataType,
                          const char *name, const char *version);

/**
 * @brief 修复数据
 * @param[in] pluginManager 插件管理器实例指针
 * @param[in] application 应用
 * @param[in] dataType 数据类型
 * @param[in] name 数据唯一标识符
 * @return 指示操作是否成功
 */
int RepairData(void *pluginManager, const char *application, const char *dataType,
               const char *name);

/**
 * @brief 修复指定版本的数据
 * @param[in] pluginManager 插件管理器实例指针
 * @param[in] application 应用
 * @param[in] dataType 数据类型
 * @param[in] name 数据唯一标识符
 * @param[in] version 版本
 * @return 指示操作是否成功
 */
int RepairDataWithVersion(void *pluginManager, const char *application, const char *dataType,
                          const char *name, const char *version);

/**
 * @brief 存储主题
 * @param[in] pluginManager 插件管理器实例指针
 * @param[in] topic 主题名称
 * @return 指示操作是否成功
 * @details
 * 外部接口说明如下：
 * - 封装后的URL: /Topic
 * - 请求HTTP Method：POST
 * - 请求参数通过HTTP Header传递：
 *   - x-cloud-topic：主题名称
 * - 响应状态码：
 *   - 200 OK：返回值为true
 *   - 500 Internal Server Error：返回值为false
 */
int StoreTopic(void *pluginManager, const char *topic);

/**
 * @brief 查询主题是否存在
 * @param[in] pluginManager 插件管理器实例指针
 * @param[in] topic 主题名称
 * @return 1表示存在，0表示不存在，-1表示错误
 * @details
 * 外部接口说明如下：
 * - 封装后的URL: /Topic
 * - 请求HTTP Method：HEAD
 * - 请求参数通过HTTP Header传递：
 *   - x-cloud-topic：主题名称
 * - 响应状态码：
 *   - 200 OK：返回值为1
 *   - 404 Not Found：返回值为0
 *   - 500 Internal Server Error：返回值为-1
 */
int QueryTopic(void *pluginManager, const char *topic);

/**
 * @brief 删除主题
 * @param[in] pluginManager 插件管理器实例指针
 * @param[in] topic 主题名称
 * @return 指示操作是否成功
 * @details
 * 外部接口说明如下：
 * - 封装后的URL: /Topic
 * - 请求HTTP Method：DELETE
 * - 请求参数通过HTTP Header传递：
 *   - x-cloud-topic：主题名称
 * - 响应状态码：
 *   - 200 OK：返回值为true
 *   - 500 Internal Server Error：返回值为false
 */
int DeleteTopic(void *pluginManager, const char *topic);

/**
 * @brief 节点信息结构体
 */
struct NodeInfo {
    /**
     * @brief 节点UUID
     */
    char *uuid;
    /**
     * @brief 节点IP地址
     */
    char *ipAddress;
};

/**
 * @brief 节点信息列表
 */
struct NodeInfoList {
    /**
     * @brief 节点数量
     */
    unsigned long count;
    /**
     * @brief 节点信息列表
     */
    struct NodeInfo **nodes;
};

/**
 * @brief 创建节点信息结构体
 * @return 空的节点信息结构体
 */
struct NodeInfo *CreateNodeInfo();

/**
 * @brief 回收节点信息结构体
 * @param[in] nodeInfo 节点信息
 */
void FreeNodeInfo(struct NodeInfo *nodeInfo);

/**
 * @brief 回收节点信息列表结构体
 * @param[in] nodeInfoList 节点信息列表
 */
void FreeNodeInfoList(struct NodeInfoList *nodeInfoList);

/**
 * @brief 创建节点
 * @param[in] pluginManager 插件管理器实例指针
 * @param[in] uuid 节点UUID
 * @param[in] ipAddress 节点IP地址
 * @return 指示操作是否成功
 * @details
 * 外部接口说明如下：
 * - 封装后的URL: /Node
 * - 请求HTTP Method：POST
 * - 请求参数通过HTTP Header传递：
 *   - x-cloud-uuid：节点UUID
 *   - x-cloud-ip-address：节点IP地址
 * - 响应状态码：
 *   - 200 OK：返回值为true
 *   - 500 Internal Server Error：返回值为false
 */
int CreateNode(void *pluginManager, const char *uuid, const char *ipAddress);

/**
 * @brief 更新节点IP地址
 * @param[in] pluginManager 插件管理器实例指针
 * @param[in] uuid 节点UUID
 * @param[in] newIpAddress 新的IP地址
 * @return 指示操作是否成功
 * @details
 * 外部接口说明如下：
 * - 封装后的URL: /Node
 * - 请求HTTP Method：PUT
 * - 请求参数通过HTTP Header传递：
 *   - x-cloud-uuid：节点UUID
 *   - x-cloud-ip-address：新的IP地址
 * - 响应状态码：
 *   - 200 OK：返回值为true
 *   - 500 Internal Server Error：返回值为false
 */
int UpdateNode(void *pluginManager, const char *uuid, const char *newIpAddress);

/**
 * @brief 删除节点
 * @param[in] pluginManager 插件管理器实例指针
 * @param[in] uuid 节点UUID
 * @return 指示操作是否成功
 * @details
 * 外部接口说明如下：
 * - 封装后的URL: /Node
 * - 请求HTTP Method：DELETE
 * - 请求参数通过HTTP Header传递：
 *   - x-cloud-uuid：节点UUID
 * - 响应状态码：
 *   - 200 OK：返回值为true
 *   - 500 Internal Server Error：返回值为false
 */
int RemoveNode(void *pluginManager, const char *uuid);

/**
 * @brief 根据UUID获取节点信息
 * @param[in] pluginManager 插件管理器实例指针
 * @param[in] uuid 节点UUID
 * @return 节点信息，如果不存在则返回nullptr
 * @details
 * 外部接口说明如下：
 * - 封装后的URL: /Node
 * - 请求HTTP Method：GET
 * - 请求参数通过HTTP Header传递：
 *   - x-cloud-uuid：节点UUID
 * - 响应状态码：
 *   - 200 OK：返回值不为nullptr
 *   - 404 Not Found：返回值为ullptr
 * - 响应体：application/json
 *   - uuid：节点UUID
 *   - ipAddress：节点IP地址
 */
struct NodeInfo *GetNodeByUuid(void *pluginManager, const char *uuid);

/**
 * @brief 根据IP地址获取节点信息
 * @param[in] pluginManager 插件管理器实例指针
 * @param[in] ipAddress IP地址
 * @return 节点信息，如果不存在则返回nullptr
 * @details
 * 外部接口说明如下：
 * - 封装后的URL: /Node
 * - 请求HTTP Method：GET
 * - 请求参数通过HTTP Header传递：
 *   - x-cloud-ip-address：节点IP地址
 * - 响应状态码：
 *   - 200 OK：返回值不为nullptr
 *   - 404 Not Found：返回值为ullptr
 * - 响应体：application/json
 *   - uuid：节点UUID
 *   - ipAddress：节点IP地址
 */
struct NodeInfo *GetNodeByIpAddress(void *pluginManager, const char *ipAddress);

/**
 * @brief 获取所有节点信息
 * @param[in] pluginManager 插件管理器实例指针
 * @return 节点信息列表
 * @details
 * 外部接口说明如下：
 * - 封装后的URL: /Node
 *   - 请求HTTP Method：GET
 * - 响应状态码：
 *   - 200 OK：返回值不为nullptr
 *   - 500 Internal Server Error：返回值为nullptr
 * - 响应体：application/json，为Node列表，其中每个元素如下
 *   - uuid：节点UUID
 *   - ipAddress：节点IP地址
 */
struct NodeInfoList *GetAllNodes(void *pluginManager);

/**
 * @brief 检查节点是否存在
 * @param[in] pluginManager 插件管理器实例指针
 * @param[in] uuid 节点UUID
 * @return 1表示存在，0表示不存在，-1表示错误
 * @details
 * 外部接口说明如下：
 * - 封装后的URL: /Node
 * - 请求HTTP Method：HEAD
 * - 请求参数通过HTTP Header传递：
 *   - x-cloud-uuid：节点UUID
 * - 响应状态码：
 *   - 200 OK：返回值为1
 *   - 404 Not Found：返回值为0
 *   - 500 Internal Server Error：返回值为-1
 */
int NodeExists(void *pluginManager, const char *uuid);

/**
 * @brief 设置存储容量控制阈值
 * @param[in] pluginManager 插件管理器实例指针
 * @param[in] size 阈值大小
 * @return   指示操作是否成功   
 */
int SetSpaceLimitSize(void *pluginManager, uint64_t size);

/**
 * @brief   查看设置的阈值大小
 * @param[in] pluginManager 插件管理器实例指针 
 * @param[out] size 阈值大小
 * @return  指示操作是否成功  
 */
int GetSpaceLimitSize(void *pluginManager, uint64_t *size);

/**
 * @brief   查看已使用容量大小
* @param[in] pluginManager 插件管理器实例指针
 * @param[out] size 已使用容量大小
 * @return     指示操作是否成功
 */
int GetUsedSpaceSize(void *pluginManager, uint64_t *size);

/**
 * @brief   查看总容量大小
 * @param[in] pluginManager 插件管理器实例指针
 * @param[out] size 总容量容量大小
 * @return  指示操作是否成功
 */
int GetTotalSpaceSize(void *pluginManager, uint64_t *size);

#include <stdint.h>
// API

// 星地
/**
 * @brief 上传数据
 * @details 收到数据存到不同的位置，之后通知应用
 * @details URL: /Data/{dataType}
 * @details Method: POST
 * @param [in] pluginManager 插件管理器
 * @param [in] dataType [URL中表示] 数据类型（raw, ...）- 参照 fleet wiki中接口规范-外部接口-数据管理，使用未定义的数据类型会返回错误，下同
 * @param [in] name     [header: x-cloud-name] 数据唯一标识符
 * @param [in] to       [header: x-cloud-to] 应用名称（云平台、智能处理、……），后续从HTTP Header Authorization中解析
 * @param [in] data     [body中获取] 数据
 * @param [in] length   [解析body长度]数据长度
 * @note 除data外，其他输入不能包含'|'或'\n'
 * @return 1: success. else: error
 */
int UploadData(void *pluginManager, const char *dataType, const char *name, const char *to,
               const char *data, int length);

/**
 * @brief 上传安全数据至自动下传区域
 * @param[in] pluginManager 插件管理器指针
 * @param[in] key 数据唯一标识符
 * @param[in] data 数据
 * @param[in] length 数据长度
 * @return 指示操作是否成功
 */
int UploadSecurityDataToAutoDownloadArea(void *pluginManager, const char *key, const char *data,
                                         int length);

/**
 * @brief 上传分片数据
 * @param[in] pluginManager 插件管理器指针
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
int UploadSegmentData(void *pluginManager, const char *dataType, const char *key, const char *owner,
                      int currentSegmentId, int totalSegmentCount, int totalSize, int segmentSize,
                      const char *segmentData);

/**
 * @brief 查询数据
 * @details 模糊查询，从全局索引中返回指定应用中模糊匹配keyword的所有name
 * @details URL: /Data/Query/{dataType}
 * @details Method: GET
 * @param [in] pluginManager 插件管理器
 * @param [in] dataType [URL中表示] 数据类型（raw, ...）
 * @param [in] name  [header: x-cloud-keyword] 查询关键字
 * @param [out] data    符合条件的name索引条目
 * @return data size > 0, < 0 表示错误
 * @note 该函数会为data分配内存，调用者需要负责释放
 * 返回的data格式为: dataName|node|dataType|dataOwner|dataSize|available|operationType|operationDetail|operationTime\n
 * 具体语义如下：
 * node:            数据所在节点
 * available:       数据是否可用 (0: 不可用, 1: 可用)
 * operationType:   最近执行的一次操作 (ADD, DELETE, UPDATE, 可忽略)
 * operationDetail: 操作细节 (通常为空，可忽略)
 * operationTime:   最后一次操作时间 (YYYY-MM-DD HH:MM:SS)
 * 例如: test|node1|raw|app1|13|1|ADD|this is a new file|2025-03-11 02:56:38\n
 */
int QueryData(void *pluginManager, const char *dataType, const char *name, char **data);

/**
 * @brief 下载数据
 * @details 从本地返回匹配数据
 * @details URL: /Data/Download/{dataType}
 * @details Method: GET
 * @param [in] pluginManager 插件管理器
 * @param [in] dataType [URL中表示] 数据类型（raw, ...）
 * @param [in] name     [header: x-cloud-name] 数据唯一标识符
 * @param [in] from     [header: x-cloud-from] 应用名称（云平台、智能处理、……），后续从HTTP Header Authorization中解析
 * @param [out] data    数据
 * @return data size > 0, < 0 表示错误
 * @note 该函数会为data分配内存，调用者需要负责释放
 * @note 若本地无匹配数据，会向其他节点请求数据
 */
int DownloadData(void *pluginManager, const char *dataType, const char *name, const char *from,
                 char **data);

/**
 * @brief 接收遥测数据
 * @details 将应用发送的遥测数据发送到地面（topic转发）
 * @details URL: /Data/Telemetry
 * @details Method: PUT
 * @param [in] pluginManager 插件管理器
 * @param [in] from [header: x-cloud-from] 应用名称（云平台、智能处理、……），后续从HTTP Header Authorization中解析
 * @return 1: success. else: error
 * @note 目前此接口仅将数据转发到地面，不做其他处理
 */
int ReceiveTelemetry(void *pluginManager, const char *from);

/**
 * @brief 采集遥测数据
 * @details 保存应用发送的遥测数据
 * @details URL: /Data/Telemetry
 * @details Method: POST
 * @param [in] pluginManager 插件管理器
 * @param [in] from     [header: x-cloud-from] 应用名称（云平台、智能处理、……），后续从HTTP Header Authorization中解析
 * @param [in] data     [body中获取] 遥测数据
 * @param [in] length   [解析body获取]数据长度
 * @return 1: success. else: error
 * @note 目前此接口仅将数据保存，不做其他处理
 */
int ReportTelemetry(void *pluginManager, const char *from, const char *data, int length);

/**
 * @brief 发送遥控指令
 * @details 从地面收到遥控指令，发送给应用
 * @details URL: /Data/RemoteControl
 * @details Method: POST
 * @param [in] pluginManager 插件管理器
 * @param [in] to       [header: x-cloud-to] 应用名称（云平台、智能处理、……），后续从HTTP Header Authorization中解析
 * @param [in] data     [body中获取] 遥控指令
 * @param [in] length   [解析body获取]数据长度
 * @return 1: success. else: error
 * @note 目前此接口仅将数据保存后转发topic，不做其他处理
 */
int SendRemoteControl(void *pluginManager, const char *to, const char *data, int length);

// 注册回调函数 TODO
// 当收到遥控指令时调用回调函数（触发HTTP请求返回），当未收到遥控指令时HTTP请求挂起
// URL: /Data/RemoteControl
// Method: GET
// Header parameter: x-cloud-to: 应用名称（云平台、智能处理、……），后续从HTTP Header
// Authorization中解析 Response body: 遥控指令
typedef void (*RemoteControlCallback)(const char *to, const char *data, int length, void *closure);
int RegisterRemoteControlCallback(void *pluginManager, RemoteControlCallback callback,
                                  void *closure);

// 星间

/**
 * @brief 同步数据
 * @details 本地数据发到{node}
 * @details URL: /Data/Sync/{datatype}
 * @details Method: POST
 * @param [in] pluginManager 插件管理器
 * @param [in] dataType [URL中表示] 数据类型（raw, ...）
 * @param [in] name     [header: x-cloud-name] 数据唯一标识符
 * @param [in] dataOwner [header: x-cloud-from] 应用名称（云平台、智能处理、……），后续从HTTP Header Authorization中解析
 * @param [in] node     [header: x-cloud-node] 目标节点
 * @return 1: success. else: error
 * @note 本质上是从本地读取数据发送，若本地无数据则返回错误
 */
int SyncData(void *pluginManager, const char *dataType, const char *name, const char *dataOwner,
             const char *node);

/**
 * @brief 备份数据
 * @details 将本地数据打包，添加版本，发送到{node}
 * @details URL: /Data/Backup/{datatype}
 * @details Method: POST
 * @param [in] pluginManager 插件管理器
 * @param [in] dataType [URL中表示] 数据类型（raw, ...）
 * @param [in] name     [header: x-cloud-name] 数据唯一标识符
 * @param [in] from     [header: x-cloud-from] 应用名称（云平台、智能处理、……），后续从HTTP Header Authorization中解析
 * @param [in] version  [header: x-cloud-version] 备份版本
 * @param [in] node     [header: x-cloud-node] 目标节点
 * @return 1: success. else: error
 * @note 本质上是从本地读取数据发送，若本地无数据则返回错误
 */
int BackupData(void *pluginManager, const char *dataType, const char *from, const char *name,
               const char *version, const char *node);

/**
 * @brief 恢复数据
 * @details 从{node}获取指定版本的备份并恢复至本地
 * @details URL: /Data/Restore/{datatype}
 * @details Method: POST
 * @param [in] pluginManager 插件管理器
 * @param [in] dataType [URL中表示] 数据类型（raw, ...）
 * @param [in] name     [header: x-cloud-name] 数据唯一标识符
 * @param [in] from [header: x-cloud-from] 应用名称（云平台、智能处理、……），后续从HTTP Header Authorization中解析
 * @param [in] version  [header: x-cloud-version] 备份版本
 * @param [in] node     [header: x-cloud-node] 备份所在节点
 * @return 1: success. else: error
 */
int RecoverData(void *pluginManager, const char *dataType, const char *from, const char *name,
                const char *version, const char *node);

/**
 * @brief 观测单
 * @details 任务规划发起，收到JSON，转发到另外一个topic（任务规划收）
 * @details URL: /Order/Observation/{uuid}
 * @details Method: POST
 * @param [in] pluginManager 插件管理器
 * @param [in] uuid [URL中表示] 消息唯一标识符
 * @param [in] data [body中获取] 数据
 * @param [in] length [解析body长度]数据长度
 * @return 1: success. else: error
 * @note 目前此接口仅将数据保存后转发topic，不做其他处理
 * @note 所有任务单数据的uuid即为dataName，dataOwner 和 dataType 指定为 RWGH 和 raw. 下同
 */
int ObservationOrder(void *pluginManager, const char *uuid, const char *data, int length);

/**
 * @brief 处理单
 * @details 任务规划发起，收到JSON，转发到另外一个topic（在轨处理收）
 * @details URL: /Order/Processing/{uuid}
 * @details Method: POST
 * @param [in] pluginManager 插件管理器
 * @param [in] uuid [URL中表示] 消息唯一标识符
 * @param [in] data [body中获取] 数据
 * @param [in] length [解析body长度]数据长度
 * @return 1: success. else: error
 * @note 目前此接口仅将数据保存后转发topic，不做其他处理
 */
int ProcessingOrder(void *pluginManager, const char *uuid, const char *data, int length);

/**
 * @brief 分发单
 * @details 任务规划发起，收到JSON，直接发数据（终端收）
 * @details URL: /Order/Dispatching/{uuid}
 * @details Method: POST
 * @param [in] pluginManager 插件管理器
 * @param [in] uuid [URL中表示] 消息唯一标识符
 * @param [in] data [body中获取] 数据
 * @param [in] length [解析body长度]数据长度
 * @return 1: success. else: error
 * @note 目前此接口仅将数据保存后转发topic，不做其他处理
 */
int DispatchingOrder(void *pluginManager, const char *uuid, const char *data, int length);

/**
 * @brief 状态查询
 * @details 从云平台收，调资源管理的接口，返回任务状态（yaml）
 * @details URL: /Order/Status/{uuid}
 * @details Method: GET
 * @param [in] pluginManager 插件管理器
 * @param [in] uuid [URL中表示] 消息唯一标识符
 * @param [out] data [返回body] yaml
 * @return data size > 0, < 0 表示错误
 * @note 本质上是从本地读取数据返回，若本地无数据则返回错误
 */
int OrderStatus(void *pluginManager, const char *uuid, char **data);

/**
 * @brief 结果查询
 * @details 从本地读结果返回
 * @details URL: /Order/Result/{uuid}
 * @details Method: GET
 * @param [in] pluginManager 插件管理器
 * @param [in] uuid [URL中表示] 消息唯一标识符
 * @param [out] data [返回body] 数据
 * @return data size > 0, < 0 表示错误
 * @note 本质上是从本地读取数据返回，若本地无数据则返回错误
 */
int OrderResult(void *pluginManager, const char *uuid, char **data);

// Free 4 Rust
void FreeMem(char *data);

/**
 * @brief 获取配置值
 * @details 从分布式键值存储系统中获取指定键的配置值，使用LWW算法确保最终一致性
 * @param[in] pluginManager 插件管理器实例指针
 * @param[in] key 配置键
 * @param[out] value 配置值指针，函数会分配内存存储配置值，调用者需要使用FreeMem释放
 * @return 配置值长度，< 0 表示错误（键不存在或已被删除）
 * @note 该函数会收集所有相关的ConfigOperation消息，使用HLC时间戳进行LWW选择
 * @note 返回的value指针需要调用者使用FreeMem函数释放内存
 */
int GetConfig(void *pluginManager, const char *key, char **value);

/**
 * @brief 设置配置值
 * @details 在分布式键值存储系统中设置指定键的配置值，自动分配HLC时间戳并广播到集群
 * @param[in] pluginManager 插件管理器实例指针
 * @param[in] key 配置键
 * @param[in] value 配置值
 * @param[in] length 配置值长度
 * @return 1: 成功，0: 失败
 * @note 该函数会创建ConfigOperation消息并通过MessagingService发布到集群
 * @note 操作会自动保存到历史消息存储中，支持故障恢复
 */
int PutConfig(void *pluginManager, const char *key, const char *value, int length);

/**
 * @brief 删除配置
 * @details 从分布式键值存储系统中删除指定键的配置，自动分配HLC时间戳并广播到集群
 * @param[in] pluginManager 插件管理器实例指针
 * @param[in] key 配置键
 * @return 1: 成功，0: 失败
 * @note 该函数会创建DELETE类型的ConfigOperation消息并发布到集群
 * @note 删除操作会保存到历史消息存储中，后续GetConfig调用会返回键不存在
 */
int RemoveConfig(void *pluginManager, const char *key);

/**
 * @brief 设置HLC时钟偏差（用于测试）
 * @details 设置节点的HLC时钟偏差，用于模拟分布式环境中的时钟不同步
 * @param[in] pluginManager 插件管理器实例指针
 * @param[in] offsetMs 时钟偏差（毫秒），正数表示时钟快，负数表示时钟慢
 * @return 1: 成功，0: 失败
 * @note 仅用于测试目的，生产环境不应使用
 */
int SetClockOffset(void *pluginManager, long long offsetMs);

/**
 * @brief 获取当前HLC时钟偏差
 * @details 获取节点当前设置的HLC时钟偏差值
 * @param[in] pluginManager 插件管理器实例指针
 * @return 当前时钟偏差（毫秒）
 */
long long GetClockOffset(void *pluginManager);

int SqliteExecuteOnMultipleNodes(void *pluginManager, int nodeIdCount, const char **nodeIdList,
                                 const char *connectionString, const char *sql);

int SqliteExecuteParameterizedOnMultipleNodes(void *pluginManager, int nodeIdCount,
                                              const char **nodeIdList, const char *connectionString,
                                              const char *sql, int valueCount, const char **types,
                                              const char **values);

int PostgreSqlExecuteOnMultipleNodes(void *pluginManager, int nodeIdCount, const char **nodeIdList,
                                     const char *connectionString, const char *sql);

int PostgreSqlExecuteParameterizedOnMultipleNodes(void *pluginManager, int nodeIdCount,
                                                  const char **nodeIdList,
                                                  const char *connectionString, const char *sql,
                                                  int valueCount, const char **types,
                                                  const char **values);

typedef void (*MessageCallback)(const char *topic, const char *uuid, int size, const char *data,
                                void *closure);

/**
 * @brief 断点续传消息发布
 * @details 发布支持断点续传的消息，自动处理大消息的分片传输和网络中断后的续传
 * @param [in] pluginManager 插件管理器
 * @param [in] topic 消息主题
 * @param [in] data 消息数据
 * @param [in] length 数据长度
 * @return 1: success. else: error
 * @note 底层自动处理消息分片、重发和断点续传机制
 */
int PublishResumableMessage(void *pluginManager, const char *topic, const char *data, int length);

/**
 * @brief 断点续传消息订阅
 * @details 订阅支持断点续传的消息，自动处理消息分片的接收、去重和合并
 * @param [in] pluginManager 插件管理器
 * @param [in] topic 消息主题
 * @param [in] callback 消息回调函数
 * @return 1: success. else: error
 * @note 回调函数将在消息完整接收并合并后被调用
 */
int SubscribeResumableMessage(void *pluginManager, const char *topic, MessageCallback callback);

/**
 * @brief 可靠消息发布（失效重传）
 * @details 发布具有失效重传机制的可靠消息，确保消息可靠传输
 * @param [in] pluginManager 插件管理器
 * @param [in] topic 消息主题
 * @param [in] data 消息数据
 * @param [in] length 数据长度
 * @return 1: success. else: error
 * @note 底层自动处理消息重传，直到收到确认或超时
 */
int PublishReliableMessage(void *pluginManager, const char *topic, const char *data, int length);

/**
 * @brief 可靠消息订阅
 * @details 订阅具有失效重传机制的可靠消息
 * @param [in] pluginManager 插件管理器
 * @param [in] topic 消息主题
 * @param [in] callback 消息回调函数
 * @return 1: success. else: error
 * @note 确保消息可靠接收，自动发送确认
 */
int SubscribeReliableMessage(void *pluginManager, const char *topic, MessageCallback callback);

/**
 * @brief 发送控制命令
 * @details 向指定节点发送控制命令消息
 * @param [in] pluginManager 插件管理器
 * @param [in] targetNode 目标节点标识
 * @param [in] command 控制命令数据
 * @param [in] length 命令数据长度
 * @return 1: success. else: error
 * @note 控制命令通过固定主题 "control.command" 传输
 */
int SendControlCommand(void *pluginManager, const char *targetNode, const char *command,
                       int length);

/**
 * @brief 订阅控制命令
 * @details 订阅来自其他节点的控制命令消息
 * @param [in] pluginManager 插件管理器
 * @param [in] callback 消息回调函数
 * @return 1: success. else: error
 * @note 监听固定主题 "control.command" 上的消息
 */
int SubscribeControlCommand(void *pluginManager, MessageCallback callback);

/**
 * @brief 发送状态数据
 * @details 向指定节点发送状态数据消息
 * @param [in] pluginManager 插件管理器
 * @param [in] targetNode 目标节点标识
 * @param [in] statusData 状态数据
 * @param [in] length 数据长度
 * @return 1: success. else: error
 * @note 状态数据通过固定主题 "status.data" 传输
 */
int SendStatusData(void *pluginManager, const char *targetNode, const char *statusData, int length);

/**
 * @brief 订阅状态数据
 * @details 订阅来自其他节点的状态数据消息
 * @param [in] pluginManager 插件管理器
 * @param [in] callback 消息回调函数
 * @return 1: success. else: error
 * @note 监听固定主题 "status.data" 上的消息
 */
int SubscribeStatusData(void *pluginManager, MessageCallback callback);

/**
 * @brief 发送任务规划信息
 * @details 向指定节点发送任务规划信息
 * @param [in] pluginManager 插件管理器
 * @param [in] targetNode 目标节点标识
 * @param [in] planningData 任务规划数据
 * @param [in] length 数据长度
 * @return 1: success. else: error
 * @note 任务规划信息通过固定主题 "task.planning" 传输
 */
int SendTaskPlanningInfo(void *pluginManager, const char *targetNode, const char *planningData,
                         int length);

/**
 * @brief 订阅任务规划信息
 * @details 订阅来自其他节点的任务规划信息
 * @param [in] pluginManager 插件管理器
 * @param [in] callback 消息回调函数
 * @return 1: success. else: error
 * @note 监听固定主题 "task.planning" 上的消息
 */
int SubscribeTaskPlanningInfo(void *pluginManager, MessageCallback callback);

/**
 * @brief 发送调度信息
 * @details 向指定节点发送调度信息
 * @param [in] pluginManager 插件管理器
 * @param [in] targetNode 目标节点标识
 * @param [in] schedulingData 调度信息数据
 * @param [in] length 数据长度
 * @return 1: success. else: error
 * @note 调度信息通过固定主题 "scheduling.info" 传输
 */
int SendSchedulingInfo(void *pluginManager, const char *targetNode, const char *schedulingData,
                       int length);

/**
 * @brief 订阅调度信息
 * @details 订阅来自其他节点的调度信息
 * @param [in] pluginManager 插件管理器
 * @param [in] callback 消息回调函数
 * @return 1: success. else: error
 * @note 监听固定主题 "scheduling.info" 上的消息
 */
int SubscribeSchedulingInfo(void *pluginManager, MessageCallback callback);

/**
 * @brief 发送资源状态信息
 * @details 向指定节点发送资源状态信息
 * @param [in] pluginManager 插件管理器
 * @param [in] targetNode 目标节点标识
 * @param [in] resourceData 资源状态数据
 * @param [in] length 数据长度
 * @return 1: success. else: error
 * @note 资源状态信息通过固定主题 "resource.status" 传输
 */
int SendResourceStatus(void *pluginManager, const char *targetNode, const char *resourceData,
                       int length);

/**
 * @brief 订阅资源状态信息
 * @details 订阅来自其他节点的资源状态信息
 * @param [in] pluginManager 插件管理器
 * @param [in] callback 消息回调函数
 * @return 1: success. else: error
 * @note 监听固定主题 "resource.status" 上的消息
 */
int SubscribeResourceStatus(void *pluginManager, MessageCallback callback);

/**
 * @brief 发布指令
 * @details 向指定主题发布指令消息，基于DoPublish方法封装
 * @param [in] pluginManager 插件管理器
 * @param [in] topic 指令主题
 * @param [in] command 指令数据
 * @param [in] length 指令数据长度
 * @return 1: success. else: error
 */
int PublishCommand(void *pluginManager, const char *topic, const char *command, int length);

/**
 * @brief 订阅指令
 * @details 订阅指定主题的指令消息，基于DoSubscribe方法封装
 * @param [in] pluginManager 插件管理器
 * @param [in] topic 指令主题
 * @param [in] callback 消息回调函数
 * @return 1: success. else: error
 */
int SubscribeCommand(void *pluginManager, const char *topic, MessageCallback callback);

/**
 * @brief 请求指令
 * @details 向指定主题发送指令请求消息，基于MessagingService的Request方法封装
 * @param [in] pluginManager 插件管理器
 * @param [in] topic 请求主题
 * @param [in] request 请求数据
 * @param [in] length 请求数据长度
 * @param [in] responseCallback 响应回调函数
 * @param [in] closure 回调函数上下文
 * @return 1: success. else: error
 */
int RequestCommand(void *pluginManager, const char *topic, const char *request, int length,
                   MessageCallback responseCallback, void *closure);

/**
 * @brief 回复指令
 * @details 向指定UUID发送指令回复消息，基于MessagingService的Reply方法封装
 * @param [in] pluginManager 插件管理器
 * @param [in] uuid 请求消息的UUID
 * @param [in] reply 回复数据
 * @param [in] length 回复数据长度
 * @return 1: success. else: error
 */
int ReplyCommand(void *pluginManager, const char *uuid, const char *reply, int length);

/**
 * @brief 发布状态信息
 * @details 向指定主题发布状态信息，基于DoPublish方法封装
 * @param [in] pluginManager 插件管理器
 * @param [in] topic 状态信息主题
 * @param [in] statusInfo 状态信息数据
 * @param [in] length 状态信息数据长度
 * @return 1: success. else: error
 */
int PublishStatusInfo(void *pluginManager, const char *topic, const char *statusInfo, int length);

/**
 * @brief 订阅状态信息
 * @details 订阅指定主题的状态信息，基于DoSubscribe方法封装
 * @param [in] pluginManager 插件管理器
 * @param [in] topic 状态信息主题
 * @param [in] callback 消息回调函数
 * @return 1: success. else: error
 */
int SubscribeStatusInfo(void *pluginManager, const char *topic, MessageCallback callback);

/**
 * @brief 请求状态信息
 * @details 向指定主题发送状态信息请求消息，基于MessagingService的Request方法封装
 * @param [in] pluginManager 插件管理器
 * @param [in] topic 请求主题
 * @param [in] request 请求数据
 * @param [in] length 请求数据长度
 * @param [in] responseCallback 响应回调函数
 * @param [in] closure 回调函数上下文
 * @return 1: success. else: error
 */
int RequestStatusInfo(void *pluginManager, const char *topic, const char *request, int length,
                      MessageCallback responseCallback, void *closure);

/**
 * @brief 回复状态信息
 * @details 向指定UUID发送状态信息回复消息，基于MessagingService的Reply方法封装
 * @param [in] pluginManager 插件管理器
 * @param [in] uuid 请求消息的UUID
 * @param [in] reply 回复数据
 * @param [in] length 回复数据长度
 * @return 1: success. else: error
 */
int ReplyStatusInfo(void *pluginManager, const char *uuid, const char *reply, int length);

/**
 * @brief 发布小文件
 * @details 向指定主题发布小文件数据，基于DoPublish方法封装
 * @param [in] pluginManager 插件管理器
 * @param [in] topic 小文件主题
 * @param [in] fileData 文件数据
 * @param [in] length 文件数据长度
 * @return 1: success. else: error
 */
int PublishSmallFile(void *pluginManager, const char *topic, const char *fileData, int length);

/**
 * @brief 订阅小文件
 * @details 订阅指定主题的小文件，基于DoSubscribe方法封装
 * @param [in] pluginManager 插件管理器
 * @param [in] topic 小文件主题
 * @param [in] callback 消息回调函数
 * @return 1: success. else: error
 */
int SubscribeSmallFile(void *pluginManager, const char *topic, MessageCallback callback);

/**
 * @brief 请求小文件
 * @details 向指定主题发送小文件请求消息，基于MessagingService的Request方法封装
 * @param [in] pluginManager 插件管理器
 * @param [in] topic 请求主题
 * @param [in] request 请求数据
 * @param [in] length 请求数据长度
 * @param [in] responseCallback 响应回调函数
 * @param [in] closure 回调函数上下文
 * @return 1: success. else: error
 */
int RequestSmallFile(void *pluginManager, const char *topic, const char *request, int length,
                     MessageCallback responseCallback, void *closure);

/**
 * @brief 回复小文件
 * @details 向指定UUID发送小文件回复消息，基于MessagingService的Reply方法封装
 * @param [in] pluginManager 插件管理器
 * @param [in] uuid 请求消息的UUID
 * @param [in] reply 回复数据
 * @param [in] length 回复数据长度
 * @return 1: success. else: error
 */
int ReplySmallFile(void *pluginManager, const char *uuid, const char *reply, int length);

/**
 * @brief  产品数据入库
 * @param[in]   pluginManager 插件管理器
 * @param[in]   name       产品唯一标识符
 * @param[in]   dataOwner  应用名称
 * @param[in]   data       数据
 * @param[in]   length     数据长度
 * @return   1: success. else: error
 */
int ProductDataStore(void *pluginManager, const char *name, const char *dataOwner, const char *data,
                     int length);

/**
 * @brief  产品映射关系解析
 * @param[in]   pluginManager 插件管理器
 * @param[in]   filePath    json 数据
 * @param[in]   length     json 数据长度
 * @param[in]   dataOwner   应用名称
 * @return     1: success. else: error
 * @note       解析结果可以通过QueryProductFilename接口查询
 */
// json 文件暂时默认为 Key:value的string格式
int ProductAnalysis(void *pluginManager, const char *data, int length, const char *dataOwner);

/**
 * @brief     查询产品映射关系
 * @param[in]   pluginManager 插件管理器
 * @param[in]   ProductName    产品唯一标识符
 * @param[in]   dataOwner      应用名称
 * @param[out]  fileName       对应的文件名称
 * @return   fileName size > 0, < 0 表示错误
 * @note     函数会自行为fileName分配内存，请传入空指针，调用后需要释放fileName
*/
int QueryProductFilename(void *pluginManager, const char *ProductName, const char *dataOwner,
                         char **fileName);

/**
 * @brief 上传分片对象
 * @param[in]   pluginManager 插件管理器
 * @param[in] key 数据唯一标识符
 * @param[in] owner 数据所有者
 * @param[in] currentSegmentId 当前分片编号（从1开始）
 * @param[in] totalSegmentCount 分片总数
 * @param[in] totalSize 数据总大小
 * @param[in] segmentSize 当前分片大小
 * @param[in] segmentData 当前分片数据
 * @return  1: success. else: error
 * @details
 * 外部接口说明如下：
 * - 封装后的URL: /Object
 * - 请求HTTP Method：POST
 * - 请求参数通过HTTP Header传递：
 *   - x-cloud-key：数据唯一标识符
 *   - x-cloud-owner：数据所有者
 *   - x-cloud-current-segment-id：当前分片编号（从1开始）
 *   - x-cloud-total-segment-count：分片总数
 *   - x-cloud-total-size：数据总大小
 *   - x-cloud-segment-size：当前分片大小
 * - 请求体：当前分片数据
 * - 响应状态码：
 *   - 200 OK：返回值为true
 *   - 500 Internal Server Error：返回值为false
 */
int UploadSegmentObject(void *pluginManager, const char *key, const char *owner,
                        int currentSegmentId, int totalSegmentCount, int totalSize, int segmentSize,
                        const char *segmentData);

/**
 * @brief Upload object
 * @param [in] pluginManager 插件管理器
 * @param [in] name Data name
 * @param [in] dataOwner Data owner(software or hardware module ....)
 * @param [in] data Original data
 * @param [in] dataLength Data length
 * @param [in] metadata object metadata data (json format)
 * @param [in] metadataLength object metadata length
 * @return 1: success. else: error
 * @details
 * 外部接口说明如下：
 * - 封装后的URL: /Object
 * - 请求HTTP Method：POST
 * - 请求参数通过HTTP Header传递：
 *   - x-cloud-key：数据唯一标识符
 *   - x-cloud-owner：数据所有者
 *   - x-cloud-size：数据大小
 *   - x-cloud-metadata：元数据（JSON格式）
 * - 请求体：数据
 * - 响应状态码：
 *   - 200 OK：返回值为true
 *   - 500 Internal Server Error：返回值为false
 */
int UploadObject(void *pluginManager, const char *name, const char *dataOwner, const char *data,
                 int dataLength, const char *metadata, int metadataLength);

/**
 * @brief     根据元数据查找对象数据
 * @param[in]   pluginManager 插件管理器
 * @param[in]   dataOwner  对象所有者
 * @param[in]   timeStamp  对象元数据时间戳字段
 * @param[in]   taskId     对象元数据任务id字段
 * @param[in]   status     对象状态id字段
 * @param[out]   data      对象数据
 * @return size > 0, < 0 means error
 * @note 函数会自行为data分配内存，请传入空指针，调用后需要释放data
 * @details
 * 外部接口说明如下：
 * - 封装后的URL: /Object
 * - 请求HTTP Method：GET
 * - 请求参数通过HTTP Header传递：
 *   - x-cloud-owner：数据所有者
 *   - x-cloud-time-stamp：对象元数据时间戳字段
 *   - x-cloud-task-id：对象元数据任务ID字段
 *   - x-cloud-status：对象状态ID字段
 * - 响应状态码：
 *   - 200 OK：返回值大于0
 *   - 404 Not Found：返回值为0
 *   - 500 Internal Server Error：返回值小于0
 * - 响应体：对象数据
 */
int QueryObjectUseMeta(void *pluginManager, const char *dataOwner, const char *timeStamp,
                       const char *taskId, const char *status, char **data);

/**
 * @brief     Get the Object Metadata object
 * @details   获取对象指定元数据字段的值
 * @param[in]   pluginManager 插件管理器
 * @param[in]   name       对象名字
 * @param[in]   dataOwner  对象所有者
 * @param[in]   metaName   对象元数据字段名称
 * @param[out]  metaValue  对象元数据字段请求对应的值
 * @return size > 0, < 0 means error
 * @note 函数会自行为metaValue分配内存，请传入空指针，调用后需要释放metaValue
 * @details
 * 外部接口说明如下：
 * - 封装后的URL: /Object
 * - 请求HTTP Method：HEAD
 * - 请求参数通过HTTP Header传递：
 *   - x-cloud-key：对象唯一标识符
 *   - x-cloud-owner：对象所有者
 *   - x-cloud-meta-name：对象元数据字段名称
 * - 响应状态码：
 *   - 200 OK：返回值大于0
 *   - 404 Not Found：返回值为0
 *   - 500 Internal Server Error：返回值小于0
 * - 响应Header
 *   - x-cloud-meta-value：对象元数据字段对应的值
 */
int GetObjectMetadata(void *pluginManager, const char *name, const char *dataOwner,
                      const char *metaName, char **metaValue);

/**
 * @brief     Set the Object Metadata object
 * @details   设置对象指定元数据字段的值
 * @param[in]   pluginManager 插件管理器
 * @param[in]   name       对象名字
 * @param[in]   dataOwner  对象所有者
 * @param[in]   metaName   对象元数据字段名称
 * @param[in]   metaValue  对象元数据字段对应的值
 * @return    1: success. else: error
 * @details
 * 外部接口说明如下：
 * - 封装后的URL: /Object
 * - 请求HTTP Method：POST
 * - 请求参数通过HTTP Header传递：
 *   - x-cloud-key：对象唯一标识符
 *   - x-cloud-owner：对象所有者
 *   - x-cloud-meta-name：对象元数据字段名称
 *   - x-cloud-meta-value：对象元数据字段对应的值
 * - 响应状态码：
 *   - 200 OK：返回值为true
 *   - 500 Internal Server Error：返回值为false
 */
int SetObjectMetadata(void *pluginManager, const char *name, const char *dataOwner,
                      const char *metaName, const char *metaValue);

/**
 * @brief     Delete the Object Metadata object
 * @details   删除对象指定元数据字段的值
 * @param[in]   pluginManager 插件管理器
 * @param[in]   name       对象名字
 * @param[in]   dataOwner  对象所有者
 * @param[in]   metaName   对象元数据字段名称
 * @return   1: success. else: error
 * @details
 * 外部接口说明如下：
 * - 封装后的URL: /Object
 * - 请求HTTP Method：DELETE
 * - 请求参数通过HTTP Header传递：
 *   - x-cloud-key：对象唯一标识符
 *   - x-cloud-owner：对象所有者
 *   - x-cloud-meta-name：对象元数据字段名称
 * - 响应状态码：
 *   - 200 OK：返回值为true
 *   - 500 Internal Server Error：返回值为false
 */
int DeleteObjectMetadata(void *pluginManager, const char *name, const char *dataOwner,
                         const char *metaName);

/**
 * @brief     Update the Object Metadata object
 * @details   更新对象指定元数据字段的值
 * @param[in]   pluginManager 插件管理器
 * @param[in]   name       对象名字
 * @param[in]   dataOwner  对象所有者
 * @param[in]   metaName   对象元数据字段名称
 * @param[in]   metaValue  对象元数据字段对应的值
 * @return   1: success. else: error
 * @details
 * 外部接口说明如下：
 * - 封装后的URL: /Object
 * - 请求HTTP Method：PUT
 * - 请求参数通过HTTP Header传递：
 *   - x-cloud-key：对象唯一标识符
 *   - x-cloud-owner：对象所有者
 *   - x-cloud-meta-name：对象元数据字段名称
 *   - x-cloud-meta-value：对象元数据字段对应的值
 * - 响应状态码：
 *   - 200 OK：返回值为true
 *   - 500 Internal Server Error：返回值为false
 */
int UpdateObjectMetadata(void *pluginManager, const char *name, const char *dataOwner,
                         const char *metaName, const char *metaValue);

/**
 * @brief sync object
 * @param [in] pluginManager 插件管理器
 * @param [in] name object name
 * @param [in] dataOwner object owner(software or hardware module ....)
 * @param [in] to  the node  sync object to
 * @return true if success
 * @details
 * 外部接口说明如下：
 * - 封装后的URL: /Object/Sync
 * - 请求HTTP Method：POST
 * - 请求参数通过HTTP Header传递：
 *   - x-cloud-key：对象唯一标识符
 *   - x-cloud-owner：对象所有者
 *   - x-cloud-to：对象同步的目标节点
 * - 响应状态码：
 *   - 200 OK：返回值为true
 *   - 500 Internal Server Error：返回值为false
 */
int SyncObject(void *pluginManager, const char *name, const char *dataOwner, const char *to);

/**
 * @brief Download object
 * @param [in] pluginManager 插件管理器
 * @param [in] name object name
 * @param [in] dataOwner  object owner(software or hardware module ....)
 * @param [out] data object
 * @return size > 0, < 0 means error
 * @note 函数会自行为data分配内存，请传入空指针，调用后需要释放data
 * @details
 * 外部接口说明如下：
 * - 封装后的URL: /Object
 * - 请求HTTP Method：GET
 * - 请求参数通过HTTP Header传递：
 *   - x-cloud-key：对象唯一标识符
 *   - x-cloud-owner：数据所有者
 * - 响应状态码：
 *   - 200 OK：返回值大于0
 *   - 404 Not Found：返回值为0
 *   - 500 Internal Server Error：返回值小于0
 * - 响应体：对象数据
 */
int DownloadObject(void *pluginManager, const char *name, const char *dataOwner, char **data);

#ifdef __cplusplus
}
#endif

#endif // LIBFLEET_DATAMGR_H
