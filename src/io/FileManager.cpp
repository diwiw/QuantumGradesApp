#include "io/FileManager.hpp"
#include "utils/Logger.hpp"
#include <fstream>
#include <filesystem>

namespace qga::io {

std::optional<std::vector<std::string>> FileManager::readAllLines(const std::string& file_path) {
	if(!exists(file_path)){
		Logger::getInstance().log(LogLevel::ERROR, "[FileManager] File not found: " + file_path);
		return std::nullopt;
	}

	std::ifstream file(file_path);
	if(!file.is_open()){
		Logger::getInstance().log(LogLevel::ERROR, "[FileManager] Cannot open file for reading: " + file_path);
		return std::nullopt;
	}

	std::vector<std::string> lines;
	std::string line;
	while(std::getline(file, line)){
		lines.push_back(line);
	}
	Logger::getInstance().log(LogLevel::INFO, "[FileManager] Read " + std::to_string(lines.size()) + " lines from: " + file_path);
	
	return lines;
}

bool FileManager::writeAllLines(const std::string& file_path, const std::vector<std::string>& lines){
	std::ofstream file(file_path, std::ios::trunc);
	if(!file.is_open()){
		Logger::getInstance().log(LogLevel::ERROR, "[FileManager] Cannot open file for writing: " + file_path);
		return false;
	}

	for (const auto& line : lines) {
		file << line << "\n";
	}

	Logger::getInstance().log(LogLevel::INFO, "[FileManager] Wrote " + std::to_string(lines.size()) + " lines to the file: " + file_path);
	
	return true;
}

bool FileManager::appendLine(const std::string& file_path, const std::string& line) {
	std::ofstream file(file_path, std::ios::app);
	if(!file.is_open()){
		Logger::getInstance().log(LogLevel::ERROR, "[FileManager] Failed to append to file: " + file_path);
		return false;
	}

	file << line << "\n";
	Logger::getInstance().log(LogLevel::INFO, "[FileManager] Appended line to: " + file_path);
	return true;
}

bool FileManager::exists(const std::string& file_path) {
	return std::filesystem::exists(file_path);
}

bool FileManager::removeFile(const std::string& file_path) {
	if(!exists(file_path)){
		Logger::getInstance().log(LogLevel::WARNING, "[FileManager] File does not exist: " + file_path);
		return false;
	}

	if(std::filesystem::remove(file_path)){
		Logger::getInstance().log(LogLevel::INFO, "[FileManager] Deleted file: " + file_path);
		return true;
	} else {
		Logger::getInstance().log(LogLevel::ERROR, "[FileManager] Failed to delete file: " + file_path);
		return false;
	}
}

} // namespace qga::io