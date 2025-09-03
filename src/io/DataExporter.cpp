#include "io/DataExporter.hpp"
#include <fstream>
#include <stdexcept>
#include <sstream>

namespace io {

    DataExporter::DataExporter(const std::string& output_path, bool append)
        : output_path_(output_path), append_(append) {}

    DataExporter::~DataExporter() = default;

    void DataExporter::exportSeries(const backtest::BarSeries& series) {
        if (series.empty()) {
            throw std::invalid_argument("BarSeries is empty");
        }

        std::ofstream out(output_path_, append_ ? std::ios::app : std::ios::trunc);
        if (!out.is_open()) {
            throw std::runtime_error("Failed to open output file: " + output_path_);
        }

        if (!append_){
            writeHeader(out);
        }
        
        for (const auto& quote : series.data()) {
            out << quote.ts_ << "," 
                << quote.open_ << "," 
                << quote.high_ << ","
                << quote.low_ << "," 
                << quote.close_ << "," 
                << quote.volume_ << "\n";
        }
    }

    void DataExporter::exportRange(const backtest::BarSeries& series, size_t from, size_t to) {
        if (series.empty() || from >= to || to > series.size()) {
            throw std::invalid_argument("Series empty or invalid range");
        }

        std::ofstream out(output_path_, append_ ? std::ios::app : std::ios::trunc);
        if (!out.is_open()) {
            throw std::runtime_error("Failed to open output file: " + output_path_);
        }
        if(!append_) {
            writeHeader(out);
        }
        
        for (size_t i = from; i < to; ++i) {
            const auto& bar = series[i];
            out << bar.ts_ << "," 
            << bar.open_ << "," 
            << bar.high_ << ","
            << bar.low_ << "," 
            << bar.close_ << ","
            << bar.volume_ << "\n";
        }
    }

    void DataExporter::writeHeader(std::ofstream& out) {
        if (!append_) {
            out << "timestamp,open,high,low,close,volume\n";
        }
    }

}   // namespace export_util
