#include "domain/backtest/Position.hpp"

namespace qga::domain::backtest{

    Position::Position(domain::Instrument instrument)
    : instrument_ (std::move(instrument)) {}

    void Position::applyFill(double fill_price, double fill_qty, bool is_buy) {
        if (fill_price <= 0.0)  throw std::invalid_argument("fill_price must be > 0");
        if (fill_qty <= 0.0)  throw std::invalid_argument("fill_qty must be > 0");

        if(is_buy){
            const double NEW_QTY = qty_ + fill_qty;
            const double NEW_AVG = (qty_ * avg_price_ + fill_qty * fill_price) / NEW_QTY;
            qty_ = NEW_QTY;
            avg_price_ = NEW_AVG;
        } else {
            const double CLOSE_QTY = std::min(qty_, fill_qty);
            realized_pnl_ += (fill_price - avg_price_) * CLOSE_QTY;
            qty_ -= CLOSE_QTY;
            if (qty_ == 0.0) avg_price_ = 0.0;
            // shorting/over-sell can be added later if needed
        }

    }

}   // namespace qga::domain::backtest
