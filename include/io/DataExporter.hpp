/**
 * @file DataExporter.hpp
 * @brief Export module for persisting BarSeries data to disk (CSV/JSON format).
 */

#pragma once

#include <string>
#include <memory>
#include <filesystem>
#include "domain/backtest/BarSeries.hpp"
#include "utils/ILogger.hpp"

namespace qga::io {
 
/**
 * @enum ExportFormat
 * @brief Supported export formats for serialized market data.
 */
enum class ExportFormat {
    CSV,   ///< Export data as plain CSV (comma-separated values).
    JSON   ///< Export data as JSON array of objects.
};    

/**
 * @class DataExporter
 * @brief Handles exporting time-series market data to disk.
 * 
 * Provides an interface for exporting BarSeries data to a specified file in different formats in different
 * formats (CSV, JSON). 
 */
class DataExporter {
public:
    /**
     * @brief Constructor with mandatory logger.
     * @param output_path The file path where data will be exported.
     * @param logger Logger instance (SpdLogger, MockLogger, NullLogger).
     * @param format Export format (CSV or JSON).
     * @param append If true, appends instead of overwriting.
     */
    explicit DataExporter(const std::string& output_path,
        std::shared_ptr<utils::ILogger> logger,
        ExportFormat format = ExportFormat::CSV,
        bool append = false);

    /**
     * @brief Constructor overload accepting std::filesystem::path.
     * @param output_path The file path where data will be exported.
     * @param logger Logger instance (SpdLogger, MockLogger, NullLogger).
     * @param format Export format (CSV or JSON).
     * @param append If true, appends instead of overwriting.
     */
    explicit DataExporter(const std::filesystem::path& output_path,
                          std::shared_ptr<utils::ILogger> logger,
                          ExportFormat format = ExportFormat::CSV,
                          bool append = false)
        : DataExporter(output_path.string(), std::move(logger), format, append) {}

    /**
     * @brief Destructor to ensure cleanup if needed.
     */
    virtual ~DataExporter();

    /**
     * @brief Exports the given BarSeries to the configured file.
     * 
     * @param series The series quotes to export.
     * @throws std::runtime_error if file operations fail.
     */
    virtual void exportSeries(const qga::domain::backtest::BarSeries& series);

    /**
     * @brief Optional: Export a subset of the series by index.
     */
    void exportRange(const qga::domain::backtest::BarSeries& series, size_t from, size_t to);


    /**
     * @brief Export the entire BarSeries to the configured file.
     * 
     * Alias for exportSeries(), but more explicit in intent.
     * 
     * @param series The full BarSeries to export.
     */
    void exportAll(const qga::domain::backtest::BarSeries& series);

private:
    std::string output_path_;
    ExportFormat format_;
    bool append_;
    std::shared_ptr<utils::ILogger> logger_;

    void writeHeader(std::ofstream& out);
    void writeCSV(std::ofstream& out,
        const qga::domain::backtest::BarSeries& series);
    void writeJSON(std::ofstream& out,
        const qga::domain::backtest::BarSeries& series);
};

} // namespace qga::io
