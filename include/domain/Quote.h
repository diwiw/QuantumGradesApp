/**
 * @file Quote.h
 * @brief Single OHLCV bar (market candle) in time series.
 */
#pragma once
#include <cstdint>

namespace domain {
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
        std::int64_t ts{};                      ///< Epoch millis (convention in this project).
        double open{}, high{}, low{}, close{};  ///< OHLC prices.
        double volume{};                        ///< Bar volume.
    };
} // namespace domain