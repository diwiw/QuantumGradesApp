#include "ingest/DataIngest.hpp"
#include "FileManager.hpp"
#include "domain/backtest/BarSeries.hpp"
#include <sstream>
#include <stdexcept>

namespace ingest {
    
    std::optional<backtest::BarSeries> DataIngest::fromCsv(const std::string& path) {
        auto lines_opt = FileManager::readAllLines(path);
        if (!lines_opt.has_value()) {
            throw std::runtime_error("Failed to read CSV file: " + path);
        }

        const auto& lines = lines_opt.value();
        backtest::BarSeries series;

        for(size_t i = 0 ; i < lines.size(); i++) {
            const std::string& line = lines[i];
            if (line.empty()) continue;

            std::istringstream stream(line);
            std::string token;
            std::vector<std::string> fields;

            while (std::getline(stream, token, ',')) {
                fields.push_back(token);
            }

            if (!validateRow(fields)) {
                throw std::runtime_error("Invalid CSV format at line " + std::to_string(i + 1));
            }

            auto quote_opt = parseRow(fields);
            if (!quote_opt.has_value()) {
                throw std::runtime_error("Failed to parse row at line " + std::to_string(i + 1));
            }

            series.add(quote_opt.value());
        }

        return series;
    }


    std::optional<backtest::BarSeries> DataIngest::fromHttpUrl(const std::string& url) {
        throw std::runtime_error("fromHttpUrl() not implemented yet: " + url);
    }

    bool DataIngest::validateRow(const std::vector<std::string>& fields) {
        return fields.size() == 6;
    }

    std::optional<domain::Quote> DataIngest::parseRow(const std::vector<std::string>& fields) {
        try {
            domain::Quote q;
            q.ts_       = std::stoll(fields[0]);
            q.open_     = std::stoll(fields[1]);
            q.high_     = std::stoll(fields[2]);
            q.low_      = std::stoll(fields[3]);
            q.close_    = std::stoll(fields[4]);
            q.volume_   = std::stoll(fields[5]);
            return q;
        } catch (const std::exception& e) {
            throw std::runtime_error("parseRow() failed: " + std::string(e.what()));
        }
    }

}   // namespace ingest