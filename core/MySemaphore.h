/**
 * @file MySemaphore.h
 * @brief 信号量同步原语实现
 * @details 基于条件变量和互斥锁实现的计数信号量
 * @author 唐震 <tangzhen12@otcaix.iscas.ac.cn>
 * @date 2025-06-29
 */

#ifndef FLEET_DATA_MANAGER_CORE_MY_SEMAPHORE_H
#define FLEET_DATA_MANAGER_CORE_MY_SEMAPHORE_H

#include <condition_variable>
#include <mutex>

namespace Fleet::DataManager::Core {
/**
 * @brief 计数信号量实现类
 * @details 使用条件变量和互斥锁实现的线程同步原语
 */
class MySemaphore {
  public:
    /**
     * @brief 构造信号量
     * @param[in] count 初始计数值
     */
    explicit MySemaphore(int count) {
        this->count = count;
    }

    /**
     * @brief 释放信号量，计数加一
     * @details 增加计数并通知等待的线程
     */
    void Notify() {
        {
            std::unique_lock<std::mutex> lock(this->mutex);
            this->count++;
        }
        this->conditionVariable.notify_one();
    }

    /**
     * @brief 等待信号量，计数减一
     * @details 如果计数为零则阻塞等待，否则减少计数并继续
     */
    void Wait() {
        std::unique_lock<std::mutex> lock(this->mutex);
        while (this->count == 0) {
            this->conditionVariable.wait(lock);
        }
        this->count--;
    }

  private:
    /// 信号量计数值
    int count;

    /// 保护计数的互斥锁
    std::mutex mutex;

    /// 用于线程等待和通知的条件变量
    std::condition_variable conditionVariable;
};
} // namespace Fleet::DataManager::Core

#endif //FLEET_DATA_MANAGER_CORE_MY_SEMAPHORE_H
