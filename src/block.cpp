#include"block.h"
CBlock::CBlock() {
	_count = 0;
}

bool CBlock::isValid() const {
	assert(_count<=MAX_COUNT);
	std::unordered_set<int> s;
	for (point_value_t *x : _points) {
		if (UNSELECTED == x->val) {
			continue;
		}
		if (!s.insert(x->val).second) {
			return false;
		}
	}
	return true;
}

bool CBlock::isFull() const {
	for (point_value_t* x : _points) {
		if (x == nullptr || UNSELECTED == x->val) {
			return false;
		}
	}
	return true;
}

void CBlock::print() const {//按行打印
	for (point_value_t* p : _points) {
		std::cout << PIPE << ' ';
		if (UNSELECTED == p->val) {
			std::cout << ' ';
		}
		else if (State::ERASED == p->state) {//对擦除的进行特殊标记
			std::cout << Modifier(ColorCode::FG_GREEN) << p->val << Modifier(ColorCode::RESET);
		}
		else {
			std::cout << p->val;
		}
		std::cout << ' ';
	}
	std::cout << PIPE << std::endl;
}

void CBlock::pushBack(point_value_t* point) {
	assert(nullptr != point);
	assert(_count <= MAX_COUNT);

	/*
	if (nullptr == point) {
		return;
	}
	if (_count > MAX_COUNT) {
		return;
	}
	*/

	_points[_count++] = point;
}







