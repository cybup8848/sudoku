#include"log.h"
#include<iostream>
Log* Log::_uniqueLog = new Log();

Log* Log::getSingleInstance() {
	return _uniqueLog;
}

bool Log::init(const std::string& fileName, const int maxLogLines,const int maxBuffSize) {
	_maxLogLines = maxLogLines;

	std::size_t pos = fileName.find("/");
	if (std::string::npos == pos) {
		_logName = fileName;
	}
	else {
		_dirName = fileName.substr(0,pos+1);
		_logName = fileName.substr(pos+1);
	}

	_maxLogLines = maxLogLines;
	_countLines = 0;

	time_t timer = time(nullptr);
	struct tm* ptimer = localtime(&timer);
	_today = ptimer->tm_mday;

	//Òì²½ÈÕÖ¾
	/*
	std::thread th(&Log::asynWriteLog,this);
	th.detach();
	*/
	_th = std::thread(&Log::asynWriteLog,this);

	std::string fullLogName = _dirName +
		std::to_string(ptimer->tm_year+1900) + "_" + std::to_string(ptimer->tm_mon + 1) + "_" + std::to_string(_today) + "_" + _logName;
	
	_ofs.open(fullLogName, std::ios::app);

	
	std::string tmp = "";
	std::ifstream ifs(fullLogName,std::ios::in);
	if (ifs.is_open()) {
		while (getline(ifs, tmp)) {
			++_countLines;
		}
		ifs.close();
	}
	return _ofs.is_open();
}

void Log::flush() {
	std::lock_guard<std::mutex> lock(_mtx);
	_ofs.flush();
}

void Log::writeLog(const char* format, ...) {
	time_t timer = time(nullptr);
	struct tm* ptimer = localtime(&timer);

	{
		std::lock_guard<std::mutex> lcok(_mtx);

		if (ptimer->tm_mday != _today || _countLines != 0 && _countLines % _maxLogLines == 0) {
			_ofs.flush();
			_ofs.close();

			if (ptimer->tm_mday != _today) {
				_today = ptimer->tm_mday;
				std::string fullLogName = _dirName +
					std::to_string(ptimer->tm_year + 1900) + "_" + std::to_string(ptimer->tm_mon + 1) + "_" + std::to_string(_today) + "_" +
					_logName;
				_countLines = 0;
				_ofs.open(fullLogName, std::ios::app);
			}
			else {
					std::string fullLogName = _dirName +
						std::to_string(ptimer->tm_year + 1900) + "_" + std::to_string(ptimer->tm_mon + 1) + "_" + std::to_string(_today) + "_" +
						_logName + std::to_string(_countLines / _maxLogLines);
					_ofs.open(fullLogName, std::ios::app);
			}
		}
	}

	char buff[2048] = { 0 };
	va_list args;
	va_start(args, format);
	vsprintf(buff,format,args);
	va_end(args);
	

	std::string logInfo(buff);
	{
		std::unique_lock<std::mutex>lock(_mtx);
    if(_infoQueue.size()<_maxBuffSize){
      _infoQueue.push(std::move(logInfo));
    }else{
      _ofs<<logInfo<<std::endl;
    }
		++_countLines;
  }

	_condition.notify_all();
}

void Log::asynWriteLog() {
	for (;;) {
		std::unique_lock<std::mutex>lock(_mtx);
		_condition.wait(lock, [this] {return !this->_infoQueue.empty();});
		std::string logInfo = std::move(_infoQueue.front());
		this->_infoQueue.pop();
		_ofs << logInfo << std::endl;
		
		_condition.notify_all();
	}
}





