import { JS_Win32_Window, JS_Win32_Button, JS_Win32_Core, JS_Win32_Text } from "./JSlib/win32.js";

function qwq() {
    console.log(111);
    console.log(JS_Win32_Core.MessageBox({
        Text: "你好啊喵",
        Title: "喵喵喵",
        Parent: window.GetHWND(),
        Type: JS_Win32_Core.MessageBoxType.MB_CANCELTRYCONTINUE + JS_Win32_Core.MessageBoxType.MB_ICONWARNING
    }))
}

function qwq2() {
    b2.Text = (Number(b2.Text) + 1).toString();
    t1.Text = b2.Text;
}
function qwq3() {
    window2.CreateWindow();
}

const window = new JS_Win32_Window({
    Width: 800,
    Height: 600,
    Text: "win32测试窗口喵",
    ID: 1
});
const window2 = new JS_Win32_Window({
    Width: 800,
    Height: 600,
    Text: "win32测试窗口喵",
    ID: 2
});
const b1 = new JS_Win32_Button({
    X: 140,
    Y: 140,
    Width: 100,
    Height: 50,
    Text: "b1",
    OnClick: qwq,
    ParentID: 1
});
const b2 = new JS_Win32_Button({
    X: 100,
    Y: 10,
    Width: 100,
    Height: 50,
    Text: "0",
    OnClick: qwq2,
    ParentID: 1
});
const b3 = new JS_Win32_Button({
    X: 100,
    Y: 100,
    Width: 100,
    Height: 50,
    Text: "b3",
    OnClick: qwq3,
    ParentID: 1
});
const b4 = new JS_Win32_Button({
    X: 100,
    Y: 100,
    Width: 100,
    Height: 50,
    Text: "b3",
    OnClick: qwq3,
    ParentID: 2
});
const t1 = new JS_Win32_Text({
    X: 0,
    Y: 0,
    Width: 60,
    Height: 50,
    Text: "哈哈哈哈哈",
    ParentID: 1,
    OnClick: qwq
});
window.CreateWindow();