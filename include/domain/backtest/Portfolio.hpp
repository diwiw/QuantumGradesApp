/**
 * @file Portfolio.hpp
 * @brief Tracks current portfolio state and applies trades in the backtest engine.
 *
 * Manages positions, cash, and realized profit/loss during a simulation.
 */

#pragma once

#include <unordered_map>
#include <string>
#include "domain/backtest/Position.hpp"
#include "domain/backtest/Trade.hpp"

namespace qga::domain::backtest {


/**
 * @class Portfolio
 * @brief Tracks position and cash state during a backtest.
 *
 * Stores and updates open positions per instrument, applies trades, and computes
 * realized PnL and cash flow. Supports multiple instruments through internal mapping.
 */
class Portfolio {
public:
    /**
    * @brief Constructs a Portfolio with given starting capital.
    * @param starting_cash Initial cash available for trading.
    */
    explicit Portfolio(double starting_cash = 10000.0) : cash_(starting_cash) {}

        /**
     * @brief Returns current cash balance.
     */
    double cash() const noexcept { return cash_; }

    /**
     * @brief Returns total realized profit/loss.
     */
    double realizedPnl() const noexcept { return realized_pnl_; }

    /**
     * @brief Access or create a position for the given instrument.
     * @param ins Financial instrument.
     * @return Reference to modifiable position.
     */
    Position& getOrCreate(const domain::Instrument& ins);

    /**
     * @brief Applies a trade to the portfolio.
     * @param t Trade to be executed (affects position, cash, PnL).
     */
    void applyTrade(const Trade& t);

    /**
     * @brief Computes the mark-to-market value for an instrument.
     * @param ins Instrument to evaluate.
     * @param mark_price Latest price for that instrument.
     * @return Net asset value contributed by that position.
     */
    double navFor(const domain::Instrument& ins, double mark_price) const;

     /**
     * @brief Returns the current total portfolio value (placeholder).
     *
     * @details
     * For now, returns 0.0 until full accounting logic is implemented.
     * In the future, should sum: cash + Σ(position.value()).
     */
    double totalValue() const noexcept{
        double total = cash_;
        for (const auto& kv : positions_) {
            total += kv.second.qty() * kv.second.avgPrice();
        }
        return total;
    }

private:
    /**
     * @brief Generates a unique key string for a given instrument.
     * Used internally for position mapping.
     */
    static std::string keyFor(const domain::Instrument& ins);

    /**
     * @brief Aggregates total realized PnL across all closed positions.
     * @return Total realized profit/loss.
     */
    double aggregateRealized() const;

    std::unordered_map<std::string, Position> positions_;   ///< Active positions by instrument.
    double cash_{0.0};                                       ///< Available cash.
    double realized_pnl_{0.0};                                  ///< Accumulated realized PnL.
};


} // namespace qga::domain::backtest
