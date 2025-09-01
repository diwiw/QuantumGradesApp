/**
 * @file Trade.hpp
 * @brief Models an executed trade (fill) resulting from an Order.
 */

#pragma once

#include <chrono>
#include "domain/backtest/Order.hpp"
#include "domain/backtest/BarSeries.hpp"


namespace backtest {

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
     */
    Trade(const Order& order,
          double executed_price,
          double executed_quantity,
          std::chrono::system_clock::time_point ts = std::chrono::system_clock::now());

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

  private:
     Order   order_;    ///< Original order metadata.
    double  price_{0.0};     ///< Execution price.
    double  quantity_{0.0};  ///< Executed quantity.
    std::chrono::system_clock::time_point ts_{}; ///< Execution timestamp.
  };

} // namespace backtest

