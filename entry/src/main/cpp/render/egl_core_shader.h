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

#ifndef NATIVE_XCOMPONENT_PLUGIN_RENDER_H
#define NATIVE_XCOMPONENT_PLUGIN_RENDER_H

#include <string>
#include <EGL/egl.h>
#include <GLES3/gl3.h>
#include <native_vsync/native_vsync.h>

class EGLCore {
public:
    explicit EGLCore(std::string& id) : id_(id) {};
    void OnSurfaceCreated(void *window, int w, int h);
    void OnSurfaceChanged(void *window, int32_t w, int32_t h);
    void OnSurfaceDestroyed();
    void RenderLoop();
    void AddMetaballAt(float x, float y);
    void ClearAllMetaballs();

private:
    void Update();
    GLuint LoadShader(GLenum type, const char *shaderSrc);
    GLuint CreateProgram(const char *vertexShader, const char *fragShader);

public:
    int32_t width_;
    int32_t height_;
    EGLNativeWindowType mEglWindow;
    EGLDisplay mEGLDisplay = EGL_NO_DISPLAY;
    EGLConfig mEGLConfig = nullptr;
    EGLContext mEGLContext = EGL_NO_CONTEXT;
    EGLContext mSharedEGLContext = EGL_NO_CONTEXT;
    EGLSurface mEGLSurface = nullptr;
    GLuint mProgramHandle;
    OH_NativeVSync *mVsync = nullptr;
    float metaballRadiusSquared_;

private:
    std::string id_;
};
#endif // NATIVE_XCOMPONENT_PLUGIN_RENDER_H
