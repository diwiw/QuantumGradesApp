/**
 * @file Position.hpp
 * @brief Models an open trading position for a single instrument.
 *
 * Tracks quantity, average entry price, and realized/unrealized PnL.
 * Used by the Portfolio during trade applications.
 */

#pragma once

#include "domain/Instrument.hpp"
#include <algorithm>
#include <stdexcept>

namespace qga::domain::backtest {


/**
 * @class Position
 * @brief Represents a trading position for a specific instrument.
 *
 * Tracks open quantity, average price, and realized profit/loss.
 * Supports fills (buy/sell) and provides mark-to-market valuation.
 */
class Position {
public:
    /**
     * @brief Constructs an empty position for the specified instrument.
     * @param instrument Financial instrument for this position.
     */
    explicit Position(domain::Instrument instrument);

    /**
     * @brief Returns a reference to the underlying instrument.
     */
    const domain::Instrument& instrument() const noexcept { return instrument_; }

    /**
    * @brief Returns current position quantity (positive = long, negative = short).
    */
    double qty() const noexcept { return qty_; }

    /**
    * @brief Returns the average entry price of the current position.
    */
    double avgPrice() const noexcept { return avg_price_; }

    /**
    * @brief Returns realized profit/loss from completed round trips.
    */
    double realizedPnl() const noexcept { return realized_pnl_; }

        /**
     * @brief Applies a fill (buy or sell) to the position.
     * @param fill_price Executed price.
     * @param fill_qty   Quantity filled.
     * @param is_buy     True if buy; false if sell.
     */
    void applyFill(double fill_price, double fill_qty, bool is_buy);

    /**
     * @brief Computes the unrealized (mark-to-market) PnL.
     * @param mark_price Current market price.
     * @return Unrealized profit/loss.
     */
    double unrealizedPnl(double mark_price) const noexcept {
        return (mark_price - avg_price_) * qty_;
    }

private:
    domain::Instrument instrument_;  ///< Instrument metadata.
    double qty_{0.0};                ///< Net position size.
    double avg_price_{0.0};          ///< Average entry price.
    double realized_pnl_{0.0};       ///< Realized profit/loss.
};


} // namespace qga::domain::backtest
