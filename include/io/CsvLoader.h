/**
 * @file CsvLoader.h
 * @brief CSV file reader for loading market data into BarSeries.
 */

#pragma once

#include <string>
#include "domain/backtest/BarSeries.h"
namespace io { 

    /**
     * @brief Load market bar data from a CSV file into a BarSeries.
     *
     * Assumes a CSV structure compatible with @ref domain::Quote fields,
     * typically containing OHLCV and timestamp information.
     *
     * @param path Absolute or relative path to the CSV file.
     * @param out  Reference to BarSeries object that will be populated.
     * @return True if loading succeeded, false otherwise.
     *
     * @note This is a simple loader for research/testing only. Production usage
     * should include schema validation and error handling.
     */
    bool loadCsv(const std::string& path, backtest::BarSeries& out);
    
}   // namespace io
