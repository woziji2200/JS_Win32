import { JS_Win32_Window, JS_Win32_Button } from "./win32.js";
import { require } from "./require.js";
import { test } from '../main.js'
import * as std from 'std'
export class JS_Win32_Parser {
    constructor(filename) {
        let file = std.open(filename, "r");
        let str = file.readAsString();
        console.log(str);
    }
}