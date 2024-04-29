#pragma once
#ifndef _SUDOKU_LOG_H_
#define _SUDOKU_LOG_H_
#include<ctime>
#include<cstdarg>
#include<fstream>
#include<queue>
#include<string>
#include<condition_variable>
#include<mutex>
#include<thread>

class Log {
public:

	static Log* getSingleInstance();

	bool init(const std::string &logName,const int maxLogLines=500000,const int maxBuffSize=1000);

	void flush();

	void writeLog(const char* format, ...);

private:
	//将构造函数声明为私有成员
	Log() {}

	~Log() {
		if (_ofs.is_open()){
			_ofs.flush();
			_ofs.close();
		}
	}

	void asynWriteLog();

private:

	std::condition_variable _condition;
	std::mutex _mtx;

	std::thread _th;
	
	int _today;//按天记录日志

	std::string _dirName;//日志文件夹名
	std::string _logName;//log文件名
	int _maxLogLines;//每个文件的日志最大行数
	int _countLines;//记录目前文件的日志行数

	std::ofstream _ofs;
	std::queue<std::string> _infoQueue;//日志缓冲区，采用异步日志
	int _maxBuffSize;//最大缓冲区大小

private:
	static Log* _uniqueLog;
};

#define LOG_INFO(format,...) {Log::getSingleInstance()->writeLog(format,##__VA_ARGS__);Log::getSingleInstance()->flush();}
#endif