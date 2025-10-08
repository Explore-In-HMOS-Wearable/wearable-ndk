/*
 * Copyright (c) 2024 Huawei Device Co., Ltd.
 * Licensed under the Apache License, Version 2.0 (the "License");
 */

#ifndef PLUGIN_RENDER_H
#define PLUGIN_RENDER_H

#include <ace/xcomponent/native_interface_xcomponent.h>
#include <napi/native_api.h>
#include <string>
#include <unordered_map>
#include "render/egl_core_shader.h"

class PluginRender {
public:
    explicit PluginRender(std::string &id);
    ~PluginRender() {}
    
    static PluginRender *GetInstance(std::string &id);
    static napi_value NapiAddMetaball(napi_env env, napi_callback_info info);
    static napi_value NapiClearMetaballs(napi_env env, napi_callback_info info);

    void SetNativeXComponent(OH_NativeXComponent *component);
    void OnSurfaceCreated(OH_NativeXComponent *component, void *window);
    void OnSurfaceChanged(OH_NativeXComponent *component, void *window);
    void OnSurfaceDestroyed(OH_NativeXComponent *component, void *window);
    void OnTouchEvent(OH_NativeXComponent *component, void *window);
    napi_value Export(napi_env env, napi_value exports);

    static OH_NativeXComponent_Callback *GetNXComponentCallback();

    EGLCore *eglCore_;

private:
    static std::unordered_map<std::string, PluginRender *> instance_;
    static OH_NativeXComponent_Callback callback_;
    
    std::string id_;
    uint64_t width_ = 0;
    uint64_t height_ = 0;
};

#endif // PLUGIN_RENDER_H