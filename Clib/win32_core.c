#include <Windows.h>

#include "../quickjs.h"
#include "win32_type.h"

#define countof(x) (sizeof(x) / sizeof((x)[0]))

static JSClassID win32_core_class_id;

static void win32_core_finalizer(JSRuntime *rt, JSValue val) {
    Win32_Core *ww = JS_GetOpaque(val, win32_core_class_id);
    js_free_rt(rt, ww);
}



static JSValue win32_core_ctor(JSContext *ctx, JSValueConst new_target,
                               int argc, JSValueConst *argv) {
    Win32_Core *ww;
    JSValue obj = JS_UNDEFINED;
    JSValue proto;
    ww = js_mallocz(ctx, sizeof(*ww));
    if (!ww) {
        return JS_EXCEPTION;
    }
    ww->ID = 0;
    proto = JS_GetPropertyStr(ctx, new_target, "prototype");
    if (JS_IsException(proto)) {
        goto fail;
    }
    obj = JS_NewObjectProtoClass(ctx, proto, win32_core_class_id);
    JS_FreeValue(ctx, proto);
    if (JS_IsException(obj)) {
        goto fail;
    }
    JS_SetOpaque(obj, ww);
    return obj;
fail:
    js_free(ctx, ww);
    JS_FreeValue(ctx, obj);
    return JS_EXCEPTION;
}



static JSValue win32_core_messagebox(JSContext *ctx, JSValueConst this_val,
                                     int argc, JSValueConst *argv) {
    
    Win32_Core *ww = JS_GetOpaque2(ctx, this_val, win32_core_class_id);
    if (!ww) {
        return JS_EXCEPTION;
    }
    int *parent = 0;
    JS_ToInt32(ctx, (int*)&parent, JS_GetPropertyStr(ctx, argv[0], "Parent"));
    const char *text = JS_ToCString(ctx, JS_GetPropertyStr(ctx, argv[0], "Text"));
    const char *title = JS_ToCString(ctx, JS_GetPropertyStr(ctx, argv[0], "Title"));
    int flag;
    JS_ToInt32(ctx, &flag, JS_GetPropertyStr(ctx, argv[0], "Flag"));
    int a = MessageBoxA((HWND)parent, text, title, flag);
    
    return JS_NewInt32(ctx, a);
}

static JSValue test(JSContext *ctx, JSValueConst this_val, int argc,
                    JSValueConst *argv) {
    return JS_NewInt32(ctx, 333);
}

static JSClassDef win32_core_class = {
    "Win32_Core",
    .finalizer = win32_core_finalizer,
};
static const JSCFunctionListEntry window_core_proto_funcs[] = {
    JS_CFUNC_DEF("MessageBox", 0, win32_core_messagebox),
    JS_CFUNC_DEF("test", 0, test),
};

static int win32_core_init(JSContext *ctx, JSModuleDef *m) {
    JSValue proto, obj;
    JS_NewClassID(&win32_core_class_id);
    JS_NewClass(JS_GetRuntime(ctx), win32_core_class_id, &win32_core_class);
    proto = JS_NewObject(ctx);
    JS_SetPropertyFunctionList(ctx, proto, window_core_proto_funcs,
                               countof(window_core_proto_funcs));
    obj = JS_NewCFunction2(ctx, win32_core_ctor, "Win32_Core", 0,
                           JS_CFUNC_constructor, 0);
    JS_SetConstructor(ctx, obj, proto);
    JS_SetClassProto(ctx, win32_core_class_id, proto);

    JS_SetModuleExport(ctx, m, "Win32_Core", obj);
    return 0;
}

JSModuleDef *js_init_module(JSContext *ctx, const char *module_name) {
    JSModuleDef *m;
    m = JS_NewCModule(ctx, module_name, win32_core_init);
    if (!m) return NULL;
    JS_AddModuleExport(ctx, m, "Win32_Core");
    return m;
}
