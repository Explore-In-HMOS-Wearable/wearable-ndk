# Copyright (c) 2024 Huawei Device Co., Ltd.
# Licensed under the Apache License, Version 2.0 (the "License");

cmake_minimum_required(VERSION 3.5.0)
project(MetaballsNDK)

set(NATIVERENDER_ROOT_PATH ${CMAKE_CURRENT_SOURCE_DIR})

# OpenGL ES 3.0 ve EGL için include path'leri
include_directories(
    ${NATIVERENDER_ROOT_PATH}
    ${NATIVERENDER_ROOT_PATH}/include
)

# Tüm kaynak dosyaları ekle
add_library(entry SHARED
    # Main entry point
    napi_init.cpp

    # Manager
    manager/plugin_manager.cpp

    # Render
    render/plugin_render.cpp
    render/egl_core_shader.cpp
)

# HarmonyOS NDK kütüphanelerini bağla
target_link_libraries(entry PUBLIC
    libace_napi.z.so          # N-API support
    libace_ndk.z.so           # XComponent support (EKSİK OLAN BU!)
    libhilog_ndk.z.so         # Logging
    libEGL.so                 # EGL
    libGLESv3.so              # OpenGL ES 3.0
    libnative_window.so       # Native window
    libnative_vsync.so        # VSync support
)

# C++17 standardını kullan
set_target_properties(entry PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
)

# Compiler flags
target_compile_options(entry PRIVATE
    -Wall
    -Wextra
    -Wno-unused-parameter
    -O2
    -fno-rtti
)

# Preprocessor definitions
target_compile_definitions(entry PRIVATE
    GL_GLEXT_PROTOTYPES
    EGL_EGLEXT_PROTOTYPES
)