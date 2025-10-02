/**
 * @file DataIngest.hpp
 * @brief High-level module for loading and validating market data from files or HTTP.
 * 
 * Provides ingest functions that return validate 'VarSerie' objets.
 * Supports extensibility for remote sources and alternative formats.
 */

#pragma once

#include <string>
#include <optional>
#include <memory>
#include "utils/ILogger.hpp"
#include "domain/backtest/BarSeries.hpp"
#include "domain/Quote.hpp"


namespace qga::ingest {


/**
 * @class DataIngest
 * @brief High-level interface for ingesting market data from various sources.
 * 
 * This class provides static methods to load, validate, and parse row CSV data 
 * into structured domain objects. The focus is correctness and extensibility.
 * 
 * Example usage:
 * @code
 * auto series = DataIngest::fromCsv("data/market.csv");
 * if (series.has_value()) {
 * // use series.value() ...
 * }
 * @endcode
 * 
 * Future extensionts include: fromHttpUrl, fromBinaryFile, streaming ingest.
 */
class DataIngest {        
public:
    /**
     * @brief Constructor accepting a logger instance.
     * 
     * @param logger Shared pointer to a logger implementing ILogger interface.
     */
    explicit DataIngest(std::shared_ptr<utils::ILogger> logger);

    /**
     * @brief Load and validate market data from a local CSV file.
     * 
     * CSV format: timestamp_ms,open,high,low,close,volume
     * 
     * @param path Path to the CSV file on disk.
     * @return Optional BarSeries if loading and parsing succeed.
     */
    std::optional<qga::domain::backtest::BarSeries> fromCsv(const std::string& path);

    /**
     * @brief Load market data from a remote CSV over HTTP.
     * 
     * Currently not fully implemented. Will be supported in v0.7.1+.
     * 
     * @param url Remote URL returning CSV content.
     * @return Optional BarSeries if request and parsing succeed.
     */
    std::optional<qga::domain::backtest::BarSeries> fromHttpUrl(const std::string& url);

private:           
    
    #ifdef UNIT_TEST
        friend class DataIngestTestAccess;     
    #endif

    /**
     * @brief Validates the structure of a parsed CSV row.
     * 
     * Checks field count and optionally value constraints.
     * 
     * @param fields Vector of CSV fields.
     * @return True if row passes validation.
     */
    bool validateRow(const std::vector<std::string>& fields);

    /**
     * @brief Converts a CSV row (string fields) into a Quote object.
     * 
     * @param fields CSV fields: timestamp, OHLC, volume
     * @return Optional Quote if parsing succeeds.
     */
    std::optional<domain::Quote> parseRow(const std::vector<std::string>& fields);

     /**
     * @brief Logger instance used for validation, error reporting and tracing.
     *
     * This logger is injected via the constructor and can be a production
     * logger (SpdLogger), a NullLogger for silent mode, or a MockLogger
     * in tests.
     */
    std::shared_ptr<utils::ILogger> logger_;
};

} // namespace qga::ingest
