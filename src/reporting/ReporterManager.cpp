#include "reporting/ReporterManager.hpp"

namespace qga::reporting {

void ReporterManager::addReporter(std::shared_ptr<IReporter> reporter) {
    reporters_.push_back(std::move(reporter));
}

void ReporterManager::notifyQuote(const domain::Quote& quote) {
    for (auto& r : reporters_) r->onQuote(quote);
}

void ReporterManager::notifyTrade(const domain::backtest::Trade& trade) {
    for (auto& r : reporters_) r->onTrade(trade);
}

void ReporterManager::notifySummary(const domain::backtest::Portfolio& portfolio) {
    for (auto& r : reporters_) r->onSummary(portfolio);
}

} // namespace qga::reporting
