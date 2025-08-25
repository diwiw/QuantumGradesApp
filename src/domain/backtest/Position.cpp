#include "domain/backtest/Position.h"

namespace backtest{

    Position::Position(domain::Instrument instrument)
    : instrument_ (std::move(instrument)) {}

    void Position::apply_fill(double fill_price, double fill_qty, bool is_buy) {
        if (fill_price <= 0.0)  throw std::invalid_argument("fill_price must be > 0");
        if (fill_qty <= 0.0)  throw std::invalid_argument("fill_qty must be > 0");

        if(is_buy){
            const double new_qty = qty_ + fill_qty;
            const double new_avg = (qty_ * avg_price_ + fill_qty * fill_price) / new_qty;
            qty_ = new_qty;
            avg_price_ = new_avg;
        } else {
            const double close_qty = std::min(qty_, fill_qty);
            realized_pnl_ += (fill_price - avg_price_) * close_qty;
            qty_ -= close_qty;
            if (qty_ == 0.0) avg_price_ = 0.0;
            // shorting/over-sell can be added later if needed
        }
        
    }

}   // namespace backtest