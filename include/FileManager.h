#pragma once
#include <string>
#include <vector>
#include <optional>

/**
 * @brief Utility class for file operations (static, stateless).
 *
 * This class provides safe read/write/append operations for text files.
 *
 * All methods return std::optional to allow error checking without exceptions.
 */
class FileManager {
public:

	//====== Prevent instantiation ====
	FileManager() = delete;
	~FileManager() = delete;
	FileManager(const FileManager&) = delete;
	FileManager& operator=(const FileManager&) = delete;

	/**
	 * @brief Reads all lines from a file
	 * @param filePath Path to the file
	 * @return Vector of lines if successful, std::nullopt if error.
	 */
	static std::optional<std::vector<std::string>> readAllLines(const std::string& file_path);

	/**
	 * @brief Writes a vector of lines to a file (overwrites existing content).
	 * @param filePath Path to the file
	 * @param lines Vector of lines to write.
	 * @return true if successful, false otherwise.
	 */
	static bool writeAllLines(const std::string& file_path, const std::vector<std::string>& lines);
	
	/**
	 * @brief Appends a line to an existing file.
	 * @param filePath Path to the file.
	 * @param line The line to append.
	 * @return true if successful, false otherwise.
	 */
	static bool appendLine(const std::string& file_path, const std::string& line);

	/**
	 * @brief Checks if a file exists.
	 * @param filePath Path to a file.
	 * @return true if exists, false otherwise.
	 */
	static bool exists(const std::string& file_path);

	/**
	 * @brief Deletes a file if it exists.
	 * @param filePath Path to the file.
	 * @return true if deleted, false if file doesn't exist or failed to delete.
	 */
	static bool removeFile(const std::string& file_path);

};
