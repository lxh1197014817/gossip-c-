// Copyright (c) 2025 Institute of Software, Chinese Academy of Sciences
// Author: Zhen Tang <tangzhen12@otcaix.iscas.ac.cn>
// Affiliation: Institute of Software, Chinese Academy of Sciences

#include "PluginContextImpl.h"
#include "Logger.h"
#include <fcntl.h>
#include <filesystem>
#include <fstream>
#include <sys/file.h>

#ifndef BUILD_VERSION
#define BUILD_VERSION "未知"
#endif

namespace Fleet::DataManager::Core {
PluginContextImpl::PluginContextImpl(PluginManager *pluginManager, uuid_t nodeId,
                                     const std::string &baseDirectory) {
    uuid_t uuid;
    if (nodeId != nullptr) {
        uuid_copy(uuid, nodeId);
    } else {
        uuid_generate(uuid);
    }
    char nodeIdString[37];
    uuid_unparse(uuid, nodeIdString);

    this->nodeId = nodeIdString;
    this->pluginManager = pluginManager;
    this->baseDirectory = baseDirectory;
    this->locked = false;
    this->lockHandle = -1;
    std::string lockFileName = this->baseDirectory + "/lock";
    if (!std::filesystem::exists(this->baseDirectory)) {
        std::filesystem::create_directories(this->baseDirectory);
    }

    this->lockHandle = open(lockFileName.c_str(), O_RDWR | O_CREAT | O_EXCL, 0666);
    if (this->lockHandle == -1) {
        if (errno != EEXIST) {
            Logger::ConsoleLogger().Error(SOURCE_LOCATION, "打开锁文件时出错: {}",
                                          strerror(errno));
            return;
        }

        // 文件已存在
        this->lockHandle = open(lockFileName.c_str(), O_RDWR | O_EXCL, 0666);
        if (this->lockHandle == -1) {
            Logger::ConsoleLogger().Error(SOURCE_LOCATION, "打开锁文件时出错: {}",
                                          strerror(errno));
            return;
        }
    }

    int status = flock(this->lockHandle, LOCK_EX | LOCK_NB);
    if (status == -1) {
        Logger::ConsoleLogger().Error(SOURCE_LOCATION, "加锁失败: {}",
                                      strerror(errno));
        Logger::ConsoleLogger().Error(SOURCE_LOCATION,
                                      "检测到多个节点使用了同一个目录，程序将退出");
        exit(1);
    } else {
        this->locked = true;
    }

    this->logDirectory = baseDirectory + "/log";
    this->dataDirectory = baseDirectory + "/data";
    this->databaseDirectory = baseDirectory + "/db";
    std::filesystem::create_directories(this->baseDirectory);
    std::filesystem::create_directories(this->logDirectory);
    std::filesystem::create_directories(this->dataDirectory);
    std::filesystem::create_directories(this->databaseDirectory);
    this->logger = std::make_unique<Logger>(this->nodeId, this->logDirectory + "/datamgr/" +
                                                              this->nodeId + ".log");
}

PluginContextImpl::~PluginContextImpl() {
    if (this->lockHandle != -1 && this->locked) {
        int status = flock(this->lockHandle, LOCK_UN);
        if (status == -1) {
            Logger::ConsoleLogger().Error(SOURCE_LOCATION, "解锁失败: {}",
                                          strerror(errno));
        }
    }
    if (std::filesystem::exists(this->baseDirectory + "/lock")) {
        std::filesystem::remove(this->baseDirectory + "/lock");
    }
}

const std::string &PluginContextImpl::GetNodeId() {
    return this->nodeId;
}

void *PluginContextImpl::GetService(const std::string &pluginName) {
    return this->pluginManager->GetService(pluginName);
}

const std::string &PluginContextImpl::GetBaseDirectory() {
    return this->baseDirectory;
}

const std::string &PluginContextImpl::GetDataDirectory() {
    return this->dataDirectory;
}

const std::string &PluginContextImpl::GetLogDirectory() {
    return this->logDirectory;
}

const std::string &PluginContextImpl::GetDatabaseDirectory() {
    return this->databaseDirectory;
}
} // namespace Fleet::DataManager::Core