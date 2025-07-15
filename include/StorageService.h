/**
 * @file StorageService.h
 * @brief 存储插件服务接口定义
 * @details 提供数据存储、设备管理、策略配置和数据库操作等完整的存储服务功能
 * @author 周亦轩 <zhouyixuan23@otcaix.iscas.ac.cn>
 * @date 2025-07-02
 */

#ifndef FLEET_DATA_MANAGER_STORAGE_STORAGE_SERVICE_H
#define FLEET_DATA_MANAGER_STORAGE_STORAGE_SERVICE_H

#include "DataBlock.h"
#include "DataInfo.h"
#include "DataKey.h"
#include "Device.h"
#include "Location.h"
#include "Strategy.h"
#include "libfleet-datamgr.h"
#include <memory>
#include <string>
#include <vector>

namespace Fleet::DataManager::Storage {
class NodeInfo;

/**
 * @brief 存储服务接口
 * @details 提供完整的数据存储管理功能，包括设备管理、存储策略、数据读写、编解码和数据库操作
 * @note 此接口为纯虚接口，由具体的存储插件实现
 */
class StorageService {
  public:
    /**
     * @brief 默认构造函数
     */
    StorageService() = default;

    /**
     * @brief 虚析构函数
     */
    virtual ~StorageService() = default;

    /**
     * @brief 禁用拷贝构造函数
     */
    StorageService(const StorageService &) = delete;

    /**
     * @brief 禁用赋值操作符
     */
    StorageService &operator=(const StorageService &) = delete;

    /**
     * @brief 创建存储设备
     * @details 在系统中注册新的存储设备，用于数据存储
     * @param[in] name 设备名称
     * @param[in] description 设备描述
     * @param[in] deviceFile 设备文件路径
     * @param[in] directory 存储目录路径
     * @return 创建成功返回true，失败返回false
     */
    virtual bool CreateDevice(const std::string &name, const std::string &description,
                              const std::string &deviceFile, const std::string &directory) = 0;

    /**
     * @brief 更新存储设备信息
     * @details 修改已存在设备的配置信息
     * @param[in] name 设备名称
     * @param[in] newDescription 新的设备描述
     * @param[in] newDeviceFile 新的设备文件路径
     * @param[in] newDirectory 新的存储目录路径
     * @return 更新成功返回true，失败返回false
     */
    virtual bool UpdateDevice(const std::string &name, const std::string &newDescription,
                              const std::string &newDeviceFile,
                              const std::string &newDirectory) = 0;

    /**
     * @brief 删除存储设备
     * @details 从系统中移除指定的存储设备
     * @param[in] name 设备名称
     * @return 删除成功返回true，失败返回false
     */
    virtual bool RemoveDevice(const std::string &name) = 0;

    /**
     * @brief 根据名称获取设备信息
     * @param[in] name 设备名称
     * @return 设备对象指针，未找到返回nullptr
     */
    virtual std::shared_ptr<Device> GetDeviceByName(const std::string &name) = 0;

    /**
     * @brief 获取默认存储设备
     * @return 默认设备对象指针，未设置返回nullptr
     */
    virtual std::shared_ptr<Device> GetDefaultDevice() = 0;

    /**
     * @brief 获取所有存储设备列表
     * @return 设备对象指针列表
     */
    virtual std::vector<std::shared_ptr<Device>> GetAllDevices() = 0;
    /**
     * @brief 创建存储策略
     * @details 定义数据存储的策略配置，包括存储位置、容错算法和生命周期
     * @param[in] name 策略名称
     * @param[in] description 策略描述
     * @param[in] locations 存储位置列表
     * @param[in] errorCorrectingAlgorithm 容错纠错算法
     * @param[in] integrityCheckAlgorithm 完整性校验算法
     * @param[in] lifeTimeInSecond 数据保存时长，单位秒
     * @return 创建成功返回true，失败返回false
     */
    virtual bool CreateStrategy(const std::string &name, const std::string &description,
                                const std::vector<Location> &locations,
                                const std::string &errorCorrectingAlgorithm,
                                const std::string &integrityCheckAlgorithm,
                                uint32_t lifeTimeInSecond) = 0;

    /**
     * @brief 更新存储策略
     * @details 修改已存在策略的配置信息
     * @param[in] name 策略名称
     * @param[in] newDescription 新的策略描述
     * @param[in] newLocations 新的存储位置列表
     * @param[in] newErrorCorrectingAlgorithm 新的容错纠错算法
     * @param[in] newIntegrityCheckAlgorithm 新的完整性校验算法
     * @param[in] newLifeTimeInSecond 新的数据保存时长，单位秒
     * @return 更新成功返回true，失败返回false
     */
    virtual bool UpdateStrategy(const std::string &name, const std::string &newDescription,
                                const std::vector<Location> &newLocations,
                                const std::string &newErrorCorrectingAlgorithm,
                                const std::string &newIntegrityCheckAlgorithm,
                                uint32_t newLifeTimeInSecond) = 0;

    /**
     * @brief 删除存储策略
     * @param[in] name 策略名称
     * @return 删除成功返回true，失败返回false
     */
    virtual bool RemoveStrategy(const std::string &name) = 0;

    /**
     * @brief 根据名称获取存储策略
     * @param[in] name 策略名称
     * @return 策略对象指针，未找到返回nullptr
     */
    virtual std::shared_ptr<Strategy> GetStrategyByName(const std::string &name) = 0;

    /**
     * @brief 获取默认存储策略
     * @return 默认策略对象指针，未设置返回nullptr
     */
    virtual std::shared_ptr<Strategy> GetDefaultStrategy() = 0;

    /**
     * @brief 获取所有存储策略列表
     * @return 策略对象指针列表
     */
    virtual std::vector<std::shared_ptr<Strategy>> GetAllStrategies() = 0;

    // ================= 存储配置文件管理 =================

    /**
     * @brief 创建应用数据类型的存储配置
     * @param[in] application 应用名称
     * @param[in] dataType 数据类型
     * @param[in] strategyName 策略名称
     * @return 创建成功返回true，失败返回false
     */
    virtual bool CreateProfile(const std::string &application, const std::string &dataType,
                               const std::string &strategyName) = 0;

    /**
     * @brief 更新应用数据类型的存储配置
     * @param[in] application 应用名称
     * @param[in] dataType 数据类型
     * @param[in] newStrategyName 新的策略名称
     * @return 更新成功返回true，失败返回false
     */
    virtual bool UpdateProfile(const std::string &application, const std::string &dataType,
                               const std::string &newStrategyName) = 0;

    /**
     * @brief 删除应用数据类型的存储配置
     * @param[in] application 应用名称
     * @param[in] dataType 数据类型
     * @return 删除成功返回true，失败返回false
     */
    virtual bool RemoveProfile(const std::string &application, const std::string &dataType) = 0;

    /**
     * @brief 获取应用数据类型的存储配置
     * @param[in] application 应用名称
     * @param[in] dataType 数据类型
     * @return 策略名称指针，未找到返回nullptr
     */
    virtual std::shared_ptr<std::string> GetProfile(const std::string &application,
                                                    const std::string &dataType) = 0;

    /**
     * @brief 为应用创建默认存储配置
     * @param[in] application 应用名称
     * @param[in] strategyName 策略名称
     * @return 创建成功返回true，失败返回false
     */
    virtual bool CreateProfileForApplication(const std::string &application,
                                             const std::string &strategyName) = 0;

    /**
     * @brief 更新应用的默认存储配置
     * @param[in] application 应用名称
     * @param[in] newStrategyName 新的策略名称
     * @return 更新成功返回true，失败返回false
     */
    virtual bool UpdateProfileForApplication(const std::string &application,
                                             const std::string &newStrategyName) = 0;

    /**
     * @brief 删除应用的默认存储配置
     * @param[in] application 应用名称
     * @return 删除成功返回true，失败返回false
     */
    virtual bool RemoveProfileForApplication(const std::string &application) = 0;

    /**
     * @brief 获取应用的默认存储配置
     * @param[in] application 应用名称
     * @return 策略名称指针，未找到返回nullptr
     */
    virtual std::shared_ptr<std::string>
    GetProfileForApplication(const std::string &application) = 0;

    /**
     * @brief 为数据类型创建默认存储配置
     * @param[in] dataType 数据类型
     * @param[in] strategyName 策略名称
     * @return 创建成功返回true，失败返回false
     */
    virtual bool CreateProfileForDataType(const std::string &dataType,
                                          const std::string &strategyName) = 0;

    /**
     * @brief 更新数据类型的默认存储配置
     * @param[in] dataType 数据类型
     * @param[in] newStrategyName 新的策略名称
     * @return 更新成功返回true，失败返回false
     */
    virtual bool UpdateProfileForDataType(const std::string &dataType,
                                          const std::string &newStrategyName) = 0;

    /**
     * @brief 删除数据类型的默认存储配置
     * @param[in] dataType 数据类型
     * @return 删除成功返回true，失败返回false
     */
    virtual bool RemoveProfileForDataType(const std::string &dataType) = 0;

    /**
     * @brief 获取数据类型的默认存储配置
     * @param[in] dataType 数据类型
     * @return 策略名称指针，未找到返回nullptr
     */
    virtual std::shared_ptr<std::string> GetProfileForDataType(const std::string &dataType) = 0;

    /**
     * @brief 查找最适合的存储配置
     * @details 按优先级查找：精确匹配 > 应用默认 > 数据类型默认 > 系统默认
     * @param[in] application 应用名称
     * @param[in] dataType 数据类型
     * @return 策略名称指针，未找到返回nullptr
     */
    virtual std::shared_ptr<std::string> FindProfile(const std::string &application,
                                                     const std::string &dataType) = 0;

    // ================= 数据读写操作 =================

    /**
     * @brief 读取数据的最新版本
     * @param[in] application 应用名称
     * @param[in] dataType 数据类型
     * @param[in] name 数据名称
     * @return 数据块对象指针，未找到返回nullptr
     */
    virtual std::shared_ptr<DataBlock> ReadData(const std::string &application,
                                                const std::string &dataType,
                                                const std::string &name) = 0;

    /**
     * @brief 读取数据的指定版本
     * @param[in] application 应用名称
     * @param[in] dataType 数据类型
     * @param[in] name 数据名称
     * @param[in] version 版本信息
     * @return 数据块对象指针，未找到返回nullptr
     */
    virtual std::shared_ptr<DataBlock> ReadData(const std::string &application,
                                                const std::string &dataType,
                                                const std::string &name,
                                                const std::string &version) = 0;

    /**
     * @brief 写入数据，自动生成版本
     * @param[in] application 应用名称
     * @param[in] dataType 数据类型
     * @param[in] name 数据名称
     * @param[in] dataBlock 数据块对象
     * @return 写入成功返回true，失败返回false
     */
    virtual bool WriteData(const std::string &application, const std::string &dataType,
                           const std::string &name,
                           const std::shared_ptr<DataBlock> &dataBlock) = 0;

    /**
     * @brief 写入数据的指定版本
     * @param[in] application 应用名称
     * @param[in] dataType 数据类型
     * @param[in] name 数据名称
     * @param[in] version 版本信息
     * @param[in] dataBlock 数据块对象
     * @return 写入成功返回true，失败返回false
     */
    virtual bool WriteData(const std::string &application, const std::string &dataType,
                           const std::string &name, const std::string &version,
                           const std::shared_ptr<DataBlock> &dataBlock) = 0;

    /**
     * @brief 删除数据的所有版本
     * @param[in] application 应用名称
     * @param[in] dataType 数据类型
     * @param[in] name 数据名称
     * @return 删除成功返回true，失败返回false
     */
    virtual bool RemoveData(const std::string &application, const std::string &dataType,
                            const std::string &name) = 0;

    /**
     * @brief 删除数据的指定版本
     * @param[in] application 应用名称
     * @param[in] dataType 数据类型
     * @param[in] name 数据名称
     * @param[in] version 版本信息
     * @return 删除成功返回true，失败返回false
     */
    virtual bool RemoveData(const std::string &application, const std::string &dataType,
                            const std::string &name, const std::string &version) = 0;

    /**
     * @brief 修复数据的最新版本
     * @details 检查并修复数据的完整性和一致性
     * @param[in] application 应用名称
     * @param[in] dataType 数据类型
     * @param[in] name 数据名称
     * @return 修复成功返回true，失败返回false
     */
    virtual bool RepairData(const std::string &application, const std::string &dataType,
                            const std::string &name) = 0;

    /**
     * @brief 修复数据的指定版本
     * @details 检查并修复数据的完整性和一致性
     * @param[in] application 应用名称
     * @param[in] dataType 数据类型
     * @param[in] name 数据名称
     * @param[in] version 版本信息
     * @return 修复成功返回true，失败返回false
     */
    virtual bool RepairData(const std::string &application, const std::string &dataType,
                            const std::string &name, const std::string &version) = 0;

    // ================= 数据关系管理 =================

    /**
     * @brief 更新数据的级联键列表
     * @details 设置数据的依赖关系，用于级联更新和删除
     * @param[in] application 应用名称
     * @param[in] dataType 数据类型
     * @param[in] name 数据名称
     * @param[in] cascadeKeys 级联键列表
     * @return 更新成功返回true，失败返回false
     */
    virtual bool UpdateCascadeKeys(const std::string &application, const std::string &dataType,
                                   const std::string &name,
                                   const std::vector<DataKey> &cascadeKeys) = 0;

    /**
     * @brief 更新指定版本数据的级联键列表
     * @param[in] application 应用名称
     * @param[in] dataType 数据类型
     * @param[in] name 数据名称
     * @param[in] version 版本信息
     * @param[in] cascadeKeys 级联键列表
     * @return 更新成功返回true，失败返回false
     */
    virtual bool UpdateCascadeKeys(const std::string &application, const std::string &dataType,
                                   const std::string &name, const std::string &version,
                                   const std::vector<DataKey> &cascadeKeys) = 0;

    /**
     * @brief 获取数据元信息的最新版本
     * @param[in] application 应用名称
     * @param[in] dataType 数据类型
     * @param[in] name 数据名称
     * @return 数据元信息对象指针，未找到返回nullptr
     */
    virtual std::shared_ptr<DataInfo> GetDataInfo(const std::string &application,
                                                  const std::string &dataType,
                                                  const std::string &name) = 0;

    /**
     * @brief 获取数据元信息的指定版本
     * @param[in] application 应用名称
     * @param[in] dataType 数据类型
     * @param[in] name 数据名称
     * @param[in] version 版本信息
     * @return 数据元信息对象指针，未找到返回nullptr
     */
    virtual std::shared_ptr<DataInfo> GetDataInfo(const std::string &application,
                                                  const std::string &dataType,
                                                  const std::string &name,
                                                  const std::string &version) = 0;

    // ================= 数据编解码 =================

    /**
     * @brief 编码数据
     * @details 根据指定类型对数据进行编码处理
     * @param[in] buffer 原始数据缓冲区
     * @param[in] len 数据长度
     * @param[in] type 编码类型
     * @return 编码后的数据块对象指针，失败返回nullptr
     */
    virtual std::shared_ptr<DataBlock> Encode(char *buffer, uint64_t len,
                                              const std::string &type) = 0;

    /**
     * @brief 解码数据
     * @details 根据指定类型对数据进行解码处理
     * @param[in] buffer 编码数据缓冲区
     * @param[in] len 数据长度
     * @param[in] type 解码类型
     * @return 解码后的数据块对象指针，失败返回nullptr
     */
    virtual std::shared_ptr<DataBlock> Decode(char *buffer, uint64_t len,
                                              const std::string &type) = 0;

    // ================= 存储空间管理 =================

    /**
     * @brief 设置存储空间限制大小
     * @param[in] size 限制大小，单位字节
     * @return 设置成功返回true，失败返回false
     */
    virtual bool SetSpaceLimitSize(uint64_t size) = 0;

    /**
     * @brief 获取存储空间限制大小
     * @param[out] size 限制大小，单位字节
     * @return 获取成功返回true，失败返回false
     */
    virtual bool GetSpaceLimitSize(uint64_t *size) = 0;

    /**
     * @brief 获取已使用存储空间大小
     * @param[out] size 已使用大小，单位字节
     * @return 获取成功返回true，失败返回false
     */
    virtual bool GetUsedSpaceSize(uint64_t *size) = 0;

    /**
     * @brief 获取总存储空间大小
     * @param[out] size 总空间大小，单位字节
     * @return 获取成功返回true，失败返回false
     */
    virtual bool GetTotalSpaceSize(uint64_t *size) = 0;

    // ================= 数据库操作 =================

    /**
     * @brief 连接到PostgreSQL数据库
     * @param[in] connectionString 数据库连接字符串
     * @return 数据库连接对象指针，失败返回nullptr
     */
    virtual void *ConnectToPostgreSql(const std::string &connectionString) = 0;

    /**
     * @brief 连接到SQLite数据库
     * @param[in] connectionString 数据库连接字符串
     * @return 数据库连接对象指针，失败返回nullptr
     */
    virtual void *ConnectToSqlite(const std::string &connectionString) = 0;

    /**
     * @brief 断开数据库连接
     * @param[in] connection 数据库连接对象指针
     * @return 断开成功返回true，失败返回false
     */
    virtual bool DisconnectFromDatabase(void *connection) = 0;

    /**
     * @brief 执行有结果集的SQL查询
     * @param[in] connection 数据库连接对象指针
     * @param[in] sql SQL查询语句
     * @param[out] columnNames 列名列表
     * @param[out] data 查询结果数据
     * @return 执行成功返回true，失败返回false
     */
    virtual bool ExecuteResult(void *connection, const std::string &sql,
                               std::vector<std::string> &columnNames,
                               std::vector<std::vector<std::shared_ptr<std::string>>> &data) = 0;

    /**
     * @brief 执行无结果集的SQL语句
     * @param[in] connection 数据库连接对象指针
     * @param[in] sql SQL语句
     * @return 执行成功返回true，失败返回false
     */
    virtual bool ExecuteNoResult(void *connection, const std::string &sql) = 0;

    /**
     * @brief 执行参数化的有结果集SQL查询
     * @param[in] connection 数据库连接对象指针
     * @param[in] sql 参数化SQL查询语句
     * @param[in] types 参数类型列表
     * @param[in] values 参数值列表
     * @param[out] columnNames 列名列表
     * @param[out] data 查询结果数据
     * @return 执行成功返回true，失败返回false
     */
    virtual bool ExecuteParameterizedResult(
        void *connection, const std::string &sql, const std::vector<std::string> &types,
        const std::vector<std::string> &values, std::vector<std::string> &columnNames,
        std::vector<std::vector<std::shared_ptr<std::string>>> &data) = 0;

    /**
     * @brief 执行参数化的无结果集SQL语句
     * @param[in] connection 数据库连接对象指针
     * @param[in] sql 参数化SQL语句
     * @param[in] types 参数类型列表
     * @param[in] values 参数值列表
     * @return 执行成功返回true，失败返回false
     */
    virtual bool ExecuteParameterizedNoResult(void *connection, const std::string &sql,
                                              const std::vector<std::string> &types,
                                              const std::vector<std::string> &values) = 0;

    // ================= 调度文件管理 =================

    /**
     * @brief 写入调度文件
     * @param[in] key 文件键值
     * @param[in] value 文件内容
     * @param[in] dataType 数据类型
     * @param[in] owner 所有者
     * @param[in] srcNode 源节点
     * @param[in] ctime 创建时间
     * @return 写入成功返回true，失败返回false
     */
    virtual bool WriteSchedulingFile(const std::string &key, const std::string &value,
                                     const std::string &dataType, const std::string &owner,
                                     const std::string &srcNode, const std::string &ctime) = 0;

    /**
     * @brief 读取调度文件
     * @param[in] key 文件键值
     * @param[in] dataType 数据类型
     * @param[in] owner 所有者
     * @param[out] srcNode 源节点
     * @param[out] cTime 创建时间
     * @return 文件内容，未找到返回空字符串
     */
    virtual std::string ReadSchedulingFile(const std::string &key, const std::string &dataType,
                                           const std::string &owner, std::string *srcNode,
                                           std::string *cTime) = 0;

    /**
     * @brief 删除调度文件
     * @param[in] key 文件键值
     * @param[in] dataType 数据类型
     * @param[in] owner 所有者
     * @return 删除成功返回true，失败返回false
     */
    virtual bool DeleteSchedulingFile(const std::string &key, const std::string &dataType,
                                      const std::string &owner) = 0;

    /**
     * @brief 清空调度目录
     * @details 删除调度目录中的所有文件
     */
    virtual void CleareSchedulingDir() = 0;
};
} // namespace Fleet::DataManager::Storage
#endif // FLEET_DATA_MANAGER_STORAGE_STORAGE_SERVICE_H