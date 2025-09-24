#include "domain/backtest/Trade.hpp"
#include <stdexcept>

namespace qga::domain::backtest {

    Trade::Trade(const Order& order,
        double executed_price,
        double executed_quantity,
        std::chrono::system_clock::time_point ts) : order_(order),
        price_(executed_price),
        quantity_(executed_quantity),
        ts_(ts)
    {
        if(price_ <= 0.0)   throw std::invalid_argument("Trade price must be > 0");
        if(quantity_ <= 0.0)    throw std::invalid_argument("Trade quantity must be > 0");    
    }

}   // namespace qga::domain::backtest