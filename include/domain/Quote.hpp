/**
 * @file Quote.hpp
 * @brief Single OHLCV bar (market candle) in time series.
 */
#pragma once
#include <cstdint>

namespace qga::domain {

/**
 * @struct Quote
 * @brief Immutable market bar with timestamp and OHLCV fields.
 *
 * @var Quote::ts
 *   Epoch milliseconds (or seconds) representing the bar time.
 * @var Quote::open/high/low/close
 *   Price components for the bar.
 * @var Quote::volume
 *   Traded volume during the bar.
 */
struct Quote{
    std::int64_t ts_{};                      ///< Epoch millis (convention in this project).
    double open_{}, high_{}, low_{}, close_{};  ///< OHLC prices.
    double volume_{};                        ///< Bar volume.
};

} // namespace qga::domain
