// src/io/CsvLoader.cpp
#include "io/CsvLoader.hpp"
#include <fstream>
#include <sstream>

namespace io {
    bool loadCsv(const std::string& path, backtest::BarSeries& out) {
    std::ifstream in(path);
    if (!in) return false;
    std::string line;
    // oczekiwany format: ts,open,high,low,close,volume (z lub bez nagłówka)
    // jeśli masz nagłówek – wczytaj pierwszą linię i pomiń.
    std::getline(in, line);
    if (line.find("ts") == std::string::npos) { // brak nagłówka – użyj pierwszej linii
        std::istringstream ss(line);
        domain::Quote q{};
        char comma;
        ss >> q.ts_ >> comma >> q.open_ >> comma >> q.high_ >> comma >> q.low_ >> comma >> q.close_ >> comma >> q.volume_;
        if (ss) out.add(q);
    }
    while (std::getline(in, line)) {
        std::istringstream ss(line);
        domain::Quote q{};
        char comma;
        ss >> q.ts_ >> comma >> q.open_ >> comma >> q.high_ >> comma >> q.low_ >> comma >> q.close_ >> comma >> q.volume_;
        if (ss) out.add(q);
    }
    return out.size() > 0;
    }
}
// src/io/CsvLoader.cpp