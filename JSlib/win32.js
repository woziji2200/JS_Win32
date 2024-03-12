import { Win32_Window } from './win32.so'
import { Win32_Button } from './win32_button.so'
import { MessageBox } from './win32_core.so'

const EMPTY_FUNCTION = function () { };
/**
 * @typedef {Object} params 窗口参数
 * @property {number} [Width] 窗口宽度
 * @property {number} [Height] 窗口高度
 * @property {string} [Title] 窗口标题
 * @property {number} [ClassName] 窗口类名
 */

export class JS_Win32_Window {
    /**
     * 创建一个新的窗口对象
     * @param {params} params 窗口参数
     */
    constructor(params) {
        this.window = new Win32_Window(
            params.Width || 800, // 窗口宽度
            params.Height || 600,// 窗口高度
            params.Text || "Window", // 窗口标题
            params.ClassName || "WindowClass" // 窗口类名
        );
    }
    CreateWindow() {
        this.window.CreateWindow();
    }
    GetHWND() { return this.window.GetHWND(); }
    get Width() { return this.window.Width; }
    set Width(value) { this.window.Width = value; }
    get Height() { return this.window.Height; }
    set Height(value) { this.window.Height = value; }
    get Text() { return this.window.Text; }
    set Text(value) { this.window.Text = value; }
    get X() { return this.window.X; }
    set X(value) { this.window.X = value; }
    get Y() { return this.window.Y; }
    set Y(value) { this.window.Y = value; }

}


export class JS_Win32_Button {
    constructor(params) {
        this.window = new Win32_Button({
            X: params.X, // X
            Y: params.Y, // Y
            Width: params.Width, // Width
            Height: params.Height, // Height
            Text: params.Text || "Button", // Text
            OnClick: params.OnClick || EMPTY_FUNCTION, // OnClick
            ClassName: params.ClassName || "BUTTON" // ClassName
        });
        
    }
    GetHWND() { return this.window.GetHWND(); }
    get Width() { return this.window.Width; }
    set Width(value) { this.window.Width = value; }
    get Height() { return this.window.Height; }
    set Height(value) { this.window.Height = value; }
    get Text() { return this.window.Text; }
    set Text(value) { this.window.Text = value; }
    get X() { return this.window.X; }
    set X(value) { this.window.X = value; }
    get Y() { return this.window.Y; }
    set Y(value) { this.window.Y = value; }
}

export function JS_Win32_MessageBox(params) {
    return MessageBox({
        Text: params.Text || "MessageBox",
        Title: params.Title || "MessageBox",
        Flag: params.Type || 0,
        Parent: params.Parent || 0,
    });
}