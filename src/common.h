#pragma once
#ifndef _SUDOKU_COMMON_H_
#define _SUDOKU_COMMON_H_
#include"utility.h"

static const char* RECORD_SEG_FLAG = "#########################################";

static const int UNSELECTED = 0;

enum class Difficulty :int {
    SIMPLE = 1,
    NORMAL,
    HARD,
};

enum class State :int {
    INITED = 0,
    ERASED
};


enum class KeyMode :int {
    NORMAL = 1,
    VIM,
    ARROW
};

struct KeyMap {
    const char Q = 0x71;//退出，113
    const char U = 0x75;//撤销，117
    char UP;
    char DOWN;
    char LEFT;
    char RIGHT;
    const char ENTER = 0x0D;//继续，13

    virtual int getchar() {
        int ch = getch();
        return ch;
    }
};

struct Normal :KeyMap {//w、a、s、d
    Normal() {
        UP = 0x77;//w、119
        DOWN = 0x73;//s、115
        LEFT = 0x61;//a、97
        RIGHT = 0x64;//d、100
    }
};

struct Vim :KeyMap {
    Vim() {
        UP = 0x6B;//k、107
        DOWN = 0x6A;//j、106
        LEFT = 0x68;//h、104
        RIGHT = 0x6C;//l、108
    }
};

#ifdef _WIN32
struct Arrow :KeyMap {//getch()����һ�η���224���ڶ��η�����ʵ�ļ���ֵ
    Arrow() {
        UP = 0x48;//72
        DOWN = 0x50;//80
        LEFT = 0x4B;//75
        RIGHT = 0x4D;//77
    }

    int getchar() {
        int ch = getch();
        if (ch == 224) {
            ch = getch();
        }
        return ch;
    }
};
#else
struct Arrow :KeyMap {//linux按下方向键，返回27 91 65~68,对应上下右左
    Arrow() {
        UP = 0x41;//65
        DOWN = 0x42;//66
        LEFT = 0x44;//68
        RIGHT = 0x43;//67
        std::cout<<"Arrow"<<std::endl;
    }

    int getchar() {
        int ch = getch();
        if (ch == 27) {
            ch = getch();
            if(ch==91){
              ch = getch();
            }
        }
        std::cout<<std::endl;
        return ch;
	}
};
#endif

using point_t = struct point_t {
    int x;
    int y;
};

using point_value_t = struct point_value_t {
    int val;
    State state;
};





#endif
