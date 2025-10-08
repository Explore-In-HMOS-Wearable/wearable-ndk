/*
 * Copyright (c) 2024 Huawei Device Co., Ltd.
 * Licensed under the Apache License, Version 2.0 (the "License");
 */

#ifndef EGL_CORE_SHADER_H
#define EGL_CORE_SHADER_H

#include <string>
#include <EGL/egl.h>
#include <EGL/eglext.h>
#include <GLES3/gl3.h>
#include <native_vsync/native_vsync.h>

class EGLCore {
public:
    explicit EGLCore(std::string &id) : id_(id) {}
    ~EGLCore() {}

    void OnSurfaceCreated(void *window, int width, int height);
    void OnSurfaceChanged(void *window, int32_t width, int32_t height);
    void OnSurfaceDestroyed();
    void RenderLoop();
    void Update();
    
    // Metaball specific methods
    void AddMetaballAt(float x, float y);
    void ClearAllMetaballs();

    GLuint LoadShader(GLenum type, const char *shaderSrc);
    GLuint CreateProgram(const char *vertexShader, const char *fragShader);

    EGLNativeWindowType mEglWindow;
    EGLDisplay mEGLDisplay = EGL_NO_DISPLAY;
    EGLConfig mEGLConfig = nullptr;
    EGLContext mEGLContext = EGL_NO_CONTEXT;
    EGLContext mSharedEGLContext = EGL_NO_CONTEXT;
    EGLSurface mEGLSurface = nullptr;
    GLuint mProgramHandle;
    OH_NativeVSync *mVsync = nullptr;

    int width_;
    int height_;
    float metaballRadiusSquared_;

private:
    std::string id_;
};

#endif // EGL_CORE_SHADER_H