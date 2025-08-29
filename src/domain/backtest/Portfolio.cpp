#include "domain/backtest/Portfolio.h"

namespace backtest {

    std::string Portfolio::keyFor(const domain::Instrument& ins) {
        //Assumes your Instrument exposes symbol() and exchange_mic()
        return ins.symbol() + "@" + ins.exchangeMic();
    }

    Position& Portfolio::getOrCreate(const domain::Instrument& ins) {
        auto k = keyFor(ins);
        auto it = positions_.find(k);
        if(it == positions_.end()) {
            it = positions_.emplace(k, Position{ins}).first;        
        }
        return it->second;
    }

    void Portfolio::applyTrade(const Trade& t) {
        cash_ += t.signedCash(); // Buy -> cash down, Sell -> cash up
        auto& pos = getOrCreate(t.order().instrument());
        pos.applyFill(t.price(), t.quantity(), t.side() == Side::Buy);
        realized_pnl_ = aggregateRealized();
    }

    double Portfolio::navFor(const domain::Instrument& ins, double mark_price) const {
        const auto K = keyFor(ins);
        double pos_val = 0.0;
        if(auto it = positions_.find(K); it != positions_.end()) {
            const auto& p = it ->second;
            pos_val = p.qty() * p.avgPrice() + p.unrealizedPnl(mark_price);
        }
        return cash_ + pos_val;
    }

    double Portfolio::aggregateRealized() const {
        double sum = 0.0;
        for(auto const& kv : positions_) sum += kv.second.realizedPnl();
        return sum;
    }
}   // namespace backtest