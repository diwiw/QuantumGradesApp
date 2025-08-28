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
        double realizedPnl() const noexcept { return realized_pnl_; }

        Position& getOrCreate(const domain::Instrument& ins);

        void applyTrade(const Trade& t);

        double navFor(const domain::Instrument& ins, double mark_price) const;

    private:
            static std::string keyFor(const domain::Instrument& ins);

            double aggregateRealized() const;

            std::unordered_map<std::string, Position> positions_;
            double cash_{0.0};
            double realized_pnl_{0.0};
    };

} // namespace backtest