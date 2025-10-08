#include <cstdint>
#include <hilog/log.h>
#include "common/native_common.h"
#include "manager/plugin_manager.h"
#include "render/plugin_render.h"


std::unordered_map<std::string, PluginRender *> PluginRender::instance_;
OH_NativeXComponent_Callback PluginRender::callback_;

void OnSurfaceCreatedCB(OH_NativeXComponent *component, void *window) {
    LOGD("OnSurfaceCreatedCB");
    int32_t ret;
    char idStr[OH_XCOMPONENT_ID_LEN_MAX + 1] = {};
    uint64_t idSize = OH_XCOMPONENT_ID_LEN_MAX + 1;
    ret = OH_NativeXComponent_GetXComponentId(component, idStr, &idSize);
    if (ret != OH_NATIVEXCOMPONENT_RESULT_SUCCESS) {
        LOGE("GetXComponentId failed");
        return;
    }

    std::string id(idStr);
    auto render = PluginRender::GetInstance(id);
    render->OnSurfaceCreated(component, window);
}

void OnSurfaceChangedCB(OH_NativeXComponent *component, void *window) {
    LOGD("OnSurfaceChangedCB");
    int32_t ret;
    char idStr[OH_XCOMPONENT_ID_LEN_MAX + 1] = {};
    uint64_t idSize = OH_XCOMPONENT_ID_LEN_MAX + 1;
    ret = OH_NativeXComponent_GetXComponentId(component, idStr, &idSize);
    if (ret != OH_NATIVEXCOMPONENT_RESULT_SUCCESS)
        return;

    std::string id(idStr);
    auto render = PluginRender::GetInstance(id);
    render->OnSurfaceChanged(component, window);
}

void OnSurfaceDestroyedCB(OH_NativeXComponent *component, void *window) {
    LOGD("OnSurfaceDestroyedCB");
    int32_t ret;
    char idStr[OH_XCOMPONENT_ID_LEN_MAX + 1] = {};
    uint64_t idSize = OH_XCOMPONENT_ID_LEN_MAX + 1;
    ret = OH_NativeXComponent_GetXComponentId(component, idStr, &idSize);
    if (ret != OH_NATIVEXCOMPONENT_RESULT_SUCCESS)
        return;

    std::string id(idStr);
    auto render = PluginRender::GetInstance(id);
    render->OnSurfaceDestroyed(component, window);
}

void DispatchTouchEventCB(OH_NativeXComponent *component, void *window) {
    LOGD("DispatchTouchEventCB");
    int32_t ret;
    char idStr[OH_XCOMPONENT_ID_LEN_MAX + 1] = {};
    uint64_t idSize = OH_XCOMPONENT_ID_LEN_MAX + 1;
    ret = OH_NativeXComponent_GetXComponentId(component, idStr, &idSize);
    if (ret != OH_NATIVEXCOMPONENT_RESULT_SUCCESS)
        return;

    std::string id(idStr);
    auto render = PluginRender::GetInstance(id);
    render->OnTouchEvent(component, window);
}

PluginRender::PluginRender(std::string &id) : id_(id) {
    eglCore_ = new EGLCore(id);
    auto renderCallback = PluginRender::GetNXComponentCallback();
    renderCallback->OnSurfaceCreated = OnSurfaceCreatedCB;
    renderCallback->OnSurfaceChanged = OnSurfaceChangedCB;
    renderCallback->OnSurfaceDestroyed = OnSurfaceDestroyedCB;
    renderCallback->DispatchTouchEvent = DispatchTouchEventCB;
}

PluginRender *PluginRender::GetInstance(std::string &id) {
    if (instance_.find(id) == instance_.end()) {
        PluginRender *instance = new PluginRender(id);
        instance_[id] = instance;
        return instance;
    } else {
        return instance_[id];
    }
}

OH_NativeXComponent_Callback *PluginRender::GetNXComponentCallback() { return &PluginRender::callback_; }

void PluginRender::SetNativeXComponent(OH_NativeXComponent *component) {
    OH_NativeXComponent_RegisterCallback(component, &PluginRender::callback_);
}

void PluginRender::OnSurfaceCreated(OH_NativeXComponent *component, void *window) {
    LOGD("PluginRender::OnSurfaceCreated");
    int32_t ret = OH_NativeXComponent_GetXComponentSize(component, window, &width_, &height_);
    if (ret == OH_NATIVEXCOMPONENT_RESULT_SUCCESS && eglCore_) {
        eglCore_->OnSurfaceCreated(window, width_, height_);
    }
}

void PluginRender::OnSurfaceChanged(OH_NativeXComponent *component, void *window) {
    LOGD("PluginRender::OnSurfaceChanged");
    int32_t ret = OH_NativeXComponent_GetXComponentSize(component, window, &width_, &height_);
    if (ret == OH_NATIVEXCOMPONENT_RESULT_SUCCESS && eglCore_) {
        eglCore_->OnSurfaceChanged(window, width_, height_);
    }
}

void PluginRender::OnSurfaceDestroyed(OH_NativeXComponent *component, void *window) {
    LOGD("PluginRender::OnSurfaceDestroyed");
    char idStr[OH_XCOMPONENT_ID_LEN_MAX + 1] = {};
    uint64_t idSize = OH_XCOMPONENT_ID_LEN_MAX + 1;
    int32_t ret = OH_NativeXComponent_GetXComponentId(component, idStr, &idSize);
    if (ret == OH_NATIVEXCOMPONENT_RESULT_SUCCESS && eglCore_) {
        eglCore_->OnSurfaceDestroyed();
    }
    if (eglCore_) {
        delete eglCore_;
        eglCore_ = nullptr;
    }

    if (instance_[id_]) {
        delete instance_[id_];
        instance_[id_] = nullptr;
        instance_.erase(id_);
    }
}

void PluginRender::OnTouchEvent(OH_NativeXComponent *component, void *window) {
    OH_NativeXComponent_TouchEvent touchEvent;
    OH_NativeXComponent_GetTouchEvent(component, window, &touchEvent);

    if (touchEvent.type == OH_NativeXComponent_TouchEventType::OH_NATIVEXCOMPONENT_DOWN) {
        float touchX = touchEvent.x;
        float touchY = touchEvent.y;

        if (eglCore_) {
            eglCore_->AddMetaballAt(touchX, touchY);
            LOGI("Touch at (%{public}f, %{public}f)", touchX, touchY);
        }
    }
}

napi_value PluginRender::Export(napi_env env, napi_value exports) {
    LOGI("PluginRender::Export");

    napi_property_descriptor desc[] = {
        DECLARE_NAPI_FUNCTION("addMetaball", PluginRender::NapiAddMetaball),
        DECLARE_NAPI_FUNCTION("clearMetaballs", PluginRender::NapiClearMetaballs),
    };
    NAPI_CALL(env, napi_define_properties(env, exports, sizeof(desc) / sizeof(desc[0]), desc));
    return exports;
}

napi_value PluginRender::NapiAddMetaball(napi_env env, napi_callback_info info) {
    LOGD("NapiAddMetaball called");

    size_t argc = 3;
    napi_value args[3] = {nullptr};

    napi_status status = napi_get_cb_info(env, info, &argc, args, nullptr, nullptr);
    if (status != napi_ok || argc < 3) {
        LOGE("NapiAddMetaball: Wrong argument count");
        return nullptr;
    }

    napi_value exportInstance = args[0];
    OH_NativeXComponent *nativeXComponent = nullptr;

    status = napi_unwrap(env, exportInstance, reinterpret_cast<void **>(&nativeXComponent));
    if (status != napi_ok) {
        LOGE("NapiAddMetaball: unwrap failed");
        return nullptr;
    }

    double x, y;
    status = napi_get_value_double(env, args[1], &x);
    if (status != napi_ok) {
        LOGE("NapiAddMetaball: failed to get x coordinate");
        return nullptr;
    }

    status = napi_get_value_double(env, args[2], &y);
    if (status != napi_ok) {
        LOGE("NapiAddMetaball: failed to get y coordinate");
        return nullptr;
    }

    std::string id("A");
    PluginRender *instance = PluginRender::GetInstance(id);
    if (instance && instance->eglCore_) {
        instance->eglCore_->AddMetaballAt((float)x, (float)y);
        LOGI("Metaball added at (%{public}f, %{public}f)", x, y);
    }
    return nullptr;
}

napi_value PluginRender::NapiClearMetaballs(napi_env env, napi_callback_info info) {
    LOGD("NapiClearMetaballs called");

    size_t argc = 1;
    napi_value args[1] = {nullptr};

    napi_status status = napi_get_cb_info(env, info, &argc, args, nullptr, nullptr);
    if (status != napi_ok || argc < 1) {
        LOGE("NapiClearMetaballs: Failed to get callback info");
        return nullptr;
    }

    napi_value exportInstance = args[0];
    OH_NativeXComponent *nativeXComponent = nullptr;

    status = napi_unwrap(env, exportInstance, reinterpret_cast<void **>(&nativeXComponent));
    if (status != napi_ok) {
        LOGE("NapiClearMetaballs: unwrap failed");
        return nullptr;
    }

    std::string id("A");
    PluginRender *instance = PluginRender::GetInstance(id);
    if (instance && instance->eglCore_) {
        instance->eglCore_->ClearAllMetaballs();
        LOGI("All metaballs cleared");
    }
    return nullptr;
}