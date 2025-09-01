#include "domain/backtest/Order.hpp"
#include "domain/backtest/BarSeries.hpp"
#include <stdexcept>

using namespace domain;

namespace backtest{

    Order::Order(Instrument instrument,
        Side side,
        double quantity,
        OrderType type,
        std::chrono::system_clock::time_point ts)

        : instrument_(std::move(instrument)),
        side_(side),
        type_(type),
        quantity_(quantity),
        ts_(ts)
    {
        if (quantity_ <= 0.0) {
            throw std::invalid_argument("Order quantity must be > 0");
        }
    }
}   // namespace backtest