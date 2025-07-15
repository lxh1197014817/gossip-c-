// Copyright (c) 2025 Institute of Software, Chinese Academy of Sciences
// Author: Zhen Tang <tangzhen12@otcaix.iscas.ac.cn>
// Affiliation: Institute of Software, Chinese Academy of Sciences

#include "Plugin.h"

namespace Fleet::DataManager::Core {
Plugin::Plugin() {
    this->handle = nullptr;
    this->plugin = nullptr;
    this->newPluginFunction = nullptr;
    this->deletePluginFunction = nullptr;
    this->dependencyList.clear();
    this->conflictList.clear();
}

Plugin::~Plugin() {
}

void *Plugin::GetHandle() const {
    return this->handle;
}

void Plugin::SetHandle(void *handle) {
    this->handle = handle;
}

const std::string &Plugin::GetPath() const {
    return this->path;
}

void Plugin::SetPath(std::string path) {
    this->path = std::move(path);
}

const std::string &Plugin::GetName() const {
    return this->name;
}

void Plugin::SetName(std::string name) {
    this->name = std::move(name);
}

const std::string &Plugin::GetVersion() const {
    return this->version;
}

void Plugin::SetVersion(std::string version) {
    this->version = std::move(version);
}

const std::string &Plugin::GetDescription() const {
    return this->description;
}

void Plugin::SetDescription(std::string description) {
    this->description = std::move(description);
}

void *Plugin::NewPlugin() {
    return this->newPluginFunction();
}

void Plugin::SetNewPluginFunction(Plugin::NewPluginFunction newPluginFunction) {
    this->newPluginFunction = newPluginFunction;
}

void Plugin::DeletePlugin() {
    this->deletePluginFunction(this->GetPlugin());
}

void Plugin::SetDeletePluginFunction(Plugin::DeletePluginFunction deletePluginFunction) {
    this->deletePluginFunction = deletePluginFunction;
}

PluginBase *Plugin::GetPlugin() {
    return this->plugin;
}

void Plugin::SetPlugin(PluginBase *plugin) {
    this->plugin = plugin;
}

const std::vector<std::string> &Plugin::GetDependencyList() {
    return this->dependencyList;
}

const std::vector<std::string> &Plugin::GetConflictList() {
    return this->conflictList;
}

void Plugin::ParseDependencyList(int dependencyCount, const char *dependencyList[]) {
    this->dependencyList.clear();
    int i = 0;
    for (i = 0; i < dependencyCount; i++) {
        this->dependencyList.emplace_back(dependencyList[i]);
    }
}

void Plugin::ParseConflictList(int conflictCount, const char *conflictList[]) {
    this->conflictList.clear();
    int i = 0;
    for (i = 0; i < conflictCount; i++) {
        this->conflictList.emplace_back(conflictList[i]);
    }
}

void Plugin::SetDependencyList(const std::vector<std::string> &dependencyList) {
    this->dependencyList.clear();
    for (const auto &elem : dependencyList) {
        this->dependencyList.push_back(elem);
    }
}

void Plugin::SetConflictList(const std::vector<std::string> &conflictList) {
    this->conflictList.clear();
    for (const auto &elem : conflictList) {
        this->conflictList.push_back(elem);
    }
}

void Plugin::From(const std::shared_ptr<Plugin> &another) {
    this->handle = another->handle;
    this->plugin = another->plugin;
    this->path = another->path;
    this->name = another->name;
    this->version = another->version;
    this->description = another->description;
    this->SetDependencyList(another->GetDependencyList());
    this->SetConflictList(another->GetConflictList());
    this->newPluginFunction = another->newPluginFunction;
    this->deletePluginFunction = another->deletePluginFunction;
}
} // namespace Fleet::DataManager::Core