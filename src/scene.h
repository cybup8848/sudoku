#pragma once
#ifndef _SUDOKU_SCENE_H_
#define _SUDOKU_SCENE_H_
#include"common.h"
#include"block.h"
#include"DisplaySymbol.h"
#include"utility.h"
#include"command.h"
#include"i18n.h"

#include<cstring>
#include<cmath>
#include<ctime>
#include<cassert>
#include<array>
#include<vector>
#include<fstream>
//#include<filesystem>
#include<sstream>
#include<array>
#include<vector>

class CScene {//���鳡����
public:
	CScene(const std::string logFile, int index = 3);
	virtual ~CScene();

	void generate();
	void show() const;

	bool setCurValue(const int curValue,int &lastValue);
	bool setPointValue(const point_t& p, const int v);
	point_t getCurPoint();

    void eraseRandomGrids(const int count);
    bool isComplete();

    void play();
    bool save(const char* filename);
    bool load(const char* filename);

    void setMode(KeyMode mode);

private:
    void init(); // ��ÿ�����ӵ�ָ��ŵ�block����
    void setValue(const int);
    void setValue(const point_t&, const int);
    void printUnderline(int line_no = -1) const;

private:
    KeyMap* _pKeyMap{};
    int _maxColumn;
    point_t _curPoint;
    
	std::array<CBlock, 9>_columnBlock;
	std::array<CBlock, 9>_rowBlock;
	std::array<std::array<CBlock, 3>, 3>_xyBlock;
	std::array<point_value_t, 81>_map;

	std::vector<CCommand> _commands;

};





#endif
