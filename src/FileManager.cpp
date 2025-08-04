#include "FileManager.h"
#include "Logger.h"
#include <fstream>
#include <filesystem>

std::optional<std::vector<std::string>> FileManager::readAllLines(const std::string& filePath) {
	if(!exists(filePath)){
		Logger::getInstance().log(LogLevel::ERROR, "[FileManager] File not found: " + filePath);
		return std::nullopt;
	}

	std::ifstream file(filePath);
	if(!file.is_open()){
		Logger::getInstance().log(LogLevel::ERROR, "[FileManager] Cannot open file for reading: " + filePath);
		return std::nullopt;
	}

	std::vector<std::string> lines;
	std::string line;
	while(std::getline(file, line)){
		lines.push_back(line);
	}
	Logger::getInstance().log(LogLevel::INFO, "[FileManager] Read " + std::to_string(lines.size()) + " lines from: " + filePath);
	
	return lines;
}

bool FileManager::writeAllLines(const std::string& filePath, const std::vector<std::string>& lines){
	std::ofstream file(filePath, std::ios::trunc);
	if(!file.is_open()){
		Logger::getInstance().log(LogLevel::ERROR, "[FileManager] Cannot open file for writing: " + filePath);
		return false;
	}

	for (const auto& line : lines) {
		file << line << "\n";
	}

	Logger::getInstance().log(LogLevel::INFO, "[FileManager] Wrote " + std::to_string(lines.size()) + " lines to the file: " + filePath);
	
	return true;
}

bool FileManager::appendLine(const std::string& filePath, const std::string& line) {
	std::ofstream file(filePath, std::ios::app);
	if(!file.is_open()){
		Logger::getInstance().log(LogLevel::ERROR, "[FileManager] Failed to append to file: " + filePath);
		return false;
	}

	file << line << "\n";
	Logger::getInstance().log(LogLevel::INFO, "[FileManager] Appended line to: " + filePath);
	return true;
}

bool FileManager::exists(const std::string& filePath) {
	return std::filesystem::exists(filePath);
}

bool FileManager::removeFile(const std::string& filePath) {
	if(!exists(filePath)){
		Logger::getInstance().log(LogLevel::WARNING, "[FileManager] File does not exist: " + filePath);
		return false;
	}

	if(std::filesystem::remove(filePath)){
		Logger::getInstance().log(LogLevel::INFO, "[FileManager] Deleted file: " + filePath);
		return true;
	} else {
		Logger::getInstance().log(LogLevel::ERROR, "[FileManager] Failed to delete file: " + filePath);
		return false;
	}
}
