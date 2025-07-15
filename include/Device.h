// Copyright (c) 2025 Institute of Software, Chinese Academy of Sciences
// Author: Zhen Tang <tangzhen12@otcaix.iscas.ac.cn>
// Affiliation: Institute of Software, Chinese Academy of Sciences

/**
 * @file Device.h
 * @brief 存储设备信息管理
 * @details 提供存储设备的配置信息管理，包括设备名称、描述、设备文件和存储目录
 * @author 唐震 <tangzhen12@otcaix.iscas.ac.cn>
 * @date 2025-07-02
 */

#ifndef FLEET_DATA_MANAGER_STORAGE_DEVICE_H
#define FLEET_DATA_MANAGER_STORAGE_DEVICE_H

#include <string>
#include <uuid/uuid.h>

namespace Fleet::DataManager::Storage {
/**
 * @brief 存储设备类
 * @details 封装存储设备的基本信息，包括设备标识、描述、设备文件路径和存储目录
 */
class Device {
  public:
    /**
     * @brief 构造存储设备对象
     * @param[in] name 设备名称
     * @param[in] description 设备描述
     * @param[in] deviceFile 设备文件路径
     * @param[in] directory 存储目录路径
     */
    Device(const std::string &name, const std::string &description, const std::string &deviceFile,
           const std::string &directory) {
        this->name = name;
        this->description = description;
        this->deviceFile = deviceFile;
        this->directory = directory;
    }

    /**
     * @brief 虚析构函数
     */
    virtual ~Device() = default;

    /**
     * @brief 禁用拷贝构造函数
     */
    Device(const Device &another) = delete;

    /**
     * @brief 禁用赋值操作符
     */
    Device &operator=(const Device &another) = delete;

    /**
     * @brief 获取设备名称
     * @return 设备名称字符串引用
     */
    const std::string &GetName() const {
        return this->name;
    }

    /**
     * @brief 获取设备描述
     * @return 设备描述字符串引用
     */
    const std::string &GetDescription() const {
        return this->description;
    }

    /**
     * @brief 获取设备文件路径
     * @return 设备文件路径字符串引用
     */
    const std::string &GetDeviceFile() const {
        return this->deviceFile;
    }

    /**
     * @brief 获取存储目录路径
     * @return 存储目录路径字符串引用
     */
    const std::string &GetDirectory() const {
        return this->directory;
    }

  private:
    /// 设备名称，用于标识存储设备
    std::string name;
    /// 设备描述，说明设备的用途和特性
    std::string description;
    /// 设备文件路径，指向物理存储设备
    std::string deviceFile;
    /// 存储目录路径，数据存储的文件系统路径
    std::string directory;
};
} // namespace Fleet::DataManager::Storage
#endif //FLEET_DATA_MANAGER_STORAGE_DEVICE_H
