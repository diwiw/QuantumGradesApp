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
     * @brief Parses a single CSV row into a Quote object.
     *
     * This function converts a vector of string fields (representing one CSV line)
     * into a `domain::Quote` instance. It validates the number of columns and
     * attempts to parse numeric fields (OHLCV) and timestamp values.
     *
     * The timestamp parser supports two formats:
     * - **Epoch milliseconds** (e.g. `1727773200000`)
     * - **ISO 8601** date-time strings (e.g. `2024-10-01T09:00:00`)
     *
     * In case of invalid or malformed input (wrong number of columns, 
     * non-numeric values, parse errors), the function logs the problem and
     * returns `std::nullopt` to indicate failure.
     *
     * @param fields A vector of string fields from a single CSV line.  
     * Expected order: `[timestamp, open, high, low, close, volume]`.
     * @return `std::optional<domain::Quote>`  
     *         - A fully parsed `Quote` if successful.  
     *         - `std::nullopt` if parsing failed.
     *
     * @note This method is resilient to format variations — it automatically 
     *       detects ISO timestamps by checking for the presence of `'T'`.
     * @warning The function assumes decimal point ('.') as the float separator.
     *          Locale-specific commas (',') will cause parsing errors.
     * 
     * @see qga::domain::Quote
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
