import { JS_Win32_Window, JS_Win32_Button, JS_Win32_Core, JS_Win32_Text } from "./JSlib/win32.js";

function qwq() {
    const http = new XMLHttpRequest();
    console.log(http);
}

function qwq2() {
    b2.Text = (Number(b2.Text) + 1).toString();
    t1.Text = b2.Text;
}
function qwq3() {
    console.log(JS_Win32_Core.MessageBox({
        Text: "你好啊喵",
        Title: "喵喵喵",
        Type: JS_Win32_Core.MessageBoxType.MB_CANCELTRYCONTINUE + JS_Win32_Core.MessageBoxType.MB_ICONWARNING
    }))
    window2.CreateWindow();
}

function close() {
    return JS_Win32_Core.MessageBox({
        Text: "确定关闭窗口？",
        Title: "关闭窗口",
        Type: JS_Win32_Core.MessageBoxType.MB_YESNO + JS_Win32_Core.MessageBoxType.MB_ICONQUESTION
    }) == JS_Win32_Core.MessageBoxResult.YES;
}
function close111() {
    return JS_Win32_Core.MessageBox({
        Text: "确定关闭窗口111？",
        Title: "关闭窗口",
        Type: JS_Win32_Core.MessageBoxType.MB_YESNO + JS_Win32_Core.MessageBoxType.MB_ICONQUESTION
    }) == JS_Win32_Core.MessageBoxResult.YES;
}
function destory(){
    console.log("销毁窗口");
}
function create() {
    console.log("创建窗口");
    return 1;
}
function create111() {
    console.log("创建窗口111");
    return 1;
}

const window = new JS_Win32_Window({
    Width: 800,
    Height: 600,
    Text: "win32测试窗口喵",
    ID: 'qwq1',
    OnClose: close,
    OnDestory: destory,
    OnCreate: create
});
const window2 = new JS_Win32_Window({
    Width:300,
    Height: 300,
    Text: "win32测试窗口喵",
    ID: 'qwq2',
    OnClose: close111,
    OnDestory: destory,
    OnCreate: create111
});
const b1 = new JS_Win32_Button({
    X: 140,
    Y: 140,
    Width: 100,
    Height: 50,
    Text: "b1",
    OnClick: qwq,
    ParentID: 'qwq1'
});
const b2 = new JS_Win32_Button({
    X: 100,
    Y: 10,
    Width: 100,
    Height: 50,
    Text: "0",
    OnClick: qwq2,
    ParentID: 'qwq1'
});
const b3 = new JS_Win32_Button({
    X: 100,
    Y: 100,
    Width: 100,
    Height: 50,
    Text: "b3",
    OnClick: qwq3,
    ParentID: 'qwq2'
});
const b4 = new JS_Win32_Button({
    X: 100,
    Y: 100,
    Width: 100,
    Height: 50,
    Text: "b3",
    OnClick: qwq3,
    ParentID: 'qwq1'
});
const t1 = new JS_Win32_Text({
    X: 0,
    Y: 0,
    Width: 60,
    Height: 50,
    Text: "哈哈哈哈哈",
    ParentID: 'qwq2',
    OnClick: qwq
});
window.CreateWindow();