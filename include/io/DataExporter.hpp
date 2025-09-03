/**
 * @file DataExporter.hpp
 * @brief Export module for persisting BarSeries data to disk (CSV format).
 */

#pragma once

#include <string>
#include "domain/backtest/BarSeries.hpp"

namespace io {
    
    /**
     * @class DataExporter
     * @brief Handles exporting time-series market data to CSV.
     * This class provides an interface for exporting BarSeries data to a specified file path.          
     */
    class DataExporter {
    public:
    /**
     * @brief Constructor with target output path.
     * @param outputPath The file path where data will be exported.
     * @param append If true, data will be appended to the file if it exists;
     */
    explicit DataExporter(const std::string& output_path, bool append = false);

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
    virtual void exportSeries(const backtest::BarSeries& series);

    /**
     * @brief Optional: Export a subset of the series by index.
     */
    void exportRange(const backtest::BarSeries& series, size_t from, size_t to);
    

    private:

    std::string output_path_;
    bool append_;

    void writeHeader(std::ofstream& out);
    };

}   // namespace export_util
