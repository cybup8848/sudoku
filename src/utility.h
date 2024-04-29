#pragma once
#ifndef _SUDOKU_UTILITY_H_
#define _SUDOKU_UTILITY_H_
#include<iostream>
#include<string>
#include<cstring>

//重载函数，可以对c语言字符串、cpp字符串
inline void message(const char* str, bool lineBreaks = true) {
	std::cout << str;
	if (lineBreaks) {
		std::cout << std::endl;
	}
}

inline void message(const std::string& str, bool lineBreaks = true) {
	message(str.c_str(),lineBreaks);
}

inline char* specialCTime() {
    time_t timer = time(nullptr);
    char* t = ctime(&timer);
    t[strlen(t) - 1] = '\0';
    return t;
}

#ifdef _WIN32
#include<conio.h>
#else
	#ifdef __linux__
	#include<termio.h>
	#include<stdio.h>
	#elif __APPLE__
	#include<termios.h>
	#endif
inline char getch() {
    struct termios tmtemp, tm;
    char c;
    int fd = 0;
    if (tcgetattr(fd, &tm) != 0)
    { /*获取当前的终端属性设置，并保存到tm结构体中*/
        return -1;
    }
    tmtemp = tm;
    cfmakeraw(&tmtemp); /*将tetemp初始化为终端原始模式的属性设置*/
    //tmtemp.c_lflag &= ~( ICANON | ECHO );
    if (tcsetattr(fd, TCSANOW, &tmtemp) != 0)
    { /*将终端设置为原始模式的设置*/
        return -1;
    }
    c = getchar();
    if (tcsetattr(fd, TCSANOW, &tm) != 0)
    { /*接收字符完毕后将终端设置回原来的属性*/
        return 0;
    }
    return c;
}

#endif

inline void cls() {
#ifdef _WIN32
	system("cls");
#elif defined(__linux__) || defined(__unix__) || defined(__APPLE__) || defined(__ANDROID__)
	system("clear");
#else
	system("clear");
#endif
}
#endif
