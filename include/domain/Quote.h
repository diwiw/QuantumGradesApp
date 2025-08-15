#pragma once
#include <cstdint>

namespace domain {

    struct Quote{
        std::int64_t ts{};
        double open{}, high{}, low{}, close{};  ///< OHLC prices.
        double volume{};
    };
} // namespace domain