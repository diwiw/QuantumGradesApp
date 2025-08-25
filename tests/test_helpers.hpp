#pragma once
#include <cstdint>
#include <vector>
#include "domain/Quote.h"
#include "domain/backtest/BarSeries.h"

namespace testlib {

inline domain::Quote bar(double c, std::int64_t ts, double vol = 0.0) {
    return {ts, c, c, c, c, vol};
}

inline backtest::BarSeries make_series(const std::vector<double>& closes,
                                       std::int64_t ts0 = 0,
                                       std::int64_t step_ms = 60'000) {
    backtest::BarSeries s;
    std::int64_t ts = ts0;
    for (double c : closes) {
        s.add(bar(c, ts));
        ts += step_ms;
    }
    return s;
}

} // namespace testlib
