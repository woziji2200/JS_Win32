import { Win32_Window } from './win32.so'
import { Win32_Button } from './win32_button.so'
import { Win32_Core } from './win32_core.so'
import { Win32_Text } from './win32_text.so'

const EMPTY_FUNCTION = function () { };

export class JS_Win32_Window {
    /**
     * @typedef {Object} window_params 窗口参数
     * @property {number} [Width] 窗口宽度
     * @property {number} [Height] 窗口高度
     * @property {string} [Text] 窗口标题
     * @property {number} [ClassName] 窗口类名
     * @property {number} [ID] 窗口ID
     */

    /**
     * 创建一个新的窗口对象
     * @param {window_params} params 窗口参数
     */
    constructor(params) {
        this.window = new Win32_Window(
            params.Width || 800, // 窗口宽度
            params.Height || 600,// 窗口高度
            params.Text || "Window", // 窗口标题
            params.ClassName || "WindowClass", // 窗口类名
            params.ID || 0 // 窗口ID
        );
    }
    /**
     * 开始创建窗口，应该在所有组件创建完成后调用
     */
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
    /**
     * @typedef {Object} button_params 按钮参数
     * @property {number} [X] X坐标，即left
     * @property {number} [Y] Y坐标，即top
     * @property {number} [Width] 宽度
     * @property {number} [Height] 高度
     * @property {string} [Text] 按钮文本
     * @property {function} [OnClick] 点击事件，提供回调函数
     * @property {string} [ClassName] 窗口类名
     * @property {number} [ID] 按钮ID，目前无用
     * @property {number} [ParentID] 父窗口ID
     */
    /**
     * 创建一个新的按钮对象
     * @param {button_params} params 按钮参数
     */
    constructor(params) {
        this.window = new Win32_Button({
            X: params.X || 0, // X
            Y: params.Y || 0, // Y
            Width: params.Width || 100, // Width
            Height: params.Height || 30, // Height
            Text: params.Text || "Button", // Text
            OnClick: params.OnClick || EMPTY_FUNCTION, // OnClick
            ClassName: params.ClassName || "BUTTON", // ClassName
            ID: params.ID || 0,
            ParentID: params.ParentID || 0,
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


export class JS_Win32_Text {
    /**
     * @typedef {Object} Text_params 文本参数
     * @property {number} [X] X坐标，即left
     * @property {number} [Y] Y坐标，即top
     * @property {number} [Width] 宽度
     * @property {number} [Height] 高度
     * @property {string} [Text] 文本
     * @property {function} [OnClick] 点击事件，提供回调函数
     * @property {string} [ClassName] 窗口类名
     * @property {number} [ID] 按钮ID，目前无用
     * @property {number} [ParentID] 父窗口ID
     */
    /**
     * 创建一个新的文本框对象
     * @param {Text_params} params 按钮参数
     */
    constructor(params) {
        this.window = new Win32_Text({
            X: params.X || 0, // X
            Y: params.Y || 0, // Y
            Width: params.Width || 100, // Width
            Height: params.Height || 30, // Height
            Text: params.Text || "Text", // Text
            OnClick: params.OnClick || EMPTY_FUNCTION, // OnClick
            ClassName: params.ClassName || "STATIC", // ClassName
            ID: params.ID || 0,
            ParentID: params.ParentID || 0,
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

const core = new Win32_Core();
export const JS_Win32_Core = {
    MessageBox(params) {
        return core.MessageBox({
            Text: params.Text || "MessageBox",
            Title: params.Title || "MessageBox",
            Flag: params.Type || 0,
            Parent: params.Parent || 0,
        });
    },

    /**
     * @typedef {Object} MessageBoxType
     * @property {number} MB_OK 按钮：将会包含确定按钮
     * @property {number} MB_OKCANCEL 按钮：将会包含确定和取消按钮
     * @property {number} MB_ABORTRETRYIGNORE 按钮：将会包含中止、重试和忽略按钮
     * @property {number} MB_YESNOCANCEL 按钮：将会包含是、否和取消按钮
     * @property {number} MB_YESNO 按钮：将会包含是和否按钮
     * @property {number} MB_RETRYCANCEL 按钮：将会包含重试和取消按钮
     * @property {number} MB_CANCELTRYCONTINUE 按钮：将会包含取消、重试和继续按钮
     * @property {number} MB_ICONHAND 图标：将会包含停止图标
     * @property {number} MB_ICONQUESTION 图标：将会包含问号图标
     * @property {number} MB_ICONEXCLAMATION 图标：将会包含感叹号图标
     * @property {number} MB_ICONASTERISK 图标：将会包含信息图标
     * @property {number} MB_USERICON 图标：将会包含用户自定义图标
     * @property {number} MB_ICONWARNING 图标：将会包含感叹号图标
     * @property {number} MB_ICONERROR 图标：将会包含停止图标
     * @property {number} MB_ICONINFORMATION 图标：将会包含信息图标
     * @property {number} MB_ICONSTOP 图标：将会包含停止图标
     * @property {number} MB_ICONQUESTION 图标：将会包含问号图标
     * @property {number} MB_ICONEXCLAMATION 图标：将会包含感叹号图标
     * @property {number} MB_ICONASTERISK 图标：将会包含信息图标
     * @property {number} MB_DEFBUTTON1 默认按钮：第一个按钮是默认按钮
     * @property {number} MB_DEFBUTTON2 默认按钮：第二个按钮是默认按钮
     * @property {number} MB_DEFBUTTON3 默认按钮：第三个按钮是默认按钮
     * @property {number} MB_DEFBUTTON4 默认按钮：第四个按钮是默认按钮
     * @property {number} MB_APPLMODAL 模态：应用程序模态
     * @property {number} MB_SYSTEMMODAL 模态：系统模态
     * @property {number} MB_TASKMODAL 模态：任务模态
     * @property {number} MB_HELP 模态：显示帮助按钮
     * @property {number} MB_NOFOCUS 模态：不获取焦点
     * @property {number} MB_SETFOREGROUND 模态：将会设置窗口为前台窗口
     * @property {number} MB_DEFAULT_DESKTOP_ONLY 模态：只在默认桌面上显示
     * @property {number} MB_TOPMOST 模态：将会置顶
     * @property {number} MB_RIGHT 模态：将会右对齐
     * @property {number} MB_RTLREADING 模态：将会从右到左读取
     * @property {number} MB_SERVICE_NOTIFICATION 模态：将会显示服务通知
     */
    /**
    * 消息框类型
    * @type {MessageBoxType}
    */
    MessageBoxType: {
        MB_OK: 0,
        MB_OKCANCEL: 1,
        MB_ABORTRETRYIGNORE: 2,
        MB_YESNOCANCEL: 3,
        MB_YESNO: 4,
        MB_RETRYCANCEL: 5,
        MB_CANCELTRYCONTINUE: 6,
        MB_ICONHAND: 16,
        MB_ICONQUESTION: 32,
        MB_ICONEXCLAMATION: 48,
        MB_ICONASTERISK: 64,
        MB_USERICON: 128,
        MB_ICONWARNING: 48,
        MB_ICONERROR: 16,
        MB_ICONINFORMATION: 64,
        MB_ICONSTOP: 16,
        MB_ICONQUESTION: 32,
        MB_ICONEXCLAMATION: 48,
        MB_ICONASTERISK: 64,
        MB_DEFBUTTON1: 0,
        MB_DEFBUTTON2: 256,
        MB_DEFBUTTON3: 512,
        MB_DEFBUTTON4: 768,
        MB_APPLMODAL: 0,
        MB_SYSTEMMODAL: 4096,
        MB_TASKMODAL: 8192,
        MB_HELP: 16384,
        MB_NOFOCUS: 32768,
        MB_SETFOREGROUND: 65536,
        MB_DEFAULT_DESKTOP_ONLY: 131072,
        MB_TOPMOST: 262144,
        MB_RIGHT: 524288,
        MB_RTLREADING: 1048576,
        MB_SERVICE_NOTIFICATION: 2097152
    }

}
