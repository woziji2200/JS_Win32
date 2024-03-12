#ifndef win32_type_h
#define win32_type_h
#include <windows.h>
#include "../quickjs.h"
#define Type_Window 0
#define Type_Button 1
typedef struct Win32_Window{
    int type;
    HWND hwnd;              // 窗口句柄
    int Width;              // 窗口宽度
    int Height;             // 窗口高度
    const char *Text;      // 窗口标题
    const char *ClassName;  // 窗口类名
    int X;                  // 窗口X坐标 
    int Y;                  // 窗口Y坐标
} Win32_Window;

typedef struct Win32_Button{
    int type;
    HWND hwnd;
    HWND parent;
    int Width;
    int Height;
    const char *Text;
    const char *ClassName;
    int X;
    int Y;
    JSValue OnClick;
} Win32_Button;

// struct Win32_Component{
//     void** component;
//     int size;
// } Win32_Component;

// void Win32_Component_Push(void* component){
//     Win32_Component.component = realloc(Win32_Component.component, sizeof(void*) * (Win32_Component.size + 1));
//     Win32_Component.component[Win32_Component.size] = component;
//     Win32_Component.size++;
// }
#endif /* EXAMPLE_H */
