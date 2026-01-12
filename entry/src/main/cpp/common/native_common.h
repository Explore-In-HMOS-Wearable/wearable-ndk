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

#ifndef NATIVE_XCOMPONENT_COMMON_H
#define NATIVE_XCOMPONENT_COMMON_H

#include <hilog/log.h>

#ifdef __cplusplus
#define EXTERN_C_START extern "C" {
#define EXTERN_C_END }
#else
#define EXTERN_C_START
#define EXTERN_C_END
#endif

#ifdef __cplusplus
extern "C" {
#endif

#define LOGE(...) OH_LOG_Print(LOG_APP, LOG_ERROR, LOG_DOMAIN, "[WearableNDK]", __VA_ARGS__)
#define LOGW(...) OH_LOG_Print(LOG_APP, LOG_WARN, LOG_DOMAIN, "[WearableNDK]", __VA_ARGS__)
#define LOGI(...) OH_LOG_Print(LOG_APP, LOG_INFO, LOG_DOMAIN, "[WearableNDK]", __VA_ARGS__)
#define LOGD(...) OH_LOG_Print(LOG_APP, LOG_DEBUG, LOG_DOMAIN, "[WearableNDK]", __VA_ARGS__)
#define LOG_DOMAIN 0xFF00
#define NAPI_CALL(env, call)                      \
    do {                                          \
        napi_status status = (call);              \
        if (status != napi_ok) {                  \
            LOGE("NAPI call failed: %{public}d", status); \
            return nullptr;                       \
        }                                         \
    } while (0)

#define DECLARE_NAPI_FUNCTION(name, func)        \
    { name, nullptr, func, nullptr, nullptr, nullptr, napi_default, nullptr }

#ifdef __cplusplus
}
#endif

#endif // NATIVE_XCOMPONENT_COMMON_H
