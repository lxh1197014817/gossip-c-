// Copyright (c) 2025 Institute of Software, Chinese Academy of Sciences
// Author: Zhen Tang <tangzhen12@otcaix.iscas.ac.cn>
// Affiliation: Institute of Software, Chinese Academy of Sciences

/**
 * @file Strategy.h
 * @brief 存储策略配置管理
 * @details 提供数据存储策略的配置管理，包括存储位置、容错算法、完整性校验和生命周期管理
 * @author 唐震 <tangzhen12@otcaix.iscas.ac.cn>
 * @date 2025-07-02
 */

#ifndef FLEET_DATA_MANAGER_STORAGE_STRATEGY_H
#define FLEET_DATA_MANAGER_STORAGE_STRATEGY_H

#include "Location.h"
#include <string>
#include <uuid/uuid.h>
#include <vector>
#include <cstdint>
namespace Fleet::DataManager::Storage {
/**
 * @brief 存储策略类
 * @details 定义数据存储的完整策略配置，包括多个存储位置、容错纠错算法、完整性校验算法和数据生命周期
 */
class Strategy {
  public:
    /**
     * @brief 构造存储策略对象
     * @param[in] name 策略名称
     * @param[in] description 策略描述
     * @param[in] locations 存储位置列表
     * @param[in] errorCorrectingAlgorithm 容错纠错算法
     * @param[in] integrityCheckAlgorithm 完整性校验算法
     * @param[in] lifeTimeInSecond 数据保存时长，单位秒
     */
    Strategy(const std::string &name, const std::string &description,
             const std::vector<Location> &locations, const std::string &errorCorrectingAlgorithm,
             const std::string &integrityCheckAlgorithm, uint32_t lifeTimeInSecond) {
        this->DoCreate(name, description, errorCorrectingAlgorithm, integrityCheckAlgorithm,
                       lifeTimeInSecond);
        this->locations.clear();
        int i = 0;
        for (i = 0; i < locations.size(); i++) {
            this->locations.push_back(locations.at(i));
        }
    }

    /**
     * @brief 从序列化位置数据构造存储策略
     * @param[in] name 策略名称
     * @param[in] description 策略描述
     * @param[in] serializedLocations 序列化的位置数据
     * @param[in] errorCorrectingAlgorithm 容错纠错算法
     * @param[in] integrityCheckAlgorithm 完整性校验算法
     * @param[in] lifeTimeInSecond 数据保存时长，单位秒
     */
    Strategy(const std::string &name, const std::string &description,
             const std::string &serializedLocations, const std::string &errorCorrectingAlgorithm,
             const std::string &integrityCheckAlgorithm, uint32_t lifeTimeInSecond) {
        this->DoCreate(name, description, errorCorrectingAlgorithm, integrityCheckAlgorithm,
                       lifeTimeInSecond);
        this->DeserializeLocations(serializedLocations);
    }

    /**
     * @brief 虚析构函数
     */
    virtual ~Strategy() = default;

    /**
     * @brief 禁用拷贝构造函数
     */
    Strategy(const Strategy &another) = delete;

    /**
     * @brief 禁用赋值操作符
     */
    Strategy &operator=(const Strategy &another) = delete;

    /**
     * @brief 获取策略名称
     * @return 策略名称字符串引用
     */
    const std::string &GetName() const {
        return this->name;
    }

    /**
     * @brief 获取策略描述
     * @return 策略描述字符串引用
     */
    const std::string &GetDescription() const {
        return this->description;
    }

    /**
     * @brief 获取存储位置列表
     * @return 存储位置列表引用
     */
    const std::vector<Location> &GetLocations() const {
        return this->locations;
    }

    /**
     * @brief 获取容错纠错算法
     * @return 容错纠错算法字符串引用
     */
    const std::string &GetErrorCorrectingAlgorithm() const {
        return this->errorCorrectingAlgorithm;
    }

    /**
     * @brief 获取完整性校验算法
     * @return 完整性校验算法字符串引用
     */
    const std::string &GetIntegrityCheckAlgorithm() const {
        return this->integrityCheckAlgorithm;
    }

    /**
     * @brief 获取数据保存时长
     * @return 数据保存时长，单位秒
     */
    uint32_t GetLifeTimeInSecond() const {
        return this->lifeTimeInSecond;
    }

    /**
     * @brief 序列化存储位置列表
     * @details 将位置列表序列化为二进制字符串格式
     * @return 序列化后的位置数据
     */
    std::string SerializeLocations() {
        std::string ret;
        // 4 bytes: location count
        uint32_t locationSize = this->locations.size();
        ret.append(std::string((const char *) &locationSize, sizeof(locationSize)));
        int i = 0;
        for (i = 0; i < this->locations.size(); i++) {
            ret.append(this->locations[i].Serialize());
        }
        return ret;
    }

  private:
    /// 策略名称，用于标识存储策略
    std::string name;
    /// 策略描述，说明策略的用途和特性
    std::string description;
    /// 存储位置列表，定义数据的存储位置
    std::vector<Location> locations;
    /// 容错纠错算法名称
    std::string errorCorrectingAlgorithm;
    /// 完整性校验算法名称
    std::string integrityCheckAlgorithm;
    /// 数据保存时长，单位秒
    uint32_t lifeTimeInSecond;

    /**
     * @brief 执行策略创建的通用逻辑
     * @param[in] name 策略名称
     * @param[in] description 策略描述
     * @param[in] errorCorrectingAlgorithm 容错纠错算法
     * @param[in] integrityCheckAlgorithm 完整性校验算法
     * @param[in] lifeTimeInSecond 数据保存时长
     */
    void DoCreate(const std::string &name, const std::string &description,
                  const std::string &errorCorrectingAlgorithm,
                  const std::string &integrityCheckAlgorithm, uint32_t lifeTimeInSecond) {
        this->name = name;
        this->description = description;
        this->errorCorrectingAlgorithm = errorCorrectingAlgorithm;
        this->integrityCheckAlgorithm = integrityCheckAlgorithm;
        this->lifeTimeInSecond = lifeTimeInSecond;
    }

    /**
     * @brief 反序列化存储位置列表
     * @details 从序列化数据中解析位置列表
     * @param[in] serializedLocation 序列化的位置数据
     */
    void DeserializeLocations(const std::string &serializedLocation) {
        uint32_t position = 0;
        this->locations.clear();
        // 4 bytes: location count
        uint32_t locationSize = *((uint32_t *) (serializedLocation.data() + position));
        position += sizeof(locationSize);
        int i = 0;
        for (i = 0; i < locationSize; i++) {
            this->locations.push_back(Location(serializedLocation, position));
        }
    }
};
} // namespace Fleet::DataManager::Storage
#endif //FLEET_DATA_MANAGER_STORAGE_STRATEGY_H
