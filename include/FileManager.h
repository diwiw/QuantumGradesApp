#pragma once
#include <string>
#include <vector>
#include <optional>

class FileManager {
public:
	// Reads a CSV file and returns vector of rows (each row = vector of string)
	static std::optional<std::vector<std::vector<std::string>>> readCSV(const std::string& filename);

	// Writes a CSV file from vector of rows
	static bool writeCSV(const std::string& filename, const std::vector<std::vector<std::string>>& data);

private:
	// Prevent instantation
	FileManager() = delete;
};
