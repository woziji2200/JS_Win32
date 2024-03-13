gcc .\Clib\win32.c -shared -DJS_SHARED_LIBRARY -o .\JSlib\win32.so -L.\lib\quickjs -I.\include\quickjs -lquickjs -lgdi32 -fexec-charset=utf-8 -finput-charset=utf-8

gcc .\Clib\win32_button.c -shared -DJS_SHARED_LIBRARY -o .\JSlib\win32_button.so -L.\lib\quickjs -I.\include\quickjs -lquickjs -lgdi32  -fexec-charset=utf-8 -finput-charset=utf-8

gcc .\Clib\win32_text.c -shared -DJS_SHARED_LIBRARY -o .\JSlib\win32_text.so -L.\lib\quickjs -I.\include\quickjs -lquickjs -lgdi32  -fexec-charset=utf-8 -finput-charset=utf-8

gcc .\Clib\win32_core.c -shared -DJS_SHARED_LIBRARY -o .\JSlib\win32_core.so -L.\lib\quickjs -I.\include\quickjs -lquickjs -lgdi32  -fexec-charset=utf-8 -finput-charset=utf-8
