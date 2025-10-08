/*
 * Copyright (c) 2024 Huawei Device Co., Ltd.
 * Licensed under the Apache License, Version 2.0 (the "License");
 */

#ifndef NATIVE_COMMON_H
#define NATIVE_COMMON_H

#include <hilog/log.h>

#ifdef __cplusplus
#define EXTERN_C_START extern "C" {
#define EXTERN_C_END }
#else
#define EXTERN_C_START
#define EXTERN_C_END
#endif

#undef LOG_DOMAIN
#undef LOG_TAG
#define LOG_DOMAIN 0x0201
#define LOG_TAG "METABALLS_NDK"

#define LOGI(...) OH_LOG_INFO(LOG_APP, __VA_ARGS__)
#define LOGD(...) OH_LOG_DEBUG(LOG_APP, __VA_ARGS__)
#define LOGW(...) OH_LOG_WARN(LOG_APP, __VA_ARGS__)
#define LOGE(...) OH_LOG_ERROR(LOG_APP, __VA_ARGS__)

#define NAPI_CALL(env, call)                                      \
    do {                                                          \
        napi_status status = (call);                              \
        if (status != napi_ok) {                                  \
            LOGE("NAPI call failed: %{public}d", status);         \
            return nullptr;                                       \
        }                                                         \
    } while (0)

#define DECLARE_NAPI_FUNCTION(name, func)     \
    {                                         \
        (name), nullptr, (func), nullptr,     \
        nullptr, nullptr, napi_default, nullptr \
    }

#endif // NATIVE_COMMON_H