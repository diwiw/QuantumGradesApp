#pragma once
#include "domain/Instrument.h"
#include <algorithm>
#include <stdexcept>

namespace backtest {

    class Position {
    public:
        explicit Position(domain::Instrument instrument);

        const domain::Instrument& instrument() const noexcept { return instrument_; }
        double qty() const noexcept { return qty_; }
        double avgPrice() const noexcept { return avg_price_; }
        double realizedPnl() const noexcept { return realized_pnl_; }

        void applyFill(double fill_price, double fill_qty, bool is_buy);

        double unrealizedPnl(double mark_price) const noexcept {
            return (mark_price - avg_price_) * qty_;
        }
    private:
        domain::Instrument instrument_;
        double qty_{0.0};
        double avg_price_{0.0};
        double realized_pnl_{0.0};
    };

}   // namespace backtest