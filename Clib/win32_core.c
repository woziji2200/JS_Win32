#include "../quickjs.h"
#include <windows.h>

#define countof(x) (sizeof(x) / sizeof((x)[0]))



static JSValue win32_core_messagebox(JSContext *ctx, JSValueConst this_val,
                      int argc, JSValueConst *argv) {
    
    const char *title = JS_ToCString(ctx, JS_GetPropertyStr(ctx, argv[0], "Title"));
    const char *text = JS_ToCString(ctx, JS_GetPropertyStr(ctx, argv[0], "Text"));
    int *parent;
    if(JS_ToInt32(ctx, &parent, JS_GetPropertyStr(ctx, argv[0], "Parent"))) parent = NULL;
    int flag;
    if(JS_ToInt32(ctx, &flag, JS_GetPropertyStr(ctx, argv[0], "Flag"))) flag = MB_OK;
    int a = MessageBoxA((HWND)parent, title, text, flag);
    JS_FreeCString(ctx, title);
    JS_FreeCString(ctx, text);
    return JS_NewInt32(ctx, a);
}

static const JSCFunctionListEntry js_fib_funcs[] = {
    JS_CFUNC_DEF("MessageBox", 1, win32_core_messagebox ),
};

static int js_point_init(JSContext *ctx, JSModuleDef *m)
{
    JSValue point_proto, point_class;
    
    /* create the Point class */
    JS_NewClassID(&js_point_class_id);
    JS_NewClass(JS_GetRuntime(ctx), js_point_class_id, &js_point_class);

    point_proto = JS_NewObject(ctx);
    JS_SetPropertyFunctionList(ctx, point_proto, js_point_proto_funcs, countof(js_point_proto_funcs));
    
    point_class = JS_NewCFunction2(ctx, NULL, "Win32_Core", 2, JS_CFUNC_constructor, 0);
    /* set proto.constructor and ctor.prototype */
    JS_SetConstructor(ctx, point_class, point_proto);
    JS_SetClassProto(ctx, js_point_class_id, point_proto);
                      
    JS_SetModuleExport(ctx, m, "Win32_Core", point_class);
    return 0;
}

JSModuleDef *js_init_module(JSContext *ctx, const char *module_name)
{
    JSModuleDef *m;
    m = JS_NewCModule(ctx, module_name, win32_core_init);
    if (!m)
        return NULL;
    JS_AddModuleExport(ctx, m, "Win32_Core");
    return m;
}