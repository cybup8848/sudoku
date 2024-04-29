#pragma once
#ifndef _SUDOKU_DISPLAY_SYMBOL_H_
#define _SUDOKU_DISPLAY_SYMBOL_H_
constexpr char CORNER[] = "\u254B";//制表符粗竖和水平 ╋
constexpr char LINE[] = "\u2501";//制表符粗横线 ━
constexpr char PIPE[] = "\u2503";//制表符粗竖 ┃
constexpr char ARROW[] = "^";

//编译器会将constexpr函数视为内联函数！所以在编译时若能求出其值，
//则会把函数调用替换成结果值。

#endif
