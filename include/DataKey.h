// Copyright (c) 2025 Institute of Software, Chinese Academy of Sciences
// Author: Zhen Tang <tangzhen12@otcaix.iscas.ac.cn>
// Affiliation: Institute of Software, Chinese Academy of Sciences

/**
 * @file DataKey.h
 * @brief 数据键定义和管理
 * @details 提供数据在存储系统中的唯一标识符管理功能
 * @author 唐震 <tangzhen12@otcaix.iscas.ac.cn>
 * @date 2025-07-02
 */

#ifndef FLEET_DATA_MANAGER_STORAGE_DATA_KEY_H
#define FLEET_DATA_MANAGER_STORAGE_DATA_KEY_H

#include <string>
#include <cstdint>
namespace Fleet::DataManager::Storage {
/**
 * @brief 数据键管理类
 * @details 用于唯一标识存储系统中的数据项，包含应用名称、数据类型、数据名称和版本信息
 */
class DataKey {
  public:
    /**
     * @brief 构造完整数据键
     * @details 使用应用名称、数据类型、数据名称和版本信息创建数据键
     * @param[in] application 应用名称
     * @param[in] dataType 数据类型
     * @param[in] name 数据名称
     * @param[in] version 版本信息
     */
    DataKey(const std::string &application, const std::string &dataType, const std::string &name,
            const std::string &version);

    /**
     * @brief 构造默认版本数据键
     * @details 使用应用名称、数据类型和数据名称创建数据键，版本自动设为default
     * @param[in] application 应用名称
     * @param[in] dataType 数据类型
     * @param[in] name 数据名称
     */
    DataKey(const std::string &application, const std::string &dataType, const std::string &name);

    /**
     * @brief 从序列化数据构造数据键
     * @details 从二进制缓冲区反序列化构造数据键对象
     * @param[in] buffer 序列化数据缓冲区
     * @param[in,out] position 缓冲区读取位置，执行后会更新到下一个位置
     */
    DataKey(const std::string &buffer, uint32_t &position);

    /**
     * @brief 析构函数
     */
    virtual ~DataKey();

    /**
     * @brief 拷贝构造函数
     * @param[in] another 源数据键对象
     */
    DataKey(const DataKey &another);

    /**
     * @brief 禁用赋值操作符
     */
    DataKey &operator=(const DataKey &another) = delete;

    /**
     * @brief 数据键相等比较
     * @details 比较两个数据键的所有字段是否完全相同
     * @param[in] another 比较对象
     * @return 相等返回true，否则返回false
     */
    bool operator==(const DataKey &another) const;

    /**
     * @brief 获取应用名称
     * @return 应用名称字符串引用
     */
    const std::string &GetApplication() const;

    /**
     * @brief 获取数据类型
     * @return 数据类型字符串引用
     */
    const std::string &GetDataType() const;

    /**
     * @brief 获取数据名称
     * @return 数据名称字符串引用
     */
    const std::string &GetName() const;

    /**
     * @brief 获取版本信息
     * @return 版本信息字符串引用
     */
    const std::string &GetVersion() const;

    /**
     * @brief 序列化数据键
     * @details 将数据键对象序列化为二进制字符串格式
     * @return 序列化后的二进制数据
     */
    std::string Serialize();

  private:
    /// 应用名称，标识数据所属的应用程序
    std::string application;
    /// 数据类型，标识数据的类别或格式
    std::string dataType;
    /// 数据名称，数据项的具体标识符
    std::string name;
    /// 版本信息，数据的版本标识
    std::string version;

    /**
     * @brief 反序列化数据键
     * @details 从二进制缓冲区中解析数据键的各个字段
     * @param[in] buffer 序列化数据缓冲区
     * @param[in,out] position 缓冲区读取位置，执行后会更新
     * @return 反序列化成功返回true，失败返回false
     */
    bool Deserialize(const std::string &buffer, uint32_t &position);
};
} // namespace Fleet::DataManager::Storage
#endif //FLEET_DATA_MANAGER_STORAGE_DATA_KEY_H
