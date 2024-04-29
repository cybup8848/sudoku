#include"scene.h"
#include"command.h"
CCommand::CCommand(CScene* pOwner) {
	_pOwner = pOwner;
	_nCurValue = 0;
}

CCommand::CCommand(CScene* pOwner, const point_t& point, int preValue, int curValue) {
	_pOwner = pOwner;
	_stPoint = point;
	_nPreValue = preValue;
	_nCurValue = curValue;
}

CCommand::CCommand(const CCommand& rhs) {
	_pOwner = rhs._pOwner;
	_stPoint = rhs._stPoint;
	_nPreValue = rhs._nPreValue;
	_nCurValue = rhs._nCurValue;
}

CCommand::~CCommand() {}

bool CCommand::execute(int nInputValue) {
	if (!_pOwner) {
		return false;
	}
	_stPoint = _pOwner->getCurPoint();//获取当前点所在位置
	return _pOwner->setCurValue(nInputValue,_nPreValue);//当时点的前一个值
}

bool CCommand::undo() {
	if (_pOwner) {
		return _pOwner->setPointValue(_stPoint,_nPreValue);//之前的值
	}
	return false;
}


