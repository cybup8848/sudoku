#pragma once
#ifndef _SUDOKU_BLOCK_H_
#define _SUDOKU_BLOCK_H_
#include"common.h"
#include"DisplaySymbol.h"
#include"color.h"

#include<cassert>
#include<iostream>
#include<unordered_set>
#include<array>

class CBlock {
public:
	CBlock();
	bool isValid() const;
	bool isFull() const;
	void print() const;
	void pushBack(point_value_t* point);//≥ı ºªØ—π»Î

private:
	static const int MAX_COUNT = 9;

private:
	int _count;
	std::array<point_value_t*, MAX_COUNT>_points;
};
#endif
