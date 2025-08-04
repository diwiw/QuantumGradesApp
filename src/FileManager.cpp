#include "FileManager.h"
#include "Logger.h"
#include <fstream>
#include <sstream>

std::optional<std::vector<std::vector<std::string>>> FileManager::readCSV(const std::string& filename) {
	std::ifstream file(filename);
	if(!file.is_open()){
		Logger::getInstance().log(LogLevel::ERROR, "Cannot open file for reading: " + filename);
		return std::nullopt;
	}

	Logger::getInstance().log(LogLevel::INFO, "Reading CSV file: " + filename);

	std::vector<std::vector<std::string>> rows;
	std::string line;

	while(std::getline(file, line)){
		std::vector<std::string> row;
		std::stringstream ss(line);
		std::string cell;
		while(std::getline(ss, cell, ',')) {
			row.push_back(cell);
		}
		rows.push_back(row);

	}

	file.close();
	return rows;
}

bool FileManager::writeCSV(const std::string& filename, const std::vector<std::vector<std::string>>& data){
	std::ofstream file(filename);
	if(!file.is_open()){
		Logger::getInstance().log(LogLevel::ERROR, "Cannot open file for writing: " + filename);
		return false;
	}

	Logger::getInstance().log(LogLevel::INFO, "Writing CSV file: " + filename);

	for (const auto& row : data) {
		for(size_t i=0; i < row.size() ; i++) {
			file << row[i];
			if (i < row.size() -1) {
				file << ",";
			}
		}
		file << "\n";
	}

	file.close();
	return true;
}
