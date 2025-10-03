#include "io/DataExporter.hpp"
#include <fstream>
#include <stdexcept>
#include <sstream>
#include "nlohmann/json.hpp"

using nlohmann::json;
namespace qga::io {

    DataExporter::DataExporter(const std::string& output_path,
        std::shared_ptr<utils::ILogger> logger,
        ExportFormat format,
        bool append)
        : output_path_(output_path),
         format_(format), 
         append_(append),
         logger_(std::move(logger)) {
        if (!logger_) {
            throw std::invalid_argument("Logger instance cannot be null");
        }
        if (output_path_.empty()) {
            throw std::invalid_argument("Output path cannot be empty");
        }
    }

    DataExporter::~DataExporter() = default;

    void DataExporter::exportSeries(const qga::domain::backtest::BarSeries& series) {
        if (series.empty()) {
            logger_->error("DataExporter: cannot export empty BarSeries");
            throw std::invalid_argument("BarSeries is empty");
        }

        std::ofstream out(output_path_, append_ ? std::ios::app : std::ios::trunc);
        if (!out.is_open()) {
            logger_->error("DataExporter: failed to open output file: {}", output_path_);
            throw std::runtime_error("Failed to open output file: " + output_path_);
        }

        if (format_ == ExportFormat::CSV) {
            if (!append_) writeHeader(out);
            writeCSV(out, series);
        } else {
            writeJSON(out, series);
        }
        
        logger_->info("DataExporter: successfully exported {} bars to {}", series.size(), output_path_);
                
    }

    void DataExporter::exportRange(const qga::domain::backtest::BarSeries& series, size_t from, size_t to) {
        if (series.empty() || from >= to || to > series.size()) {
            logger_->error("DataExporter: invalid range export request [{}:{})", from, to);
            throw std::invalid_argument("Series empty or invalid range");
        }

        std::ofstream out(output_path_, append_ ? std::ios::app : std::ios::trunc);
        if (!out.is_open()) {
            logger_->error("DataExporter: failed to open output file {}", output_path_);
            throw std::runtime_error("Failed to open output file: " + output_path_);
        }

        qga::domain::backtest::BarSeries subset;
        for (size_t i = from; i < to; ++i) {
            subset.add(series[i]);
        }

        if (format_ == ExportFormat::CSV) {
            if (!append_) writeHeader(out);
            writeCSV(out, subset);
        } else {
            writeJSON(out, subset);
        }

        logger_->info("DataExporter: exported {} rows (range) to {}", subset.size(), output_path_);
    }

    void DataExporter::writeHeader(std::ofstream& out) {
        if (!append_) {
            out << "timestamp,open,high,low,close,volume\n";
        }
    }

    void DataExporter::writeCSV(std::ofstream& out,
        const qga::domain::backtest::BarSeries& series) {
        for (const auto& bar : series.data()) {
            out << bar.ts_ << "," 
                << bar.open_ << "," 
                << bar.high_ << ","
                << bar.low_ << "," 
                << bar.close_ << ","
                << bar.volume_ << "\n";
        }
    }

    void DataExporter::writeJSON(std::ofstream& out,
        const qga::domain::backtest::BarSeries& series) {
        json j = json::array();
        for (const auto& bar : series.data()) {
            j.push_back({
                {"timestamp", bar.ts_},
                {"open", bar.open_},
                {"high", bar.high_},
                {"low", bar.low_},
                {"close", bar.close_},
                {"volume", bar.volume_}
            });
        }
        out << j.dump(4); // Pretty print with 4 spaces indent
    }

    void DataExporter::exportAll(const qga::domain::backtest::BarSeries& series) {
        if (series.empty()) {
            logger_->warn("DataExporter: tried to export empty BarSeries");
            return;
        }
        exportSeries(series);  // delegujemy do istniejącej metody
    }


    

}   // namespace export_util
