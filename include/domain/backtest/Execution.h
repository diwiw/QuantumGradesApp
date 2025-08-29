/**
 * @file Execution.h
 * @brief Order execution helpers: slippage and fill price modeling.
 *
 * Provides utility functions to model realistic order execution behavior
 * in backtests, including slippage and commission costs.
 */
#pragma once

namespace backtest {
    /**
     * @brief Parameters used to simulate realistic execution costs.
     *
     * Includes both fixed and proportional commissions, as well as slippage 
     * adjustments for buy/sell orders.
     */
    struct ExecParams {
    double commission_fixed_ = 0.0;  ///< Fixed commission per trade (currency units).
    double commission_bps_   = 0.0;  ///< Commission in basis points (bps) of notional value.
    double slippage_bps_     = 0.0;  ///< Price slippage in bps: BUY -> +bps, SELL -> -bps.
    };
    /**
     * @brief Applies slippage to a reference price.
     *
     * @param px      Reference price (e.g. mid or last).
     * @param bps     Slippage in basis points (1 bps = 0.01%).
     * @param is_buy  Indicates whether this is a buy (true) or sell (false) order.
     * @return Adjusted price including slippage.
     *
     * For example, with `px = 100.0` and `bps = 10`:
     * - Buy:  → 100.10
     * - Sell: →  99.90
     */
    inline double applySlippage(double px, double bps, bool is_buy) {
        const double FACTOR = bps / 10000.0;
        return is_buy ? px * (1.0 + FACTOR) : px * (1.0 -FACTOR);
    }
    /**
     * @brief Computes the total commission cost of a trade.
     *
     * @param px     Execution price.
     * @param qty    Quantity traded.
     * @param fixed  Fixed commission (currency units).
     * @param bps    Proportional commission in bps of notional value.
     * @return Total cost of commission.
     */
    inline double commissionCost(double px, double qty, double fixed, double bps) {
        const double NOTIONAL = px * qty;
        return fixed + (bps / 10000.0) * NOTIONAL;
    }  

}   // namespace backtest