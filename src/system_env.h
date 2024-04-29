#pragma once
#ifndef _SYSTEM_ENV_H_
#define _SYSTEM_ENV_H_

#if _WIN32
#include <Windows.h>
#endif

inline void SetWindowsEnv() {
#if _WIN32
	SetConsoleOutputCP(CP_UTF8);
	//API函数SetConsoleOutputCP用于设置控制台程序输出代码页。
	//代码页是字符集编码的别名，也有人称"内码表"。
	//早期，代码页是IBM称呼电脑BIOS本身支持的字符集编码的名称。
#endif
}

inline void SetSystemEnv() { SetWindowsEnv(); }

#endif