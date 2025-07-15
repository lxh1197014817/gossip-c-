/**
 * @file StringTools.h
 * @brief 字符串处理工具
 * @details 提供字符串分割等常用字符串操作功能
 * @author 唐震 <tangzhen12@otcaix.iscas.ac.cn>
 * @date 2025-07-04
 */

#ifndef FLEET_DATA_MANAGER_CORE_STRING_TOOLS_H
#define FLEET_DATA_MANAGER_CORE_STRING_TOOLS_H

#include <string>
#include <vector>

namespace Fleet::DataManager::Core {

/**
 * @brief 按分隔符分割字符串
 * @details 函数仅返回非空的分割结果
 * @param[in] source 待分割的源字符串
 * @param[in] delimiter 分隔符字符
 * @param[out] result 分割结果字符串列表
 */
void StringSplit(const std::string &source, char delimiter, std::vector<std::string> &result);

} // namespace Fleet::DataManager::Core
#endif // FLEET_DATA_MANAGER_CORE_STRING_TOOLS_H
