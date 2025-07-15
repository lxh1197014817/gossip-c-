/**
 * @file DynamicPluginBase.h
 * @brief 动态插件契约接口定义
 * @details 定义动态加载插件必须实现的C接口函数和导出变量，用于插件的创建、销毁和元数据管理
 * @author 唐震 <tangzhen12@otcaix.iscas.ac.cn>
 * @date 2025-07-02
 */

#ifndef FLEET_DATA_MANAGER_CORE_DYNAMIC_PLUGIN_BASE_H
#define FLEET_DATA_MANAGER_CORE_DYNAMIC_PLUGIN_BASE_H

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief 创建插件实例
 * @details 动态插件必须实现此函数，用于创建插件对象实例
 * @return 插件对象指针，失败返回nullptr
 * @note 函数必须具有默认可见性，供动态加载器调用
 */
__attribute__((visibility("default"), used)) extern void *NewPlugin();

/**
 * @brief 销毁插件实例
 * @details 动态插件必须实现此函数，用于销毁插件对象实例
 * @param[in] plugin 插件对象指针
 * @note 函数必须具有默认可见性，供动态加载器调用
 */
__attribute__((visibility("default"), used)) extern void DeletePlugin(void *plugin);

/// 插件名称，动态插件必须导出此变量
extern const char *PluginName;
/// 插件版本，动态插件必须导出此变量
extern const char *PluginVersion;
/// 插件描述，动态插件必须导出此变量
extern const char *PluginDescription;

/// 依赖插件数量，动态插件必须导出此变量
extern int DependencyCount;
/// 依赖插件名称列表，动态插件必须导出此变量
extern const char *DependencyList[];
/// 冲突插件数量，动态插件必须导出此变量
extern int ConflictCount;
/// 冲突插件名称列表，动态插件必须导出此变量
extern const char *ConflictList[];

#ifdef __cplusplus
}
#endif

#endif // FLEET_DATA_MANAGER_CORE_DYNAMIC_PLUGIN_BASE_H
