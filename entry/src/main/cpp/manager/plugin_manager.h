/*
 * Copyright (c) 2024 Huawei Device Co., Ltd.
 * Licensed under the Apache License, Version 2.0 (the "License");
 */

#ifndef PLUGIN_MANAGER_H
#define PLUGIN_MANAGER_H

#include <ace/xcomponent/native_interface_xcomponent.h>
#include <napi/native_api.h>
#include <string>
#include <unordered_map>
#include "render/plugin_render.h"

class PluginManager {
public:
    ~PluginManager() {}

    static PluginManager *GetInstance() {
        return &manager_;
    }

    static napi_value GetContext(napi_env env, napi_callback_info info);
    
    void SetNativeXComponent(std::string &id, OH_NativeXComponent *nativeXComponent);
    PluginRender *GetRender(std::string &id);
    bool Export(napi_env env, napi_value exports);

private:
    static PluginManager manager_;
    std::unordered_map<std::string, OH_NativeXComponent *> nativeXComponentMap_;
    std::unordered_map<std::string, PluginRender *> pluginRenderMap_;
};

#endif // PLUGIN_MANAGER_H