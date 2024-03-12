#include <Windows.h>
#include "../quickjs.h"
#include "win32_type.h"
#define countof(x) \
    (sizeof(x) / sizeof((x)[0]))  // 定义宏，用于获取数组的元素个数
HANDLE CSharedMemory, CNumSharedMemory;
static JSClassID win32_window_class_id;

void **Win32_Component;
volatile LONG *Win32_Component_Num;

static JSRuntime *_rt;
static JSContext *_ctx;

static void win32_window_finalizer(JSRuntime *rt, JSValue val) {
    Win32_Window *ww = JS_GetOpaque(val, win32_window_class_id);
    // for (int i = 0; i < *Win32_Component_Num; i++) {
    //     int *type = (int *)(Win32_Component[i]);
    //     if (*type == Type_Button) {
    //         Win32_Button *button = Win32_Component[i];
    //         JS_FreeValue(_ctx, button->OnClick);
    //     }
    // }
    js_free_rt(rt, ww);
}

static JSValue win32_window_ctor(JSContext *ctx, JSValueConst new_target,
                                 int argc, JSValueConst *argv) {
    Win32_Window *ww;
    JSValue obj = JS_UNDEFINED;
    JSValue proto;

    ww = js_mallocz(ctx, sizeof(*ww));
    if (!ww) return JS_EXCEPTION;

    if (JS_ToInt32(ctx, &ww->Width, argv[0])) goto fail;
    if (JS_ToInt32(ctx, &ww->Height, argv[1])) goto fail;
    ww->Text = JS_ToCString(ctx, argv[2]);
    if (!ww->Text) goto fail;
    ww->ClassName = JS_ToCString(ctx, argv[3]);
    if (!ww->ClassName) goto fail;
    proto = JS_GetPropertyStr(ctx, new_target, "prototype");
    if (JS_IsException(proto)) goto fail;
    ww->type = Type_Window;
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

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam,
                            LPARAM lParam) {
    switch (uMsg) {
        case WM_CLOSE:  // 处理窗口关闭事件
            DestroyWindow(hwnd);
            break;
        case WM_DESTROY:  // 处理销毁窗口事件
            PostQuitMessage(0);
            break;
        case WM_PAINT:  // 处理绘制窗口事件
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hwnd, &ps);
            // 在这里绘制你的窗口内容
            EndPaint(hwnd, &ps);
            break;

        case WM_COMMAND: {
            // 检查消息是否来自按钮
            for (int i = 0; i < *Win32_Component_Num; i++) {
                int *type = (int *)(Win32_Component[i]);
                if (*type == Type_Button) {
                    Win32_Button *button = Win32_Component[i];
                    if ((HWND)lParam == button->hwnd) {
                        // 调用按钮点击事件处理函数
                        JSValue ret = JS_Call(_ctx, button->OnClick,
                                              JS_UNDEFINED, 0, NULL);
                        if (JS_IsException(ret)) {
                            JSValue e = JS_GetException(_ctx);
                            JS_FreeValue(_ctx, e);
                        }
                        JS_FreeValue(_ctx, ret);
                    }
                }
            }
            // if (LOWORD(wParam) == BUTTON_ID && HIWORD(wParam) == BN_CLICKED)
            // {
            //     // 调用按钮点击事件处理函数
            //     OnButtonClick(hwnd);
            // }
            break;
        }
        default:
            return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }
    return 0;
}

static JSValue win32_window_create_window(JSContext *ctx, JSValueConst this_val,
                                          int argc, JSValueConst *argv) {
    Win32_Window *ww = JS_GetOpaque2(ctx, this_val, win32_window_class_id);
    if (!ww) return JS_EXCEPTION;

    WNDCLASS wc = {0};
    wc.lpfnWndProc = WindowProc;
    // wc.hInstance = hInstance;
    wc.hbrBackground = CreateSolidBrush(RGB(255, 255, 255));
    wc.lpszClassName = ww->ClassName;
    RegisterClass(&wc);
    ww->hwnd = CreateWindowEx(0, ww->ClassName, ww->Text, WS_OVERLAPPEDWINDOW,
                              CW_USEDEFAULT, CW_USEDEFAULT, ww->Width,
                              ww->Height, NULL, NULL, NULL, NULL);
    if (!ww->hwnd) return JS_EXCEPTION;

    for (int i = 0; i < *Win32_Component_Num; i++) {
        int *type = (int *)(Win32_Component[i]);
        if (*type == Type_Button) {
            Win32_Button *button = Win32_Component[i];
            button->hwnd = CreateWindowEx(
                0, "BUTTON", button->Text,
                WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, button->X, button->Y,
                button->Width, button->Height, ww->hwnd, NULL, NULL, NULL);
            if (!button->hwnd) return JS_EXCEPTION;
            button->parent = ww->hwnd;
        }
    }

    ShowWindow(ww->hwnd, SW_SHOW);
    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return JS_UNDEFINED;
}

static JSValue win32_window_get_width(JSContext *ctx, JSValueConst this_val,
                                      int magic) {
    Win32_Window *ww = JS_GetOpaque2(ctx, this_val, win32_window_class_id);
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
    Win32_Window *ww = JS_GetOpaque2(ctx, this_val, win32_window_class_id);
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
    Win32_Window *ww = JS_GetOpaque2(ctx, this_val, win32_window_class_id);
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
    Win32_Window *ww = JS_GetOpaque2(ctx, this_val, win32_window_class_id);
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
    Win32_Window *ww = JS_GetOpaque2(ctx, this_val, win32_window_class_id);
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
    Win32_Window *ww = JS_GetOpaque2(ctx, this_val, win32_window_class_id);
    if (!ww) return JS_EXCEPTION;
    const char *text = JS_ToCString(ctx, val);
    SetWindowText(ww->hwnd, text);
    return JS_UNDEFINED;
}
static JSValue win32_window_get_x(JSContext *ctx, JSValueConst this_val,
                                  int magic) {
    Win32_Window *ww = JS_GetOpaque2(ctx, this_val, win32_window_class_id);
    if (!ww) return JS_EXCEPTION;
    if (magic == 0) {
        RECT rect;
        GetWindowRect(ww->hwnd, &rect);
        int x = rect.left;
        return JS_NewInt32(ctx, x);
    };
    return JS_UNDEFINED;
}
static JSValue win32_window_set_x(JSContext *ctx, JSValueConst this_val,
                                  JSValueConst val, int magic) {
    Win32_Window *ww = JS_GetOpaque2(ctx, this_val, win32_window_class_id);
    if (!ww) return JS_EXCEPTION;
    RECT rect;
    GetWindowRect(ww->hwnd, &rect);
    if (JS_ToInt32(ctx, &ww->X, val)) return JS_EXCEPTION;
    SetWindowPos(ww->hwnd, HWND_TOP, ww->X, rect.top, 0, 0,
                 SWP_NOSIZE | SWP_NOZORDER | SWP_NOACTIVATE);

    return JS_UNDEFINED;
}
static JSValue win32_window_get_y(JSContext *ctx, JSValueConst this_val,
                                  int magic) {
    Win32_Window *ww = JS_GetOpaque2(ctx, this_val, win32_window_class_id);
    if (!ww) return JS_EXCEPTION;
    if (magic == 0) {
        RECT rect;
        GetWindowRect(ww->hwnd, &rect);
        int y = rect.top;
        return JS_NewInt32(ctx, y);
    }
    return JS_UNDEFINED;
}
static JSValue win32_window_set_y(JSContext *ctx, JSValueConst this_val,
                                  JSValueConst val, int magic) {
    Win32_Window *ww = JS_GetOpaque2(ctx, this_val, win32_window_class_id);
    if (!ww) return JS_EXCEPTION;
    if (JS_ToInt32(ctx, &ww->Y, val)) return JS_EXCEPTION;
         RECT rect;
    GetWindowRect(ww->hwnd, &rect);
    SetWindowPos(ww->hwnd, HWND_TOP,rect.left, ww->Y, 0, 0,
                 SWP_NOSIZE | SWP_NOZORDER | SWP_NOACTIVATE);
    return JS_UNDEFINED;
}
static JSValue win32_window_get_HWND(JSContext *ctx, JSValueConst this_val) {
    Win32_Button *ww = JS_GetOpaque2(ctx, this_val, win32_window_class_id);
    if (!ww) return JS_EXCEPTION;
    return JS_NewInt64(ctx, (int)(int *)ww->hwnd);
}

static const JSCFunctionListEntry win32_window_proto_funcs[] = {
    // JS_CFUNC_DEF("testadd", 0, testadd),
    JS_CFUNC_DEF("CreateWindow", 0, win32_window_create_window),
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
};

static JSClassDef win32_window_class = {
    "Win32_Window",
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
    ctor = JS_NewCFunction2(ctx, win32_window_ctor, "Win32_Window", 4,
                            JS_CFUNC_constructor, 0);
    JS_SetConstructor(ctx, ctor, proto);
    JS_SetClassProto(ctx, win32_window_class_id, proto);
    JS_SetModuleExport(ctx, m, "Win32_Window", ctor);
    return 0;
}

JSModuleDef *js_init_module(JSContext *ctx, const char *module_name) {
    _ctx = ctx;
    JSModuleDef *m;
    m = JS_NewCModule(ctx, module_name, window_init);
    if (!m) return NULL;
    JS_AddModuleExport(ctx, m, "Win32_Window");

    CSharedMemory =
        CreateFileMapping(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, 4096,
                          "JSWin32SharedMemory");
    // if (CSharedMemory == NULL) {
    //     printf("CreateFileMapping failed, error code: %d\n", GetLastError());
    //     return NULL;
    // }
    Win32_Component =
        MapViewOfFile(CSharedMemory, FILE_MAP_ALL_ACCESS, 0, 0, 4096);
    CNumSharedMemory =
        CreateFileMapping(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0,
                          sizeof(int), "JSWin32NumSharedMemory");
    Win32_Component_Num = MapViewOfFile(CNumSharedMemory, FILE_MAP_ALL_ACCESS,
                                        0, 0, sizeof(volatile LONG));
    *Win32_Component_Num = 0;
    return m;
}
