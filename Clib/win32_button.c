#include <Windows.h>

#include "../quickjs.h"
#include "win32_type.h"
#define countof(x) \
    (sizeof(x) / sizeof((x)[0]))  // 定义宏，用于获取数组的元素个数
void **Win32_Component;
volatile LONG *Win32_Component_Num;
void Win32_Component_Push(void *component) {
    Win32_Component[*Win32_Component_Num] = component;
    _InterlockedIncrement(Win32_Component_Num);
}
static JSClassID win32_window_class_id;

static void win32_window_finalizer(JSRuntime *rt, JSValue val) {
    Win32_Button *ww = JS_GetOpaque(val, win32_window_class_id);
    // JS_FreeValueRT(rt, ww->OnClick);
    js_free_rt(rt, ww);
}

static JSValue win32_window_ctor(JSContext *ctx, JSValueConst new_target,
                                 int argc, JSValueConst *argv) {
    Win32_Button *ww;
    JSValue obj = JS_UNDEFINED;
    JSValue proto;

    ww = js_mallocz(ctx, sizeof(*ww));
    if (!ww) return JS_EXCEPTION;

    ww->Text = JS_ToCString(ctx, JS_GetPropertyStr(ctx, argv[0], "Text"));
    // ww->Text = "Button";
    ww->ClassName =
        JS_ToCString(ctx, JS_GetPropertyStr(ctx, argv[0], "ClassName"));
    if (JS_ToInt32(ctx, &ww->X, JS_GetPropertyStr(ctx, argv[0], "X")))
        goto fail;
    if (JS_ToInt32(ctx, &ww->Y, JS_GetPropertyStr(ctx, argv[0], "Y")))
        goto fail;
    if (JS_ToInt32(ctx, &ww->Width, JS_GetPropertyStr(ctx, argv[0], "Width")))
        goto fail;
    if (JS_ToInt32(ctx, &ww->Height, JS_GetPropertyStr(ctx, argv[0], "Height")))
        goto fail;
    ww->OnClick = JS_GetPropertyStr(ctx, argv[0], "OnClick");
    JS_FreeValue(ctx, ww->OnClick);
    ww->type = Type_Button;
    Win32_Component_Push(ww);
    proto = JS_GetPropertyStr(ctx, new_target, "prototype");
    if (JS_IsException(proto)) goto fail;
    obj = JS_NewObjectProtoClass(ctx, proto, win32_window_class_id);
    JS_FreeValue(ctx, proto);
    if (JS_IsException(obj)) goto fail;
    JS_SetOpaque(obj, ww);
    return obj;
fail:
    js_free(ctx, ww);
    JS_FreeValue(ctx, obj);
    return JS_EXCEPTION;
}

static JSValue win32_window_get_width(JSContext *ctx, JSValueConst this_val,
                                      int magic) {
    Win32_Button *ww = JS_GetOpaque2(ctx, this_val, win32_window_class_id);
    if (!ww) return JS_EXCEPTION;
    if (magic == 0) {
        RECT rect;
        GetWindowRect(ww->hwnd, &rect);
        int width = rect.right - rect.left;
        return JS_NewInt32(ctx, width);
    };
    return JS_UNDEFINED;
}

static JSValue win32_window_get_height(JSContext *ctx, JSValueConst this_val,
                                       int magic) {
    Win32_Button *ww = JS_GetOpaque2(ctx, this_val, win32_window_class_id);
    if (!ww) return JS_EXCEPTION;
    if (magic == 0) {
        RECT rect;
        GetWindowRect(ww->hwnd, &rect);
        int height = rect.bottom - rect.top;
        return JS_NewInt32(ctx, height);
    }
    return JS_UNDEFINED;
}

static JSValue win32_window_set_width(JSContext *ctx, JSValueConst this_val,
                                      JSValueConst val, int magic) {
    Win32_Button *ww = JS_GetOpaque2(ctx, this_val, win32_window_class_id);
    if (!ww) return JS_EXCEPTION;
    // if (JS_ToInt32(ctx, &ww->width, val)) return JS_EXCEPTION;
    if (JS_ToInt32(ctx, &ww->Width, val)) return JS_EXCEPTION;

    RECT rect;
    GetWindowRect(ww->hwnd, &rect);
    int currentHeight = rect.bottom - rect.top;
    int currentX = rect.left;
    int currentY = rect.top;
    // 根据新的宽度和当前窗口的高度、左上角坐标来计算新的右下角坐标
    int newRight = currentX + ww->Width;
    int newBottom = currentY + currentHeight;
    // 使用 SetWindowPos 函数修改窗口大小
    SetWindowPos(ww->hwnd, HWND_TOP, currentX, currentY, newRight - currentX,
                 newBottom - currentY, SWP_NOMOVE);

    return JS_UNDEFINED;
}

static JSValue win32_window_set_height(JSContext *ctx, JSValueConst this_val,
                                       JSValueConst val, int magic) {
    Win32_Button *ww = JS_GetOpaque2(ctx, this_val, win32_window_class_id);
    if (!ww) return JS_EXCEPTION;
    if (JS_ToInt32(ctx, &ww->Height, val)) return JS_EXCEPTION;

    RECT rect;
    GetWindowRect(ww->hwnd, &rect);
    int currentWidth = rect.right - rect.left;
    int currentX = rect.left;
    int currentY = rect.top;
    // 根据当前窗口的宽度、新的高度、左上角坐标来计算新的右下角坐标
    int newRight = currentX + currentWidth;
    int newBottom = currentY + ww->Height;
    // 使用 SetWindowPos 函数修改窗口大小
    SetWindowPos(ww->hwnd, HWND_TOP, currentX, currentY, newRight - currentX,
                 newBottom - currentY, SWP_NOMOVE);

    return JS_UNDEFINED;
}

static JSValue win32_window_get_text(JSContext *ctx, JSValueConst this_val,
                                     int magic) {
    Win32_Button *ww = JS_GetOpaque2(ctx, this_val, win32_window_class_id);
    if (!ww) return JS_EXCEPTION;
    if (magic == 0) {
        int length = GetWindowTextLength(ww->hwnd) + 1;
        char *text = js_malloc(ctx, length);
        GetWindowText(ww->hwnd, text, length);
        return JS_NewString(ctx, text);
    };
    return JS_UNDEFINED;
}
static JSValue win32_window_set_text(JSContext *ctx, JSValueConst this_val,
                                     JSValueConst val, int magic) {
    Win32_Button *ww = JS_GetOpaque2(ctx, this_val, win32_window_class_id);
    if (!ww) return JS_EXCEPTION;
    const char *text = JS_ToCString(ctx, val);
    SetWindowText(ww->hwnd, text);
    return JS_UNDEFINED;
}
static JSValue win32_window_get_x(JSContext *ctx, JSValueConst this_val,
                                  int magic) {
    Win32_Button *ww = JS_GetOpaque2(ctx, this_val, win32_window_class_id);
    if (!ww) return JS_EXCEPTION;
    if (magic == 0) {
        RECT rect;
        GetWindowRect(ww->hwnd, &rect);
        // 将坐标转换为相对于父窗口的坐标系
        ScreenToClient(ww->parent, (LPPOINT)&rect.left);
        ScreenToClient(ww->parent, (LPPOINT)&rect.right);
        return JS_NewInt32(ctx, rect.left);
    }
    return JS_UNDEFINED;
}
static JSValue win32_window_set_x(JSContext *ctx, JSValueConst this_val,
                                  JSValueConst val, int magic) {
    Win32_Button *ww = JS_GetOpaque2(ctx, this_val, win32_window_class_id);
    if (!ww) return JS_EXCEPTION;
    RECT rect;
    GetWindowRect(ww->hwnd, &rect);
    // 将坐标转换为相对于父窗口的坐标系
    HWND hParentWnd = ww->parent;
    POINT pt = {rect.left, rect.top};
    ScreenToClient(hParentWnd, &pt);
    // 计算新的 X 坐标
    int newX;
    if (JS_ToInt32(ctx, &newX, val)) return JS_EXCEPTION;
    SetWindowPos(ww->hwnd, NULL, newX, pt.y, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
    return JS_UNDEFINED;
}
static JSValue win32_window_get_y(JSContext *ctx, JSValueConst this_val,
                                  int magic) {
    Win32_Button *ww = JS_GetOpaque2(ctx, this_val, win32_window_class_id);
    if (!ww) return JS_EXCEPTION;
    if (magic == 0) {
        RECT rect;
        GetWindowRect(ww->hwnd, &rect);
        ScreenToClient(ww->parent, (LPPOINT)&rect.left);
        ScreenToClient(ww->parent, (LPPOINT)&rect.right);
        return JS_NewInt32(ctx, rect.top);
    }
    return JS_UNDEFINED;
}
static JSValue win32_window_set_y(JSContext *ctx, JSValueConst this_val,
                                  JSValueConst val, int magic) {
    Win32_Button *ww = JS_GetOpaque2(ctx, this_val, win32_window_class_id);
    if (!ww) return JS_EXCEPTION;
    RECT rect;
    GetWindowRect(ww->hwnd, &rect);

    // 将坐标转换为相对于父窗口的坐标系
    HWND hParentWnd = ww->parent;
    POINT pt = { rect.left, rect.top };
    ScreenToClient(hParentWnd, &pt);
    // 计算新的坐标
    int newX = pt.x;
    // 调用 SetWindowPos 函数设置新的位置
    int newY;
    if (JS_ToInt32(ctx, &newY, val)) return JS_EXCEPTION;
    SetWindowPos(ww->hwnd, NULL, newX, newY, 0, 0, SWP_NOSIZE | SWP_NOZORDER);    return JS_UNDEFINED;
}
static JSValue win32_window_get_HWND(JSContext *ctx, JSValueConst this_val) {
    Win32_Button *ww = JS_GetOpaque2(ctx, this_val, win32_window_class_id);
    if (!ww) return JS_EXCEPTION;
    return JS_NewInt64(ctx, (int)(int *)ww->hwnd);
}

static const JSCFunctionListEntry win32_window_proto_funcs[] = {
    // JS_CFUNC_DEF("testadd", 0, testadd),
    JS_CFUNC_DEF("GetHWND", 0, win32_window_get_HWND),
    // JS_CFUNC_DEF("testglobal", 0, testglobal),
    JS_CGETSET_MAGIC_DEF("Width", win32_window_get_width,
                         win32_window_set_width, 0),
    JS_CGETSET_MAGIC_DEF("Height", win32_window_get_height,
                         win32_window_set_height, 0),
    JS_CGETSET_MAGIC_DEF("Text", win32_window_get_text, win32_window_set_text,
                         0),
    JS_CGETSET_MAGIC_DEF("X", win32_window_get_x, win32_window_set_x, 0),
    JS_CGETSET_MAGIC_DEF("Y", win32_window_get_y, win32_window_set_y, 0),
    // JS_CFUNC_DEF("testglobal", 0, testglobal),
};

static JSClassDef win32_window_class = {
    "Win32_Button",
    .finalizer = win32_window_finalizer,
};

static int window_init(JSContext *ctx, JSModuleDef *m) {
    JSValue proto;
    JS_NewClassID(&win32_window_class_id);
    JS_NewClass(JS_GetRuntime(ctx), win32_window_class_id, &win32_window_class);
    proto = JS_NewObject(ctx);
    JS_SetPropertyFunctionList(ctx, proto, win32_window_proto_funcs,
                               countof(win32_window_proto_funcs));
    JSValue ctor;
    ctor = JS_NewCFunction2(ctx, win32_window_ctor, "Win32_Button", 4,
                            JS_CFUNC_constructor, 0);
    JS_SetConstructor(ctx, ctor, proto);
    JS_SetClassProto(ctx, win32_window_class_id, proto);
    JS_SetModuleExport(ctx, m, "Win32_Button", ctor);
    return 0;
}

JSModuleDef *js_init_module(JSContext *ctx, const char *module_name) {
    JSModuleDef *m;
    m = JS_NewCModule(ctx, module_name, window_init);
    if (!m) return NULL;
    JS_AddModuleExport(ctx, m, "Win32_Button");
    HANDLE hSharedMemory =
        OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, "JSWin32SharedMemory");
    Win32_Component =
        MapViewOfFile(hSharedMemory, FILE_MAP_ALL_ACCESS, 0, 0, 4096);

    HANDLE hNumSharedMemory =
        OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, "JSWin32NumSharedMemory");
    Win32_Component_Num = MapViewOfFile(hNumSharedMemory, FILE_MAP_ALL_ACCESS,
                                        0, 0, sizeof(volatile LONG));
    return m;
}
