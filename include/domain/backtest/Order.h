#pragma once
#include "domain/Instrument.h"
#include <string>
#include <chrono>

namespace domain {
    enum class Side { Buy, Sell };


    enum class OrderType { Market };

    class Order {
        public:
        Order(domain::Instrument instrument,
            Side side,
            double quantity,
            OrderType type = OrderType::Market,
            std::chrono::system_clock::time_point ts = std::chrono::system_clock::now());

        const domain::Instrument& instrument() const noexcept { return instrument_; }
        Side side() const noexcept { return side_; }
        OrderType type() const noexcept { return type_; }
        double quantity() const noexcept { return quantity_ ; }
        std::chrono::system_clock::time_point timestamp() const noexcept { return ts_; }

        bool valid() const noexcept { return quantity_ > 0.0; }

        private:
        domain::Instrument instrument_;         // copy of instrument metadata
        Side side_{Side::Buy};
        OrderType type_{OrderType::Market};
        double quantity_{0.0};
        std::chrono::system_clock::time_point ts_{};
    };
}   // namespace backtest