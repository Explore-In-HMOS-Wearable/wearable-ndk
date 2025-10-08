#include <hilog/log.h>
#include "common/native_common.h"
#include "manager/plugin_manager.h"
#include "napi/native_api.h"

EXTERN_C_START
static napi_value Init(napi_env env, napi_value exports)
{
    napi_property_descriptor desc[] = {
        { "addMetaball", nullptr, PluginRender::NapiAddMetaball, nullptr, nullptr, nullptr, napi_default, nullptr },
        { "clearMetaballs", nullptr, PluginRender::NapiClearMetaballs, nullptr, nullptr, nullptr, napi_default, nullptr },
    };

    NAPI_CALL(env, napi_define_properties(env, exports, sizeof(desc) / sizeof(desc[0]), desc));

    bool ret = PluginManager::GetInstance()->Export(env, exports);
    if (!ret) {
        LOGE("Init failed");
    }
    return exports;
}
EXTERN_C_END

static napi_module demoModule = {
    .nm_version = 1,
    .nm_flags = 0,
    .nm_filename = nullptr,
    .nm_register_func = Init,
    .nm_modname = "entry",
    .nm_priv = ((void *)0),
    .reserved = {0},
};

extern "C" __attribute__((constructor)) void RegisterEntryModule(void) { 
    napi_module_register(&demoModule); 
}