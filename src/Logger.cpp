#include "Logger.h"
#include <iostream>
#include <stdexcept>
#include <ctime>

Logger::Logger() : currentLevel(LogLevel::INFO) {
	logfile.open("logs/app.log", std::ios::app);
	if(!logfile){
		throw std::runtime_error("Can't open log file.");
	}
}

Logger::~Logger() {
	if(logfile.is_open()){
		logfile.close();
	}
}

Logger& Logger::getInstance(){
	static Logger instance;
	return instance;
}

void Logger::setLogLevel(LogLevel level){
	currentLevel = level;
}

void Logger::log(LogLevel level, const std::string& message) {
	if ( level < currentLevel) return;

	std::string levelStr;
	switch(level){
		case LogLevel::DEBUG:	levelStr = "[DEBUG]"; break;
		case LogLevel::INFO:	levelStr = "[INFO]"; break;
		case LogLevel::WARNING: levelStr = "[WARNING]"; break;
		case LogLevel::ERROR:	levelStr = "[ERROR]"; break;
		case LogLevel::CRITICAL: levelStr = "[CRITICAL]"; break;
		default: levelStr = "[UNKNOWN]"; break;
	}

	std::time_t now = std::time(nullptr);
	char timeBuf[100];
	std::strftime(timeBuf, sizeof(timeBuf), "%Y-%m-%d %H:%M:%S", std::localtime(&now));

	logfile << timeBuf << " " << levelStr << " " << message << std::endl;
}
