#pragma once
#include <chrono>
#include "domain/backtest/Order.h"
#include "domain/backtest/BarSeries.h"


namespace backtest {

  /**
   * @brief Immutable execution (fill) resulting from an Order.
   * SRP: records what was executed (price/qty/side/time).
   */
  class Trade {
  public:
    Trade(const Order& order,
          double executed_price,
          double executed_quantity,
          std::chrono::system_clock::time_point ts = std::chrono::system_clock::now());

    const Order&   order()    const noexcept { return order_; }
    double         price()    const noexcept { return price_; }
    double         quantity() const noexcept { return quantity_; }
    Side           side()     const noexcept { return order_.side(); }
    std::chrono::system_clock::time_point timestamp() const noexcept { return ts_; }

    /// Cash delta: Buy -> negative, Sell -> positive.
    double signed_cash() const noexcept {
      return (side() == Side::Buy ? -price_ * quantity_ : price_ * quantity_);
    }

  private:
    Order   order_;
    double  price_{0.0};
    double  quantity_{0.0};
    std::chrono::system_clock::time_point ts_{};
  };

} // namespace backtest

