#pragma once

namespace backtest {

    struct ExecParams {
        double commission_fixed = 0.0;  ///< per trade, currency units
        double commission_bps   = 0.0;  ///< per trade, bps of national (value * bps/10000)
        double slippage_bps     = 0.0;  ///> price adjust in bps: BUY -> +bps, SELL -> -bps
    };

    inline double applySlippage(double px, double bps, bool is_buy) {
        const double factor = bps / 10000.0;
        return is_buy ? px * (1.0 + factor) : px * (1.0 -factor);
    }

    inline double commissionCost(double px, double qty, double fixed, double bps) {
        const double notional = px * qty;
        return fixed + (bps / 10000.0) * notional;
    }  

}   // namespace backtest