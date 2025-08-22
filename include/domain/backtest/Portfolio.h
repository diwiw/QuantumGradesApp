#pragma once
#include <unordered_map>
#include <string>
#include "domain/backtest/Position.h"
#include "domain/backtest/Trade.h"

namespace backtest {

    class Portfolio {
    public:
        explicit Portfolio(double starting_cash = 10000.0) : cash_(starting_cash) {}

        double cash() const noexcept { return cash_; }
        double realized_pnl() const noexcept { return realized_pnl_; }

        Position& get_or_create(const domain::Instrument& ins);

        void apply_trade(const Trade& t);

        double nav_for(const domain::Instrument& ins, double mark_price) const;

    private:
            static std::string key_for(const domain::Instrument& ins);

            double aggregate_realized() const;

            std::unordered_map<std::string, Position> positions_;
            double cash_{0.0};
            double realized_pnl_{0.0};
    };

} // namespace backtest