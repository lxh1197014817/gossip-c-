// Author: Zhen Tang <tangzhen12@otcaix.iscas.ac.cn>
// Affiliation: Institute of Software, Chinese Academy of Sciences

#include "libfleet-datamgr.h"
#include "ApiService.h"
#include "Device.h"
#include "DynamicPluginManager.h"
#include "Location.h"
#include "Logger.h"
#include "MessagingService.h"
#include "PluginManager.h"
#include "PortalService.h"
#include "StaticPluginManager.h"
#include "StorageService.h"
#include "Strategy.h"
#include <cstring>
#include <mutex>
#include <set>

std::set<void *> pluginManagerSet;
std::mutex pluginManagerSetMutex;

#ifdef DYNAMIC_PLUGIN_MANAGER
#define NewDynamicPluginManager NewPluginManager
#define NewDynamicPluginManagerByUuid NewPluginManagerByUuid
#define NewDynamicPluginManagerByDirectory NewPluginManagerByDirectory
#define NewDynamicPluginManagerByUuidAndDirectory NewPluginManagerByUuidAndDirectory
#define DeleteDynamicPluginManager DeletePluginManager
#elif STATIC_PLUGIN_MANAGER
#define NewStaticPluginManager NewPluginManager
#define NewStaticPluginManagerByUuid NewPluginManagerByUuid
#define NewStaticPluginManagerByDirectory NewPluginManagerByDirectory
#define NewStaticPluginManagerByUuidAndDirectory NewPluginManagerByUuidAndDirectory
#define DeleteStaticPluginManager DeletePluginManager
#endif

void AddPluginManager(void *pluginManager) {
    Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                            "调用");
    std::lock_guard<std::mutex> lock(pluginManagerSetMutex);
    pluginManagerSet.insert(pluginManager);
    Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                            "返回");
}

void RemovePluginManager(void *pluginManager) {
    Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                            "调用");
    std::lock_guard<std::mutex> lock(pluginManagerSetMutex);
    pluginManagerSet.erase(pluginManager);
    Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                            "返回");
}

bool IsValidPluginManager(void *pluginManager) {
    Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                            "调用");
    std::lock_guard<std::mutex> lock(pluginManagerSetMutex);
    if (pluginManagerSet.find(pluginManager) == pluginManagerSet.end()) {
        Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                                "返回 false");
        return false;
    }
    Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                            "返回 true");
    return true;
}

void *NewDynamicPluginManager() {
    Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                            "调用");
    auto ret = new Fleet::DataManager::Core::DynamicPluginManager(nullptr, "/root/iscas/fleet");
    AddPluginManager(ret);
    Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                            "返回");
    return ret;
}

void *NewDynamicPluginManagerByUuid(const char *id) {
    Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                            "调用");
    uuid_t uuid;
    uuid_parse(id, uuid);
    auto ret = new Fleet::DataManager::Core::DynamicPluginManager(uuid, "/root/iscas/fleet");
    AddPluginManager(ret);
    Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                            "返回");
    return ret;
}

void *NewDynamicPluginManagerByDirectory(const char *directory) {
    Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                            "调用");
    auto ret = new Fleet::DataManager::Core::DynamicPluginManager(nullptr, directory);
    AddPluginManager(ret);
    Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                            "返回");
    return ret;
}

void *NewDynamicPluginManagerByUuidAndDirectory(const char *id, const char *directory) {
    Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                            "调用");
    uuid_t uuid;
    uuid_parse(id, uuid);
    auto ret = new Fleet::DataManager::Core::DynamicPluginManager(uuid, directory);
    AddPluginManager(ret);
    Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                            "返回");
    return ret;
}

void DeleteDynamicPluginManager(void *pluginManager) {
    Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                            "调用");
    if (!IsValidPluginManager(pluginManager)) {
        Fleet::DataManager::Core::Logger::ConsoleLogger().Error(SOURCE_LOCATION,
                                                                "无效的插件管理器指针");
        Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                                "返回");
        return;
    }
    RemovePluginManager(pluginManager);
    delete ((Fleet::DataManager::Core::DynamicPluginManager *) pluginManager);
    Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                            "返回");
}

void *NewStaticPluginManager() {
    Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                            "调用");
    auto ret = new Fleet::DataManager::Core::StaticPluginManager(nullptr, "/root/iscas/fleet");
    AddPluginManager(ret);
    Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                            "返回");
    return ret;
}

void *NewStaticPluginManagerByUuid(const char *id) {
    Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                            "调用");
    uuid_t uuid;
    uuid_parse(id, uuid);
    auto ret = new Fleet::DataManager::Core::StaticPluginManager(uuid, "/root/iscas/fleet");
    AddPluginManager(ret);
    Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                            "返回");
    return ret;
}

void *NewStaticPluginManagerByDirectory(const char *directory) {
    Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                            "调用");
    auto ret = new Fleet::DataManager::Core::StaticPluginManager(nullptr, directory);
    AddPluginManager(ret);
    Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                            "返回");
    return ret;
}

void *NewStaticPluginManagerByUuidAndDirectory(const char *id, const char *directory) {
    Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                            "调用");
    uuid_t uuid;
    uuid_parse(id, uuid);
    auto ret = new Fleet::DataManager::Core::StaticPluginManager(uuid, directory);
    AddPluginManager(ret);
    Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                            "返回");
    return ret;
}

void DeleteStaticPluginManager(void *pluginManager) {
    Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                            "调用");
    if (!IsValidPluginManager(pluginManager)) {
        Fleet::DataManager::Core::Logger::ConsoleLogger().Error(SOURCE_LOCATION,
                                                                "无效的插件管理器指针");
        Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                                "返回");
        return;
    }
    RemovePluginManager(pluginManager);
    delete ((Fleet::DataManager::Core::StaticPluginManager *) pluginManager);
    Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                            "返回");
}

const char *GetNodeId(void *pluginManager) {
    Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                            "调用");
    if (!IsValidPluginManager(pluginManager)) {
        Fleet::DataManager::Core::Logger::ConsoleLogger().Error(SOURCE_LOCATION,
                                                                "无效的插件管理器指针");
        Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                                "返回");
        return nullptr;
    }
    Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                            "返回");
    return ((Fleet::DataManager::Core::PluginManager *) pluginManager)->GetNodeId().c_str();
}

void SetParameter(void *pluginManager, const char *key, const char *value) {
    Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                            "调用");
    if (!IsValidPluginManager(pluginManager)) {
        Fleet::DataManager::Core::Logger::ConsoleLogger().Error(SOURCE_LOCATION,
                                                                "无效的插件管理器指针");
        Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                                "返回");
        return;
    }
    ((Fleet::DataManager::Core::PluginManager *) pluginManager)->SetParameter(key, value);
    Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                            "返回");
}

void LoadPlugins(void *pluginManager) {
    Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                            "调用");
    if (!IsValidPluginManager(pluginManager)) {
        Fleet::DataManager::Core::Logger::ConsoleLogger().Error(SOURCE_LOCATION,
                                                                "无效的插件管理器指针");
        Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                                "返回");
        return;
    }
    ((Fleet::DataManager::Core::PluginManager *) pluginManager)->LoadPluginsFromParameter();
    Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                            "返回");
}

void UnloadPlugins(void *pluginManager) {
    Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                            "调用");
    if (!IsValidPluginManager(pluginManager)) {
        Fleet::DataManager::Core::Logger::ConsoleLogger().Error(SOURCE_LOCATION,
                                                                "无效的插件管理器指针");
        Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                                "返回");
        return;
    }
    ((Fleet::DataManager::Core::PluginManager *) pluginManager)->UnloadAllPlugins();
    Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                            "返回");
}

int StartApiServer(void *pluginManager, const char *address) {
    Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                            "调用");
    if (!IsValidPluginManager(pluginManager)) {
        Fleet::DataManager::Core::Logger::ConsoleLogger().Error(SOURCE_LOCATION,
                                                                "无效的插件管理器指针");
        Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                                "返回");
        return false;
    }
    Fleet::DataManager::Api::ApiService *apiService =
        (Fleet::DataManager::Api::ApiService *) ((Fleet::DataManager::Core::PluginManager *)
                                                     pluginManager)
            ->GetService("Api");
    if (apiService != nullptr) {
        Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                                "返回");
        return apiService->Start(address);
    } else {
        Fleet::DataManager::Core::Logger::ConsoleLogger().Error(SOURCE_LOCATION,
                                                                "未找到 外部接口 插件");
        Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                                "返回");
        return false;
    }
}

int StartApiServerByPort(void *pluginManager, const char *address, int port) {
    Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                            "调用");
    if (!IsValidPluginManager(pluginManager)) {
        Fleet::DataManager::Core::Logger::ConsoleLogger().Error(SOURCE_LOCATION,
                                                                "无效的插件管理器指针");
        Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                                "返回");
        return false;
    }
    Fleet::DataManager::Api::ApiService *apiService =
        (Fleet::DataManager::Api::ApiService *) ((Fleet::DataManager::Core::PluginManager *)
                                                     pluginManager)
            ->GetService("Api");
    if (apiService != nullptr) {
        Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                                "返回");
        return apiService->Start(address, port);
    } else {
        Fleet::DataManager::Core::Logger::ConsoleLogger().Error(SOURCE_LOCATION,
                                                                "未找到 外部接口 插件");
        Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                                "返回");
        return false;
    }
}

int StopApiServer(void *pluginManager) {
    Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                            "调用");
    if (!IsValidPluginManager(pluginManager)) {
        Fleet::DataManager::Core::Logger::ConsoleLogger().Error(SOURCE_LOCATION,
                                                                "无效的插件管理器指针");
        Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                                "返回");
        return false;
    }
    Fleet::DataManager::Api::ApiService *apiService =
        (Fleet::DataManager::Api::ApiService *) ((Fleet::DataManager::Core::PluginManager *)
                                                     pluginManager)
            ->GetService("Api");
    if (apiService != nullptr) {
        Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                                "返回");
        return apiService->Stop();
    } else {
        Fleet::DataManager::Core::Logger::ConsoleLogger().Error(SOURCE_LOCATION,
                                                                "未找到 外部接口 插件");
        Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                                "返回");
        return false;
    }
}

int GetApiPort(void *pluginManager) {
    Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                            "调用");
    if (!IsValidPluginManager(pluginManager)) {
        Fleet::DataManager::Core::Logger::ConsoleLogger().Error(SOURCE_LOCATION,
                                                                "无效的插件管理器指针");
        Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                                "返回");
        return -1;
    }
    Fleet::DataManager::Api::ApiService *apiService =
        (Fleet::DataManager::Api::ApiService *) ((Fleet::DataManager::Core::PluginManager *)
                                                     pluginManager)
            ->GetService("Api");
    if (apiService != nullptr) {
        Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                                "返回");
        return apiService->GetApiPort();
    } else {
        Fleet::DataManager::Core::Logger::ConsoleLogger().Error(SOURCE_LOCATION,
                                                                "未找到 外部接口 插件");
        Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                                "返回");
        return -1;
    }
}

void StartTcp(void *pluginManager, const char *address) {
    Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                            "调用");
    if (!IsValidPluginManager(pluginManager)) {
        Fleet::DataManager::Core::Logger::ConsoleLogger().Error(SOURCE_LOCATION,
                                                                "无效的插件管理器指针");
        Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                                "返回");
        return;
    }
    Fleet::DataManager::Messaging::MessagingService *messagingService =
        (Fleet::DataManager::Messaging::MessagingService
             *) ((Fleet::DataManager::Core::PluginManager *) pluginManager)
            ->GetService("Messaging");
    if (messagingService != nullptr) {
        messagingService->StartTcp(address, 0);
    } else {
        Fleet::DataManager::Core::Logger::ConsoleLogger().Info(SOURCE_LOCATION,
                                                               "未找到 消息协同 插件");
    }
    Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                            "返回");
}

void StartTcpByPort(void *pluginManager, const char *address, int port) {
    Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                            "调用");
    if (!IsValidPluginManager(pluginManager)) {
        Fleet::DataManager::Core::Logger::ConsoleLogger().Error(SOURCE_LOCATION,
                                                                "无效的插件管理器指针");
        Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                                "返回");
        return;
    }
    Fleet::DataManager::Messaging::MessagingService *messagingService =
        (Fleet::DataManager::Messaging::MessagingService
             *) ((Fleet::DataManager::Core::PluginManager *) pluginManager)
            ->GetService("Messaging");
    if (messagingService != nullptr) {
        messagingService->StartTcp(address, port);
    } else {
        Fleet::DataManager::Core::Logger::ConsoleLogger().Info(SOURCE_LOCATION,
                                                               "未找到 消息协同 插件");
    }
    Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                            "返回");
}

void StopTcp(void *pluginManager) {
    Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                            "调用");
    if (!IsValidPluginManager(pluginManager)) {
        Fleet::DataManager::Core::Logger::ConsoleLogger().Error(SOURCE_LOCATION,
                                                                "无效的插件管理器指针");
        Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                                "返回");
        return;
    }
    Fleet::DataManager::Messaging::MessagingService *messagingService =
        (Fleet::DataManager::Messaging::MessagingService
             *) ((Fleet::DataManager::Core::PluginManager *) pluginManager)
            ->GetService("Messaging");
    if (messagingService != nullptr) {
        messagingService->StopTcp();
    } else {
        Fleet::DataManager::Core::Logger::ConsoleLogger().Info(SOURCE_LOCATION,
                                                               "未找到 消息协同 插件");
    }
    Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                            "返回");
}

int GetTcpPort(void *pluginManager) {
    Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                            "调用");
    if (!IsValidPluginManager(pluginManager)) {
        Fleet::DataManager::Core::Logger::ConsoleLogger().Error(SOURCE_LOCATION,
                                                                "无效的插件管理器指针");
        Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                                "返回");
        return -1;
    }
    Fleet::DataManager::Messaging::MessagingService *messagingService =
        (Fleet::DataManager::Messaging::MessagingService
             *) ((Fleet::DataManager::Core::PluginManager *) pluginManager)
            ->GetService("Messaging");
    if (messagingService != nullptr) {
        Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                                "返回");
        return messagingService->GetTcpPort();
    } else {
        Fleet::DataManager::Core::Logger::ConsoleLogger().Error(SOURCE_LOCATION,
                                                                "未找到 消息协同 插件");
        Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                                "返回");
        return -1;
    }
}

void StartUdp(void *pluginManager, const char *address) {
    Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                            "调用");
    if (!IsValidPluginManager(pluginManager)) {
        Fleet::DataManager::Core::Logger::ConsoleLogger().Error(SOURCE_LOCATION,
                                                                "无效的插件管理器指针");
        Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                                "返回");
        return;
    }
    Fleet::DataManager::Messaging::MessagingService *messagingService =
        (Fleet::DataManager::Messaging::MessagingService
             *) ((Fleet::DataManager::Core::PluginManager *) pluginManager)
            ->GetService("Messaging");
    if (messagingService != nullptr) {
        messagingService->StartUdp(address, 0);
    } else {
        Fleet::DataManager::Core::Logger::ConsoleLogger().Error(SOURCE_LOCATION,
                                                                "未找到 消息协同 插件");
    }
    Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                            "返回");
}

void StartUdpByPort(void *pluginManager, const char *address, int port) {
    Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                            "调用");
    if (!IsValidPluginManager(pluginManager)) {
        Fleet::DataManager::Core::Logger::ConsoleLogger().Error(SOURCE_LOCATION,
                                                                "无效的插件管理器指针");
        Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                                "返回");
        return;
    }
    Fleet::DataManager::Messaging::MessagingService *messagingService =
        (Fleet::DataManager::Messaging::MessagingService
             *) ((Fleet::DataManager::Core::PluginManager *) pluginManager)
            ->GetService("Messaging");
    if (messagingService != nullptr) {
        messagingService->StartUdp(address, port);
    } else {
        Fleet::DataManager::Core::Logger::ConsoleLogger().Error(SOURCE_LOCATION,
                                                                "未找到 消息协同 插件");
    }
    Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                            "返回");
}

void StopUdp(void *pluginManager) {
    Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                            "调用");
    if (!IsValidPluginManager(pluginManager)) {
        Fleet::DataManager::Core::Logger::ConsoleLogger().Error(SOURCE_LOCATION,
                                                                "无效的插件管理器指针");
        Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                                "返回");
        return;
    }
    Fleet::DataManager::Messaging::MessagingService *messagingService =
        (Fleet::DataManager::Messaging::MessagingService
             *) ((Fleet::DataManager::Core::PluginManager *) pluginManager)
            ->GetService("Messaging");
    if (messagingService != nullptr) {
        messagingService->StopUdp();
    } else {
        Fleet::DataManager::Core::Logger::ConsoleLogger().Error(SOURCE_LOCATION,
                                                                "未找到 消息协同 插件");
    }
    Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                            "返回");
}

int GetUdpPort(void *pluginManager) {
    Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                            "调用");
    if (!IsValidPluginManager(pluginManager)) {
        Fleet::DataManager::Core::Logger::ConsoleLogger().Error(SOURCE_LOCATION,
                                                                "无效的插件管理器指针");
        Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                                "返回");
        return -1;
    }
    Fleet::DataManager::Messaging::MessagingService *messagingService =
        (Fleet::DataManager::Messaging::MessagingService
             *) ((Fleet::DataManager::Core::PluginManager *) pluginManager)
            ->GetService("Messaging");
    if (messagingService != nullptr) {
        Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                                "返回");
        return messagingService->GetUdpPort();
    } else {
        Fleet::DataManager::Core::Logger::ConsoleLogger().Error(SOURCE_LOCATION,
                                                                "未找到 消息协同 插件");
        Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                                "返回");
        return -1;
    }
}

int StartXQuic(void *pluginManager, const char *address) {
    Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                            "调用");
    if (!IsValidPluginManager(pluginManager)) {
        Fleet::DataManager::Core::Logger::ConsoleLogger().Error(SOURCE_LOCATION,
                                                                "无效的插件管理器指针");
        Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                                "返回");
        return false;
    }

    Fleet::DataManager::Core::PluginManager *pm =
        (Fleet::DataManager::Core::PluginManager *) pluginManager;

    if (pm->GetService("XQuic") == nullptr) {
        Fleet::DataManager::Core::Logger::ConsoleLogger().Error(
            SOURCE_LOCATION, "未找到 Alibaba XQUIC SDK适配 插件");
        Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                                "返回");
        return false;
    }

    Fleet::DataManager::Messaging::MessagingService *messagingService =
        (Fleet::DataManager::Messaging::MessagingService *) pm->GetService("Messaging");

    if (messagingService != nullptr) {
        messagingService->StartXQuic(address, 0);
        Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                                "返回");
        return true;
    } else {
        Fleet::DataManager::Core::Logger::ConsoleLogger().Error(SOURCE_LOCATION,
                                                                "未找到 消息协同 插件");
        Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                                "返回");
        return false;
    }
}

int StartXQuicByPort(void *pluginManager, const char *address, int port) {
    Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                            "调用");
    if (!IsValidPluginManager(pluginManager)) {
        Fleet::DataManager::Core::Logger::ConsoleLogger().Error(SOURCE_LOCATION,
                                                                "无效的插件管理器指针");
        Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                                "返回");
        return false;
    }

    Fleet::DataManager::Core::PluginManager *pm =
        (Fleet::DataManager::Core::PluginManager *) pluginManager;

    if (pm->GetService("XQuic") == nullptr) {
        Fleet::DataManager::Core::Logger::ConsoleLogger().Error(
            SOURCE_LOCATION, "未找到 Alibaba XQUIC SDK适配 插件");
        Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                                "返回");
        return false;
    }

    Fleet::DataManager::Messaging::MessagingService *messagingService =
        (Fleet::DataManager::Messaging::MessagingService *) pm->GetService("Messaging");

    if (messagingService != nullptr) {
        messagingService->StartXQuic(address, port);
        Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                                "返回");
        return true;
    } else {
        Fleet::DataManager::Core::Logger::ConsoleLogger().Error(SOURCE_LOCATION,
                                                                "未找到 消息协同 插件");
        Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                                "返回");
        return false;
    }
}

void StopXQuic(void *pluginManager) {
    Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                            "调用");
    if (!IsValidPluginManager(pluginManager)) {
        Fleet::DataManager::Core::Logger::ConsoleLogger().Error(SOURCE_LOCATION,
                                                                "无效的插件管理器指针");
        Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                                "返回");
        return;
    }

    Fleet::DataManager::Core::PluginManager *pm =
        (Fleet::DataManager::Core::PluginManager *) pluginManager;

    if (pm->GetService("XQuic") == nullptr) {
        Fleet::DataManager::Core::Logger::ConsoleLogger().Error(
            SOURCE_LOCATION, "未找到 Alibaba XQUIC SDK适配 插件");
        Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                                "返回");
        return;
    }

    Fleet::DataManager::Messaging::MessagingService *messagingService =
        (Fleet::DataManager::Messaging::MessagingService *) pm->GetService("Messaging");

    if (messagingService != nullptr) {
        messagingService->StopXQuic();
    } else {
        Fleet::DataManager::Core::Logger::ConsoleLogger().Error(SOURCE_LOCATION,
                                                                "未找到 消息协同 插件");
    }
    Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                            "返回");
}

int GetXQuicPort(void *pluginManager) {
    Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                            "调用");
    if (!IsValidPluginManager(pluginManager)) {
        Fleet::DataManager::Core::Logger::ConsoleLogger().Error(SOURCE_LOCATION,
                                                                "无效的插件管理器指针");
        Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                                "返回");
        return -1;
    }
    Fleet::DataManager::Messaging::MessagingService *messagingService =
        (Fleet::DataManager::Messaging::MessagingService
             *) ((Fleet::DataManager::Core::PluginManager *) pluginManager)
            ->GetService("Messaging");
    if (messagingService != nullptr) {
        Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                                "返回");
        return messagingService->GetXQuicPort();
    } else {
        Fleet::DataManager::Core::Logger::ConsoleLogger().Error(SOURCE_LOCATION,
                                                                "未找到 消息协同 插件");
        Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                                "返回");
        return -1;
    }
}

void StartSharedMemory(void *pluginManager) {
    Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                            "调用");
    if (!IsValidPluginManager(pluginManager)) {
        Fleet::DataManager::Core::Logger::ConsoleLogger().Error(SOURCE_LOCATION,
                                                                "无效的插件管理器指针");
        Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                                "返回");
        return;
    }

    if (((Fleet::DataManager::Core::PluginManager *) pluginManager)->GetService("SharedMemory") ==
        nullptr) {
        Fleet::DataManager::Core::Logger::ConsoleLogger().Error(SOURCE_LOCATION,
                                                                "未找到 共享内存传输支持 插件");
        return;
    }

    Fleet::DataManager::Messaging::MessagingService *messagingService =
        (Fleet::DataManager::Messaging::MessagingService
             *) ((Fleet::DataManager::Core::PluginManager *) pluginManager)
            ->GetService("Messaging");
    if (messagingService != nullptr) {
        messagingService->StartSharedMemory();
    } else {
        Fleet::DataManager::Core::Logger::ConsoleLogger().Error(SOURCE_LOCATION,
                                                                "未找到 消息协同 插件");
    }
    Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                            "返回");
}

void StopSharedMemory(void *pluginManager) {
    Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                            "调用");
    if (!IsValidPluginManager(pluginManager)) {
        Fleet::DataManager::Core::Logger::ConsoleLogger().Error(SOURCE_LOCATION,
                                                                "无效的插件管理器指针");
        Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                                "返回");
        return;
    }

    if (((Fleet::DataManager::Core::PluginManager *) pluginManager)->GetService("SharedMemory") ==
        nullptr) {
        Fleet::DataManager::Core::Logger::ConsoleLogger().Error(SOURCE_LOCATION,
                                                                "未找到 共享内存传输支持 插件");
        return;
    }

    Fleet::DataManager::Messaging::MessagingService *messagingService =
        (Fleet::DataManager::Messaging::MessagingService
             *) ((Fleet::DataManager::Core::PluginManager *) pluginManager)
            ->GetService("Messaging");
    if (messagingService != nullptr) {
        messagingService->StopSharedMemory();
    } else {
        Fleet::DataManager::Core::Logger::ConsoleLogger().Error(SOURCE_LOCATION,
                                                                "未找到 消息协同 插件");
    }
    Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                            "返回");
}

void Join(void *pluginManager, const char *id, const char *address, int port) {
    Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                            "调用");
    if (!IsValidPluginManager(pluginManager)) {
        Fleet::DataManager::Core::Logger::ConsoleLogger().Error(SOURCE_LOCATION,
                                                                "无效的插件管理器指针");
        Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                                "返回");
        return;
    }
    Fleet::DataManager::Messaging::MessagingService *messagingService =
        (Fleet::DataManager::Messaging::MessagingService
             *) ((Fleet::DataManager::Core::PluginManager *) pluginManager)
            ->GetService("Messaging");
    if (messagingService != nullptr) {
        messagingService->Join(id, address, port);
    } else {
        Fleet::DataManager::Core::Logger::ConsoleLogger().Error(SOURCE_LOCATION,
                                                                "未找到 消息协同 插件");
    }
    Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                            "返回");
}

void Leave(void *pluginManager, const char *id) {
    Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                            "调用");
    if (!IsValidPluginManager(pluginManager)) {
        Fleet::DataManager::Core::Logger::ConsoleLogger().Error(SOURCE_LOCATION,
                                                                "无效的插件管理器指针");
        Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                                "返回");
        return;
    }
    Fleet::DataManager::Messaging::MessagingService *messagingService =
        (Fleet::DataManager::Messaging::MessagingService
             *) ((Fleet::DataManager::Core::PluginManager *) pluginManager)
            ->GetService("Messaging");
    if (messagingService != nullptr) {
        messagingService->Leave(id);
    } else {
        Fleet::DataManager::Core::Logger::ConsoleLogger().Error(SOURCE_LOCATION,
                                                                "未找到 消息协同 插件");
    }
    Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                            "返回");
}

int Publish(void *pluginManager, const char *topic, int size, const char *data) {
    Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                            "调用");
    if (!IsValidPluginManager(pluginManager)) {
        Fleet::DataManager::Core::Logger::ConsoleLogger().Error(SOURCE_LOCATION,
                                                                "无效的插件管理器指针");
        Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                                "返回");
        return false;
    }
    Fleet::DataManager::Messaging::MessagingService *messagingService =
        (Fleet::DataManager::Messaging::MessagingService
             *) ((Fleet::DataManager::Core::PluginManager *) pluginManager)
            ->GetService("Messaging");
    if (messagingService != nullptr) {
        Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                                "返回");
        return messagingService->Publish(topic, size, data);
    } else {
        Fleet::DataManager::Core::Logger::ConsoleLogger().Error(SOURCE_LOCATION,
                                                                "未找到 消息协同 插件");
        Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                                "返回");
        return false;
    }
}

int Subscribe(void *pluginManager, const char *topic, MessageCallback messageCallback,
              void *closure) {
    Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                            "调用");
    if (!IsValidPluginManager(pluginManager)) {
        Fleet::DataManager::Core::Logger::ConsoleLogger().Error(SOURCE_LOCATION,
                                                                "无效的插件管理器指针");
        Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                                "返回");
        return false;
    }
    Fleet::DataManager::Messaging::MessagingService *messagingService =
        (Fleet::DataManager::Messaging::MessagingService
             *) ((Fleet::DataManager::Core::PluginManager *) pluginManager)
            ->GetService("Messaging");
    if (messagingService != nullptr) {
        Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                                "返回");
        return messagingService->Subscribe(topic, messageCallback, closure);
    } else {
        Fleet::DataManager::Core::Logger::ConsoleLogger().Error(SOURCE_LOCATION,
                                                                "未找到 消息协同 插件");
        Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                                "返回");
        return false;
    }
}

int Unsubscribe(void *pluginManager, const char *topic, MessageCallback messageCallback) {
    Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                            "调用");
    if (!IsValidPluginManager(pluginManager)) {
        Fleet::DataManager::Core::Logger::ConsoleLogger().Error(SOURCE_LOCATION,
                                                                "无效的插件管理器指针");
        Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                                "返回");
        return false;
    }
    Fleet::DataManager::Messaging::MessagingService *messagingService =
        (Fleet::DataManager::Messaging::MessagingService
             *) ((Fleet::DataManager::Core::PluginManager *) pluginManager)
            ->GetService("Messaging");
    if (messagingService != nullptr) {
        Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                                "返回");
        return messagingService->Unsubscribe(topic, messageCallback);
    } else {
        Fleet::DataManager::Core::Logger::ConsoleLogger().Error(SOURCE_LOCATION,
                                                                "未找到 消息协同 插件");
        Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                                "返回");
        return false;
    }
}

int Request(void *pluginManager, const char *topic, int requestSize, const char *requestData,
            MessageCallback responseCallback, void *closure) {
    Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                            "调用");
    if (!IsValidPluginManager(pluginManager)) {
        Fleet::DataManager::Core::Logger::ConsoleLogger().Error(SOURCE_LOCATION,
                                                                "无效的插件管理器指针");
        Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                                "返回");
        return false;
    }
    Fleet::DataManager::Messaging::MessagingService *messagingService =
        (Fleet::DataManager::Messaging::MessagingService
             *) ((Fleet::DataManager::Core::PluginManager *) pluginManager)
            ->GetService("Messaging");
    if (messagingService != nullptr) {
        Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                                "返回");
        return messagingService->Request(topic, requestSize, requestData, responseCallback,
                                         closure);
    } else {
        Fleet::DataManager::Core::Logger::ConsoleLogger().Error(SOURCE_LOCATION,
                                                                "未找到 消息协同 插件");
        Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                                "返回");
        return false;
    }
}

int Reply(void *pluginManager, const char *uuid, int responseSize, const char *responseData) {
    Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                            "调用");
    if (!IsValidPluginManager(pluginManager)) {
        Fleet::DataManager::Core::Logger::ConsoleLogger().Error(SOURCE_LOCATION,
                                                                "无效的插件管理器指针");
        Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                                "返回");
        return false;
    }
    Fleet::DataManager::Messaging::MessagingService *messagingService =
        (Fleet::DataManager::Messaging::MessagingService
             *) ((Fleet::DataManager::Core::PluginManager *) pluginManager)
            ->GetService("Messaging");
    if (messagingService != nullptr) {
        Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                                "返回");
        return messagingService->Reply(uuid, responseSize, responseData);
    } else {
        Fleet::DataManager::Core::Logger::ConsoleLogger().Error(SOURCE_LOCATION,
                                                                "未找到 消息协同 插件");
        Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                                "返回");
        return false;
    }
}

int CancelRequest(void *pluginManager, const char *uuid) {
    Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                            "调用，UUID: {}", uuid);
    if (!IsValidPluginManager(pluginManager)) {
        Fleet::DataManager::Core::Logger::ConsoleLogger().Error(SOURCE_LOCATION,
                                                                "无效的插件管理器指针");
        Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                                "返回");
        return 0;
    }
    Fleet::DataManager::Messaging::MessagingService *messagingService =
        (Fleet::DataManager::Messaging::MessagingService
             *) ((Fleet::DataManager::Core::PluginManager *) pluginManager)
            ->GetService("Messaging");
    if (messagingService != nullptr) {
        bool result = messagingService->CancelRequest(std::string(uuid));
        Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                                "返回，结果: {}", result);
        return result ? 1 : 0;
    } else {
        Fleet::DataManager::Core::Logger::ConsoleLogger().Error(SOURCE_LOCATION,
                                                                "未找到 消息协同 插件");
        Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                                "返回");
        return 0;
    }
}

void FillDbResponse(struct DbResponse *response, std::vector<std::string> &columnNames,
                    std::vector<std::vector<std::shared_ptr<std::string>>> data) {
    int i = 0;
    int j = 0;
    response->RowCount = (int) data.size();
    response->ColumnCount = (int) columnNames.size();
    response->ColumnNames = new char *[columnNames.size()];
    for (i = 0; i < columnNames.size(); i++) {
        response->ColumnNames[i] = new char[columnNames.at(i).size() + 1];
        strcpy(response->ColumnNames[i], columnNames.at(i).c_str());
    }
    response->Data = new char **[data.size()];
    for (i = 0; i < data.size(); i++) {
        response->Data[i] = new char *[data.at(i).size()];
        for (j = 0; j < data.at(i).size(); j++) {
            if (data.at(i).at(j) == nullptr) {
                response->Data[i][j] = nullptr;
            } else {
                response->Data[i][j] = new char[data.at(i).at(j)->size() + 1];
                strcpy(response->Data[i][j], data.at(i).at(j)->c_str());
            }
        }
    }
}

int SqliteExecuteResult(void *pluginManager, const char *path, const char *sql,
                        struct DbResponse *response) {
    Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                            "调用");
    if (!IsValidPluginManager(pluginManager)) {
        Fleet::DataManager::Core::Logger::ConsoleLogger().Error(SOURCE_LOCATION,
                                                                "无效的插件管理器指针");
        Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                                "返回");
        return false;
    }
    Fleet::DataManager::Storage::StorageService *storageService =
        (Fleet::DataManager::Storage::StorageService *) ((Fleet::DataManager::Core::PluginManager *)
                                                             pluginManager)
            ->GetService("Storage");
    if (storageService != nullptr) {
        void *connection = storageService->ConnectToSqlite(path);
        if (connection == nullptr) {
            return false;
        }
        std::vector<std::string> columnNames;
        std::vector<std::vector<std::shared_ptr<std::string>>> data;
        bool success = storageService->ExecuteResult(connection, sql, columnNames, data);
        FillDbResponse(response, columnNames, data);
        storageService->DisconnectFromDatabase(connection);
        Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                                "返回");
        return success;
    } else {
        Fleet::DataManager::Core::Logger::ConsoleLogger().Error(SOURCE_LOCATION,
                                                                "未找到 本地存储 插件");
        Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                                "返回");
        return false;
    }
}

int SqliteExecuteNoResult(void *pluginManager, const char *path, const char *sql) {
    Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                            "调用");
    if (!IsValidPluginManager(pluginManager)) {
        Fleet::DataManager::Core::Logger::ConsoleLogger().Error(SOURCE_LOCATION,
                                                                "无效的插件管理器指针");
        Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                                "返回");
        return false;
    }
    Fleet::DataManager::Storage::StorageService *storageService =
        (Fleet::DataManager::Storage::StorageService *) ((Fleet::DataManager::Core::PluginManager *)
                                                             pluginManager)
            ->GetService("Storage");
    if (storageService != nullptr) {
        void *connection = storageService->ConnectToSqlite(path);
        if (connection == nullptr) {
            return false;
        }
        bool success = storageService->ExecuteNoResult(connection, sql);
        storageService->DisconnectFromDatabase(connection);
        Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                                "返回");
        return success;
    } else {
        Fleet::DataManager::Core::Logger::ConsoleLogger().Error(SOURCE_LOCATION,
                                                                "未找到 本地存储 插件");
        Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                                "返回");
        return false;
    }
}

int SqliteExecuteParameterizedResult(void *pluginManager, const char *path, const char *sql,
                                     int parameterCount, const char **parameterTypes,
                                     const char **parameterValues, struct DbResponse *response) {
    Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                            "调用");
    if (!IsValidPluginManager(pluginManager)) {
        Fleet::DataManager::Core::Logger::ConsoleLogger().Error(SOURCE_LOCATION,
                                                                "无效的插件管理器指针");
        Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                                "返回");
        return false;
    }
    Fleet::DataManager::Storage::StorageService *storageService =
        (Fleet::DataManager::Storage::StorageService *) ((Fleet::DataManager::Core::PluginManager *)
                                                             pluginManager)
            ->GetService("Storage");
    if (storageService != nullptr) {
        void *connection = storageService->ConnectToSqlite(path);
        if (connection == nullptr) {
            return false;
        }
        std::vector<std::string> columnNames;
        std::vector<std::vector<std::shared_ptr<std::string>>> data;
        std::vector<std::string> types;
        std::vector<std::string> values;
        int i = 0;
        for (i = 0; i < parameterCount; i++) {
            types.emplace_back(parameterTypes[i]);
            values.emplace_back(parameterValues[i]);
        }
        bool success = storageService->ExecuteParameterizedResult(connection, sql, types, values,
                                                                  columnNames, data);
        FillDbResponse(response, columnNames, data);
        storageService->DisconnectFromDatabase(connection);
        Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                                "返回");
        return success;
    } else {
        Fleet::DataManager::Core::Logger::ConsoleLogger().Error(SOURCE_LOCATION,
                                                                "未找到 本地存储 插件");
        Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                                "返回");
        return false;
    }
}

int SqliteExecuteParameterizedNoResult(void *pluginManager, const char *path, const char *sql,
                                       int parameterCount, const char **parameterTypes,
                                       const char **parameterValues) {
    Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                            "调用");
    if (!IsValidPluginManager(pluginManager)) {
        Fleet::DataManager::Core::Logger::ConsoleLogger().Error(SOURCE_LOCATION,
                                                                "无效的插件管理器指针");
        Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                                "返回");
        return false;
    }
    Fleet::DataManager::Storage::StorageService *storageService =
        (Fleet::DataManager::Storage::StorageService *) ((Fleet::DataManager::Core::PluginManager *)
                                                             pluginManager)
            ->GetService("Storage");
    if (storageService != nullptr) {
        void *connection = storageService->ConnectToSqlite(path);
        if (connection == nullptr) {
            return false;
        }
        std::vector<std::string> types;
        std::vector<std::string> values;
        int i = 0;
        for (i = 0; i < parameterCount; i++) {
            types.emplace_back(parameterTypes[i]);
            values.emplace_back(parameterValues[i]);
        }
        bool success = storageService->ExecuteParameterizedNoResult(connection, sql, types, values);
        storageService->DisconnectFromDatabase(connection);
        Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                                "返回");
        return success;
    } else {
        Fleet::DataManager::Core::Logger::ConsoleLogger().Error(SOURCE_LOCATION,
                                                                "未找到 本地存储 插件");
        Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                                "返回");
        return false;
    }
}

int PostgreSqlExecuteResult(void *pluginManager, const char *connectionString, const char *sql,
                            struct DbResponse *response) {
    Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                            "调用");
    if (!IsValidPluginManager(pluginManager)) {
        Fleet::DataManager::Core::Logger::ConsoleLogger().Error(SOURCE_LOCATION,
                                                                "无效的插件管理器指针");
        Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                                "返回");
        return false;
    }
    Fleet::DataManager::Storage::StorageService *storageService =
        (Fleet::DataManager::Storage::StorageService *) ((Fleet::DataManager::Core::PluginManager *)
                                                             pluginManager)
            ->GetService("Storage");
    if (storageService != nullptr) {
        void *connection = storageService->ConnectToPostgreSql(connectionString);
        if (connection == nullptr) {
            return false;
        }
        std::vector<std::string> columnNames;
        std::vector<std::vector<std::shared_ptr<std::string>>> data;
        bool success = storageService->ExecuteResult(connection, sql, columnNames, data);
        FillDbResponse(response, columnNames, data);
        storageService->DisconnectFromDatabase(connection);
        Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                                "返回");
        return success;
    } else {
        Fleet::DataManager::Core::Logger::ConsoleLogger().Error(SOURCE_LOCATION,
                                                                "未找到 本地存储 插件");
        Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                                "返回");
        return false;
    }
}

int PostgreSqlExecuteNoResult(void *pluginManager, const char *connectionString, const char *sql) {
    Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                            "调用");
    if (!IsValidPluginManager(pluginManager)) {
        Fleet::DataManager::Core::Logger::ConsoleLogger().Error(SOURCE_LOCATION,
                                                                "无效的插件管理器指针");
        Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                                "返回");
        return false;
    }
    Fleet::DataManager::Storage::StorageService *storageService =
        (Fleet::DataManager::Storage::StorageService *) ((Fleet::DataManager::Core::PluginManager *)
                                                             pluginManager)
            ->GetService("Storage");
    if (storageService != nullptr) {
        void *connection = storageService->ConnectToPostgreSql(connectionString);
        if (connection == nullptr) {
            return false;
        }
        bool success = storageService->ExecuteNoResult(connection, sql);
        storageService->DisconnectFromDatabase(connection);
        Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                                "返回");
        return success;
    } else {
        Fleet::DataManager::Core::Logger::ConsoleLogger().Error(SOURCE_LOCATION,
                                                                "未找到 本地存储 插件");
        Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                                "返回");
        return false;
    }
}

int PostgreSqlExecuteParameterizedResult(void *pluginManager, const char *connectionString,
                                         const char *sql, int parameterCount,
                                         const char **parameterTypes, const char **parameterValues,
                                         struct DbResponse *response) {
    Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                            "调用");
    if (!IsValidPluginManager(pluginManager)) {
        Fleet::DataManager::Core::Logger::ConsoleLogger().Error(SOURCE_LOCATION,
                                                                "无效的插件管理器指针");
        Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                                "返回");
        return false;
    }
    Fleet::DataManager::Storage::StorageService *storageService =
        (Fleet::DataManager::Storage::StorageService *) ((Fleet::DataManager::Core::PluginManager *)
                                                             pluginManager)
            ->GetService("Storage");
    if (storageService != nullptr) {
        void *connection = storageService->ConnectToPostgreSql(connectionString);
        if (connection == nullptr) {
            Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                                    "返回");
            return false;
        }
        std::vector<std::string> columnNames;
        std::vector<std::vector<std::shared_ptr<std::string>>> data;
        std::vector<std::string> types;
        std::vector<std::string> values;
        int i = 0;
        for (i = 0; i < parameterCount; i++) {
            types.emplace_back(parameterTypes[i]);
            values.emplace_back(parameterValues[i]);
        }
        bool success = storageService->ExecuteParameterizedResult(connection, sql, types, values,
                                                                  columnNames, data);
        FillDbResponse(response, columnNames, data);
        storageService->DisconnectFromDatabase(connection);
        Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                                "返回");
        return success;
    } else {
        Fleet::DataManager::Core::Logger::ConsoleLogger().Error(SOURCE_LOCATION,
                                                                "未找到 本地存储 插件");
        Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                                "返回");
        return false;
    }
}

int PostgreSqlExecuteParameterizedNoResult(void *pluginManager, const char *connectionString,
                                           const char *sql, int parameterCount,
                                           const char **parameterTypes,
                                           const char **parameterValues) {
    Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                            "调用");
    if (!IsValidPluginManager(pluginManager)) {
        Fleet::DataManager::Core::Logger::ConsoleLogger().Error(SOURCE_LOCATION,
                                                                "无效的插件管理器指针");
        Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                                "返回");
        return false;
    }
    Fleet::DataManager::Storage::StorageService *storageService =
        (Fleet::DataManager::Storage::StorageService *) ((Fleet::DataManager::Core::PluginManager *)
                                                             pluginManager)
            ->GetService("Storage");
    if (storageService != nullptr) {
        void *connection = storageService->ConnectToPostgreSql(connectionString);
        if (connection == nullptr) {
            return false;
        }
        std::vector<std::string> types;
        std::vector<std::string> values;
        int i = 0;
        for (i = 0; i < parameterCount; i++) {
            types.emplace_back(parameterTypes[i]);
            values.emplace_back(parameterValues[i]);
        }
        bool success = storageService->ExecuteParameterizedNoResult(connection, sql, types, values);
        storageService->DisconnectFromDatabase(connection);
        Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                                "返回");
        return success;
    } else {
        Fleet::DataManager::Core::Logger::ConsoleLogger().Error(SOURCE_LOCATION,
                                                                "未找到 本地存储 插件");
        Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                                "返回");
        return false;
    }
}

DbResponse *CreateDbResponse() {
    DbResponse *response = new DbResponse;
    response->RowCount = 0;
    response->ColumnCount = 0;
    response->ColumnNames = nullptr;
    response->Data = nullptr;
    return response;
}

void FreeDbResponse(DbResponse *response) {
    if (response == nullptr) {
        return;
    }

    int i = 0;
    int j = 0;

    for (i = 0; i < response->RowCount; i++) {
        for (j = 0; j < response->ColumnCount; j++) {
            if (response->Data[i][j] != nullptr) {
                delete[] response->Data[i][j];
            }
        }
        delete[] response->Data[i];
    }
    delete[] response->Data;
    for (i = 0; i < response->ColumnCount; i++) {
        delete[] response->ColumnNames[i];
    }
    delete[] response->ColumnNames;
    delete response;
}

struct DataInfo *GetDataInfo(void *pluginManager, const char *name, const char *dataType,
                             const char *owner) {
    Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                            "调用");
    if (!IsValidPluginManager(pluginManager)) {
        Fleet::DataManager::Core::Logger::ConsoleLogger().Error(SOURCE_LOCATION,
                                                                "无效的插件管理器指针");
        Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                                "返回");
        return nullptr;
    }
    Fleet::DataManager::Storage::StorageService *storageService =
        (Fleet::DataManager::Storage::StorageService *) ((Fleet::DataManager::Core::PluginManager *)
                                                             pluginManager)
            ->GetService("Storage");
    if (storageService == nullptr) {
        Fleet::DataManager::Core::Logger::ConsoleLogger().Error(SOURCE_LOCATION,
                                                                "未找到 本地存储 插件");
        Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                                "返回");
        return nullptr;
    }

    auto metadata = storageService->GetDataInfo(owner, dataType, name);
    if (metadata->GetLocations().empty()) {
        Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                                "返回");
        return nullptr;
    }
    auto ret = new DataInfo;
    ret->size = metadata->GetSize();
    ret->lastUpdate = metadata->GetLastUpdate();
    ret->pathCount = (int) metadata->GetLocations().size();
    ret->paths = new char *[ret->pathCount];
    int i = 0;
    for (i = 0; i < metadata->GetLocations().size(); i++) {
        ret->paths[i] = new char[metadata->GetLocations()[i].size() + 1];
        strcpy(ret->paths[i], metadata->GetLocations()[i].c_str());
    }
    Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                            "返回");
    return ret;
}

struct DataInfo *GetDataInfoWithVersion(void *pluginManager, const char *name, const char *dataType,
                                        const char *owner, const char *version) {
    Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                            "调用");
    if (!IsValidPluginManager(pluginManager)) {
        Fleet::DataManager::Core::Logger::ConsoleLogger().Error(SOURCE_LOCATION,
                                                                "无效的插件管理器指针");
        Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                                "返回");
        return nullptr;
    }
    Fleet::DataManager::Storage::StorageService *storageService =
        (Fleet::DataManager::Storage::StorageService *) ((Fleet::DataManager::Core::PluginManager *)
                                                             pluginManager)
            ->GetService("Storage");
    if (storageService == nullptr) {
        Fleet::DataManager::Core::Logger::ConsoleLogger().Error(SOURCE_LOCATION,
                                                                "未找到 本地存储 插件");
        Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                                "返回");
        return nullptr;
    }

    auto metadata = storageService->GetDataInfo(owner, dataType, name, version);
    if (metadata->GetLocations().empty()) {
        Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                                "返回");
        return nullptr;
    }
    auto ret = new DataInfo;
    ret->size = metadata->GetSize();
    ret->lastUpdate = metadata->GetLastUpdate();
    ret->pathCount = (int) metadata->GetLocations().size();
    ret->paths = new char *[ret->pathCount];
    int i = 0;
    for (i = 0; i < metadata->GetLocations().size(); i++) {
        ret->paths[i] = new char[metadata->GetLocations()[i].size() + 1];
        strcpy(ret->paths[i], metadata->GetLocations()[i].c_str());
    }
    Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                            "返回");
    return ret;
}

void FreeDataInfo(struct DataInfo *dataInfo) {
    if (dataInfo == nullptr) {
        return;
    }

    int i = 0;
    for (i = 0; i < dataInfo->pathCount; i++) {
        delete[] dataInfo->paths[i];
    }
    delete[] dataInfo->paths;
    delete dataInfo;
}

void FreeDevice(struct Device *device) {
    if (device == nullptr) {
        return;
    }

    delete[] device->name;
    delete[] device->description;
    delete[] device->deviceFile;
    delete[] device->directory;
    delete device;
}

void FreeDeviceList(struct DeviceList *deviceList) {
    if (deviceList == nullptr) {
        return;
    }

    int i = 0;
    for (i = 0; i < deviceList->count; i++) {
        FreeDevice(deviceList->devices[i]);
    }
    delete[] deviceList->devices;
    delete deviceList;
}

int CreateDevice(void *pluginManager, const char *name, const char *description,
                 const char *deviceFile, const char *directory) {
    Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                            "调用");
    if (!IsValidPluginManager(pluginManager)) {
        Fleet::DataManager::Core::Logger::ConsoleLogger().Error(SOURCE_LOCATION,
                                                                "无效的插件管理器指针");
        Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                                "返回");
        return false;
    }
    Fleet::DataManager::Storage::StorageService *storageService =
        (Fleet::DataManager::Storage::StorageService *) ((Fleet::DataManager::Core::PluginManager *)
                                                             pluginManager)
            ->GetService("Storage");
    if (storageService == nullptr) {
        Fleet::DataManager::Core::Logger::ConsoleLogger().Error(SOURCE_LOCATION,
                                                                "未找到 本地存储 插件");
        Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                                "返回");
        return false;
    }
    bool success = storageService->CreateDevice(name, description, deviceFile, directory);
    Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                            "返回");
    return success;
}

int UpdateDevice(void *pluginManager, const char *name, const char *newDescription,
                 const char *newDeviceFile, const char *newDirectory) {
    Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                            "调用");
    if (!IsValidPluginManager(pluginManager)) {
        Fleet::DataManager::Core::Logger::ConsoleLogger().Error(SOURCE_LOCATION,
                                                                "无效的插件管理器指针");
        Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                                "返回");
        return false;
    }
    Fleet::DataManager::Storage::StorageService *storageService =
        (Fleet::DataManager::Storage::StorageService *) ((Fleet::DataManager::Core::PluginManager *)
                                                             pluginManager)
            ->GetService("Storage");
    if (storageService == nullptr) {
        Fleet::DataManager::Core::Logger::ConsoleLogger().Error(SOURCE_LOCATION,
                                                                "未找到 本地存储 插件");
        Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                                "返回");
        return false;
    }
    bool success = storageService->UpdateDevice(name, newDescription, newDeviceFile, newDirectory);
    Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                            "返回");
    return success;
}

int RemoveDevice(void *pluginManager, const char *name) {
    Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                            "调用");
    if (!IsValidPluginManager(pluginManager)) {
        Fleet::DataManager::Core::Logger::ConsoleLogger().Error(SOURCE_LOCATION,
                                                                "无效的插件管理器指针");
        Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                                "返回");
        return false;
    }
    Fleet::DataManager::Storage::StorageService *storageService =
        (Fleet::DataManager::Storage::StorageService *) ((Fleet::DataManager::Core::PluginManager *)
                                                             pluginManager)
            ->GetService("Storage");
    if (storageService == nullptr) {
        Fleet::DataManager::Core::Logger::ConsoleLogger().Error(SOURCE_LOCATION,
                                                                "未找到 本地存储 插件");
        Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                                "返回");
        return false;
    }
    bool success = storageService->RemoveDevice(name);
    Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                            "返回");
    return success;
}

struct Device *BuildDevice(const std::shared_ptr<Fleet::DataManager::Storage::Device> &device) {
    if (device == nullptr) {
        return nullptr;
    }
    struct Device *ret = new struct Device;
    ret->name = new char[device->GetName().size() + 1];
    strcpy(ret->name, device->GetName().c_str());
    ret->description = new char[device->GetDescription().size() + 1];
    strcpy(ret->description, device->GetDescription().c_str());
    ret->deviceFile = new char[device->GetDeviceFile().size() + 1];
    strcpy(ret->deviceFile, device->GetDeviceFile().c_str());
    ret->directory = new char[device->GetDirectory().size() + 1];
    strcpy(ret->directory, device->GetDirectory().c_str());
    return ret;
}

struct DeviceList *BuildDeviceList(
    const std::vector<std::shared_ptr<Fleet::DataManager::Storage::Device>> &deviceList) {
    if (deviceList.empty()) {
        return nullptr;
    }
    struct DeviceList *ret = new struct DeviceList;
    ret->count = deviceList.size();
    ret->devices = new struct Device *[ret->count];
    int i = 0;
    for (i = 0; i < deviceList.size(); i++) {
        ret->devices[i] = BuildDevice(deviceList.at(i));
    }
    return ret;
}

struct Device *GetDeviceByName(void *pluginManager, const char *name) {
    Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                            "调用");
    if (!IsValidPluginManager(pluginManager)) {
        Fleet::DataManager::Core::Logger::ConsoleLogger().Error(SOURCE_LOCATION,
                                                                "无效的插件管理器指针");
        Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                                "返回");
        return nullptr;
    }
    Fleet::DataManager::Storage::StorageService *storageService =
        (Fleet::DataManager::Storage::StorageService *) ((Fleet::DataManager::Core::PluginManager *)
                                                             pluginManager)
            ->GetService("Storage");
    if (storageService == nullptr) {
        Fleet::DataManager::Core::Logger::ConsoleLogger().Error(SOURCE_LOCATION,
                                                                "未找到 本地存储 插件");
        Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                                "返回");
        return nullptr;
    }
    auto device = storageService->GetDeviceByName(name);
    struct Device *ret = BuildDevice(device);
    Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                            "返回");
    return ret;
}

struct Device *GetDefaultDevice(void *pluginManager) {
    Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                            "调用");
    if (!IsValidPluginManager(pluginManager)) {
        Fleet::DataManager::Core::Logger::ConsoleLogger().Error(SOURCE_LOCATION,
                                                                "无效的插件管理器指针");
        Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                                "返回");
        return nullptr;
    }
    Fleet::DataManager::Storage::StorageService *storageService =
        (Fleet::DataManager::Storage::StorageService *) ((Fleet::DataManager::Core::PluginManager *)
                                                             pluginManager)
            ->GetService("Storage");
    if (storageService == nullptr) {
        Fleet::DataManager::Core::Logger::ConsoleLogger().Error(SOURCE_LOCATION,
                                                                "未找到 本地存储 插件");
        Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                                "返回");
        return nullptr;
    }
    auto device = storageService->GetDefaultDevice();
    struct Device *ret = BuildDevice(device);
    Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                            "返回");
    return ret;
}

struct DeviceList *GetAllDevices(void *pluginManager) {
    Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                            "调用");
    if (!IsValidPluginManager(pluginManager)) {
        Fleet::DataManager::Core::Logger::ConsoleLogger().Error(SOURCE_LOCATION,
                                                                "无效的插件管理器指针");
        Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                                "返回");
        return nullptr;
    }
    Fleet::DataManager::Storage::StorageService *storageService =
        (Fleet::DataManager::Storage::StorageService *) ((Fleet::DataManager::Core::PluginManager *)
                                                             pluginManager)
            ->GetService("Storage");
    if (storageService == nullptr) {
        Fleet::DataManager::Core::Logger::ConsoleLogger().Error(SOURCE_LOCATION,
                                                                "未找到 本地存储 插件");
        Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                                "返回");
        return nullptr;
    }
    auto deviceList = storageService->GetAllDevices();
    struct DeviceList *ret = BuildDeviceList(deviceList);
    Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                            "返回");
    return ret;
}

void FreeStrategy(struct Strategy *strategy) {
    if (strategy == nullptr) {
        return;
    }

    delete[] strategy->name;
    delete[] strategy->description;
    int i = 0;
    for (i = 0; i < strategy->locationCount; i++) {
        delete[] strategy->locationDeviceNames[i];
    }
    delete[] strategy->locationDeviceNames;
    for (i = 0; i < strategy->locationCount; i++) {
        delete[] strategy->locationRelativePaths[i];
    }
    delete[] strategy->locationRelativePaths;
    delete[] strategy->errorCorrectingAlgorithm;
    delete[] strategy->integrityCheckAlgorithm;
    delete strategy;
}

void FreeStrategyList(struct StrategyList *strategyList) {
    if (strategyList == nullptr) {
        return;
    }

    int i = 0;
    for (i = 0; i < strategyList->count; i++) {
        FreeStrategy(strategyList->strategies[i]);
    }
    delete[] strategyList->strategies;
    delete strategyList;
}

int CreateStrategy(void *pluginManager, const char *name, const char *description,
                   int locationCount, const char **locationDeviceNames,
                   const char **locationRelativePaths, const char *errorCorrectingAlgorithm,
                   const char *integrityCheckAlgorithm, unsigned long lifeTimeInSecond) {
    Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                            "调用");
    if (!IsValidPluginManager(pluginManager)) {
        Fleet::DataManager::Core::Logger::ConsoleLogger().Error(SOURCE_LOCATION,
                                                                "无效的插件管理器指针");
        Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                                "返回");
        return false;
    }
    Fleet::DataManager::Storage::StorageService *storageService =
        (Fleet::DataManager::Storage::StorageService *) ((Fleet::DataManager::Core::PluginManager *)
                                                             pluginManager)
            ->GetService("Storage");
    if (storageService == nullptr) {
        Fleet::DataManager::Core::Logger::ConsoleLogger().Error(SOURCE_LOCATION,
                                                                "未找到 本地存储 插件");
        Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                                "返回");
        return false;
    }
    std::vector<Fleet::DataManager::Storage::Location> locations;
    int i = 0;
    for (i = 0; i < locationCount; i++) {
        locations.push_back(Fleet::DataManager::Storage::Location(locationDeviceNames[i],
                                                                  locationRelativePaths[i]));
    }
    bool success =
        storageService->CreateStrategy(name, description, locations, errorCorrectingAlgorithm,
                                       integrityCheckAlgorithm, lifeTimeInSecond);
    Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                            "返回");
    return success;
}

int UpdateStrategy(void *pluginManager, const char *name, const char *newDescription,
                   int newLocationCount, const char **newLocationDeviceNames,
                   const char **newLocationRelativePaths, const char *newErrorCorrectingAlgorithm,
                   const char *newIntegrityCheckAlgorithm, unsigned long newLifeTimeInSecond) {
    Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                            "调用");
    if (!IsValidPluginManager(pluginManager)) {
        Fleet::DataManager::Core::Logger::ConsoleLogger().Error(SOURCE_LOCATION,
                                                                "无效的插件管理器指针");
        Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                                "返回");
        return false;
    }
    Fleet::DataManager::Storage::StorageService *storageService =
        (Fleet::DataManager::Storage::StorageService *) ((Fleet::DataManager::Core::PluginManager *)
                                                             pluginManager)
            ->GetService("Storage");
    if (storageService == nullptr) {
        Fleet::DataManager::Core::Logger::ConsoleLogger().Error(SOURCE_LOCATION,
                                                                "未找到 本地存储 插件");
        Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                                "返回");
        return false;
    }
    std::vector<Fleet::DataManager::Storage::Location> newLocations;
    int i = 0;
    for (i = 0; i < newLocationCount; i++) {
        newLocations.push_back(Fleet::DataManager::Storage::Location(newLocationDeviceNames[i],
                                                                     newLocationRelativePaths[i]));
    }
    bool success = storageService->UpdateStrategy(name, newDescription, newLocations,
                                                  newErrorCorrectingAlgorithm,
                                                  newIntegrityCheckAlgorithm, newLifeTimeInSecond);
    Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                            "返回");
    return success;
}

int RemoveStrategy(void *pluginManager, const char *name) {
    Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                            "调用");
    if (!IsValidPluginManager(pluginManager)) {
        Fleet::DataManager::Core::Logger::ConsoleLogger().Error(SOURCE_LOCATION,
                                                                "无效的插件管理器指针");
        Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                                "返回");
        return false;
    }
    Fleet::DataManager::Storage::StorageService *storageService =
        (Fleet::DataManager::Storage::StorageService *) ((Fleet::DataManager::Core::PluginManager *)
                                                             pluginManager)
            ->GetService("Storage");
    if (storageService == nullptr) {
        Fleet::DataManager::Core::Logger::ConsoleLogger().Error(SOURCE_LOCATION,
                                                                "未找到 本地存储 插件");
        Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                                "返回");
        return false;
    }
    bool success = storageService->RemoveStrategy(name);
    Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                            "返回");
    return success;
}

struct Strategy *
BuildStrategy(const std::shared_ptr<Fleet::DataManager::Storage::Strategy> &strategy) {
    if (strategy == nullptr) {
        return nullptr;
    }
    struct Strategy *ret = new struct Strategy;
    ret->name = new char[strategy->GetName().size() + 1];
    strcpy(ret->name, strategy->GetName().c_str());
    ret->description = new char[strategy->GetDescription().size() + 1];
    strcpy(ret->description, strategy->GetDescription().c_str());
    ret->locationCount = strategy->GetLocations().size();
    ret->locationDeviceNames = new char *[ret->locationCount];
    ret->locationRelativePaths = new char *[ret->locationCount];
    int i = 0;
    for (i = 0; i < strategy->GetLocations().size(); i++) {
        const auto &location = strategy->GetLocations().at(i);
        ret->locationDeviceNames[i] = new char[location.GetDeviceName().size() + 1];
        strcpy(ret->locationDeviceNames[i], location.GetDeviceName().c_str());
        ret->locationRelativePaths[i] = new char[location.GetRelativePath().size() + 1];
        strcpy(ret->locationRelativePaths[i], location.GetRelativePath().c_str());
    }
    ret->errorCorrectingAlgorithm = new char[strategy->GetErrorCorrectingAlgorithm().size() + 1];
    strcpy(ret->errorCorrectingAlgorithm, strategy->GetErrorCorrectingAlgorithm().c_str());
    ret->integrityCheckAlgorithm = new char[strategy->GetIntegrityCheckAlgorithm().size() + 1];
    strcpy(ret->integrityCheckAlgorithm, strategy->GetIntegrityCheckAlgorithm().c_str());
    ret->lifeTimeInSecond = strategy->GetLifeTimeInSecond();
    return ret;
}

struct StrategyList *BuildStrategyList(
    const std::vector<std::shared_ptr<Fleet::DataManager::Storage::Strategy>> &strategyList) {
    if (strategyList.empty()) {
        return nullptr;
    }
    struct StrategyList *ret = new struct StrategyList;
    ret->count = strategyList.size();
    ret->strategies = new struct Strategy *[ret->count];
    int i = 0;
    for (i = 0; i < strategyList.size(); i++) {
        ret->strategies[i] = BuildStrategy(strategyList.at(i));
    }
    return ret;
}

struct Strategy *GetStrategyByName(void *pluginManager, const char *name) {
    Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                            "调用");
    if (!IsValidPluginManager(pluginManager)) {
        Fleet::DataManager::Core::Logger::ConsoleLogger().Error(SOURCE_LOCATION,
                                                                "无效的插件管理器指针");
        Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                                "返回");
        return nullptr;
    }
    Fleet::DataManager::Storage::StorageService *storageService =
        (Fleet::DataManager::Storage::StorageService *) ((Fleet::DataManager::Core::PluginManager *)
                                                             pluginManager)
            ->GetService("Storage");
    if (storageService == nullptr) {
        Fleet::DataManager::Core::Logger::ConsoleLogger().Error(SOURCE_LOCATION,
                                                                "未找到 本地存储 插件");
        Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                                "返回");
        return nullptr;
    }
    auto strategy = storageService->GetStrategyByName(name);
    struct Strategy *ret = BuildStrategy(strategy);
    Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                            "返回");
    return ret;
}

struct Strategy *GetDefaultStrategy(void *pluginManager) {
    Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                            "调用");
    if (!IsValidPluginManager(pluginManager)) {
        Fleet::DataManager::Core::Logger::ConsoleLogger().Error(SOURCE_LOCATION,
                                                                "无效的插件管理器指针");
        Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                                "返回");
        return nullptr;
    }
    Fleet::DataManager::Storage::StorageService *storageService =
        (Fleet::DataManager::Storage::StorageService *) ((Fleet::DataManager::Core::PluginManager *)
                                                             pluginManager)
            ->GetService("Storage");
    if (storageService == nullptr) {
        Fleet::DataManager::Core::Logger::ConsoleLogger().Error(SOURCE_LOCATION,
                                                                "未找到 本地存储 插件");
        Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                                "返回");
        return nullptr;
    }
    auto strategy = storageService->GetDefaultStrategy();
    struct Strategy *ret = BuildStrategy(strategy);
    Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                            "返回");
    return ret;
}

struct StrategyList *GetAllStrategies(void *pluginManager) {
    Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                            "调用");
    if (!IsValidPluginManager(pluginManager)) {
        Fleet::DataManager::Core::Logger::ConsoleLogger().Error(SOURCE_LOCATION,
                                                                "无效的插件管理器指针");
        Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                                "返回");
        return nullptr;
    }
    Fleet::DataManager::Storage::StorageService *storageService =
        (Fleet::DataManager::Storage::StorageService *) ((Fleet::DataManager::Core::PluginManager *)
                                                             pluginManager)
            ->GetService("Storage");
    if (storageService == nullptr) {
        Fleet::DataManager::Core::Logger::ConsoleLogger().Error(SOURCE_LOCATION,
                                                                "未找到 本地存储 插件");
        Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                                "返回");
        return nullptr;
    }
    auto strategyList = storageService->GetAllStrategies();
    struct StrategyList *ret = BuildStrategyList(strategyList);
    Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                            "返回");
    return ret;
}

void FreeString(const char *string) {
    if (string == nullptr) {
        return;
    }

    delete[] string;
}

int CreateProfile(void *pluginManager, const char *application, const char *dataType,
                  const char *strategyName) {
    Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                            "调用");
    if (!IsValidPluginManager(pluginManager)) {
        Fleet::DataManager::Core::Logger::ConsoleLogger().Error(SOURCE_LOCATION,
                                                                "无效的插件管理器指针");
        Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                                "返回");
        return false;
    }
    Fleet::DataManager::Storage::StorageService *storageService =
        (Fleet::DataManager::Storage::StorageService *) ((Fleet::DataManager::Core::PluginManager *)
                                                             pluginManager)
            ->GetService("Storage");
    if (storageService == nullptr) {
        Fleet::DataManager::Core::Logger::ConsoleLogger().Error(SOURCE_LOCATION,
                                                                "未找到 本地存储 插件");
        Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                                "返回");
        return false;
    }
    bool success = storageService->CreateProfile(application, dataType, strategyName);
    Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                            "返回");
    return success;
}

int UpdateProfile(void *pluginManager, const char *application, const char *dataType,
                  const char *newStrategyName) {
    Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                            "调用");
    if (!IsValidPluginManager(pluginManager)) {
        Fleet::DataManager::Core::Logger::ConsoleLogger().Error(SOURCE_LOCATION,
                                                                "无效的插件管理器指针");
        Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                                "返回");
        return false;
    }
    Fleet::DataManager::Storage::StorageService *storageService =
        (Fleet::DataManager::Storage::StorageService *) ((Fleet::DataManager::Core::PluginManager *)
                                                             pluginManager)
            ->GetService("Storage");
    if (storageService == nullptr) {
        Fleet::DataManager::Core::Logger::ConsoleLogger().Error(SOURCE_LOCATION,
                                                                "未找到 本地存储 插件");
        Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                                "返回");
        return false;
    }
    bool success = storageService->UpdateProfile(application, dataType, newStrategyName);
    Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                            "返回");
    return success;
}

int RemoveProfile(void *pluginManager, const char *application, const char *dataType) {
    Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                            "调用");
    if (!IsValidPluginManager(pluginManager)) {
        Fleet::DataManager::Core::Logger::ConsoleLogger().Error(SOURCE_LOCATION,
                                                                "无效的插件管理器指针");
        Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                                "返回");
        return false;
    }
    Fleet::DataManager::Storage::StorageService *storageService =
        (Fleet::DataManager::Storage::StorageService *) ((Fleet::DataManager::Core::PluginManager *)
                                                             pluginManager)
            ->GetService("Storage");
    if (storageService == nullptr) {
        Fleet::DataManager::Core::Logger::ConsoleLogger().Error(SOURCE_LOCATION,
                                                                "未找到 本地存储 插件");
        Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                                "返回");
        return false;
    }
    bool success = storageService->RemoveProfile(application, dataType);
    Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                            "返回");
    return success;
}

char *GetProfile(void *pluginManager, const char *application, const char *dataType) {
    Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                            "调用");
    if (!IsValidPluginManager(pluginManager)) {
        Fleet::DataManager::Core::Logger::ConsoleLogger().Error(SOURCE_LOCATION,
                                                                "无效的插件管理器指针");
        Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                                "返回");
        return nullptr;
    }
    Fleet::DataManager::Storage::StorageService *storageService =
        (Fleet::DataManager::Storage::StorageService *) ((Fleet::DataManager::Core::PluginManager *)
                                                             pluginManager)
            ->GetService("Storage");
    if (storageService == nullptr) {
        Fleet::DataManager::Core::Logger::ConsoleLogger().Error(SOURCE_LOCATION,
                                                                "未找到 本地存储 插件");
        Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                                "返回");
        return nullptr;
    }
    auto profile = storageService->GetProfile(application, dataType);
    char *ret = nullptr;
    if (profile != nullptr) {
        ret = new char[profile->size() + 1];
        strcpy(ret, profile->c_str());
    }
    Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                            "返回");
    return ret;
}

int CreateProfileForApplication(void *pluginManager, const char *application,
                                const char *strategyName) {
    Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                            "调用");
    if (!IsValidPluginManager(pluginManager)) {
        Fleet::DataManager::Core::Logger::ConsoleLogger().Error(SOURCE_LOCATION,
                                                                "无效的插件管理器指针");
        Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                                "返回");
        return false;
    }
    Fleet::DataManager::Storage::StorageService *storageService =
        (Fleet::DataManager::Storage::StorageService *) ((Fleet::DataManager::Core::PluginManager *)
                                                             pluginManager)
            ->GetService("Storage");
    if (storageService == nullptr) {
        Fleet::DataManager::Core::Logger::ConsoleLogger().Error(SOURCE_LOCATION,
                                                                "未找到 本地存储 插件");
        Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                                "返回");
        return false;
    }
    bool success = storageService->CreateProfileForApplication(application, strategyName);
    Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                            "返回");
    return success;
}

int UpdateProfileForApplication(void *pluginManager, const char *application,
                                const char *newStrategyName) {
    Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                            "调用");
    if (!IsValidPluginManager(pluginManager)) {
        Fleet::DataManager::Core::Logger::ConsoleLogger().Error(SOURCE_LOCATION,
                                                                "无效的插件管理器指针");
        Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                                "返回");
        return false;
    }
    Fleet::DataManager::Storage::StorageService *storageService =
        (Fleet::DataManager::Storage::StorageService *) ((Fleet::DataManager::Core::PluginManager *)
                                                             pluginManager)
            ->GetService("Storage");
    if (storageService == nullptr) {
        Fleet::DataManager::Core::Logger::ConsoleLogger().Error(SOURCE_LOCATION,
                                                                "未找到 本地存储 插件");
        Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                                "返回");
        return false;
    }
    bool success = storageService->UpdateProfileForApplication(application, newStrategyName);
    Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                            "返回");
    return success;
}

int RemoveProfileForApplication(void *pluginManager, const char *application) {
    Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                            "调用");
    if (!IsValidPluginManager(pluginManager)) {
        Fleet::DataManager::Core::Logger::ConsoleLogger().Error(SOURCE_LOCATION,
                                                                "无效的插件管理器指针");
        Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                                "返回");
        return false;
    }
    Fleet::DataManager::Storage::StorageService *storageService =
        (Fleet::DataManager::Storage::StorageService *) ((Fleet::DataManager::Core::PluginManager *)
                                                             pluginManager)
            ->GetService("Storage");
    if (storageService == nullptr) {
        Fleet::DataManager::Core::Logger::ConsoleLogger().Error(SOURCE_LOCATION,
                                                                "未找到 本地存储 插件");
        Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                                "返回");
        return false;
    }
    bool success = storageService->RemoveProfileForApplication(application);
    Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                            "返回");
    return success;
}

char *GetProfileForApplication(void *pluginManager, const char *application) {
    Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                            "调用");
    if (!IsValidPluginManager(pluginManager)) {
        Fleet::DataManager::Core::Logger::ConsoleLogger().Error(SOURCE_LOCATION,
                                                                "无效的插件管理器指针");
        Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                                "返回");
        return nullptr;
    }
    Fleet::DataManager::Storage::StorageService *storageService =
        (Fleet::DataManager::Storage::StorageService *) ((Fleet::DataManager::Core::PluginManager *)
                                                             pluginManager)
            ->GetService("Storage");
    if (storageService == nullptr) {
        Fleet::DataManager::Core::Logger::ConsoleLogger().Error(SOURCE_LOCATION,
                                                                "未找到 本地存储 插件");
        Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                                "返回");
        return nullptr;
    }
    auto profile = storageService->GetProfileForApplication(application);
    char *ret = nullptr;
    if (profile != nullptr) {
        ret = new char[profile->size() + 1];
        strcpy(ret, profile->c_str());
    }
    Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                            "返回");
    return ret;
}

int CreateProfileForDataType(void *pluginManager, const char *dataType, const char *strategyName) {
    Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                            "调用");
    if (!IsValidPluginManager(pluginManager)) {
        Fleet::DataManager::Core::Logger::ConsoleLogger().Error(SOURCE_LOCATION,
                                                                "无效的插件管理器指针");
        Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                                "返回");
        return false;
    }
    Fleet::DataManager::Storage::StorageService *storageService =
        (Fleet::DataManager::Storage::StorageService *) ((Fleet::DataManager::Core::PluginManager *)
                                                             pluginManager)
            ->GetService("Storage");
    if (storageService == nullptr) {
        Fleet::DataManager::Core::Logger::ConsoleLogger().Error(SOURCE_LOCATION,
                                                                "未找到 本地存储 插件");
        Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                                "返回");
        return false;
    }
    bool success = storageService->CreateProfileForDataType(dataType, strategyName);
    Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                            "返回");
    return success;
}

int UpdateProfileForDataType(void *pluginManager, const char *dataType,
                             const char *newStrategyName) {
    Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                            "调用");
    if (!IsValidPluginManager(pluginManager)) {
        Fleet::DataManager::Core::Logger::ConsoleLogger().Error(SOURCE_LOCATION,
                                                                "无效的插件管理器指针");
        Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                                "返回");
        return false;
    }
    Fleet::DataManager::Storage::StorageService *storageService =
        (Fleet::DataManager::Storage::StorageService *) ((Fleet::DataManager::Core::PluginManager *)
                                                             pluginManager)
            ->GetService("Storage");
    if (storageService == nullptr) {
        Fleet::DataManager::Core::Logger::ConsoleLogger().Error(SOURCE_LOCATION,
                                                                "未找到 本地存储 插件");
        Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                                "返回");
        return false;
    }
    bool success = storageService->UpdateProfileForDataType(dataType, newStrategyName);
    Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                            "返回");
    return success;
}

int RemoveProfileForDataType(void *pluginManager, const char *dataType) {
    Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                            "调用");
    if (!IsValidPluginManager(pluginManager)) {
        Fleet::DataManager::Core::Logger::ConsoleLogger().Error(SOURCE_LOCATION,
                                                                "无效的插件管理器指针");
        Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                                "返回");
        return false;
    }
    Fleet::DataManager::Storage::StorageService *storageService =
        (Fleet::DataManager::Storage::StorageService *) ((Fleet::DataManager::Core::PluginManager *)
                                                             pluginManager)
            ->GetService("Storage");
    if (storageService == nullptr) {
        Fleet::DataManager::Core::Logger::ConsoleLogger().Error(SOURCE_LOCATION,
                                                                "未找到 本地存储 插件");
        Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                                "返回");
        return false;
    }
    bool success = storageService->RemoveProfileForDataType(dataType);
    Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                            "返回");
    return success;
}

char *GetProfileForDataType(void *pluginManager, const char *dataType) {
    Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                            "调用");
    if (!IsValidPluginManager(pluginManager)) {
        Fleet::DataManager::Core::Logger::ConsoleLogger().Error(SOURCE_LOCATION,
                                                                "无效的插件管理器指针");
        Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                                "返回");
        return nullptr;
    }
    Fleet::DataManager::Storage::StorageService *storageService =
        (Fleet::DataManager::Storage::StorageService *) ((Fleet::DataManager::Core::PluginManager *)
                                                             pluginManager)
            ->GetService("Storage");
    if (storageService == nullptr) {
        Fleet::DataManager::Core::Logger::ConsoleLogger().Error(SOURCE_LOCATION,
                                                                "未找到 本地存储 插件");
        Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                                "返回");
        return nullptr;
    }
    auto profile = storageService->GetProfileForDataType(dataType);
    char *ret = nullptr;
    if (profile != nullptr) {
        ret = new char[profile->size() + 1];
        strcpy(ret, profile->c_str());
    }
    Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                            "返回");
    return ret;
}

char *FindProfile(void *pluginManager, const char *application, const char *dataType) {
    Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                            "调用");
    if (!IsValidPluginManager(pluginManager)) {
        Fleet::DataManager::Core::Logger::ConsoleLogger().Error(SOURCE_LOCATION,
                                                                "无效的插件管理器指针");
        Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                                "返回");
        return nullptr;
    }
    Fleet::DataManager::Storage::StorageService *storageService =
        (Fleet::DataManager::Storage::StorageService *) ((Fleet::DataManager::Core::PluginManager *)
                                                             pluginManager)
            ->GetService("Storage");
    if (storageService == nullptr) {
        Fleet::DataManager::Core::Logger::ConsoleLogger().Error(SOURCE_LOCATION,
                                                                "未找到 本地存储 插件");
        Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                                "返回");
        return nullptr;
    }
    auto profile = storageService->FindProfile(application, dataType);
    char *ret = nullptr;
    if (profile != nullptr) {
        ret = new char[profile->size() + 1];
        strcpy(ret, profile->c_str());
    }
    Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                            "返回");
    return ret;
}

void FreeDataBlock(struct DataBlock *dataBlock) {
    if (dataBlock == nullptr) {
        return;
    }

    delete[] dataBlock->data;
    delete dataBlock;
}

struct DataBlock *
BuildDataBlock(const std::shared_ptr<Fleet::DataManager::Storage::DataBlock> &dataBlock) {
    if (dataBlock == nullptr) {
        return nullptr;
    }
    struct DataBlock *ret = new struct DataBlock;
    ret->size = dataBlock->GetSize();
    ret->data = new char[ret->size];
    memcpy(ret->data, dataBlock->GetData(), ret->size);
    return ret;
}

struct DataBlock *ReadData(void *pluginManager, const char *application, const char *dataType,
                           const char *name) {
    Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                            "调用");
    if (!IsValidPluginManager(pluginManager)) {
        Fleet::DataManager::Core::Logger::ConsoleLogger().Error(SOURCE_LOCATION,
                                                                "无效的插件管理器指针");
        Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                                "返回");
        return nullptr;
    }
    Fleet::DataManager::Storage::StorageService *storageService =
        (Fleet::DataManager::Storage::StorageService *) ((Fleet::DataManager::Core::PluginManager *)
                                                             pluginManager)
            ->GetService("Storage");
    if (storageService == nullptr) {
        Fleet::DataManager::Core::Logger::ConsoleLogger().Error(SOURCE_LOCATION,
                                                                "未找到 本地存储 插件");
        Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                                "返回");
        return nullptr;
    }
    auto dataBlock = storageService->ReadData(application, dataType, name);
    auto ret = BuildDataBlock(dataBlock);
    Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                            "返回");
    return ret;
}

struct DataBlock *ReadDataWithVersion(void *pluginManager, const char *application,
                                      const char *dataType, const char *name, const char *version) {
    Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                            "调用");
    if (!IsValidPluginManager(pluginManager)) {
        Fleet::DataManager::Core::Logger::ConsoleLogger().Error(SOURCE_LOCATION,
                                                                "无效的插件管理器指针");
        Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                                "返回");
        return nullptr;
    }
    Fleet::DataManager::Storage::StorageService *storageService =
        (Fleet::DataManager::Storage::StorageService *) ((Fleet::DataManager::Core::PluginManager *)
                                                             pluginManager)
            ->GetService("Storage");
    if (storageService == nullptr) {
        Fleet::DataManager::Core::Logger::ConsoleLogger().Error(SOURCE_LOCATION,
                                                                "未找到 本地存储 插件");
        Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                                "返回");
        return nullptr;
    }
    auto dataBlock = storageService->ReadData(application, dataType, name, version);
    auto ret = BuildDataBlock(dataBlock);
    Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                            "返回");
    return ret;
}

int WriteData(void *pluginManager, const char *application, const char *dataType, const char *name,
              unsigned long size, const char *data) {
    Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                            "调用");
    if (!IsValidPluginManager(pluginManager)) {
        Fleet::DataManager::Core::Logger::ConsoleLogger().Error(SOURCE_LOCATION,
                                                                "无效的插件管理器指针");
        Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                                "返回");
        return false;
    }
    Fleet::DataManager::Storage::StorageService *storageService =
        (Fleet::DataManager::Storage::StorageService *) ((Fleet::DataManager::Core::PluginManager *)
                                                             pluginManager)
            ->GetService("Storage");
    if (storageService == nullptr) {
        Fleet::DataManager::Core::Logger::ConsoleLogger().Error(SOURCE_LOCATION,
                                                                "未找到 本地存储 插件");
        Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                                "返回");
        return false;
    }
    auto dataBlock = std::make_shared<Fleet::DataManager::Storage::DataBlock>(size, data);
    bool success = storageService->WriteData(application, dataType, name, dataBlock);
    Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                            "返回");
    return success;
}

int WriteDataWithVersion(void *pluginManager, const char *application, const char *dataType,
                         const char *name, const char *version, unsigned long size,
                         const char *data) {
    Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                            "调用");
    if (!IsValidPluginManager(pluginManager)) {
        Fleet::DataManager::Core::Logger::ConsoleLogger().Error(SOURCE_LOCATION,
                                                                "无效的插件管理器指针");
        Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                                "返回");
        return false;
    }
    Fleet::DataManager::Storage::StorageService *storageService =
        (Fleet::DataManager::Storage::StorageService *) ((Fleet::DataManager::Core::PluginManager *)
                                                             pluginManager)
            ->GetService("Storage");
    if (storageService == nullptr) {
        Fleet::DataManager::Core::Logger::ConsoleLogger().Error(SOURCE_LOCATION,
                                                                "未找到 本地存储 插件");
        Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                                "返回");
        return false;
    }
    auto dataBlock = std::make_shared<Fleet::DataManager::Storage::DataBlock>(size, data);
    bool success = storageService->WriteData(application, dataType, name, version, dataBlock);
    Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                            "返回");
    return success;
}

int RemoveData(void *pluginManager, const char *application, const char *dataType,
               const char *name) {
    Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                            "调用");
    if (!IsValidPluginManager(pluginManager)) {
        Fleet::DataManager::Core::Logger::ConsoleLogger().Error(SOURCE_LOCATION,
                                                                "无效的插件管理器指针");
        Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                                "返回");
        return false;
    }
    Fleet::DataManager::Storage::StorageService *storageService =
        (Fleet::DataManager::Storage::StorageService *) ((Fleet::DataManager::Core::PluginManager *)
                                                             pluginManager)
            ->GetService("Storage");
    if (storageService == nullptr) {
        Fleet::DataManager::Core::Logger::ConsoleLogger().Error(SOURCE_LOCATION,
                                                                "未找到 本地存储 插件");
        Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                                "返回");
        return false;
    }
    bool success = storageService->RemoveData(application, dataType, name);
    Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                            "返回");
    return success;
}

int RemoveDataWithVersion(void *pluginManager, const char *application, const char *dataType,
                          const char *name, const char *version) {
    Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                            "调用");
    if (!IsValidPluginManager(pluginManager)) {
        Fleet::DataManager::Core::Logger::ConsoleLogger().Error(SOURCE_LOCATION,
                                                                "无效的插件管理器指针");
        Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                                "返回");
        return false;
    }
    Fleet::DataManager::Storage::StorageService *storageService =
        (Fleet::DataManager::Storage::StorageService *) ((Fleet::DataManager::Core::PluginManager *)
                                                             pluginManager)
            ->GetService("Storage");
    if (storageService == nullptr) {
        Fleet::DataManager::Core::Logger::ConsoleLogger().Error(SOURCE_LOCATION,
                                                                "未找到 本地存储 插件");
        Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                                "返回");
        return false;
    }
    bool success = storageService->RemoveData(application, dataType, name, version);
    Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                            "返回");
    return success;
}

int RepairData(void *pluginManager, const char *application, const char *dataType,
               const char *name) {
    Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                            "调用");
    if (!IsValidPluginManager(pluginManager)) {
        Fleet::DataManager::Core::Logger::ConsoleLogger().Error(SOURCE_LOCATION,
                                                                "无效的插件管理器指针");
        Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                                "返回");
        return false;
    }
    Fleet::DataManager::Storage::StorageService *storageService =
        (Fleet::DataManager::Storage::StorageService *) ((Fleet::DataManager::Core::PluginManager *)
                                                             pluginManager)
            ->GetService("Storage");
    if (storageService == nullptr) {
        Fleet::DataManager::Core::Logger::ConsoleLogger().Error(SOURCE_LOCATION,
                                                                "未找到 本地存储 插件");
        Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                                "返回");
        return false;
    }
    bool success = storageService->RepairData(application, dataType, name);
    Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                            "返回");
    return success;
}

int RepairDataWithVersion(void *pluginManager, const char *application, const char *dataType,
                          const char *name, const char *version) {
    Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                            "调用");
    if (!IsValidPluginManager(pluginManager)) {
        Fleet::DataManager::Core::Logger::ConsoleLogger().Error(SOURCE_LOCATION,
                                                                "无效的插件管理器指针");
        Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                                "返回");
        return false;
    }
    Fleet::DataManager::Storage::StorageService *storageService =
        (Fleet::DataManager::Storage::StorageService *) ((Fleet::DataManager::Core::PluginManager *)
                                                             pluginManager)
            ->GetService("Storage");
    if (storageService == nullptr) {
        Fleet::DataManager::Core::Logger::ConsoleLogger().Error(SOURCE_LOCATION,
                                                                "未找到 本地存储 插件");
        Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                                "返回");
        return false;
    }
    bool success = storageService->RepairData(application, dataType, name, version);
    Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                            "返回");
    return success;
}

int StoreTopic(void *pluginManager, const char *topic) {
    Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                            "调用");
    if (!IsValidPluginManager(pluginManager)) {
        Fleet::DataManager::Core::Logger::ConsoleLogger().Error(SOURCE_LOCATION,
                                                                "无效的插件管理器指针");
        Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                                "返回");
        return false;
    }

    if (topic == nullptr) {
        Fleet::DataManager::Core::Logger::ConsoleLogger().Error(SOURCE_LOCATION,
                                                                "主题参数为空");
        Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                                "返回");
        return false;
    }

    Fleet::DataManager::Messaging::MessagingService *messagingService =
        (Fleet::DataManager::Messaging::MessagingService
             *) ((Fleet::DataManager::Core::PluginManager *) pluginManager)
            ->GetService("Messaging");
    if (messagingService == nullptr) {
        Fleet::DataManager::Core::Logger::ConsoleLogger().Error(SOURCE_LOCATION,
                                                                "未找到消息协同插件");
        Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                                "返回");
        return false;
    }

    bool result = messagingService->StoreTopic(std::string(topic));
    Fleet::DataManager::Core::Logger::ConsoleLogger().Info(
        SOURCE_LOCATION, "StoreTopic操作完成，主题: {}，结果: {}", topic, result);
    Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                            "返回");
    return result ? 1 : 0;
}

int QueryTopic(void *pluginManager, const char *topic) {
    Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                            "调用");
    if (!IsValidPluginManager(pluginManager)) {
        Fleet::DataManager::Core::Logger::ConsoleLogger().Error(SOURCE_LOCATION,
                                                                "无效的插件管理器指针");
        Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                                "返回");
        return -1;
    }

    if (topic == nullptr) {
        Fleet::DataManager::Core::Logger::ConsoleLogger().Error(SOURCE_LOCATION,
                                                                "主题参数为空");
        Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                                "返回");
        return -1;
    }

    Fleet::DataManager::Messaging::MessagingService *messagingService =
        (Fleet::DataManager::Messaging::MessagingService
             *) ((Fleet::DataManager::Core::PluginManager *) pluginManager)
            ->GetService("Messaging");
    if (messagingService == nullptr) {
        Fleet::DataManager::Core::Logger::ConsoleLogger().Error(SOURCE_LOCATION,
                                                                "未找到消息协同插件");
        Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                                "返回");
        return -1;
    }

    bool result = messagingService->QueryTopic(std::string(topic));
    Fleet::DataManager::Core::Logger::ConsoleLogger().Info(
        SOURCE_LOCATION, "QueryTopic操作完成，主题: {}，结果: {}", topic, result);
    Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                            "返回");
    return result ? 1 : 0;
}

int DeleteTopic(void *pluginManager, const char *topic) {
    Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                            "调用");
    if (!IsValidPluginManager(pluginManager)) {
        Fleet::DataManager::Core::Logger::ConsoleLogger().Error(SOURCE_LOCATION,
                                                                "无效的插件管理器指针");
        Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                                "返回");
        return false;
    }

    if (topic == nullptr) {
        Fleet::DataManager::Core::Logger::ConsoleLogger().Error(SOURCE_LOCATION,
                                                                "主题参数为空");
        Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                                "返回");
        return false;
    }

    Fleet::DataManager::Messaging::MessagingService *messagingService =
        (Fleet::DataManager::Messaging::MessagingService
             *) ((Fleet::DataManager::Core::PluginManager *) pluginManager)
            ->GetService("Messaging");
    if (messagingService == nullptr) {
        Fleet::DataManager::Core::Logger::ConsoleLogger().Error(SOURCE_LOCATION,
                                                                "未找到消息协同插件");
        Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                                "返回");
        return false;
    }

    bool result = messagingService->DeleteTopic(std::string(topic));
    Fleet::DataManager::Core::Logger::ConsoleLogger().Info(
        SOURCE_LOCATION, "DeleteTopic操作完成，主题: {}，结果: {}", topic, result);
    Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                            "返回");
    return result ? 1 : 0;
}

int SetSpaceLimitSize(void *pluginManager, uint64_t size) {
    Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                            "调用");
    if (!IsValidPluginManager(pluginManager)) {
        Fleet::DataManager::Core::Logger::ConsoleLogger().Error(SOURCE_LOCATION,
                                                                "无效的插件管理器指针");
        Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                                "返回");
        return false;
    }
    Fleet::DataManager::Storage::StorageService *storageService =
        (Fleet::DataManager::Storage::StorageService *) ((Fleet::DataManager::Core::PluginManager *)
                                                             pluginManager)
            ->GetService("Storage");
    if (storageService == nullptr) {
        Fleet::DataManager::Core::Logger::ConsoleLogger().Error(SOURCE_LOCATION,
                                                                "未找到 本地存储 插件");
        Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                                "返回");
        return false;
    }
    bool success = storageService->SetSpaceLimitSize(size);
    Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                            "返回");
    return success;
}

int GetSpaceLimitSize(void *pluginManager, uint64_t *size) {
    Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                            "调用");
    if (!IsValidPluginManager(pluginManager)) {
        Fleet::DataManager::Core::Logger::ConsoleLogger().Error(SOURCE_LOCATION,
                                                                "无效的插件管理器指针");
        Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                                "返回");
        return false;
    }
    Fleet::DataManager::Storage::StorageService *storageService =
        (Fleet::DataManager::Storage::StorageService *) ((Fleet::DataManager::Core::PluginManager *)
                                                             pluginManager)
            ->GetService("Storage");
    if (storageService == nullptr) {
        Fleet::DataManager::Core::Logger::ConsoleLogger().Error(SOURCE_LOCATION,
                                                                "未找到 本地存储 插件");
        Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                                "返回");
        return false;
    }
    bool success = storageService->GetSpaceLimitSize(size);
    Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                            "返回");
    return success;
}

int GetUsedSpaceSize(void *pluginManager, uint64_t *size) {
    Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                            "调用");
    if (!IsValidPluginManager(pluginManager)) {
        Fleet::DataManager::Core::Logger::ConsoleLogger().Error(SOURCE_LOCATION,
                                                                "无效的插件管理器指针");
        Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                                "返回");
        return false;
    }
    Fleet::DataManager::Storage::StorageService *storageService =
        (Fleet::DataManager::Storage::StorageService *) ((Fleet::DataManager::Core::PluginManager *)
                                                             pluginManager)
            ->GetService("Storage");
    if (storageService == nullptr) {
        Fleet::DataManager::Core::Logger::ConsoleLogger().Error(SOURCE_LOCATION,
                                                                "未找到 本地存储 插件");
        Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                                "返回");
        return false;
    }
    bool success = storageService->GetUsedSpaceSize(size);
    Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                            "返回");
    return success;
}

int GetTotalSpaceSize(void *pluginManager, uint64_t *size) {
    Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                            "调用");
    if (!IsValidPluginManager(pluginManager)) {
        Fleet::DataManager::Core::Logger::ConsoleLogger().Error(SOURCE_LOCATION,
                                                                "无效的插件管理器指针");
        Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                                "返回");
        return false;
    }
    Fleet::DataManager::Storage::StorageService *storageService =
        (Fleet::DataManager::Storage::StorageService *) ((Fleet::DataManager::Core::PluginManager *)
                                                             pluginManager)
            ->GetService("Storage");
    if (storageService == nullptr) {
        Fleet::DataManager::Core::Logger::ConsoleLogger().Error(SOURCE_LOCATION,
                                                                "未找到 本地存储 插件");
        Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                                "返回");
        return false;
    }
    bool success = storageService->GetTotalSpaceSize(size);
    Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                            "返回");
    return success;
}

struct NodeInfo *CreateNodeInfo() {
    Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                            "调用");
    struct NodeInfo *nodeInfo = new struct NodeInfo();
    if (nodeInfo != nullptr) {
        nodeInfo->uuid = nullptr;
        nodeInfo->ipAddress = nullptr;
    }
    Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                            "返回");
    return nodeInfo;
}

void FreeNodeInfo(struct NodeInfo *nodeInfo) {
    Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                            "调用");
    if (nodeInfo != nullptr) {
        if (nodeInfo->uuid != nullptr) {
            delete[] nodeInfo->uuid;
            nodeInfo->uuid = nullptr;
        }
        if (nodeInfo->ipAddress != nullptr) {
            delete[] nodeInfo->ipAddress;
            nodeInfo->ipAddress = nullptr;
        }
        delete nodeInfo;
    }
    Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                            "返回");
}

void FreeNodeInfoList(struct NodeInfoList *nodeInfoList) {
    Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                            "调用");
    if (nodeInfoList != nullptr) {
        if (nodeInfoList->nodes != nullptr) {
            for (unsigned long i = 0; i < nodeInfoList->count; i++) {
                FreeNodeInfo(nodeInfoList->nodes[i]);
            }
            delete[] nodeInfoList->nodes;
            nodeInfoList->nodes = nullptr;
        }
        delete nodeInfoList;
    }
    Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                            "返回");
}

int CreateNode(void *pluginManager, const char *uuid, const char *ipAddress) {
    Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                            "调用");
    if (!IsValidPluginManager(pluginManager)) {
        Fleet::DataManager::Core::Logger::ConsoleLogger().Error(SOURCE_LOCATION,
                                                                "无效的插件管理器指针");
        Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                                "返回");
        return false;
    }

    if (uuid == nullptr || ipAddress == nullptr) {
        Fleet::DataManager::Core::Logger::ConsoleLogger().Error(SOURCE_LOCATION,
                                                                "UUID或IP地址参数为空");
        Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                                "返回");
        return false;
    }

    Fleet::DataManager::Messaging::MessagingService *messagingService =
        (Fleet::DataManager::Messaging::MessagingService
             *) ((Fleet::DataManager::Core::PluginManager *) pluginManager)
            ->GetService("Messaging");
    if (messagingService == nullptr) {
        Fleet::DataManager::Core::Logger::ConsoleLogger().Error(SOURCE_LOCATION,
                                                                "未找到 消息协同 插件");
        Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                                "返回");
        return false;
    }

    bool success = messagingService->CreateNode(std::string(uuid), std::string(ipAddress));
    Fleet::DataManager::Core::Logger::ConsoleLogger().Info(
        SOURCE_LOCATION, "创建节点操作完成，UUID: {}，IP: {}，结果: {}", uuid,
        ipAddress, success);
    Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                            "返回");
    return success ? 1 : 0;
}

int UpdateNode(void *pluginManager, const char *uuid, const char *newIpAddress) {
    Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                            "调用");
    if (!IsValidPluginManager(pluginManager)) {
        Fleet::DataManager::Core::Logger::ConsoleLogger().Error(SOURCE_LOCATION,
                                                                "无效的插件管理器指针");
        Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                                "返回");
        return false;
    }

    if (uuid == nullptr || newIpAddress == nullptr) {
        Fleet::DataManager::Core::Logger::ConsoleLogger().Error(SOURCE_LOCATION,
                                                                "UUID或新IP地址参数为空");
        Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                                "返回");
        return false;
    }

    Fleet::DataManager::Messaging::MessagingService *messagingService =
        (Fleet::DataManager::Messaging::MessagingService
             *) ((Fleet::DataManager::Core::PluginManager *) pluginManager)
            ->GetService("Messaging");
    if (messagingService == nullptr) {
        Fleet::DataManager::Core::Logger::ConsoleLogger().Error(SOURCE_LOCATION,
                                                                "未找到 消息协同 插件");
        Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                                "返回");
        return false;
    }

    bool success = messagingService->UpdateNode(std::string(uuid), std::string(newIpAddress));
    Fleet::DataManager::Core::Logger::ConsoleLogger().Info(
        SOURCE_LOCATION, "更新节点操作完成，UUID: {}，新IP: {}，结果: {}", uuid,
        newIpAddress, success);
    Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                            "返回");
    return success ? 1 : 0;
}

int RemoveNode(void *pluginManager, const char *uuid) {
    Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                            "调用");
    if (!IsValidPluginManager(pluginManager)) {
        Fleet::DataManager::Core::Logger::ConsoleLogger().Error(SOURCE_LOCATION,
                                                                "无效的插件管理器指针");
        Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                                "返回");
        return false;
    }

    if (uuid == nullptr) {
        Fleet::DataManager::Core::Logger::ConsoleLogger().Error(SOURCE_LOCATION,
                                                                "UUID参数为空");
        Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                                "返回");
        return false;
    }

    Fleet::DataManager::Messaging::MessagingService *messagingService =
        (Fleet::DataManager::Messaging::MessagingService
             *) ((Fleet::DataManager::Core::PluginManager *) pluginManager)
            ->GetService("Messaging");
    if (messagingService == nullptr) {
        Fleet::DataManager::Core::Logger::ConsoleLogger().Error(SOURCE_LOCATION,
                                                                "未找到 消息协同 插件");
        Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                                "返回");
        return false;
    }

    bool success = messagingService->RemoveNode(std::string(uuid));
    Fleet::DataManager::Core::Logger::ConsoleLogger().Info(
        SOURCE_LOCATION, "删除节点操作完成，UUID: {}，结果: {}", uuid, success);
    Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                            "返回");
    return success ? 1 : 0;
}

struct NodeInfo *GetNodeByUuid(void *pluginManager, const char *uuid) {
    Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                            "调用");
    if (!IsValidPluginManager(pluginManager)) {
        Fleet::DataManager::Core::Logger::ConsoleLogger().Error(SOURCE_LOCATION,
                                                                "无效的插件管理器指针");
        Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                                "返回");
        return nullptr;
    }

    if (uuid == nullptr) {
        Fleet::DataManager::Core::Logger::ConsoleLogger().Error(SOURCE_LOCATION,
                                                                "UUID参数为空");
        Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                                "返回");
        return nullptr;
    }

    Fleet::DataManager::Messaging::MessagingService *messagingService =
        (Fleet::DataManager::Messaging::MessagingService
             *) ((Fleet::DataManager::Core::PluginManager *) pluginManager)
            ->GetService("Messaging");
    if (messagingService == nullptr) {
        Fleet::DataManager::Core::Logger::ConsoleLogger().Error(SOURCE_LOCATION,
                                                                "未找到 消息协同 插件");
        Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                                "返回");
        return nullptr;
    }

    auto nodeInfo = messagingService->GetNodeByUuid(std::string(uuid));
    if (nodeInfo == nullptr) {
        Fleet::DataManager::Core::Logger::ConsoleLogger().Info(SOURCE_LOCATION,
                                                               "未找到UUID为 {} 的节点", uuid);
        Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                                "返回");
        return nullptr;
    }

    struct NodeInfo *result = CreateNodeInfo();
    if (result != nullptr) {
        const std::string &uuidStr = nodeInfo->GetUuid();
        const std::string &ipStr = nodeInfo->GetIpAddress();
        result->uuid = new char[uuidStr.size() + 1];
        result->ipAddress = new char[ipStr.size() + 1];
        strcpy(result->uuid, uuidStr.c_str());
        strcpy(result->ipAddress, ipStr.c_str());
    }

    Fleet::DataManager::Core::Logger::ConsoleLogger().Info(SOURCE_LOCATION,
                                                           "查询节点操作完成，UUID: {}，IP: {}",
                                                           uuid, nodeInfo->GetIpAddress().c_str());
    Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                            "返回");
    return result;
}

struct NodeInfo *GetNodeByIpAddress(void *pluginManager, const char *ipAddress) {
    Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                            "调用");
    if (!IsValidPluginManager(pluginManager)) {
        Fleet::DataManager::Core::Logger::ConsoleLogger().Error(SOURCE_LOCATION,
                                                                "无效的插件管理器指针");
        Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                                "返回");
        return nullptr;
    }

    if (ipAddress == nullptr) {
        Fleet::DataManager::Core::Logger::ConsoleLogger().Error(SOURCE_LOCATION,
                                                                "IP地址参数为空");
        Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                                "返回");
        return nullptr;
    }

    Fleet::DataManager::Messaging::MessagingService *messagingService =
        (Fleet::DataManager::Messaging::MessagingService
             *) ((Fleet::DataManager::Core::PluginManager *) pluginManager)
            ->GetService("Messaging");
    if (messagingService == nullptr) {
        Fleet::DataManager::Core::Logger::ConsoleLogger().Error(SOURCE_LOCATION,
                                                                "未找到 消息协同 插件");
        Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                                "返回");
        return nullptr;
    }

    auto nodeInfo = messagingService->GetNodeByIpAddress(std::string(ipAddress));
    if (nodeInfo == nullptr) {
        Fleet::DataManager::Core::Logger::ConsoleLogger().Info(
            SOURCE_LOCATION, "未找到IP地址为 {} 的节点", ipAddress);
        Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                                "返回");
        return nullptr;
    }

    struct NodeInfo *result = CreateNodeInfo();
    if (result != nullptr) {
        const std::string &uuidStr = nodeInfo->GetUuid();
        const std::string &ipStr = nodeInfo->GetIpAddress();
        result->uuid = new char[uuidStr.size() + 1];
        result->ipAddress = new char[ipStr.size() + 1];
        strcpy(result->uuid, uuidStr.c_str());
        strcpy(result->ipAddress, ipStr.c_str());
    }

    Fleet::DataManager::Core::Logger::ConsoleLogger().Info(SOURCE_LOCATION,
                                                           "查询节点操作完成，IP: {}，UUID: {}",
                                                           ipAddress, nodeInfo->GetUuid().c_str());
    Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                            "返回");
    return result;
}

struct NodeInfoList *GetAllNodes(void *pluginManager) {
    Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                            "调用");
    if (!IsValidPluginManager(pluginManager)) {
        Fleet::DataManager::Core::Logger::ConsoleLogger().Error(SOURCE_LOCATION,
                                                                "无效的插件管理器指针");
        Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                                "返回");
        return nullptr;
    }

    Fleet::DataManager::Messaging::MessagingService *messagingService =
        (Fleet::DataManager::Messaging::MessagingService
             *) ((Fleet::DataManager::Core::PluginManager *) pluginManager)
            ->GetService("Messaging");
    if (messagingService == nullptr) {
        Fleet::DataManager::Core::Logger::ConsoleLogger().Error(SOURCE_LOCATION,
                                                                "未找到 消息协同 插件");
        Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                                "返回");
        return nullptr;
    }

    auto nodeInfoList = messagingService->GetAllNodes();

    struct NodeInfoList *result = new struct NodeInfoList();
    if (result == nullptr) {
        Fleet::DataManager::Core::Logger::ConsoleLogger().Error(SOURCE_LOCATION,
                                                                "内存分配失败");
        Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                                "返回");
        return nullptr;
    }

    result->count = nodeInfoList.size();
    if (result->count > 0) {
        result->nodes = new struct NodeInfo *[result->count];
        if (result->nodes == nullptr) {
            delete result;
            Fleet::DataManager::Core::Logger::ConsoleLogger().Error(SOURCE_LOCATION,
                                                                    "内存分配失败");
            Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                                    "返回");
            return nullptr;
        }

        for (size_t i = 0; i < result->count; i++) {
            result->nodes[i] = CreateNodeInfo();
            if (result->nodes[i] != nullptr) {
                const std::string &uuidStr = nodeInfoList[i]->GetUuid();
                const std::string &ipStr = nodeInfoList[i]->GetIpAddress();
                result->nodes[i]->uuid = new char[uuidStr.size() + 1];
                result->nodes[i]->ipAddress = new char[ipStr.size() + 1];
                strcpy(result->nodes[i]->uuid, uuidStr.c_str());
                strcpy(result->nodes[i]->ipAddress, ipStr.c_str());
            }
        }
    } else {
        result->nodes = nullptr;
    }

    Fleet::DataManager::Core::Logger::ConsoleLogger().Info(
        SOURCE_LOCATION, "获取所有节点操作完成，共 {} 个节点", result->count);
    Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                            "返回");
    return result;
}

int NodeExists(void *pluginManager, const char *uuid) {
    Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                            "调用");
    if (!IsValidPluginManager(pluginManager)) {
        Fleet::DataManager::Core::Logger::ConsoleLogger().Error(SOURCE_LOCATION,
                                                                "无效的插件管理器指针");
        Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                                "返回");
        return -1;
    }

    if (uuid == nullptr) {
        Fleet::DataManager::Core::Logger::ConsoleLogger().Error(SOURCE_LOCATION,
                                                                "UUID参数为空");
        Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                                "返回");
        return -1;
    }

    Fleet::DataManager::Messaging::MessagingService *messagingService =
        (Fleet::DataManager::Messaging::MessagingService
             *) ((Fleet::DataManager::Core::PluginManager *) pluginManager)
            ->GetService("Messaging");
    if (messagingService == nullptr) {
        Fleet::DataManager::Core::Logger::ConsoleLogger().Error(SOURCE_LOCATION,
                                                                "未找到 消息协同 插件");
        Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                                "返回");
        return -1;
    }

    bool exists = messagingService->NodeExists(std::string(uuid));
    Fleet::DataManager::Core::Logger::ConsoleLogger().Info(
        SOURCE_LOCATION, "检查节点存在性操作完成，UUID: {}，结果: {}", uuid,
        exists);
    Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                            "返回");
    return exists ? 1 : 0;
}

int UploadData(void *pluginManager, const char *dataType, const char *name, const char *to,
               const char *data, int length) {
    Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                            "调用");
    if (!IsValidPluginManager(pluginManager)) {
        Fleet::DataManager::Core::Logger::ConsoleLogger().Error(SOURCE_LOCATION,
                                                                "无效的插件管理器指针");
        Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                                "返回");
        return false;
    }
    Fleet::DataManager::Portal::PortalService *portalService =
        (Fleet::DataManager::Portal::PortalService *) ((Fleet::DataManager::Core::PluginManager *)
                                                           pluginManager)
            ->GetService("Portal");
    if (portalService != nullptr) {
        Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                                "返回");
        return portalService->UploadData(dataType, name, to, data, length);
    } else {
        Fleet::DataManager::Core::Logger::ConsoleLogger().Error(SOURCE_LOCATION,
                                                                "未找到 存储门户 插件");
        Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                                "返回");
        return false;
    }
}

int UploadSecurityDataToAutoDownloadArea(void *pluginManager, const char *key, const char *data,
                                         int length) {
    Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                            "调用");
    if (!IsValidPluginManager(pluginManager)) {
        Fleet::DataManager::Core::Logger::ConsoleLogger().Error(SOURCE_LOCATION,
                                                                "无效的插件管理器指针");
        Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                                "返回");
        return false;
    }
    Fleet::DataManager::Portal::PortalService *portalService =
        (Fleet::DataManager::Portal::PortalService *) ((Fleet::DataManager::Core::PluginManager *)
                                                           pluginManager)
            ->GetService("Portal");
    if (portalService != nullptr) {
        Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                                "返回");
        return portalService->UploadSecurityDataToAutoDownloadArea(key, data, length);
    } else {
        Fleet::DataManager::Core::Logger::ConsoleLogger().Error(SOURCE_LOCATION,
                                                                "未找到 存储门户 插件");
        Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                                "返回");
        return false;
    }
}

int UploadSegmentData(void *pluginManager, const char *dataType, const char *key, const char *owner,
                      int currentSegmentId, int totalSegmentCount, int totalSize, int segmentSize,
                      const char *segmentData) {
    Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                            "调用");
    if (!IsValidPluginManager(pluginManager)) {
        Fleet::DataManager::Core::Logger::ConsoleLogger().Error(SOURCE_LOCATION,
                                                                "无效的插件管理器指针");
        Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                                "返回");
        return false;
    }
    Fleet::DataManager::Portal::PortalService *portalService =
        (Fleet::DataManager::Portal::PortalService *) ((Fleet::DataManager::Core::PluginManager *)
                                                           pluginManager)
            ->GetService("Portal");
    if (portalService != nullptr) {
        Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                                "返回");
        return portalService->UploadSegmentData(dataType, key, owner, currentSegmentId,
                                                totalSegmentCount, totalSize, segmentSize,
                                                segmentData);
    } else {
        Fleet::DataManager::Core::Logger::ConsoleLogger().Error(SOURCE_LOCATION,
                                                                "未找到 存储门户 插件");
        Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                                "返回");
        return false;
    }
}

int DownloadData(void *pluginManager, const char *dataType, const char *name, const char *from,
                 char **data) {
    Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                            "调用");
    if (!IsValidPluginManager(pluginManager)) {
        Fleet::DataManager::Core::Logger::ConsoleLogger().Error(SOURCE_LOCATION,
                                                                "无效的插件管理器指针");
        Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                                "返回");
        return false;
    }
    Fleet::DataManager::Portal::PortalService *portalService =
        (Fleet::DataManager::Portal::PortalService *) ((Fleet::DataManager::Core::PluginManager *)
                                                           pluginManager)
            ->GetService("Portal");
    if (portalService != nullptr) {
        Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                                "返回");
        return portalService->DownloadData(dataType, name, from, *data);
    } else {
        Fleet::DataManager::Core::Logger::ConsoleLogger().Error(SOURCE_LOCATION,
                                                                "未找到 存储门户 插件");
        Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                                "返回");
        return false;
    }
}

int QueryData(void *pluginManager, const char *dataType, const char *name, char **data) {
    Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                            "调用");
    if (!IsValidPluginManager(pluginManager)) {
        Fleet::DataManager::Core::Logger::ConsoleLogger().Error(SOURCE_LOCATION,
                                                                "无效的插件管理器指针");
        Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                                "返回");
        return false;
    }
    Fleet::DataManager::Portal::PortalService *portalService =
        (Fleet::DataManager::Portal::PortalService *) ((Fleet::DataManager::Core::PluginManager *)
                                                           pluginManager)
            ->GetService("Portal");
    if (portalService != nullptr) {
        Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                                "返回");
        return portalService->QueryData(dataType, name, *data);
    } else {
        Fleet::DataManager::Core::Logger::ConsoleLogger().Error(SOURCE_LOCATION,
                                                                "未找到 存储门户 插件");
        Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                                "返回");
        return false;
    }
}

int SyncData(void *pluginManager, const char *dataType, const char *name, const char *dataOwner,
             const char *node) {
    Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                            "调用");
    if (!IsValidPluginManager(pluginManager)) {
        Fleet::DataManager::Core::Logger::ConsoleLogger().Error(SOURCE_LOCATION,
                                                                "无效的插件管理器指针");
        Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                                "返回");
        return false;
    }
    Fleet::DataManager::Portal::PortalService *portalService =
        (Fleet::DataManager::Portal::PortalService *) ((Fleet::DataManager::Core::PluginManager *)
                                                           pluginManager)
            ->GetService("Portal");
    if (portalService != nullptr) {
        Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                                "返回");
        return portalService->SyncData(dataType, name, dataOwner, node);
    } else {
        Fleet::DataManager::Core::Logger::ConsoleLogger().Error(SOURCE_LOCATION,
                                                                "未找到 存储门户 插件");
        Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                                "返回");
        return false;
    }
}

int SyncObject(void *pluginManager, const char *name, const char *dataOwner, const char *node) {
    Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                            "调用");
    if (!IsValidPluginManager(pluginManager)) {
        Fleet::DataManager::Core::Logger::ConsoleLogger().Error(SOURCE_LOCATION,
                                                                "无效的插件管理器指针");
        Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                                "返回");
        return false;
    }
    Fleet::DataManager::Portal::PortalService *portalService =
        (Fleet::DataManager::Portal::PortalService *) ((Fleet::DataManager::Core::PluginManager *)
                                                           pluginManager)
            ->GetService("Portal");
    if (portalService != nullptr) {
        Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                                "返回");
        return portalService->SyncObject(name, dataOwner, node);
    } else {
        Fleet::DataManager::Core::Logger::ConsoleLogger().Error(SOURCE_LOCATION,
                                                                "未找到 存储门户 插件");
        Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                                "返回");
        return false;
    }
}

int ReceiveTelemetry(void *pluginManager, const char *from) {
    Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                            "调用");
    if (!IsValidPluginManager(pluginManager)) {
        Fleet::DataManager::Core::Logger::ConsoleLogger().Error(SOURCE_LOCATION,
                                                                "无效的插件管理器指针");
        Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                                "返回");
        return false;
    }
    Fleet::DataManager::Portal::PortalService *portalService =
        (Fleet::DataManager::Portal::PortalService *) ((Fleet::DataManager::Core::PluginManager *)
                                                           pluginManager)
            ->GetService("Portal");
    if (portalService != nullptr) {
        Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                                "返回");
        return portalService->ReceiveTelemetry(from);
    } else {
        Fleet::DataManager::Core::Logger::ConsoleLogger().Error(SOURCE_LOCATION,
                                                                "未找到 存储门户 插件");
        Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                                "返回");
        return false;
    }
}

int ReportTelemetry(void *pluginManager, const char *from, const char *data, int length) {
    Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                            "调用");
    if (!IsValidPluginManager(pluginManager)) {
        Fleet::DataManager::Core::Logger::ConsoleLogger().Error(SOURCE_LOCATION,
                                                                "无效的插件管理器指针");
        Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                                "返回");
        return false;
    }
    Fleet::DataManager::Portal::PortalService *portalService =
        (Fleet::DataManager::Portal::PortalService *) ((Fleet::DataManager::Core::PluginManager *)
                                                           pluginManager)
            ->GetService("Portal");
    if (portalService != nullptr) {
        Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                                "返回");
        return portalService->ReportTelemetry(from, data, length);
    } else {
        Fleet::DataManager::Core::Logger::ConsoleLogger().Error(SOURCE_LOCATION,
                                                                "未找到 存储门户 插件");
        Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                                "返回");
        return false;
    }
}

int SendRemoteControl(void *pluginManager, const char *to, const char *data, int length) {
    Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                            "调用");
    if (!IsValidPluginManager(pluginManager)) {
        Fleet::DataManager::Core::Logger::ConsoleLogger().Error(SOURCE_LOCATION,
                                                                "无效的插件管理器指针");
        Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                                "返回");
        return false;
    }
    Fleet::DataManager::Portal::PortalService *portalService =
        (Fleet::DataManager::Portal::PortalService *) ((Fleet::DataManager::Core::PluginManager *)
                                                           pluginManager)
            ->GetService("Portal");
    if (portalService != nullptr) {
        Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                                "返回");
        return portalService->SendRemoteControl(to, data, length);
    } else {
        Fleet::DataManager::Core::Logger::ConsoleLogger().Error(SOURCE_LOCATION,
                                                                "未找到 存储门户 插件");
        Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                                "返回");
        return false;
    }
}

int RegisterRemoteControlCallback(void *pluginManager, RemoteControlCallback callback,
                                  void *closure) {
    Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                            "调用");
    if (!IsValidPluginManager(pluginManager)) {
        Fleet::DataManager::Core::Logger::ConsoleLogger().Error(SOURCE_LOCATION,
                                                                "无效的插件管理器指针");
        Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                                "返回");
        return false;
    }
    // TODO: 适配API
    Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                            "返回");
    return false;
}

int BackupData(void *pluginManager, const char *dataType, const char *from, const char *name,
               const char *version, const char *node) {
    Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                            "调用");
    if (!IsValidPluginManager(pluginManager)) {
        Fleet::DataManager::Core::Logger::ConsoleLogger().Error(SOURCE_LOCATION,
                                                                "无效的插件管理器指针");
        Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                                "返回");
        return false;
    }
    Fleet::DataManager::Portal::PortalService *portalService =
        (Fleet::DataManager::Portal::PortalService *) ((Fleet::DataManager::Core::PluginManager *)
                                                           pluginManager)
            ->GetService("Portal");
    if (portalService != nullptr) {
        Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                                "返回");
        return portalService->BackupData(dataType, name, from, version, node);
    } else {
        Fleet::DataManager::Core::Logger::ConsoleLogger().Error(SOURCE_LOCATION,
                                                                "未找到 存储门户 插件");
        Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                                "返回");
        return false;
    }
}

int RecoverData(void *pluginManager, const char *dataType, const char *from, const char *name,
                const char *version, const char *node) {
    Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                            "调用");
    if (!IsValidPluginManager(pluginManager)) {
        Fleet::DataManager::Core::Logger::ConsoleLogger().Error(SOURCE_LOCATION,
                                                                "无效的插件管理器指针");
        Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                                "返回");
        return false;
    }
    Fleet::DataManager::Portal::PortalService *portalService =
        (Fleet::DataManager::Portal::PortalService *) ((Fleet::DataManager::Core::PluginManager *)
                                                           pluginManager)
            ->GetService("Portal");
    if (portalService != nullptr) {
        Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                                "返回");
        return portalService->RecoverData(dataType, name, from, version, node);
    } else {
        Fleet::DataManager::Core::Logger::ConsoleLogger().Error(SOURCE_LOCATION,
                                                                "未找到 存储门户 插件");
        Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                                "返回");
        return false;
    }
}

int ObservationOrder(void *pluginManager, const char *uuid, const char *data, int length) {
    Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                            "调用");
    if (!IsValidPluginManager(pluginManager)) {
        Fleet::DataManager::Core::Logger::ConsoleLogger().Error(SOURCE_LOCATION,
                                                                "无效的插件管理器指针");
        Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                                "返回");
        return false;
    }
    Fleet::DataManager::Portal::PortalService *portalService =
        (Fleet::DataManager::Portal::PortalService *) ((Fleet::DataManager::Core::PluginManager *)
                                                           pluginManager)
            ->GetService("Portal");
    if (portalService != nullptr) {
        Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                                "返回");
        return portalService->ObservationOrder(uuid, data, length);
    } else {
        Fleet::DataManager::Core::Logger::ConsoleLogger().Error(SOURCE_LOCATION,
                                                                "未找到 存储门户 插件");
        Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                                "返回");
        return false;
    }
}

int ProcessingOrder(void *pluginManager, const char *uuid, const char *data, int length) {
    Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                            "调用");
    if (!IsValidPluginManager(pluginManager)) {
        Fleet::DataManager::Core::Logger::ConsoleLogger().Error(SOURCE_LOCATION,
                                                                "无效的插件管理器指针");
        Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                                "返回");
        return false;
    }
    Fleet::DataManager::Portal::PortalService *portalService =
        (Fleet::DataManager::Portal::PortalService *) ((Fleet::DataManager::Core::PluginManager *)
                                                           pluginManager)
            ->GetService("Portal");
    if (portalService != nullptr) {
        Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                                "返回");
        return portalService->ProcessingOrder(uuid, data, length);
    } else {
        Fleet::DataManager::Core::Logger::ConsoleLogger().Error(SOURCE_LOCATION,
                                                                "未找到 存储门户 插件");
        Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                                "返回");
        return false;
    }
}

int DispatchingOrder(void *pluginManager, const char *uuid, const char *data, int length) {
    Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                            "调用");
    if (!IsValidPluginManager(pluginManager)) {
        Fleet::DataManager::Core::Logger::ConsoleLogger().Error(SOURCE_LOCATION,
                                                                "无效的插件管理器指针");
        Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                                "返回");
        return false;
    }
    Fleet::DataManager::Portal::PortalService *portalService =
        (Fleet::DataManager::Portal::PortalService *) ((Fleet::DataManager::Core::PluginManager *)
                                                           pluginManager)
            ->GetService("Portal");
    if (portalService != nullptr) {
        Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                                "返回");
        return portalService->DispatchingOrder(uuid, data, length);
    } else {
        Fleet::DataManager::Core::Logger::ConsoleLogger().Error(SOURCE_LOCATION,
                                                                "未找到 存储门户 插件");
        Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                                "返回");
        return false;
    }
}

int OrderStatus(void *pluginManager, const char *uuid, char **data) {
    Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                            "调用");
    if (!IsValidPluginManager(pluginManager)) {
        Fleet::DataManager::Core::Logger::ConsoleLogger().Error(SOURCE_LOCATION,
                                                                "无效的插件管理器指针");
        Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                                "返回");
        return false;
    }
    Fleet::DataManager::Portal::PortalService *portalService =
        (Fleet::DataManager::Portal::PortalService *) ((Fleet::DataManager::Core::PluginManager *)
                                                           pluginManager)
            ->GetService("Portal");
    if (portalService != nullptr) {
        Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                                "返回");
        return portalService->OrderStatus(uuid, *data);
    } else {
        Fleet::DataManager::Core::Logger::ConsoleLogger().Error(SOURCE_LOCATION,
                                                                "未找到 存储门户 插件");
        Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                                "返回");
        return false;
    }
}

int OrderResult(void *pluginManager, const char *uuid, char **data) {
    Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                            "调用");
    if (!IsValidPluginManager(pluginManager)) {
        Fleet::DataManager::Core::Logger::ConsoleLogger().Error(SOURCE_LOCATION,
                                                                "无效的插件管理器指针");
        Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                                "返回");
        return false;
    }
    Fleet::DataManager::Portal::PortalService *portalService =
        (Fleet::DataManager::Portal::PortalService *) ((Fleet::DataManager::Core::PluginManager *)
                                                           pluginManager)
            ->GetService("Portal");
    if (portalService != nullptr) {
        Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                                "返回");
        return portalService->OrderResult(uuid, *data);
    } else {
        Fleet::DataManager::Core::Logger::ConsoleLogger().Error(SOURCE_LOCATION,
                                                                "未找到 存储门户 插件");
        Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                                "返回");
        return false;
    }
}

int SqliteExecuteOnMultipleNodes(void *pluginManager, int nodeIdCount, const char **nodeIdList,
                                 const char *connectionString, const char *sql) {
    Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                            "调用");
    if (!IsValidPluginManager(pluginManager)) {
        Fleet::DataManager::Core::Logger::ConsoleLogger().Error(SOURCE_LOCATION,
                                                                "无效的插件管理器指针");
        Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                                "返回");
        return false;
    }
    Fleet::DataManager::Portal::PortalService *portalService =
        (Fleet::DataManager::Portal::PortalService *) ((Fleet::DataManager::Core::PluginManager *)
                                                           pluginManager)
            ->GetService("Portal");
    if (portalService != nullptr) {
        Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                                "返回");
        std::vector<std::string> nodeIds;
        int i = 0;
        for (i = 0; i < nodeIdCount; i++) {
            nodeIds.emplace_back(nodeIdList[i]);
        }
        return portalService->SqliteExecute(nodeIds, connectionString, sql);
    } else {
        Fleet::DataManager::Core::Logger::ConsoleLogger().Error(SOURCE_LOCATION,
                                                                "未找到 存储门户 插件");
        Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                                "返回");
        return false;
    }
}

int SqliteExecuteParameterizedOnMultipleNodes(void *pluginManager, int nodeIdCount,
                                              const char **nodeIdList, const char *connectionString,
                                              const char *sql, int valueCount, const char **types,
                                              const char **values) {
    Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                            "调用");
    if (!IsValidPluginManager(pluginManager)) {
        Fleet::DataManager::Core::Logger::ConsoleLogger().Error(SOURCE_LOCATION,
                                                                "无效的插件管理器指针");
        Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                                "返回");
        return false;
    }
    Fleet::DataManager::Portal::PortalService *portalService =
        (Fleet::DataManager::Portal::PortalService *) ((Fleet::DataManager::Core::PluginManager *)
                                                           pluginManager)
            ->GetService("Portal");
    if (portalService != nullptr) {
        Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                                "返回");
        std::vector<std::string> nodeIds;
        int i = 0;
        for (i = 0; i < nodeIdCount; i++) {
            nodeIds.emplace_back(nodeIdList[i]);
        }
        std::vector<std::string> typeList;
        std::vector<std::string> valueList;
        for (i = 0; i < valueCount; i++) {
            typeList.emplace_back(types[i]);
            valueList.emplace_back(values[i]);
        }
        return portalService->SqliteExecuteParameterized(nodeIds, connectionString, sql, typeList,
                                                         valueList);
    } else {
        Fleet::DataManager::Core::Logger::ConsoleLogger().Error(SOURCE_LOCATION,
                                                                "未找到 存储门户 插件");
        Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                                "返回");
        return false;
    }
}

int PostgreSqlExecuteOnMultipleNodes(void *pluginManager, int nodeIdCount, const char **nodeIdList,
                                     const char *connectionString, const char *sql) {
    Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                            "调用");
    if (!IsValidPluginManager(pluginManager)) {
        Fleet::DataManager::Core::Logger::ConsoleLogger().Error(SOURCE_LOCATION,
                                                                "无效的插件管理器指针");
        Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                                "返回");
        return false;
    }
    Fleet::DataManager::Portal::PortalService *portalService =
        (Fleet::DataManager::Portal::PortalService *) ((Fleet::DataManager::Core::PluginManager *)
                                                           pluginManager)
            ->GetService("Portal");
    if (portalService != nullptr) {
        Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                                "返回");
        std::vector<std::string> nodeIds;
        int i = 0;
        for (i = 0; i < nodeIdCount; i++) {
            nodeIds.emplace_back(nodeIdList[i]);
        }
        return portalService->PostgreSqlExecute(nodeIds, connectionString, sql);
    } else {
        Fleet::DataManager::Core::Logger::ConsoleLogger().Error(SOURCE_LOCATION,
                                                                "未找到 存储门户 插件");
        Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                                "返回");
        return false;
    }
}

int PostgreSqlExecuteParameterizedOnMultipleNodes(void *pluginManager, int nodeIdCount,
                                                  const char **nodeIdList,
                                                  const char *connectionString, const char *sql,
                                                  int valueCount, const char **types,
                                                  const char **values) {
    Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                            "调用");
    if (!IsValidPluginManager(pluginManager)) {
        Fleet::DataManager::Core::Logger::ConsoleLogger().Error(SOURCE_LOCATION,
                                                                "无效的插件管理器指针");
        Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                                "返回");
        return false;
    }
    Fleet::DataManager::Portal::PortalService *portalService =
        (Fleet::DataManager::Portal::PortalService *) ((Fleet::DataManager::Core::PluginManager *)
                                                           pluginManager)
            ->GetService("Portal");
    if (portalService != nullptr) {
        Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                                "返回");
        std::vector<std::string> nodeIds;
        int i = 0;
        for (i = 0; i < nodeIdCount; i++) {
            nodeIds.emplace_back(nodeIdList[i]);
        }
        std::vector<std::string> typeList;
        std::vector<std::string> valueList;
        for (i = 0; i < valueCount; i++) {
            typeList.emplace_back(types[i]);
            valueList.emplace_back(values[i]);
        }
        return portalService->PostgreSqlExecuteParameterized(nodeIds, connectionString, sql,
                                                             typeList, valueList);
    } else {
        Fleet::DataManager::Core::Logger::ConsoleLogger().Error(SOURCE_LOCATION,
                                                                "未找到 存储门户 插件");
        Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                                "返回");
        return false;
    }
}

void FreeMem(char *data) {
    if (data != nullptr) {
        delete[] data;
    }
}

int GetConfig(void *pluginManager, const char *key, char **value) {
    Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION, "调用，键: {}",
                                                            key ? key : "nullptr");
    if (!IsValidPluginManager(pluginManager)) {
        Fleet::DataManager::Core::Logger::ConsoleLogger().Error(SOURCE_LOCATION,
                                                                "无效的插件管理器指针");
        Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION, "返回");
        return -1;
    }

    // 检查空指针参数
    if (key == nullptr) {
        Fleet::DataManager::Core::Logger::ConsoleLogger().Error(SOURCE_LOCATION, "键参数为空指针");
        return -1;
    }

    if (value == nullptr) {
        Fleet::DataManager::Core::Logger::ConsoleLogger().Error(SOURCE_LOCATION,
                                                                "值指针参数为空指针");
        return -1;
    }

    Fleet::DataManager::Messaging::MessagingService *messagingService =
        (Fleet::DataManager::Messaging::MessagingService
             *) ((Fleet::DataManager::Core::PluginManager *) pluginManager)
            ->GetService("Messaging");
    if (messagingService != nullptr) {
        int result = messagingService->GetConfig(key, value);
        Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION, "返回，结果: {}",
                                                                result);
        return result;
    } else {
        Fleet::DataManager::Core::Logger::ConsoleLogger().Error(SOURCE_LOCATION,
                                                                "未找到 消息协同 插件");
        Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION, "返回");
        return -1;
    }
}

int PutConfig(void *pluginManager, const char *key, const char *value, int length) {
    Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(
        SOURCE_LOCATION, "调用，键: {}，值长度: {}", key ? key : "nullptr", length);
    if (!IsValidPluginManager(pluginManager)) {
        Fleet::DataManager::Core::Logger::ConsoleLogger().Error(SOURCE_LOCATION,
                                                                "无效的插件管理器指针");
        Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION, "返回");
        return 0;
    }

    // 检查空指针参数
    if (key == nullptr) {
        Fleet::DataManager::Core::Logger::ConsoleLogger().Error(SOURCE_LOCATION, "键参数为空指针");
        return 0;
    }

    if (value == nullptr) {
        Fleet::DataManager::Core::Logger::ConsoleLogger().Error(SOURCE_LOCATION, "值参数为空指针");
        return 0;
    }

    Fleet::DataManager::Messaging::MessagingService *messagingService =
        (Fleet::DataManager::Messaging::MessagingService
             *) ((Fleet::DataManager::Core::PluginManager *) pluginManager)
            ->GetService("Messaging");
    if (messagingService != nullptr) {
        std::string valueStr(value, length);
        bool result = messagingService->PutConfig(key, valueStr, length);
        Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION, "返回，结果: {}",
                                                                result);
        return result ? 1 : 0;
    } else {
        Fleet::DataManager::Core::Logger::ConsoleLogger().Error(SOURCE_LOCATION,
                                                                "未找到 消息协同 插件");
        Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION, "返回");
        return 0;
    }
}

int RemoveConfig(void *pluginManager, const char *key) {
    Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION, "调用，键: {}",
                                                            key ? key : "nullptr");
    if (!IsValidPluginManager(pluginManager)) {
        Fleet::DataManager::Core::Logger::ConsoleLogger().Error(SOURCE_LOCATION,
                                                                "无效的插件管理器指针");
        Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION, "返回");
        return 0;
    }

    // 检查空指针参数
    if (key == nullptr) {
        Fleet::DataManager::Core::Logger::ConsoleLogger().Error(SOURCE_LOCATION, "键参数为空指针");
        return 0;
    }

    Fleet::DataManager::Messaging::MessagingService *messagingService =
        (Fleet::DataManager::Messaging::MessagingService
             *) ((Fleet::DataManager::Core::PluginManager *) pluginManager)
            ->GetService("Messaging");
    if (messagingService != nullptr) {
        bool result = messagingService->RemoveConfig(key);
        Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION, "返回，结果: {}",
                                                                result);
        return result ? 1 : 0;
    } else {
        Fleet::DataManager::Core::Logger::ConsoleLogger().Error(SOURCE_LOCATION,
                                                                "未找到 消息协同 插件");
        Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION, "返回");
        return 0;
    }
}

int SetClockOffset(void *pluginManager, long long offsetMs) {
    Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                            "调用，时钟偏差: {} 毫秒", offsetMs);
    if (!IsValidPluginManager(pluginManager)) {
        Fleet::DataManager::Core::Logger::ConsoleLogger().Error(SOURCE_LOCATION,
                                                                "无效的插件管理器指针");
        Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION, "返回");
        return 0;
    }

    Fleet::DataManager::Messaging::MessagingService *messagingService =
        (Fleet::DataManager::Messaging::MessagingService
             *) ((Fleet::DataManager::Core::PluginManager *) pluginManager)
            ->GetService("Messaging");
    if (messagingService != nullptr) {
        bool result = messagingService->SetClockOffset(static_cast<int64_t>(offsetMs));
        Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION, "返回，结果: {}",
                                                                result);
        return result ? 1 : 0;
    } else {
        Fleet::DataManager::Core::Logger::ConsoleLogger().Error(SOURCE_LOCATION,
                                                                "未找到 消息协同 插件");
        Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION, "返回");
        return 0;
    }
}

long long GetClockOffset(void *pluginManager) {
    Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION, "调用");
    if (!IsValidPluginManager(pluginManager)) {
        Fleet::DataManager::Core::Logger::ConsoleLogger().Error(SOURCE_LOCATION,
                                                                "无效的插件管理器指针");
        Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION, "返回");
        return 0;
    }

    Fleet::DataManager::Messaging::MessagingService *messagingService =
        (Fleet::DataManager::Messaging::MessagingService
             *) ((Fleet::DataManager::Core::PluginManager *) pluginManager)
            ->GetService("Messaging");
    if (messagingService != nullptr) {
        int64_t result = messagingService->GetClockOffset();
        Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                                "返回，时钟偏差: {} 毫秒", result);
        return static_cast<long long>(result);
    } else {
        Fleet::DataManager::Core::Logger::ConsoleLogger().Error(SOURCE_LOCATION,
                                                                "未找到 消息协同 插件");
        Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION, "返回");
        return 0;
    }
}

// 断点续传消息发布
int PublishResumableMessage(void *pluginManager, const char *topic, const char *data, int length) {
    Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                            "调用");
    if (!IsValidPluginManager(pluginManager)) {
        Fleet::DataManager::Core::Logger::ConsoleLogger().Error(SOURCE_LOCATION,
                                                                "无效的插件管理器指针");
        Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                                "返回");
        return false;
    }
    Fleet::DataManager::Portal::PortalService *portalService =
        (Fleet::DataManager::Portal::PortalService *) ((Fleet::DataManager::Core::PluginManager *)
                                                           pluginManager)
            ->GetService("Portal");
    if (portalService != nullptr) {
        Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                                "返回");
        return portalService->PublishResumableMessage(topic, data, length);
    } else {
        Fleet::DataManager::Core::Logger::ConsoleLogger().Error(SOURCE_LOCATION,
                                                                "未找到 存储门户 插件");
        Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                                "返回");
        return false;
    }
}

// 断点续传消息订阅
int SubscribeResumableMessage(void *pluginManager, const char *topic,
                              Fleet::DataManager::Messaging::MessageCallback callback) {
    Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                            "调用");
    if (!IsValidPluginManager(pluginManager)) {
        Fleet::DataManager::Core::Logger::ConsoleLogger().Error(SOURCE_LOCATION,
                                                                "无效的插件管理器指针");
        Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                                "返回");
        return false;
    }
    Fleet::DataManager::Portal::PortalService *portalService =
        (Fleet::DataManager::Portal::PortalService *) ((Fleet::DataManager::Core::PluginManager *)
                                                           pluginManager)
            ->GetService("Portal");
    if (portalService != nullptr) {
        Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                                "返回");
        return portalService->SubscribeResumableMessage(topic, callback);
    } else {
        Fleet::DataManager::Core::Logger::ConsoleLogger().Error(SOURCE_LOCATION,
                                                                "未找到 存储门户 插件");
        Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                                "返回");
        return false;
    }
}

// 可靠消息发布（失效重传）
int PublishReliableMessage(void *pluginManager, const char *topic, const char *data, int length) {
    Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                            "调用");
    if (!IsValidPluginManager(pluginManager)) {
        Fleet::DataManager::Core::Logger::ConsoleLogger().Error(SOURCE_LOCATION,
                                                                "无效的插件管理器指针");
        Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                                "返回");
        return false;
    }
    Fleet::DataManager::Portal::PortalService *portalService =
        (Fleet::DataManager::Portal::PortalService *) ((Fleet::DataManager::Core::PluginManager *)
                                                           pluginManager)
            ->GetService("Portal");
    if (portalService != nullptr) {
        Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                                "返回");
        return portalService->PublishReliableMessage(topic, data, length);
    } else {
        Fleet::DataManager::Core::Logger::ConsoleLogger().Error(SOURCE_LOCATION,
                                                                "未找到 存储门户 插件");
        Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                                "返回");
        return false;
    }
}

// 可靠消息订阅
int SubscribeReliableMessage(void *pluginManager, const char *topic,
                             Fleet::DataManager::Messaging::MessageCallback callback) {
    Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                            "调用");
    if (!IsValidPluginManager(pluginManager)) {
        Fleet::DataManager::Core::Logger::ConsoleLogger().Error(SOURCE_LOCATION,
                                                                "无效的插件管理器指针");
        Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                                "返回");
        return false;
    }
    Fleet::DataManager::Portal::PortalService *portalService =
        (Fleet::DataManager::Portal::PortalService *) ((Fleet::DataManager::Core::PluginManager *)
                                                           pluginManager)
            ->GetService("Portal");
    if (portalService != nullptr) {
        Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                                "返回");
        return portalService->SubscribeReliableMessage(topic, callback);
    } else {
        Fleet::DataManager::Core::Logger::ConsoleLogger().Error(SOURCE_LOCATION,
                                                                "未找到 存储门户 插件");
        Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                                "返回");
        return false;
    }
}

// 发送控制命令
int SendControlCommand(void *pluginManager, const char *targetNode, const char *command,
                       int length) {
    Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                            "调用");
    if (!IsValidPluginManager(pluginManager)) {
        Fleet::DataManager::Core::Logger::ConsoleLogger().Error(SOURCE_LOCATION,
                                                                "无效的插件管理器指针");
        Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                                "返回");
        return false;
    }
    Fleet::DataManager::Portal::PortalService *portalService =
        (Fleet::DataManager::Portal::PortalService *) ((Fleet::DataManager::Core::PluginManager *)
                                                           pluginManager)
            ->GetService("Portal");
    if (portalService != nullptr) {
        Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                                "返回");
        return portalService->SendControlCommand(targetNode, command, length);
    } else {
        Fleet::DataManager::Core::Logger::ConsoleLogger().Error(SOURCE_LOCATION,
                                                                "未找到 存储门户 插件");
        Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                                "返回");
        return false;
    }
}

// 订阅控制命令
int SubscribeControlCommand(void *pluginManager,
                            Fleet::DataManager::Messaging::MessageCallback callback) {
    Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                            "调用");
    if (!IsValidPluginManager(pluginManager)) {
        Fleet::DataManager::Core::Logger::ConsoleLogger().Error(SOURCE_LOCATION,
                                                                "无效的插件管理器指针");
        Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                                "返回");
        return false;
    }
    Fleet::DataManager::Portal::PortalService *portalService =
        (Fleet::DataManager::Portal::PortalService *) ((Fleet::DataManager::Core::PluginManager *)
                                                           pluginManager)
            ->GetService("Portal");
    if (portalService != nullptr) {
        Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                                "返回");
        return portalService->SubscribeControlCommand(callback);
    } else {
        Fleet::DataManager::Core::Logger::ConsoleLogger().Error(SOURCE_LOCATION,
                                                                "未找到 存储门户 插件");
        Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                                "返回");
        return false;
    }
}

// 发送状态数据
int SendStatusData(void *pluginManager, const char *targetNode, const char *statusData,
                   int length) {
    Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                            "调用");
    if (!IsValidPluginManager(pluginManager)) {
        Fleet::DataManager::Core::Logger::ConsoleLogger().Error(SOURCE_LOCATION,
                                                                "无效的插件管理器指针");
        Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                                "返回");
        return false;
    }
    Fleet::DataManager::Portal::PortalService *portalService =
        (Fleet::DataManager::Portal::PortalService *) ((Fleet::DataManager::Core::PluginManager *)
                                                           pluginManager)
            ->GetService("Portal");
    if (portalService != nullptr) {
        Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                                "返回");
        return portalService->SendStatusData(targetNode, statusData, length);
    } else {
        Fleet::DataManager::Core::Logger::ConsoleLogger().Error(SOURCE_LOCATION,
                                                                "未找到 存储门户 插件");
        Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                                "返回");
        return false;
    }
}

// 订阅状态数据
int SubscribeStatusData(void *pluginManager,
                        Fleet::DataManager::Messaging::MessageCallback callback) {
    Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                            "调用");
    if (!IsValidPluginManager(pluginManager)) {
        Fleet::DataManager::Core::Logger::ConsoleLogger().Error(SOURCE_LOCATION,
                                                                "无效的插件管理器指针");
        Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                                "返回");
        return false;
    }
    Fleet::DataManager::Portal::PortalService *portalService =
        (Fleet::DataManager::Portal::PortalService *) ((Fleet::DataManager::Core::PluginManager *)
                                                           pluginManager)
            ->GetService("Portal");
    if (portalService != nullptr) {
        Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                                "返回");
        return portalService->SubscribeStatusData(callback);
    } else {
        Fleet::DataManager::Core::Logger::ConsoleLogger().Error(SOURCE_LOCATION,
                                                                "未找到 存储门户 插件");
        Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                                "返回");
        return false;
    }
}

// 发送任务规划信息
int SendTaskPlanningInfo(void *pluginManager, const char *targetNode, const char *planningData,
                         int length) {
    Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                            "调用");
    if (!IsValidPluginManager(pluginManager)) {
        Fleet::DataManager::Core::Logger::ConsoleLogger().Error(SOURCE_LOCATION,
                                                                "无效的插件管理器指针");
        Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                                "返回");
        return false;
    }
    Fleet::DataManager::Portal::PortalService *portalService =
        (Fleet::DataManager::Portal::PortalService *) ((Fleet::DataManager::Core::PluginManager *)
                                                           pluginManager)
            ->GetService("Portal");
    if (portalService != nullptr) {
        Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                                "返回");
        return portalService->SendTaskPlanningInfo(targetNode, planningData, length);
    } else {
        Fleet::DataManager::Core::Logger::ConsoleLogger().Error(SOURCE_LOCATION,
                                                                "未找到 存储门户 插件");
        Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                                "返回");
        return false;
    }
}

// 订阅任务规划信息
int SubscribeTaskPlanningInfo(void *pluginManager,
                              Fleet::DataManager::Messaging::MessageCallback callback) {
    Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                            "调用");
    if (!IsValidPluginManager(pluginManager)) {
        Fleet::DataManager::Core::Logger::ConsoleLogger().Error(SOURCE_LOCATION,
                                                                "无效的插件管理器指针");
        Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                                "返回");
        return false;
    }
    Fleet::DataManager::Portal::PortalService *portalService =
        (Fleet::DataManager::Portal::PortalService *) ((Fleet::DataManager::Core::PluginManager *)
                                                           pluginManager)
            ->GetService("Portal");
    if (portalService != nullptr) {
        Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                                "返回");
        return portalService->SubscribeTaskPlanningInfo(callback);
    } else {
        Fleet::DataManager::Core::Logger::ConsoleLogger().Error(SOURCE_LOCATION,
                                                                "未找到 存储门户 插件");
        Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                                "返回");
        return false;
    }
}

// 发送调度信息
int SendSchedulingInfo(void *pluginManager, const char *targetNode, const char *schedulingData,
                       int length) {
    Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                            "调用");
    if (!IsValidPluginManager(pluginManager)) {
        Fleet::DataManager::Core::Logger::ConsoleLogger().Error(SOURCE_LOCATION,
                                                                "无效的插件管理器指针");
        Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                                "返回");
        return false;
    }
    Fleet::DataManager::Portal::PortalService *portalService =
        (Fleet::DataManager::Portal::PortalService *) ((Fleet::DataManager::Core::PluginManager *)
                                                           pluginManager)
            ->GetService("Portal");
    if (portalService != nullptr) {
        Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                                "返回");
        return portalService->SendSchedulingInfo(targetNode, schedulingData, length);
    } else {
        Fleet::DataManager::Core::Logger::ConsoleLogger().Error(SOURCE_LOCATION,
                                                                "未找到 存储门户 插件");
        Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                                "返回");
        return false;
    }
}

// 订阅调度信息
int SubscribeSchedulingInfo(void *pluginManager,
                            Fleet::DataManager::Messaging::MessageCallback callback) {
    Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                            "调用");
    if (!IsValidPluginManager(pluginManager)) {
        Fleet::DataManager::Core::Logger::ConsoleLogger().Error(SOURCE_LOCATION,
                                                                "无效的插件管理器指针");
        Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                                "返回");
        return false;
    }
    Fleet::DataManager::Portal::PortalService *portalService =
        (Fleet::DataManager::Portal::PortalService *) ((Fleet::DataManager::Core::PluginManager *)
                                                           pluginManager)
            ->GetService("Portal");
    if (portalService != nullptr) {
        Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                                "返回");
        return portalService->SubscribeSchedulingInfo(callback);
    } else {
        Fleet::DataManager::Core::Logger::ConsoleLogger().Error(SOURCE_LOCATION,
                                                                "未找到 存储门户 插件");
        Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                                "返回");
        return false;
    }
}

// 发送资源状态信息
int SendResourceStatus(void *pluginManager, const char *targetNode, const char *resourceData,
                       int length) {
    Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                            "调用");
    if (!IsValidPluginManager(pluginManager)) {
        Fleet::DataManager::Core::Logger::ConsoleLogger().Error(SOURCE_LOCATION,
                                                                "无效的插件管理器指针");
        Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                                "返回");
        return false;
    }
    Fleet::DataManager::Portal::PortalService *portalService =
        (Fleet::DataManager::Portal::PortalService *) ((Fleet::DataManager::Core::PluginManager *)
                                                           pluginManager)
            ->GetService("Portal");
    if (portalService != nullptr) {
        Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                                "返回");
        return portalService->SendResourceStatus(targetNode, resourceData, length);
    } else {
        Fleet::DataManager::Core::Logger::ConsoleLogger().Error(SOURCE_LOCATION,
                                                                "未找到 存储门户 插件");
        Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                                "返回");
        return false;
    }
}

// 订阅资源状态信息
int SubscribeResourceStatus(void *pluginManager,
                            Fleet::DataManager::Messaging::MessageCallback callback) {
    Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                            "调用");
    if (!IsValidPluginManager(pluginManager)) {
        Fleet::DataManager::Core::Logger::ConsoleLogger().Error(SOURCE_LOCATION,
                                                                "无效的插件管理器指针");
        Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                                "返回");
        return false;
    }
    Fleet::DataManager::Portal::PortalService *portalService =
        (Fleet::DataManager::Portal::PortalService *) ((Fleet::DataManager::Core::PluginManager *)
                                                           pluginManager)
            ->GetService("Portal");
    if (portalService != nullptr) {
        Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                                "返回");
        return portalService->SubscribeResourceStatus(callback);
    } else {
        Fleet::DataManager::Core::Logger::ConsoleLogger().Error(SOURCE_LOCATION,
                                                                "未找到 存储门户 插件");
        Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                                "返回");
        return false;
    }
}

int PublishCommand(void *pluginManager, const char *topic, const char *command, int length) {
    Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                            "调用");
    if (!IsValidPluginManager(pluginManager)) {
        Fleet::DataManager::Core::Logger::ConsoleLogger().Error(SOURCE_LOCATION,
                                                                "无效的插件管理器指针");
        Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                                "返回");
        return false;
    }
    Fleet::DataManager::Portal::PortalService *portalService =
        (Fleet::DataManager::Portal::PortalService *) ((Fleet::DataManager::Core::PluginManager *)
                                                           pluginManager)
            ->GetService("Portal");
    if (portalService != nullptr) {
        Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                                "返回");
        return portalService->PublishCommand(topic, command, length) ? 1 : 0;
    } else {
        Fleet::DataManager::Core::Logger::ConsoleLogger().Error(SOURCE_LOCATION,
                                                                "未找到 存储门户 插件");
        Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                                "返回");
        return false;
    }
}

int SubscribeCommand(void *pluginManager, const char *topic, MessageCallback callback) {
    Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                            "调用");
    if (!IsValidPluginManager(pluginManager)) {
        Fleet::DataManager::Core::Logger::ConsoleLogger().Error(SOURCE_LOCATION,
                                                                "无效的插件管理器指针");
        Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                                "返回");
        return false;
    }
    Fleet::DataManager::Portal::PortalService *portalService =
        (Fleet::DataManager::Portal::PortalService *) ((Fleet::DataManager::Core::PluginManager *)
                                                           pluginManager)
            ->GetService("Portal");
    if (portalService != nullptr) {
        Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                                "返回");
        return portalService->SubscribeCommand(topic, callback) ? 1 : 0;
    } else {
        Fleet::DataManager::Core::Logger::ConsoleLogger().Error(SOURCE_LOCATION,
                                                                "未找到 存储门户 插件");
        Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                                "返回");
        return false;
    }
}

int RequestCommand(void *pluginManager, const char *topic, const char *request, int length,
                   MessageCallback responseCallback, void *closure) {
    Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                            "调用");
    if (!IsValidPluginManager(pluginManager)) {
        Fleet::DataManager::Core::Logger::ConsoleLogger().Error(SOURCE_LOCATION,
                                                                "无效的插件管理器指针");
        Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                                "返回");
        return false;
    }
    Fleet::DataManager::Portal::PortalService *portalService =
        (Fleet::DataManager::Portal::PortalService *) ((Fleet::DataManager::Core::PluginManager *)
                                                           pluginManager)
            ->GetService("Portal");
    if (portalService != nullptr) {
        Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                                "返回");
        return portalService->RequestCommand(topic, request, length, responseCallback, closure) ? 1
                                                                                                : 0;
    } else {
        Fleet::DataManager::Core::Logger::ConsoleLogger().Error(SOURCE_LOCATION,
                                                                "未找到 存储门户 插件");
        Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                                "返回");
        return false;
    }
}

int ReplyCommand(void *pluginManager, const char *uuid, const char *reply, int length) {
    Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                            "调用");
    if (!IsValidPluginManager(pluginManager)) {
        Fleet::DataManager::Core::Logger::ConsoleLogger().Error(SOURCE_LOCATION,
                                                                "无效的插件管理器指针");
        Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                                "返回");
        return false;
    }
    Fleet::DataManager::Portal::PortalService *portalService =
        (Fleet::DataManager::Portal::PortalService *) ((Fleet::DataManager::Core::PluginManager *)
                                                           pluginManager)
            ->GetService("Portal");
    if (portalService != nullptr) {
        Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                                "返回");
        return portalService->ReplyCommand(uuid, reply, length) ? 1 : 0;
    } else {
        Fleet::DataManager::Core::Logger::ConsoleLogger().Error(SOURCE_LOCATION,
                                                                "未找到 存储门户 插件");
        Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                                "返回");
        return false;
    }
}

int PublishStatusInfo(void *pluginManager, const char *topic, const char *statusInfo, int length) {
    Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                            "调用");
    if (!IsValidPluginManager(pluginManager)) {
        Fleet::DataManager::Core::Logger::ConsoleLogger().Error(SOURCE_LOCATION,
                                                                "无效的插件管理器指针");
        Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                                "返回");
        return false;
    }
    Fleet::DataManager::Portal::PortalService *portalService =
        (Fleet::DataManager::Portal::PortalService *) ((Fleet::DataManager::Core::PluginManager *)
                                                           pluginManager)
            ->GetService("Portal");
    if (portalService != nullptr) {
        Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                                "返回");
        return portalService->PublishStatusInfo(topic, statusInfo, length) ? 1 : 0;
    } else {
        Fleet::DataManager::Core::Logger::ConsoleLogger().Error(SOURCE_LOCATION,
                                                                "未找到 存储门户 插件");
        Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                                "返回");
        return false;
    }
}

int SubscribeStatusInfo(void *pluginManager, const char *topic, MessageCallback callback) {
    Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                            "调用");
    if (!IsValidPluginManager(pluginManager)) {
        Fleet::DataManager::Core::Logger::ConsoleLogger().Error(SOURCE_LOCATION,
                                                                "无效的插件管理器指针");
        Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                                "返回");
        return false;
    }
    Fleet::DataManager::Portal::PortalService *portalService =
        (Fleet::DataManager::Portal::PortalService *) ((Fleet::DataManager::Core::PluginManager *)
                                                           pluginManager)
            ->GetService("Portal");
    if (portalService != nullptr) {
        Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                                "返回");
        return portalService->SubscribeStatusInfo(topic, callback) ? 1 : 0;
    } else {
        Fleet::DataManager::Core::Logger::ConsoleLogger().Error(SOURCE_LOCATION,
                                                                "未找到 存储门户 插件");
        Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                                "返回");
        return false;
    }
}

int RequestStatusInfo(void *pluginManager, const char *topic, const char *request, int length,
                      MessageCallback responseCallback, void *closure) {
    Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                            "调用");
    if (!IsValidPluginManager(pluginManager)) {
        Fleet::DataManager::Core::Logger::ConsoleLogger().Error(SOURCE_LOCATION,
                                                                "无效的插件管理器指针");
        Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                                "返回");
        return false;
    }
    Fleet::DataManager::Portal::PortalService *portalService =
        (Fleet::DataManager::Portal::PortalService *) ((Fleet::DataManager::Core::PluginManager *)
                                                           pluginManager)
            ->GetService("Portal");
    if (portalService != nullptr) {
        Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                                "返回");
        return portalService->RequestStatusInfo(topic, request, length, responseCallback, closure)
                   ? 1
                   : 0;
    } else {
        Fleet::DataManager::Core::Logger::ConsoleLogger().Error(SOURCE_LOCATION,
                                                                "未找到 存储门户 插件");
        Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                                "返回");
        return false;
    }
}

int ReplyStatusInfo(void *pluginManager, const char *uuid, const char *reply, int length) {
    Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                            "调用");
    if (!IsValidPluginManager(pluginManager)) {
        Fleet::DataManager::Core::Logger::ConsoleLogger().Error(SOURCE_LOCATION,
                                                                "无效的插件管理器指针");
        Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                                "返回");
        return false;
    }
    Fleet::DataManager::Portal::PortalService *portalService =
        (Fleet::DataManager::Portal::PortalService *) ((Fleet::DataManager::Core::PluginManager *)
                                                           pluginManager)
            ->GetService("Portal");
    if (portalService != nullptr) {
        Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                                "返回");
        return portalService->ReplyStatusInfo(uuid, reply, length) ? 1 : 0;
    } else {
        Fleet::DataManager::Core::Logger::ConsoleLogger().Error(SOURCE_LOCATION,
                                                                "未找到 存储门户 插件");
        Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                                "返回");
        return false;
    }
}

int PublishSmallFile(void *pluginManager, const char *topic, const char *fileData, int length) {
    Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                            "调用");
    if (!IsValidPluginManager(pluginManager)) {
        Fleet::DataManager::Core::Logger::ConsoleLogger().Error(SOURCE_LOCATION,
                                                                "无效的插件管理器指针");
        Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                                "返回");
        return false;
    }
    Fleet::DataManager::Portal::PortalService *portalService =
        (Fleet::DataManager::Portal::PortalService *) ((Fleet::DataManager::Core::PluginManager *)
                                                           pluginManager)
            ->GetService("Portal");
    if (portalService != nullptr) {
        Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                                "返回");
        return portalService->PublishSmallFile(topic, fileData, length) ? 1 : 0;
    } else {
        Fleet::DataManager::Core::Logger::ConsoleLogger().Error(SOURCE_LOCATION,
                                                                "未找到 存储门户 插件");
        Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                                "返回");
        return false;
    }
}

int SubscribeSmallFile(void *pluginManager, const char *topic, MessageCallback callback) {
    Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                            "调用");
    if (!IsValidPluginManager(pluginManager)) {
        Fleet::DataManager::Core::Logger::ConsoleLogger().Error(SOURCE_LOCATION,
                                                                "无效的插件管理器指针");
        Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                                "返回");
        return false;
    }
    Fleet::DataManager::Portal::PortalService *portalService =
        (Fleet::DataManager::Portal::PortalService *) ((Fleet::DataManager::Core::PluginManager *)
                                                           pluginManager)
            ->GetService("Portal");
    if (portalService != nullptr) {
        Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                                "返回");
        return portalService->SubscribeSmallFile(topic, callback) ? 1 : 0;
    } else {
        Fleet::DataManager::Core::Logger::ConsoleLogger().Error(SOURCE_LOCATION,
                                                                "未找到 存储门户 插件");
        Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                                "返回");
        return false;
    }
}

int RequestSmallFile(void *pluginManager, const char *topic, const char *request, int length,
                     MessageCallback responseCallback, void *closure) {
    Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                            "调用");
    if (!IsValidPluginManager(pluginManager)) {
        Fleet::DataManager::Core::Logger::ConsoleLogger().Error(SOURCE_LOCATION,
                                                                "无效的插件管理器指针");
        Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                                "返回");
        return false;
    }
    Fleet::DataManager::Portal::PortalService *portalService =
        (Fleet::DataManager::Portal::PortalService *) ((Fleet::DataManager::Core::PluginManager *)
                                                           pluginManager)
            ->GetService("Portal");
    if (portalService != nullptr) {
        Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                                "返回");
        return portalService->RequestSmallFile(topic, request, length, responseCallback, closure)
                   ? 1
                   : 0;
    } else {
        Fleet::DataManager::Core::Logger::ConsoleLogger().Error(SOURCE_LOCATION,
                                                                "未找到 存储门户 插件");
        Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                                "返回");
        return false;
    }
}

int ReplySmallFile(void *pluginManager, const char *uuid, const char *reply, int length) {
    Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                            "调用");
    if (!IsValidPluginManager(pluginManager)) {
        Fleet::DataManager::Core::Logger::ConsoleLogger().Error(SOURCE_LOCATION,
                                                                "无效的插件管理器指针");
        Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                                "返回");
        return false;
    }
    Fleet::DataManager::Portal::PortalService *portalService =
        (Fleet::DataManager::Portal::PortalService *) ((Fleet::DataManager::Core::PluginManager *)
                                                           pluginManager)
            ->GetService("Portal");
    if (portalService != nullptr) {
        Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                                "返回");
        return portalService->ReplySmallFile(uuid, reply, length) ? 1 : 0;
    } else {
        Fleet::DataManager::Core::Logger::ConsoleLogger().Error(SOURCE_LOCATION,
                                                                "未找到 存储门户 插件");
        Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                                "返回");
        return false;
    }
}

int ProductDataStore(void *pluginManager, const char *name, const char *dataOwner, const char *data,
                     int length) {
    Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                            "调用");
    if (!IsValidPluginManager(pluginManager)) {
        Fleet::DataManager::Core::Logger::ConsoleLogger().Error(SOURCE_LOCATION,
                                                                "无效的插件管理器指针");
        Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                                "返回");
        return false;
    }
    Fleet::DataManager::Portal::PortalService *portalService =
        (Fleet::DataManager::Portal::PortalService *) ((Fleet::DataManager::Core::PluginManager *)
                                                           pluginManager)
            ->GetService("Portal");
    if (portalService != nullptr) {
        Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                                "返回");
        return portalService->ProductDataStore(name, dataOwner, data, length);
    } else {
        Fleet::DataManager::Core::Logger::ConsoleLogger().Error(SOURCE_LOCATION,
                                                                "未找到 存储门户 插件");
        Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                                "返回");
        return false;
    }
}

int ProductAnalysis(void *pluginManager, const char *data, int length, const char *dataOwner) {
    Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                            "调用");
    if (!IsValidPluginManager(pluginManager)) {
        Fleet::DataManager::Core::Logger::ConsoleLogger().Error(SOURCE_LOCATION,
                                                                "无效的插件管理器指针");
        Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                                "返回");
        return false;
    }
    Fleet::DataManager::Portal::PortalService *portalService =
        (Fleet::DataManager::Portal::PortalService *) ((Fleet::DataManager::Core::PluginManager *)
                                                           pluginManager)
            ->GetService("Portal");
    if (portalService != nullptr) {
        Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                                "返回");
        return portalService->ProductAnalysis(data, length, dataOwner);
    } else {
        Fleet::DataManager::Core::Logger::ConsoleLogger().Error(SOURCE_LOCATION,
                                                                "未找到 存储门户 插件");
        Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                                "返回");
        return false;
    }
}

int QueryProductFilename(void *pluginManager, const char *ProductName, const char *dataOwner,
                         char **fileName) {
    Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                            "调用");
    if (!IsValidPluginManager(pluginManager)) {
        Fleet::DataManager::Core::Logger::ConsoleLogger().Error(SOURCE_LOCATION,
                                                                "无效的插件管理器指针");
        Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                                "返回");
        return false;
    }
    Fleet::DataManager::Portal::PortalService *portalService =
        (Fleet::DataManager::Portal::PortalService *) ((Fleet::DataManager::Core::PluginManager *)
                                                           pluginManager)
            ->GetService("Portal");
    if (portalService != nullptr) {
        Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                                "返回");
        return portalService->QueryProductFilename(ProductName, dataOwner, *fileName);
    } else {
        Fleet::DataManager::Core::Logger::ConsoleLogger().Error(SOURCE_LOCATION,
                                                                "未找到 存储门户 插件");
        Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                                "返回");
        return false;
    }
}

int UploadSegmentObject(void *pluginManager, const char *key, const char *owner,
                        int currentSegmentId, int totalSegmentCount, int totalSize, int segmentSize,
                        const char *segmentData) {
    Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                            "调用");
    if (!IsValidPluginManager(pluginManager)) {
        Fleet::DataManager::Core::Logger::ConsoleLogger().Error(SOURCE_LOCATION,
                                                                "无效的插件管理器指针");
        Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                                "返回");
        return false;
    }
    Fleet::DataManager::Portal::PortalService *portalService =
        (Fleet::DataManager::Portal::PortalService *) ((Fleet::DataManager::Core::PluginManager *)
                                                           pluginManager)
            ->GetService("Portal");
    if (portalService != nullptr) {
        Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                                "返回");
        return portalService->UploadSegmentObject(key, owner, currentSegmentId, totalSegmentCount,
                                                  totalSize, segmentSize, segmentData);
    } else {
        Fleet::DataManager::Core::Logger::ConsoleLogger().Error(SOURCE_LOCATION,
                                                                "未找到 存储门户 插件");
        Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                                "返回");
        return false;
    }
}

int UploadObject(void *pluginManager, const char *name, const char *dataOwner, const char *data,
                 int dataLength, const char *metadata, int metadataLength) {
    Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                            "调用");
    if (!IsValidPluginManager(pluginManager)) {
        Fleet::DataManager::Core::Logger::ConsoleLogger().Error(SOURCE_LOCATION,
                                                                "无效的插件管理器指针");
        Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                                "返回");
        return false;
    }
    Fleet::DataManager::Portal::PortalService *portalService =
        (Fleet::DataManager::Portal::PortalService *) ((Fleet::DataManager::Core::PluginManager *)
                                                           pluginManager)
            ->GetService("Portal");
    if (portalService != nullptr) {
        Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                                "返回");
        return portalService->UploadObject(name, dataOwner, data, dataLength, metadata,
                                           metadataLength);
    } else {
        Fleet::DataManager::Core::Logger::ConsoleLogger().Error(SOURCE_LOCATION,
                                                                "未找到 存储门户 插件");
        Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                                "返回");
        return false;
    }
}

int QueryObjectUseMeta(void *pluginManager, const char *dataOwner, const char *timeStamp,
                       const char *taskId, const char *status, char **data) {
    Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                            "调用");
    if (!IsValidPluginManager(pluginManager)) {
        Fleet::DataManager::Core::Logger::ConsoleLogger().Error(SOURCE_LOCATION,
                                                                "无效的插件管理器指针");
        Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                                "返回");
        return false;
    }
    Fleet::DataManager::Portal::PortalService *portalService =
        (Fleet::DataManager::Portal::PortalService *) ((Fleet::DataManager::Core::PluginManager *)
                                                           pluginManager)
            ->GetService("Portal");
    if (portalService != nullptr) {
        Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                                "返回");
        return portalService->QueryObjectUseMeta(dataOwner, timeStamp, taskId, status, *data);
    } else {
        Fleet::DataManager::Core::Logger::ConsoleLogger().Error(SOURCE_LOCATION,
                                                                "未找到 存储门户 插件");
        Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                                "返回");
        return false;
    }
}

int GetObjectMetadata(void *pluginManager, const char *name, const char *dataOwner,
                      const char *metaName, char **metaValue) {
    Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                            "调用");
    if (!IsValidPluginManager(pluginManager)) {
        Fleet::DataManager::Core::Logger::ConsoleLogger().Error(SOURCE_LOCATION,
                                                                "无效的插件管理器指针");
        Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                                "返回");
        return false;
    }
    Fleet::DataManager::Portal::PortalService *portalService =
        (Fleet::DataManager::Portal::PortalService *) ((Fleet::DataManager::Core::PluginManager *)
                                                           pluginManager)
            ->GetService("Portal");
    if (portalService != nullptr) {
        Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                                "返回");
        return portalService->GetObjectMetadata(name, dataOwner, metaName, *metaValue);
    } else {
        Fleet::DataManager::Core::Logger::ConsoleLogger().Error(SOURCE_LOCATION,
                                                                "未找到 存储门户 插件");
        Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                                "返回");
        return false;
    }
}

int SetObjectMetadata(void *pluginManager, const char *name, const char *dataOwner,
                      const char *metaName, const char *metaValue) {
    Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                            "调用");
    if (!IsValidPluginManager(pluginManager)) {
        Fleet::DataManager::Core::Logger::ConsoleLogger().Error(SOURCE_LOCATION,
                                                                "无效的插件管理器指针");
        Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                                "返回");
        return false;
    }
    Fleet::DataManager::Portal::PortalService *portalService =
        (Fleet::DataManager::Portal::PortalService *) ((Fleet::DataManager::Core::PluginManager *)
                                                           pluginManager)
            ->GetService("Portal");
    if (portalService != nullptr) {
        Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                                "返回");
        return portalService->SetObjectMetadata(name, dataOwner, metaName, metaValue);
    } else {
        Fleet::DataManager::Core::Logger::ConsoleLogger().Error(SOURCE_LOCATION,
                                                                "未找到 存储门户 插件");
        Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                                "返回");
        return false;
    }
}

int DeleteObjectMetadata(void *pluginManager, const char *name, const char *dataOwner,
                         const char *metaName) {
    Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                            "调用");
    if (!IsValidPluginManager(pluginManager)) {
        Fleet::DataManager::Core::Logger::ConsoleLogger().Error(SOURCE_LOCATION,
                                                                "无效的插件管理器指针");
        Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                                "返回");
        return false;
    }
    Fleet::DataManager::Portal::PortalService *portalService =
        (Fleet::DataManager::Portal::PortalService *) ((Fleet::DataManager::Core::PluginManager *)
                                                           pluginManager)
            ->GetService("Portal");
    if (portalService != nullptr) {
        Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                                "返回");
        return portalService->DeleteObjectMetadata(name, dataOwner, metaName);
    } else {
        Fleet::DataManager::Core::Logger::ConsoleLogger().Error(SOURCE_LOCATION,
                                                                "未找到 存储门户 插件");
        Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                                "返回");
        return false;
    }
}

int UpdateObjectMetadata(void *pluginManager, const char *name, const char *dataOwner,
                         const char *metaName, const char *metaValue) {
    Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                            "调用");
    if (!IsValidPluginManager(pluginManager)) {
        Fleet::DataManager::Core::Logger::ConsoleLogger().Error(SOURCE_LOCATION,
                                                                "无效的插件管理器指针");
        Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                                "返回");
        return false;
    }
    Fleet::DataManager::Portal::PortalService *portalService =
        (Fleet::DataManager::Portal::PortalService *) ((Fleet::DataManager::Core::PluginManager *)
                                                           pluginManager)
            ->GetService("Portal");
    if (portalService != nullptr) {
        Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                                "返回");
        return portalService->UpdateObjectMetadata(name, dataOwner, metaName, metaValue);
    } else {
        Fleet::DataManager::Core::Logger::ConsoleLogger().Error(SOURCE_LOCATION,
                                                                "未找到 存储门户 插件");
        Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                                "返回");
        return false;
    }
}

int DownloadObject(void *pluginManager, const char *name, const char *dataOwner, char **data) {
    Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                            "调用");
    if (!IsValidPluginManager(pluginManager)) {
        Fleet::DataManager::Core::Logger::ConsoleLogger().Error(SOURCE_LOCATION,
                                                                "无效的插件管理器指针");
        Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                                "返回");
        return false;
    }
    Fleet::DataManager::Portal::PortalService *portalService =
        (Fleet::DataManager::Portal::PortalService *) ((Fleet::DataManager::Core::PluginManager *)
                                                           pluginManager)
            ->GetService("Portal");
    if (portalService != nullptr) {
        Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                                "返回");
        return portalService->DownloadObject(name, dataOwner, *data);
    } else {
        Fleet::DataManager::Core::Logger::ConsoleLogger().Error(SOURCE_LOCATION,
                                                                "未找到 存储门户 插件");
        Fleet::DataManager::Core::Logger::ConsoleLogger().Trace(SOURCE_LOCATION,
                                                                "返回");
        return false;
    }
}