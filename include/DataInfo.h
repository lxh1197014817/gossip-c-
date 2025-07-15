// Copyright (c) 2025 Institute of Software, Chinese Academy of Sciences
// Author: Zhen Tang <tangzhen12@otcaix.iscas.ac.cn>
// Affiliation: Institute of Software, Chinese Academy of Sciences

/**
 * @file DataInfo.h
 * @brief 数据元信息管理
 * @details 提供数据的元信息管理，包括数据标识、大小、更新时间和存储位置等信息
 * @author 唐震 <tangzhen12@otcaix.iscas.ac.cn>
 * @date 2025-07-02
 */

#ifndef FLEET_DATA_MANAGER_STORAGE_DATA_INFO_H
#define FLEET_DATA_MANAGER_STORAGE_DATA_INFO_H

#include <string>
#include <vector>

namespace Fleet::DataManager::Storage {
/**
 * @brief 数据元信息类
 * @details 封装数据的元信息，包括标识信息、大小、最后更新时间和存储位置列表
 */
class DataInfo {
  public:
    /**
     * @brief 构造数据元信息对象
     * @param[in] application 应用名称
     * @param[in] dataType 数据类型
     * @param[in] name 数据名称
     * @param[in] version 版本信息
     * @param[in] size 数据大小，单位字节
     * @param[in] lastUpdate 最后更新时间
     * @param[in] locations 存储位置列表
     */
    DataInfo(const std::string &application, const std::string &dataType, const std::string &name,
             const std::string &version, uint64_t size, time_t lastUpdate,
             const std::vector<std::string> &locations) {
        this->application = application;
        this->dataType = dataType;
        this->name = name;
        this->version = version;
        this->size = size;
        this->lastUpdate = lastUpdate;
        this->locations.clear();
        for (const auto &location : locations) {
            this->locations.push_back(location);
        }
    }

    /**
     * @brief 虚析构函数
     */
    virtual ~DataInfo() = default;

    /**
     * @brief 禁用拷贝构造函数
     */
    DataInfo(const DataInfo &another) = delete;

    /**
     * @brief 禁用赋值操作符
     */
    DataInfo &operator=(const DataInfo &another) = delete;

    /**
     * @brief 获取应用名称
     * @return 应用名称字符串引用
     */
    const std::string &GetApplication() const {
        return this->application;
    }

    /**
     * @brief 获取数据类型
     * @return 数据类型字符串引用
     */
    const std::string &GetDataType() const {
        return this->dataType;
    }

    /**
     * @brief 获取数据名称
     * @return 数据名称字符串引用
     */
    const std::string &GetName() const {
        return this->name;
    }

    /**
     * @brief 获取版本信息
     * @return 版本信息字符串引用
     */
    const std::string &GetVersion() const {
        return this->version;
    }

    /**
     * @brief 获取数据大小
     * @return 数据大小，单位字节
     */
    uint64_t GetSize() const {
        return this->size;
    }

    /**
     * @brief 获取最后更新时间
     * @return 最后更新时间戳
     */
    time_t GetLastUpdate() const {
        return this->lastUpdate;
    }

    /**
     * @brief 获取存储位置列表
     * @return 存储位置字符串列表引用
     */
    const std::vector<std::string> &GetLocations() const {
        return this->locations;
    }

  private:
    /// 应用名称，标识数据所属的应用程序
    std::string application;
    /// 数据类型，标识数据的类别或格式
    std::string dataType;
    /// 数据名称，数据项的具体标识符
    std::string name;
    /// 版本信息，数据的版本标识
    std::string version;
    /// 数据大小，单位字节
    uint64_t size;
    /// 最后更新时间戳
    time_t lastUpdate;
    /// 存储位置列表，记录数据在各个位置的存储路径
    std::vector<std::string> locations;
};
} // namespace Fleet::DataManager::Storage
#endif //FLEET_DATA_MANAGER_STORAGE_DATA_INFO_H
