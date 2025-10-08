
#include <hilog/log.h>
#include <random>
#include <cmath>
#include <vector>
#include "render/egl_core_shader.h"
#include "common/native_common.h"

const char *METABALL_SYNC_NAME = "metaballVSync";

#ifndef EGL_GL_COLORSPACE_KHR
#define EGL_GL_COLORSPACE_KHR 0x309D
#endif

#ifndef EGL_GL_COLORSPACE_SRGB_KHR
#define EGL_GL_COLORSPACE_SRGB_KHR 0x3089
#endif

#define MAX_METABALLS 100
#define PI 3.14159265359f

char vertexShader[] = "#version 300 es\n"
                      "layout(location = 0) in vec4 a_position;\n"
                      "void main()\n"
                      "{\n"
                      "   gl_Position = a_position;\n"
                      "}\n";

char fragmentShader[] = "#version 300 es\n"
                        "precision highp float;\n"
                        "out vec4 fragColor;\n"
                        "uniform vec2 metaballArray[100];\n"
                        "uniform int numMetaballs;\n"
                        "uniform float screenHeight;\n"
                        "uniform float metaballRadiusSquared;\n"
                        "void main()\n"
                        "{\n"
                        "   vec2 pixelCoord = gl_FragCoord.xy;\n"
                        "   pixelCoord.y = screenHeight - pixelCoord.y;\n"
                        "   float sum = 0.0;\n"
                        "   for(int i = 0; i < numMetaballs; i++) {\n"
                        "       vec2 diff = metaballArray[i] - pixelCoord;\n"
                        "       float distSquared = dot(diff, diff);\n"
                        "       if(distSquared < 0.001) distSquared = 0.001;\n"
                        "       sum += metaballRadiusSquared / distSquared;\n"
                        "   }\n"
                        "   vec3 color = vec3(0.0);\n"
                        "   if(sum >= 1.0) {\n"
                        "       color = vec3(0.1, 0.8, 0.9);\n"
                        "   } else if(sum >= 0.5) {\n"
                        "       color = vec3(0.05, 0.4, 0.6);\n"
                        "   }\n"
                        "   fragColor = vec4(color, 1.0);\n"
                        "}\n";

struct Metaball {
    float x, y;
    float dirX, dirY;
    float radius;
    bool active;
};

std::vector<Metaball> metaballs;
float metaballPositions[2 * MAX_METABALLS] = {0};
std::mt19937 rng;

void InitMetaballs() {
    metaballs.clear();
    rng.seed(std::random_device{}());
    LOGI("Metaballs initialized");
}

void AddMetaball(float x, float y, float screenWidth, float screenHeight, float radius) {
    if (metaballs.size() >= MAX_METABALLS) {
        LOGW("Maximum metaballs reached");
        return;
    }

    std::uniform_real_distribution<float> angleDist(0, 2.0f * PI);
    float angle = angleDist(rng);

    Metaball mb;
    mb.x = x;
    mb.y = y;
    mb.dirX = std::cos(angle);
    mb.dirY = std::sin(angle);
    mb.radius = radius;
    mb.active = true;

    metaballs.push_back(mb);
    LOGI("Metaball added at (%{public}f, %{public}f), total: %{public}zu", x, y, metaballs.size());
}

void UpdateMetaballs(float screenWidth, float screenHeight, float speed) {
    for (size_t i = 0; i < metaballs.size(); i++) {
        if (!metaballs[i].active)
            continue;

        metaballs[i].x += metaballs[i].dirX * speed;
        metaballs[i].y += metaballs[i].dirY * speed;

        metaballPositions[2 * i] = metaballs[i].x;
        metaballPositions[2 * i + 1] = metaballs[i].y;

        if (metaballs[i].x >= screenWidth || metaballs[i].x <= 0) {
            metaballs[i].dirX *= -1.0f;
        }
        if (metaballs[i].y >= screenHeight || metaballs[i].y <= 0) {
            metaballs[i].dirY *= -1.0f;
        }
    }
}

struct SyncParam {
    EGLCore *eglCore = nullptr;
    void *window = nullptr;
};

static EGLConfig getConfig(EGLDisplay eglDisplay) {
    int attribList[] = {EGL_SURFACE_TYPE,
                        EGL_WINDOW_BIT,
                        EGL_RED_SIZE,
                        8,
                        EGL_GREEN_SIZE,
                        8,
                        EGL_BLUE_SIZE,
                        8,
                        EGL_ALPHA_SIZE,
                        8,
                        EGL_RENDERABLE_TYPE,
                        EGL_OPENGL_ES3_BIT,
                        EGL_NONE};
    EGLConfig configs = NULL;
    int configsNum;
    if (!eglChooseConfig(eglDisplay, attribList, &configs, 1, &configsNum)) {
        LOGE("eglChooseConfig ERROR");
        return NULL;
    }
    return configs;
}


void EGLCore::OnSurfaceCreated(void *window, int w, int h) {
    LOGD("EGLCore::OnSurfaceCreated w=%{public}d, h=%{public}d", w, h);
    width_ = w;
    height_ = h;

    InitMetaballs();

    SyncParam *param = new SyncParam();
    param->eglCore = this;
    param->window = window;
    mVsync = OH_NativeVSync_Create(METABALL_SYNC_NAME, 3);

    if (!mVsync) {
        LOGE("Create mVsync failed");
        return;
    }

    OH_NativeVSync_RequestFrame(
        mVsync,
        [](long long timestamp, void *data) {
            SyncParam *syncParam = (SyncParam *)data;
            if (!syncParam) return;

            EGLCore *eglCore = syncParam->eglCore;
            void *window = syncParam->window;
            if (!eglCore || !window) return;

            // Düzeltme: reinterpret_cast kullan
            eglCore->mEglWindow = reinterpret_cast<EGLNativeWindowType>(window);
            eglCore->mEGLDisplay = eglGetDisplay(EGL_DEFAULT_DISPLAY);

            if (eglCore->mEGLDisplay == EGL_NO_DISPLAY) {
                LOGE("Unable to get EGL display");
                return;
            }

            EGLint eglMajVers, eglMinVers;
            if (!eglInitialize(eglCore->mEGLDisplay, &eglMajVers, &eglMinVers)) {
                LOGE("Unable to initialize display");
                return;
            }

            eglCore->mEGLConfig = getConfig(eglCore->mEGLDisplay);
            if (!eglCore->mEGLConfig) {
                LOGE("Config ERROR");
                return;
            }

            EGLint winAttribs[] = {EGL_GL_COLORSPACE_KHR, EGL_GL_COLORSPACE_SRGB_KHR, EGL_NONE};
            eglCore->mEGLSurface =
                eglCreateWindowSurface(eglCore->mEGLDisplay, eglCore->mEGLConfig, eglCore->mEglWindow, winAttribs);

            if (!eglCore->mEGLSurface) {
                LOGE("eglSurface is null");
                return;
            }

            int attrib3_list[] = {EGL_CONTEXT_CLIENT_VERSION, 3, EGL_NONE};
            eglCore->mEGLContext =
                eglCreateContext(eglCore->mEGLDisplay, eglCore->mEGLConfig, eglCore->mSharedEGLContext, attrib3_list);

            if (!eglMakeCurrent(eglCore->mEGLDisplay, eglCore->mEGLSurface, eglCore->mEGLSurface,
                                eglCore->mEGLContext)) {
                LOGE("eglMakeCurrent error = %{public}d", eglGetError());
                return;
            }

            eglCore->mProgramHandle = eglCore->CreateProgram(vertexShader, fragmentShader);
            if (!eglCore->mProgramHandle) {
                LOGE("Could not create program");
                return;
            }

            float metaballRadius = 25.0f;
            eglCore->metaballRadiusSquared_ = metaballRadius * metaballRadius;

            glUseProgram(eglCore->mProgramHandle);
            GLint screenHeightLoc = glGetUniformLocation(eglCore->mProgramHandle, "screenHeight");
            glUniform1f(screenHeightLoc, (float)eglCore->height_);
            
            GLint radiusLoc = glGetUniformLocation(eglCore->mProgramHandle, "metaballRadiusSquared");
            glUniform1f(radiusLoc, eglCore->metaballRadiusSquared_);

            LOGI("EGL initialized successfully, starting render loop");
            eglCore->RenderLoop();
        },
        param);
}


void EGLCore::RenderLoop() {
    if (!eglMakeCurrent(mEGLDisplay, mEGLSurface, mEGLSurface, mEGLContext)) {
        LOGE("RenderLoop: eglMakeCurrent error = %{public}d", eglGetError());
        return;
    }

    UpdateMetaballs((float)width_, (float)height_, 2.0f);

    glViewport(0, 0, width_, height_);
    glClearColor(0.04f, 0.04f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    glUseProgram(mProgramHandle);

    GLint numMetaballsLoc = glGetUniformLocation(mProgramHandle, "numMetaballs");
    glUniform1i(numMetaballsLoc, (int)metaballs.size());

    GLint metaballArrayLoc = glGetUniformLocation(mProgramHandle, "metaballArray");
    glUniform2fv(metaballArrayLoc, MAX_METABALLS, metaballPositions);

    GLfloat vertices[] = {-1.0f, -1.0f, 1.0f, -1.0f, -1.0f, 1.0f, 1.0f, 1.0f};

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, vertices);
    glEnableVertexAttribArray(0);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    glDisableVertexAttribArray(0);

    glFlush();
    glFinish();
    eglSwapBuffers(mEGLDisplay, mEGLSurface);

    OH_NativeVSync_RequestFrame(
        mVsync, [](long long timestamp, void *data) { (reinterpret_cast<EGLCore *>(data))->RenderLoop(); },
        (void *)this);
}

void EGLCore::AddMetaballAt(float x, float y) { AddMetaball(x, y, (float)width_, (float)height_, 25.0f); }

void EGLCore::ClearAllMetaballs() {
    metaballs.clear();
    LOGI("All metaballs cleared");
}

void EGLCore::Update() { eglSwapBuffers(mEGLDisplay, mEGLSurface); }

GLuint EGLCore::LoadShader(GLenum type, const char *shaderSrc) {
    GLuint shader = glCreateShader(type);
    if (shader == 0)
        return 0;

    glShaderSource(shader, 1, &shaderSrc, nullptr);
    glCompileShader(shader);

    GLint compiled;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
    if (!compiled) {
        GLint infoLen = 0;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLen);
        if (infoLen > 1) {
            char *infoLog = (char *)malloc(sizeof(char) * infoLen);
            glGetShaderInfoLog(shader, infoLen, nullptr, infoLog);
            LOGE("Shader compile error: %{public}s", infoLog);
            free(infoLog);
        }
        glDeleteShader(shader);
        return 0;
    }
    return shader;
}

GLuint EGLCore::CreateProgram(const char *vertexShader, const char *fragShader) {
    GLuint vertex = LoadShader(GL_VERTEX_SHADER, vertexShader);
    GLuint fragment = LoadShader(GL_FRAGMENT_SHADER, fragShader);
    GLuint program = glCreateProgram();

    if (vertex == 0 || fragment == 0 || program == 0)
        return 0;

    glAttachShader(program, vertex);
    glAttachShader(program, fragment);
    glLinkProgram(program);

    GLint linked;
    glGetProgramiv(program, GL_LINK_STATUS, &linked);
    if (!linked) {
        GLint infoLen = 0;
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &infoLen);
        if (infoLen > 1) {
            char *infoLog = (char *)malloc(sizeof(char) * infoLen);
            glGetProgramInfoLog(program, infoLen, nullptr, infoLog);
            LOGE("Program link error: %{public}s", infoLog);
            free(infoLog);
        }
        glDeleteShader(vertex);
        glDeleteShader(fragment);
        glDeleteProgram(program);
        return 0;
    }

    glDeleteShader(vertex);
    glDeleteShader(fragment);
    return program;
}

void EGLCore::OnSurfaceDestroyed() {
    LOGI("EGLCore::OnSurfaceDestroyed");
    if (mVsync) {
        OH_NativeVSync_Destroy(mVsync);
        mVsync = nullptr;
    }
    if (mEGLContext != EGL_NO_CONTEXT) {
        eglDestroyContext(mEGLDisplay, mEGLContext);
        mEGLContext = EGL_NO_CONTEXT;
    }
    if (mEGLSurface != EGL_NO_SURFACE) {
        eglDestroySurface(mEGLDisplay, mEGLSurface);
        mEGLSurface = EGL_NO_SURFACE;
    }
}

void EGLCore::OnSurfaceChanged(void *window, int32_t w, int32_t h) {
    width_ = w;
    height_ = h;
}