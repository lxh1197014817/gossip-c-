// Copyright (c) 2025 Institute of Software, Chinese Academy of Sciences
// Author: Zhen Tang <tangzhen12@otcaix.iscas.ac.cn>
// Affiliation: Institute of Software, Chinese Academy of Sciences

/**
 * @file DataBlock.h
 * @brief 数据块封装类
 * @details 提供二进制数据的内存管理和访问功能，用于存储系统中的数据传输
 * @author 唐震 <tangzhen12@otcaix.iscas.ac.cn>
 * @date 2025-07-02
 */

#ifndef FLEET_DATA_MANAGER_STORAGE_DATA_BLOCK_H
#define FLEET_DATA_MANAGER_STORAGE_DATA_BLOCK_H

#include <cstdint>
#include <cstring>

namespace Fleet::DataManager::Storage {
/**
 * @brief 数据块类
 * @details 封装二进制数据的存储和访问，提供内存管理和数据拷贝功能
 * @note 对象析构时会自动释放内存，禁用拷贝构造和赋值操作
 */
class DataBlock {
  public:
    /**
     * @brief 构造数据块
     * @details 创建数据块并拷贝输入数据到内部缓冲区
     * @param[in] size 数据大小，单位字节
     * @param[in] data 数据内容指针
     * @note 会分配新内存并拷贝数据，调用方可安全释放原数据
     */
    DataBlock(uint64_t size, const char *data) {
        this->size = size;
        this->data = new char[this->size];
        memcpy(this->data, data, size);
    }

    /**
     * @brief 析构函数
     * @details 释放内部分配的数据缓冲区
     */
    virtual ~DataBlock() {
        delete[] this->data;
    }

    /**
     * @brief 禁用拷贝构造函数
     */
    DataBlock(const DataBlock &another) = delete;

    /**
     * @brief 禁用赋值操作符
     */
    DataBlock &operator=(const DataBlock &another) = delete;

    /**
     * @brief 获取数据大小
     * @return 数据大小，单位字节
     */
    uint64_t GetSize() const {
        return this->size;
    }

    /**
     * @brief 获取数据内容指针
     * @return 数据内容指针
     * @note 返回的指针生命周期与对象相同，调用方不应释放
     */
    const char *GetData() {
        return this->data;
    }

  private:
    /// 数据大小，单位字节
    uint64_t size;
    /// 数据内容缓冲区
    char *data;
};
} // namespace Fleet::DataManager::Storage

#endif //FLEET_DATA_MANAGER_STORAGE_DATA_BLOCK_H
