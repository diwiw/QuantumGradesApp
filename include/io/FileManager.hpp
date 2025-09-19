/**
 * @file FileManager.hpp
 * @brief Utility class for file operations (static, stateless).
 *
 * This class provides safe read/write/append operations for text files.
 * All methods return std::optional or boolean flags to support error checking without exceptions.
 * 
 * Designed to be used as a stateless static utility — cannot be instantiated.
 */

#pragma once

#include <string>
#include <vector>
#include <optional>

namespace qga::io {
/**
 * @class FileManager
 * @brief Static utility class for safe text file operations.
 *
 * This class cannot be instantiated. It provides static methods for:
 *  - Reading lines from a file into a vector.
 *  - Writing/overwriting all lines to a file.
 *  - Appending a line to a file.
 *  - Checking file existence.
 *  - Removing files.
 */
class FileManager {
public:

	/// @name Deleted constructors and operators
    /// @{
    FileManager() = delete;                           ///< Deleted constructor — prevents instantiation.
    ~FileManager() = delete;                          ///< Deleted destructor.
    FileManager(const FileManager&) = delete;         ///< Deleted copy constructor.
    FileManager& operator=(const FileManager&) = delete; ///< Deleted copy assignment.
    /// @}

	/**
     * @brief Reads all lines from a text file into a vector.
     * 
     * @param file_path Path to the file.
     * @return Vector of lines if successful, std::nullopt if file not found or error occurs.
     */
	static std::optional<std::vector<std::string>> readAllLines(const std::string& file_path);

	/**
     * @brief Writes a list of lines to a file, replacing existing content.
     *
     * @param file_path Path to the file.
     * @param lines Vector of lines to write.
     * @return true if file was successfully written, false otherwise.
     */
	static bool writeAllLines(const std::string& file_path, const std::vector<std::string>& lines);
	
	/**
     * @brief Appends a single line to the end of the specified file.
     * 
     * @param file_path Path to the file.
     * @param line Line of text to append.
     * @return true if the line was successfully appended, false otherwise.
     */
	static bool appendLine(const std::string& file_path, const std::string& line);

	/**
     * @brief Checks whether a file exists at the given path.
     * 
     * @param file_path Path to the file.
     * @return true if the file exists, false otherwise.
     */
	static bool exists(const std::string& file_path);

	/**
     * @brief Removes the file at the given path, if it exists.
     *
     * @param file_path Path to the file.
     * @return true if the file was successfully deleted, false if not found or error occurred.
     */
	static bool removeFile(const std::string& file_path);

};

} // namespace qga::io
