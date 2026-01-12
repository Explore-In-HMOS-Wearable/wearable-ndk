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

#ifndef PLUGIN_PLUGIN_MANAGER_H
#define PLUGIN_PLUGIN_MANAGER_H

#include <ace/xcomponent/native_interface_xcomponent.h>
#include <napi/native_api.h>
#include <string>
#include <unordered_map>
#include "render/plugin_render.h"

class PluginManager {
public:
    ~PluginManager() {}

    static PluginManager *GetInstance()
    {
        return &PluginManager::manager_;
    }

    static napi_value GetContext(napi_env env, napi_callback_info info);

    bool Export(napi_env env, napi_value exports);

    void SetNativeXComponent(std::string& id, OH_NativeXComponent* nativeXComponent);

    PluginRender *GetRender(std::string& id);

private:
    static PluginManager manager_;
    std::unordered_map<std::string, OH_NativeXComponent *> nativeXComponentMap_;
    std::unordered_map<std::string, PluginRender *> pluginRenderMap_;
};
#endif // PLUGIN_PLUGIN_MANAGER_H
