#include "ingest/DataIngest.hpp"
#include <fstream>
#include <sstream>
#include <iostream>
#include <curl/curl.h>  // For HTTP requests

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

namespace ingest {

// === PUBLIC ===

std::optional<backtest::BarSeries> DataIngest::fromCsv(const std::string& path) {
    std::ifstream file(path);
    if (!file.is_open()) {
        std::cerr << "[DataIngest] Failed to open file: " << path << "\n";
        return std::nullopt;
    }

    backtest::BarSeries series;
    std::string line;
    while (std::getline(file, line)) {
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
        std::cerr << "[DataIngest] No valid rows found in file: " << path << "\n";
        return std::nullopt;
    }

    return series;
}

std::optional<backtest::BarSeries> DataIngest::fromHttpUrl(const std::string& url) {
    auto content = fetchHttpContent(url);
    if (!content.has_value()) {
        std::cerr << "[DataIngest] Failed to fetch HTTP content from: " << url << "\n";
        return std::nullopt;
    }


    backtest::BarSeries series;
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
        std::cerr << "[DataIngest] No valid rows fetched from HTTP source.\n";
        return std::nullopt;
    }

    return series;
}

// === PRIVATE ===

bool DataIngest::validateRow(const std::vector<std::string>& fields) {
    return fields.size() == 6;
}

std::optional<domain::Quote> DataIngest::parseRow(const std::vector<std::string>& fields) {
    if (fields.size() != 6) return std::nullopt;

    try {
        int64_t ts     = std::stoll(fields[0]);
        double open    = std::stod(fields[1]);
        double high    = std::stod(fields[2]);
        double low     = std::stod(fields[3]);
        double close   = std::stod(fields[4]);
        double volume  = std::stod(fields[5]);

        return domain::Quote{ ts, open, high, low, close, volume };
    } catch (const std::exception& e) {
        std::cerr << "[DataIngest] parseRow failed: " << e.what() << "\n";
        return std::nullopt;
    }
}

} // namespace ingest