#pragma once
#include <memory>
#include "domain/Quote.hpp"
#include "domain/backtest/Trade.hpp"
#include "domain/backtest/Portfolio.hpp"

namespace qga::reporting {

/**
 * @interface IReporter
 * @brief Observer interface for receiving notifications about backtest events.
 */
class IReporter {
public:
    virtual ~IReporter() = default;

    /// Called after each processed market quote (OHLCV bar)
    virtual void onQuote(const domain::Quote& quote) = 0;

    /// Called after each trade execution
    virtual void onTrade(const domain::backtest::Trade& trade) = 0;

    /// Called after portfolio summary (e.g. end of backtest)
    virtual void onSummary(const domain::backtest::Portfolio& portfolio) = 0;
};

} // namespace qga::reporting
