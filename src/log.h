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
	//�����캯������Ϊ˽�г�Ա
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
	
	int _today;//�����¼��־

	std::string _dirName;//��־�ļ�����
	std::string _logName;//log�ļ���
	int _maxLogLines;//ÿ���ļ�����־�������
	int _countLines;//��¼Ŀǰ�ļ�����־����

	std::ofstream _ofs;
	std::queue<std::string> _infoQueue;//��־�������������첽��־
	int _maxBuffSize;//��󻺳�����С

private:
	static Log* _uniqueLog;
};

#define LOG_INFO(format,...) {Log::getSingleInstance()->writeLog(format,##__VA_ARGS__);Log::getSingleInstance()->flush();}
#endif