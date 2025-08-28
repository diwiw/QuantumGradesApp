#pragma once

namespace backtest {

    struct ExecParams {
        double commission_fixed_ = 0.0;  ///< per trade, currency units
        double commission_bps_   = 0.0;  ///< per trade, bps of national (value * bps/10000)
        double slippage_bps_     = 0.0;  ///> price adjust in bps: BUY -> +bps, SELL -> -bps
    };

    inline double applySlippage(double px, double bps, bool is_buy) {
        const double FACTOR = bps / 10000.0;
        return is_buy ? px * (1.0 + FACTOR) : px * (1.0 -FACTOR);
    }

    inline double commissionCost(double px, double qty, double fixed, double bps) {
        const double NOTIONAL = px * qty;
        return fixed + (bps / 10000.0) * NOTIONAL;
    }  

}   // namespace backtest