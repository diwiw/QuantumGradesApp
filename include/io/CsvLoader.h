// src/io/CsvLoader.h
#pragma once
#include <string>
#include "domain/backtest/BarSeries.h"
namespace io { 
    bool load_csv(const std::string& path, backtest::BarSeries& out);
    
}
