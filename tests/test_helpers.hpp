#pragma once
#include <cstdint>
#include <vector>
#include "domain/Quote.hpp"
#include "domain/backtest/BarSeries.hpp"

namespace testlib {

inline qga::domain::Quote bar(double c, std::int64_t ts, double vol = 0.0) {
    return {ts, c, c, c, c, vol};
}

inline qga::domain::backtest::BarSeries makeSeries(const std::vector<double>& closes,
                                       std::int64_t ts0 = 0,
                                       std::int64_t step_ms = 60'000) {
    qga::domain::backtest::BarSeries s;
    std::int64_t ts = ts0;
    for (double c : closes) {
        s.add(bar(c, ts));
        ts += step_ms;
    }
    return s;
}

} // namespace testlib
