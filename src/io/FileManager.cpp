#include "common/LogLevel.hpp"
#include "io/FileManager.hpp"
#include "utils/ILogger.hpp"
#include "utils/LoggerFactory.hpp"
#include <fstream>
#include <filesystem>

namespace qga::io {

	namespace {
		auto s_logger = qga::utils::LoggerFactory::createConsoleLogger("FileManager", qga::LogLevel::Info);
	}


std::optional<std::vector<std::string>> FileManager::readAllLines(const std::string& file_path) {
	if(!exists(file_path)){
		s_logger->log(qga::LogLevel::Err, "[FileManager] File not found: " + file_path);
		return std::nullopt;
	}

	std::ifstream file(file_path);
	if(!file.is_open()){
		s_logger->log(qga::LogLevel::Err, "[FileManager] Cannot open file for reading: " + file_path);
		return std::nullopt;
	}

	std::vector<std::string> lines;
	std::string line;
	while(std::getline(file, line)){
		lines.push_back(line);
	}
	s_logger->log(qga::LogLevel::Info, "[FileManager] Read " + std::to_string(lines.size()) + " lines from: " + file_path);

	return lines;
}

bool FileManager::writeAllLines(const std::string& file_path, const std::vector<std::string>& lines){
	std::ofstream file(file_path, std::ios::trunc);
	if(!file.is_open()){
		s_logger->log(qga::LogLevel::Err, "[FileManager] Cannot open file for writing: " + file_path);
		return false;
	}

	for (const auto& line : lines) {
		file << line << "\n";
	}

	s_logger->log(qga::LogLevel::Info, "[FileManager] Wrote " + std::to_string(lines.size()) + " lines to the file: " + file_path);

	return true;
}

bool FileManager::appendLine(const std::string& file_path, const std::string& line) {
	std::ofstream file(file_path, std::ios::app);
	if(!file.is_open()){
		s_logger->log(qga::LogLevel::Err, "[FileManager] Failed to append to file: " + file_path);
		return false;
	}

	file << line << "\n";
	s_logger->log(qga::LogLevel::Info, "[FileManager] Appended line to: " + file_path);
	return true;
}

bool FileManager::exists(const std::string& file_path) {
	return std::filesystem::exists(file_path);
}

bool FileManager::removeFile(const std::string& file_path) {
	if(!exists(file_path)){
		s_logger->log(qga::LogLevel::Warn, "[FileManager] File does not exist: " + file_path);
		return false;
	}

	if(std::filesystem::remove(file_path)){
		s_logger->log(qga::LogLevel::Info, "[FileManager] Deleted file: " + file_path);
		return true;
	} else {
		s_logger->log(qga::LogLevel::Err, "[FileManager] Failed to delete file: " + file_path);
		return false;
	}
}

} // namespace qga::io