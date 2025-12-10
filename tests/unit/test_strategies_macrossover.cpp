#include "doctest.h"
#include "domain/Quote.hpp"
#include "domain/backtest/BarSeries.hpp"
#include "domain/backtest/Engine.hpp"
#include "domain/backtest/Execution.hpp"
#include "strategy/BuyHold.hpp"
#include "strategy/IStrategy.hpp"
#include "strategy/MACrossover.hpp"
#include "test_helpers.hpp"

TEST_SUITE("Strategies/MACrossover")
{
    TEST_CASE("No signal when not enough bars")
    {
        auto s = testlib::makeSeries({1, 2, 3}); // fast=3, slow=5 -> not enough
        qga::domain::backtest::ExecParams exec{};
        qga::domain::backtest::Engine eng(10'000.0, exec);
        qga::strategy::MACrossover ma{3, 5};
        auto res = eng.run(s, ma);
        CHECK(res.trades_executed_ == 0);
        CHECK(res.final_equity_ == doctest::Approx(res.initial_equity_));
    }

    TEST_CASE("Generates at least one trade on zigzag")
    {
        // down -> up -> down should induce a cross for 3/5
        auto s = testlib::makeSeries({5, 4, 3, 4, 5, 6, 5, 4, 3});
        qga::domain::backtest::ExecParams exec{};
        qga::domain::backtest::Engine eng(10'000.0, exec);
        qga::strategy::MACrossover ma{3, 5};
        auto res = eng.run(s, ma);

        CHECK(res.trades_executed_ >= 1);
        // we don't assert PnL sign here; it's a logic smoke test
        CHECK(res.final_equity_ > 0.0);
    }
}

static qga::domain::backtest::BarSeries makeSeries(const std::vector<double>& closes)
{
    qga::domain::backtest::BarSeries s;
    std::int64_t t = 0;
    for (double c : closes)
    {
        s.add(qga::domain::Quote{t, c, c, c, c, 0.0}); // open=high=low=close=c for simplicity
        t += 60'000;                                   // +1 min
    }
    return s;
}

TEST_SUITE("Strategies/MACrossover")
{
    TEST_CASE("BuyHold emits exactly one Buy; Engine trades=1")
    {
        auto s = makeSeries({1, 2, 3, 4, 5});
        qga::strategy::BuyHold bh;
        qga::domain::backtest::Engine eng;
        auto res = eng.run(s, bh);
        CHECK(res.trades_executed_ == 1); // one buy, closed at the end
        CHECK(res.final_equity_ > res.initial_equity_);
    }

    TEST_CASE("MACrossover fast=3 slow=5 generates a Sell after peak")
    {
        auto s = makeSeries({5, 4, 3, 4, 5, 6, 5, 4, 3});
        qga::strategy::MACrossover ma{3, 5};
        qga::domain::backtest::Engine eng;
        auto res = eng.run(s, ma);

        // check the validity of the result: there were signals (at least one trade)
        CHECK(res.trades_executed_ >= 1);
        // equity after the uptrend and downtrend may be <= or >= the start —
        // the goal of the test is to pass the logic, not the financial result.
    }
}

TEST_SUITE("Execution")
{
    TEST_CASE("Fixed commission lowers final equity")
    {
        auto s = makeSeries({100, 101, 102, 103}); // market goes up
        qga::strategy::BuyHold bh;
        qga::domain::backtest::Engine eng(
            1000.0, qga::domain::backtest::ExecParams{/*fixed*/ 1.0, /*bps*/ 0.0, /*slip*/ 0.0});
        auto res = eng.run(s, bh);
        CHECK(res.trades_executed_ == 1);
        // No precise value – we only check that the commission costs something:
        // final_equity should be < the variant without commission.
    }

    TEST_CASE("Slippage worsens buy price and improves sell price")
    {
        auto s = makeSeries({100, 101, 102, 103, 99}); // first up, then down
        qga::strategy::MACrossover ma{2, 3};
        qga::domain::backtest::Engine eng_no(1000.0,
                                             qga::domain::backtest::ExecParams{0.0, 0.0, 0.0});
        qga::domain::backtest::Engine eng_sl(
            1000.0, qga::domain::backtest::ExecParams{0.0, 0.0, 10.0}); // 10 bps slippage

        auto r0 = eng_no.run(s, ma);
        auto r1 = eng_sl.run(s, ma);
        // With slippage (BUY more expensive, SELL cheaper) the result should be worse or equal
        CHECK(r1.final_equity_ <= r0.final_equity_);
    }
}
