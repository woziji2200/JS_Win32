import { JS_Win32_Window, JS_Win32_Button } from "./JSlib/win32.js";

function qwq() {
    console.log(b1.X, b1.Y, b1.GetHWND());
    // b1.X = 10
    // b1.Y += 10
    // b1.Height = 100
    b1.Width = 300
    b1.Text = "qwq"
    
}
function qwq2() {
    window.X = 100
}
function qwq3() {
    console.log(window.X, window.Y, window.Text);
    window.Y= 100
}

const window = new JS_Win32_Window({
    Width: 800,
    Height: 600,
    Text: "win32测试窗口喵",
});
const b1 = new JS_Win32_Button({
    X: 140,
    Y: 140,
    Width: 100,
    Height: 50,
    Text: "b1",
    OnClick: qwq,
});
const b2 = new JS_Win32_Button({
    X: 100,
    Y: 10,
    Width: 100,
    Height: 50,
    Text: "b2",
    OnClick: qwq2,
});
const b3 = new JS_Win32_Button({
    X: 100,
    Y: 100,
    Width: 100,
    Height: 50,
    Text: "b3",
    OnClick: qwq3,
});
window.CreateWindow();