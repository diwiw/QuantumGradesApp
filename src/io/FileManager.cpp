#include "io/FileManager.hpp"
#include "common/LogLevel.hpp"
#include "utils/ILogger.hpp"
#include "utils/LoggerFactory.hpp"
#include <filesystem>
#include <fstream>

namespace qga::io
{

    namespace
    {
        auto s_logger =
            qga::utils::LoggerFactory::createConsoleLogger("FileManager", qga::LogLevel::Info);
    }

    std::optional<std::vector<std::string>> FileManager::readAllLines(const std::string& file_path)
    {
        if (!exists(file_path))
        {
            s_logger->log(qga::LogLevel::Err, "[FileManager] File not found: " + file_path);
            return std::nullopt;
        }

        std::ifstream file(file_path);
        if (!file.is_open())
        {
            s_logger->log(qga::LogLevel::Err,
                          "[FileManager] Cannot open file for reading: " + file_path);
            return std::nullopt;
        }

        std::vector<std::string> lines;
        std::string line;
        while (std::getline(file, line))
        {
            lines.push_back(line);
        }
        s_logger->log(qga::LogLevel::Info, "[FileManager] Read " + std::to_string(lines.size()) +
                                               " lines from: " + file_path);

        return lines;
    }

    bool FileManager::writeAllLines(const std::string& file_path,
                                    const std::vector<std::string>& lines)
    {
        try
        {
            std::filesystem::path path(file_path);
            if (path.has_parent_path())
                std::filesystem::create_directories(
                    path.parent_path()); // ✅ automatycznie tworzy folder

            std::ofstream file(file_path, std::ios::trunc);
            if (!file.is_open())
            {
                s_logger->log(qga::LogLevel::Err,
                              "[FileManager] Cannot open file for writing: " + file_path);
                return false;
            }

            for (const auto& line : lines)
                file << line << "\n";

            s_logger->log(qga::LogLevel::Info, "[FileManager] Wrote " +
                                                   std::to_string(lines.size()) +
                                                   " lines to the file: " + file_path);
            return true;
        }
        catch (const std::exception& e)
        {
            s_logger->log(qga::LogLevel::Err,
                          std::string("[FileManager] Exception during write: ") + e.what());
            return false;
        }
    }

    bool FileManager::appendLine(const std::string& file_path, const std::string& line)
    {
        try
        {
            std::filesystem::path path(file_path);
            if (path.has_parent_path())
                std::filesystem::create_directories(path.parent_path()); // ✅ też tu

            std::ofstream file(file_path, std::ios::app);
            if (!file.is_open())
            {
                s_logger->log(qga::LogLevel::Err,
                              "[FileManager] Failed to append to file: " + file_path);
                return false;
            }

            file << line << "\n";
            s_logger->log(qga::LogLevel::Info, "[FileManager] Appended line to: " + file_path);
            return true;
        }
        catch (const std::exception& e)
        {
            s_logger->log(qga::LogLevel::Err,
                          std::string("[FileManager] Exception during append: ") + e.what());
            return false;
        }
    }

    bool FileManager::exists(const std::string& file_path)
    {
        return std::filesystem::exists(file_path);
    }

    bool FileManager::removeFile(const std::string& file_path)
    {
        if (!exists(file_path))
        {
            s_logger->log(qga::LogLevel::Warn, "[FileManager] File does not exist: " + file_path);
            return false;
        }

        if (std::filesystem::remove(file_path))
        {
            s_logger->log(qga::LogLevel::Info, "[FileManager] Deleted file: " + file_path);
            return true;
        }
        else
        {
            s_logger->log(qga::LogLevel::Err, "[FileManager] Failed to delete file: " + file_path);
            return false;
        }
    }

} // namespace qga::io