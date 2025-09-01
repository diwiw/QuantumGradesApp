#include "Logger.hpp"
#include <iostream>
#include <stdexcept>
#include <ctime>
#include <filesystem>

Logger::Logger() : current_level_(LogLevel::INFO) {
	if(!std::filesystem::exists("logs"))
		std::filesystem::create_directory("logs");
	logfile_.open("logs/app.log", std::ios::app);
	if(!logfile_){
		throw std::runtime_error("[Logger] Can't open log file.");
	}
}

Logger::~Logger() {
	if(logfile_.is_open()){
		logfile_.close();
	}
}

Logger& Logger::getInstance(){
	static Logger instance;
	return instance;
}

void Logger::setLogLevel(LogLevel level){
	current_level_ = level;
}

void Logger::setLogFile(const std::string& filename) {
	if (logfile_.is_open()) logfile_.close();
	logfile_.open(filename, std::ios::app);
	if(!logfile_.is_open()) {
		throw std::runtime_error("[Logger] Cannot open log file: " + filename);
	}
}

void Logger::log(LogLevel level, const std::string& message) {
	if ( level < current_level_) return;

	std::string level_str;
	switch(level){
		case LogLevel::DEBUG:	level_str = "[DEBUG]"; break;
		case LogLevel::INFO:	level_str = "[INFO]"; break;
		case LogLevel::WARNING: level_str = "[WARNING]"; break;
		case LogLevel::ERROR:	level_str = "[ERROR]"; break;
		case LogLevel::CRITICAL: level_str = "[CRITICAL]"; break;
		default: level_str = "[UNKNOWN]"; break;
	}

	std::time_t now = std::time(nullptr);
	char time_buf[100];
	std::strftime(time_buf, sizeof(time_buf), "[%Y-%m-%d %H:%M:%S]", std::localtime(&now));

	logfile_ << time_buf << " " << level_str << " " << message << std::endl;
}
