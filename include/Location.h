// Copyright (c) 2025 Institute of Software, Chinese Academy of Sciences
// Author: Zhen Tang <tangzhen12@otcaix.iscas.ac.cn>
// Affiliation: Institute of Software, Chinese Academy of Sciences

/**
 * @file Location.h
 * @brief 存储位置信息管理
 * @details 提供存储位置的设备名称和相对路径管理，支持序列化和反序列化
 * @author 唐震 <tangzhen12@otcaix.iscas.ac.cn>
 * @date 2025-07-02
 */

#ifndef FLEET_DATA_MANAGER_STORAGE_LOCATION_H
#define FLEET_DATA_MANAGER_STORAGE_LOCATION_H

#include <string>
#include <cstdint>
namespace Fleet::DataManager::Storage {
/**
 * @brief 存储位置类
 * @details 封装数据存储的具体位置信息，包括设备名称和相对路径，支持序列化传输
 */
class Location {
  public:
    /**
     * @brief 构造存储位置对象
     * @param[in] deviceName 设备名称
     * @param[in] relativePath 相对路径
     */
    Location(const std::string &deviceName, const std::string &relativePath) {
        this->deviceName = deviceName;
        this->relativePath = relativePath;
    }

    /**
     * @brief 从序列化数据构造存储位置
     * @details 从二进制缓冲区反序列化构造位置对象
     * @param[in] buffer 序列化数据缓冲区
     * @param[in,out] position 缓冲区读取位置，执行后会更新到下一个位置
     */
    Location(const std::string &buffer, uint32_t &position) {
        this->Deserialize(buffer, position);
    }

    /**
     * @brief 虚析构函数
     */
    virtual ~Location() = default;

    /**
     * @brief 拷贝构造函数
     * @param[in] another 源位置对象
     */
    Location(const Location &another) {
        this->deviceName = another.deviceName;
        this->relativePath = another.relativePath;
    }

    /**
     * @brief 禁用赋值操作符
     */
    Location &operator=(const Location &another) = delete;

    /**
     * @brief 获取设备名称
     * @return 设备名称字符串引用
     */
    const std::string &GetDeviceName() const {
        return this->deviceName;
    }

    /**
     * @brief 获取相对路径
     * @return 相对路径字符串引用
     */
    const std::string &GetRelativePath() const {
        return this->relativePath;
    }

    /**
     * @brief 更新存储位置信息
     * @param[in] newDeviceName 新的设备名称
     * @param[in] newRelativePath 新的相对路径
     * @return 更新成功返回true
     */
    bool Update(const std::string &newDeviceName, const std::string &newRelativePath) {
        this->deviceName = newDeviceName;
        this->relativePath = newRelativePath;
        return true;
    }

    /**
     * @brief 序列化存储位置
     * @details 将位置对象序列化为二进制字符串格式
     * @return 序列化后的二进制数据
     */
    std::string Serialize() {
        std::string ret;
        // 4 bytes: device name size
        uint32_t deviceNameSize = this->deviceName.size();
        ret.append(std::string((const char *) &deviceNameSize, sizeof(deviceNameSize)));
        // n bytes: device name
        ret.append(this->deviceName);
        // 4 bytes: relative path size
        uint32_t relativePathSize = this->relativePath.size();
        ret.append(std::string((const char *) &relativePathSize, sizeof(relativePathSize)));
        // n bytes: relative path
        ret.append(this->relativePath);
        return ret;
    }

  private:
    /// 设备名称，标识存储设备
    std::string deviceName;
    /// 相对路径，在设备上的存储路径
    std::string relativePath;

    /**
     * @brief 反序列化存储位置
     * @details 从二进制缓冲区中解析位置信息的各个字段
     * @param[in] buffer 序列化数据缓冲区
     * @param[in,out] position 缓冲区读取位置，执行后会更新
     * @return 反序列化成功返回true，失败返回false
     */
    bool Deserialize(const std::string &buffer, uint32_t &position) {
        // 4 bytes: device name size
        uint32_t deviceNameSize = *((uint32_t *) (buffer.data() + position));
        position += sizeof(deviceNameSize);
        // n bytes: device name
        this->deviceName = std::string((const char *) (buffer.data() + position), deviceNameSize);
        position += deviceNameSize;
        // 4 bytes: relative path size
        uint32_t relativePathSize = *((uint32_t *) (buffer.data() + position));
        position += sizeof(relativePathSize);
        // n bytes: relative path
        this->relativePath =
            std::string((const char *) (buffer.data() + position), relativePathSize);
        position += relativePathSize;
        return true;
    }
};
} // namespace Fleet::DataManager::Storage

#endif //FLEET_DATA_MANAGER_STORAGE_LOCATION_H
