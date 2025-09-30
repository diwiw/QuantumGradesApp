#include "utils/Logger.hpp"
#include <iostream>
#include <stdexcept>
#include <ctime>
#include <filesystem>

namespace qga::utils {

Logger::Logger() : current_level_(qga::LogLevel::Info) {
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

void Logger::setLogLevel(qga::LogLevel level){
	current_level_ = level;
}

void Logger::setLogFile(const std::string& filename) {
	if (logfile_.is_open()) logfile_.close();
	logfile_.open(filename, std::ios::app);
	if(!logfile_.is_open()) {
		throw std::runtime_error("[Logger] Cannot open log file: " + filename);
	}
}

void Logger::log(qga::LogLevel level, const std::string& message) {
	if ( level < current_level_) return;

	std::string level_str;
	switch(level){
		case qga::LogLevel::Debug:	level_str = "[DEBUG]"; break;
		case qga::LogLevel::Info:	level_str = "[INFO]"; break;
		case qga::LogLevel::Warn: level_str = "[WARNING]"; break;
		case qga::LogLevel::Err:	level_str = "[ERROR]"; break;
		case qga::LogLevel::Critical: level_str = "[CRITICAL]"; break;
		default: level_str = "[UNKNOWN]"; break;
	}

	std::time_t now = std::time(nullptr);
	char time_buf[100];
	std::strftime(time_buf, sizeof(time_buf), "[%Y-%m-%d %H:%M:%S]", std::localtime(&now));

	logfile_ << time_buf << " " << level_str << " " << message << std::endl;
}

} // namespace qga::utils