/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2025-2025. All rights reserved.
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 * 
 *     http://www.apache.org/licenses/LICENSE-2.0
 * 
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef PLUGIN_RENDER_H
#define PLUGIN_RENDER_H

#include <string>
#include <unordered_map>
#include <ace/xcomponent/native_interface_xcomponent.h>
#include <napi/native_api.h>
#include "render/egl_core_shader.h"

class PluginRender {
public:
    explicit PluginRender(std::string& id);
    static PluginRender* GetInstance(std::string& id);
    static napi_value NapiAddMetaball(napi_env env, napi_callback_info info);
    static napi_value NapiClearMetaballs(napi_env env, napi_callback_info info);
    static OH_NativeXComponent_Callback* GetNXComponentCallback();
    void SetNativeXComponent(OH_NativeXComponent* component);
    void OnSurfaceCreated(OH_NativeXComponent* component, void* window);
    void OnSurfaceChanged(OH_NativeXComponent* component, void* window);
    void OnSurfaceDestroyed(OH_NativeXComponent* component, void* window);
    void OnTouchEvent(OH_NativeXComponent* component, void* window);
    napi_value Export(napi_env env, napi_value exports);

public:
    static std::unordered_map<std::string, PluginRender*> instance_;
    EGLCore* eglCore_;

private:
    static OH_NativeXComponent_Callback callback_;
    std::string id_;
    uint64_t width_ = 0;
    uint64_t height_ = 0;
};

#endif // PLUGIN_RENDER_H
