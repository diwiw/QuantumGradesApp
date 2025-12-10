#include "ingest/DataIngest.hpp"
#include <fstream>
#include <sstream>
#include <iostream>
#include <curl/curl.h>  // For HTTP requests
#include <ctime>
#include <chrono>
#include <iomanip>     // For std::get_time

namespace {

    // === GLOBAL CURL INIT (RAII-style) ===
    struct CurlGlobalInit {
        CurlGlobalInit() { curl_global_init(CURL_GLOBAL_DEFAULT); }
        ~CurlGlobalInit() { curl_global_cleanup(); }
    };

    CurlGlobalInit global_curl_init;

    // === CALLBACK: Append HTTP body to std::string ===
    size_t writeToString(void* contents, size_t size, size_t nmemb, std::string* userp) {
        size_t total = size * nmemb;
        auto* buffer = static_cast<std::string*>(userp);
        buffer->append(static_cast<char*>(contents), total);
        return total;
    }

    // === HTTP GET using libcurl ===
    std::optional<std::string> fetchHttpContent(const std::string& url) {
        CURL* curl = curl_easy_init();
        if (!curl) return std::nullopt;

        std::string response;
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeToString);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);
        curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L); // handle redirects

        CURLcode res = curl_easy_perform(curl);
        curl_easy_cleanup(curl);

        if (res != CURLE_OK) {
            std::cerr << "[DataIngest] HTTP error: " << curl_easy_strerror(res) << "\n";
            return std::nullopt;
        }

        return response;
    }
}   // namespace

namespace qga::ingest {

// === PUBLIC ===

DataIngest::DataIngest(std::shared_ptr<utils::ILogger> logger)
    : logger_(std::move(logger)) {}

std::optional<domain::backtest::BarSeries> DataIngest::fromCsv(const std::string& path) {
    std::ifstream file(path);
    if (!file.is_open()) {
        logger_->error("Failed to open file: {}", path);
        return std::nullopt;
    }

    domain::backtest::BarSeries series;
    std::string line;
    bool is_header = true;
    while (std::getline(file, line)) {
        std::vector<std::string> fields;
        std::stringstream ss(line);
        std::string item;
        if (is_header) {
            // Skip header row
            is_header = false;
            logger_->debug("Skipping header row: {}", line);
            continue;
        }
        while (std::getline(ss, item, ',')) {
            fields.push_back(item);
        }

        if (!validateRow(fields)) continue;
        auto quote = parseRow(fields);
        if (quote) {
            series.add(*quote);
        }
    }

    if (series.empty()) {
        logger_->error("No valid rows found in file: {}", path);
        return std::nullopt;
    }

    return series;
}

std::optional<domain::backtest::BarSeries> DataIngest::fromHttpUrl(const std::string& url) {
    auto content = fetchHttpContent(url);
    if (!content.has_value()) {
        logger_->error("Failed to fetch HTTP content from: {}", url);
        return std::nullopt;
    }

    domain::backtest::BarSeries series;
    std::istringstream iss(*content);
    std::string line;
    while (std::getline(iss, line)) {
        std::vector<std::string> fields;
        std::stringstream ss(line);
        std::string item;
        while (std::getline(ss, item, ',')) {
            fields.push_back(item);
        }

        if (!validateRow(fields)) continue;
        auto quote = parseRow(fields);
        if (quote) {
            series.add(*quote);
        }
    }

    if (series.empty()) {
        logger_->error("No valid rows fetched from HTTP source.");
        return std::nullopt;
    }

    return series;
}

// === PRIVATE ===

bool DataIngest::validateRow(const std::vector<std::string>& fields) {
    return fields.size() == 6;
}

std::optional<domain::Quote> DataIngest::parseRow(const std::vector<std::string>& fields) {

    try {
        domain::Quote quote;
        // --- Parse timestamp (ISO 8601 or epoch)
        if (fields[0].find('T') != std::string::npos) {

            // ISO 8601 format
            std::istringstream ss(fields[0]);
            std::tm tm = {};
            ss >> std::get_time(&tm, "%Y-%m-%dT%H:%M:%S");
            if (ss.fail()) {
                logger_->error("Invalid ISO timestamp: {}", fields[0]);
                throw std::runtime_error("Invalid ISO timestamp");
            }
            auto time_point = std::chrono::system_clock::from_time_t(std::mktime(&tm));

            quote.ts_ = std::chrono::duration_cast<std::chrono::milliseconds>(time_point.time_since_epoch()).count();
        } else {
            // Epoch milliseconds
            quote.ts_ = std::stoll(fields[0]); // fallback: epoch millis
        }

        // --- Parse OHLCV ---
        quote.open_ = std::stod(fields[1]);
        quote.high_ = std::stod(fields[2]);
        quote.low_ = std::stod(fields[3]);
        quote.close_ = std::stod(fields[4]);
        quote.volume_ = std::stod(fields[5]);

        return quote;
    } catch (const std::exception& e) {
        if (logger_) logger_->error("parseRow failed: {}", e.what());
        return std::nullopt;
    }


}

} // namespace qga::ingest
