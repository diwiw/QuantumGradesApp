#pragma once
#include <vector>
#include <memory>
#include "reporting/IReporter.hpp"

namespace qga::reporting {

/**
 * @class ReporterManager
 * @brief Manages a collection of reporters and dispatches events to them.
 */
class ReporterManager {
public:
    void addReporter(std::shared_ptr<IReporter> reporter);

    void notifyQuote(const domain::Quote& quote);
    void notifyTrade(const domain::backtest::Trade& trade);
    void notifySummary(const domain::backtest::Portfolio& portfolio);

private:
    std::vector<std::shared_ptr<IReporter>> reporters_;
};

} // namespace qga::reporting
