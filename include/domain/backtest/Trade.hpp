/**
 * @file Trade.hpp
 * @brief Models an executed trade (fill) resulting from an Order.
 */

#pragma once

#include <chrono>
#include "domain/backtest/Order.hpp"
#include "domain/backtest/BarSeries.hpp"

namespace qga::domain::backtest {


/**
 * @class Trade
 * @brief Immutable record of an executed order (fill).
 *
 * Contains details such as price, quantity, time of execution,
 * and the original order metadata. Used in backtest accounting.
 */
class Trade {
public:
  /**
   * @brief Constructs a new Trade instance based on an Order.
   * @param order             The original order that was executed.
   * @param executed_price    Price at which the trade was executed.
   * @param executed_quantity Quantity that was filled.
   * @param ts                Optional timestamp of execution (default: now).
   * @param entry_price       Optional reference (entry) price used for PnL computation.
   *                          - For buy trades: `pnl = (executed_price - entry_price) * quantity`
   *                          - For sell trades: `pnl = (entry_price - executed_price) * quantity`
   *                          - If omitted or zero, `pnl()` will return 0.0.
   *
   * @throws std::invalid_argument if `executed_price` or `executed_quantity` ≤ 0.
   *
   * @details
   * - For Buy:  `(exit_price - entry_price) * quantity`
   * - For Sell: `(entry_price - exit_price) * quantity`
   * - Returns 0.0 if `entry_price_` is unset or zero.
   */
  Trade(const Order& order,
        double executed_price,
        double executed_quantity,
        std::chrono::system_clock::time_point ts = std::chrono::system_clock::now(),
        double entry_price = 0.0)
      : order_(order),
        price_(executed_price),
        quantity_(executed_quantity),
        entry_price_(entry_price),
        ts_(ts)
  {
      if (price_ <= 0.0)
          throw std::invalid_argument("Trade: executed_price must be > 0");
      if (quantity_ <= 0.0)
          throw std::invalid_argument("Trade: executed_quantity must be > 0");
  }

  #ifdef UNIT_TEST
    Trade() = default; // only for test mocks
  #endif


  /**
   * @brief Returns the original order associated with this trade.
   */
  const Order&   order()    const noexcept { return order_; }

  /**
   * @brief Executed price of the trade.
   */
  double         price()    const noexcept { return price_; }

  /**
   * @brief Executed quantity of the trade.
   */
  double         quantity() const noexcept { return quantity_; }

  /**
   * @brief Side of the trade (Buy or Sell).
   */
  Side           side()     const noexcept { return order_.side(); }

  /**
   * @brief Timestamp of execution.
   */
  std::chrono::system_clock::time_point timestamp() const noexcept { return ts_; }

  /**
   * @brief Signed cash flow associated with the trade.
   * @return Negative value for Buy (cash out), positive for Sell (cash in).
   */
  double signedCash() const noexcept {
    return (side() == Side::Buy ? -price_ * quantity_ : price_ * quantity_);
  }

  /**
     * @brief Calculates profit or loss relative to order entry price.
     * @details
     *  - For Buy:  (exit_price - entry_price) * quantity
     *  - For Sell: (entry_price - exit_price) * quantity
     */
    double pnl() const noexcept {
        if (entry_price_ <= 0.0) return 0.0;
        if (side() == Side::Buy)
            return (price_ - entry_price_) * quantity_;
        else
            return (entry_price_ - price_) * quantity_;
    }
private:
    Order   order_;    ///< Original order metadata.
  double  price_{0.0};     ///< Execution price.
  double  quantity_{0.0};  ///< Executed quantity.
  double entry_price_{0.0}; ///< Entry price from the original order.
  std::chrono::system_clock::time_point ts_{}; ///< Execution timestamp.
};

} // namespace qga::domain::backtest
