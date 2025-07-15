// Copyright (c) 2025 Institute of Software, Chinese Academy of Sciences
// Author: Zhen Tang <tangzhen12@otcaix.iscas.ac.cn>
// Affiliation: Institute of Software, Chinese Academy of Sciences

#include "StringTools.h"
#include <string>
#include <vector>

namespace Fleet::DataManager::Core {
void StringSplit(const std::string &source, char delimiter, std::vector<std::string> &result) {
    auto lastPos = source.find_first_not_of(delimiter, 0);
    auto pos = source.find_first_of(delimiter, lastPos);
    while (pos != std::string::npos || lastPos != std::string::npos) {
        result.push_back(source.substr(lastPos, pos - lastPos));
        lastPos = source.find_first_not_of(delimiter, pos);
        pos = source.find_first_of(delimiter, lastPos);
    }
}
} // namespace Fleet::DataManager::Core
