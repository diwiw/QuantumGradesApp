#include "domain/backtest/Portfolio.h"

namespace backtest {

    std::string Portfolio::key_for(const domain::Instrument& ins) {
        //Assumes your Instrument exposes symbol() and exchange_mic()
        return ins.symbol() + "@" + ins.exchange_mic();
    }

    Position& Portfolio::get_or_create(const domain::Instrument& ins) {
        auto k = key_for(ins);
        auto it = positions_.find(k);
        if(it == positions_.end()) {
            it = positions_.emplace(k, Position{ins}).first;        
        }
        return it->second;
    }

    void Portfolio::apply_trade(const Trade& t) {
        cash_ += t.signed_cash(); // Buy -> cash down, Sell -> cash up
        auto& pos = get_or_create(t.order().instrument());
        pos.apply_fill(t.price(), t.quantity(), t.side() == Side::Buy);
        realized_pnl_ = aggregate_realized();
    }

    double Portfolio::nav_for(const domain::Instrument& ins, double mark_price) const {
        const auto k = key_for(ins);
        double pos_val = 0.0;
        if(auto it = positions_.find(k); it != positions_.end()) {
            const auto& p = it ->second;
            pos_val = p.qty() * p.avg_price() + p.unrealized_pnl(mark_price);
        }
        return cash_ + pos_val;
    }

    double Portfolio::aggregate_realized() const {
        double sum = 0.0;
        for(auto const& kv : positions_) sum += kv.second.realized_pnl();
        return sum;
    }
}   // namespace backtest