#include "doctest.h"
#include "reporting/IReporter.hpp"
#include "reporting/ReporterManager.hpp"
#include "domain/Quote.hpp"
#include "domain/backtest/Trade.hpp"
#include "domain/backtest/Portfolio.hpp"

using namespace qga::reporting;

struct MockReporter : public IReporter {
    int quotes_ = 0, trades_ = 0, summaries_ = 0;
    void onQuote(const qga::domain::Quote&) override { quotes_++; }
    void onTrade(const qga::domain::backtest::Trade&) override { trades_++; }
    void onSummary(const qga::domain::backtest::Portfolio&) override { summaries_++; }
};

TEST_CASE("ReporterManager notifies all reporters correctly") {
    ReporterManager mgr;
    auto r1 = std::make_shared<MockReporter>();
    auto r2 = std::make_shared<MockReporter>();
    mgr.addReporter(r1);
    mgr.addReporter(r2);

    qga::domain::Quote q{};
    qga::domain::backtest::Trade t{};
    qga::domain::backtest::Portfolio p{};

    mgr.notifyQuote(q);
    mgr.notifyTrade(t);
    mgr.notifySummary(p);

    CHECK(r1->quotes_ == 1);
    CHECK(r1->trades_ == 1);
    CHECK(r1->summaries_ == 1);
    CHECK(r2->quotes_ == 1);
    CHECK(r2->trades_ == 1);
    CHECK(r2->summaries_ == 1);
}
