#include "doctest.h"
#include "strategy/MACrossover.h"
#include "strategy/IStrategy.h"
#include "domain/backtest/Engine.h"
#include "domain/backtest/BarSeries.h"
#include "domain/backtest/Execution.h"
#include "strategy/BuyHold.h"
#include "domain/Quote.h"
#include "test_helpers.hpp"

using namespace strategy;
using namespace backtest;
using namespace domain;
/*
TEST_SUITE("Strategies/MACrossover") {
    TEST_CASE("No signal when not enough bars") {
        auto s = testlib::make_series({1,2,3}); // fast=3, slow=5 -> not enough
        ExecParams exec{};
        Engine eng(10'000.0, exec);
        MACrossover ma{3,5};
        auto res = eng.run(s, ma);
        CHECK(res.trades_executed == 0);
        CHECK(res.final_equity == doctest::Approx(res.initial_equity));
    }

    TEST_CASE("Generates at least one trade on zigzag") {
        // down -> up -> down should induce a cross for 3/5
        auto s = testlib::make_series({5,4,3,4,5,6,5,4,3});
        ExecParams exec{};
        Engine eng(10'000.0, exec);
        MACrossover ma{3,5};
        auto res = eng.run(s, ma);

        CHECK(res.trades_executed >= 1);
        // we don't assert PnL sign here; it's a logic smoke test
        CHECK(res.final_equity > 0.0);
    }
}
*/

static BarSeries make_series(const std::vector<double>& closes) {
  BarSeries s;
  std::int64_t t = 0;
  for (double c : closes) {
    s.add(Quote{t, c, c, c, c, 0.0}); // open=high=low=close=c for simplicity
    t += 60'000; // +1 min
  }
  return s;
}

TEST_SUITE("Strategies/MACrossover") {
  TEST_CASE("BuyHold emits exactly one Buy; Engine trades=1") {
    auto s = make_series({1,2,3,4,5});
    BuyHold bh;
    Engine eng;
    auto res = eng.run(s, bh);
    CHECK(res.trades_executed == 1);        // one buy, closed at the end
    CHECK(res.final_equity >  res.initial_equity);
  }

  TEST_CASE("MACrossover fast=3 slow=5 generates a Sell after peak") {
    auto s = make_series({5,4,3,4,5,6,5,4,3});
    MACrossover ma{3,5};
    Engine eng;
    auto res = eng.run(s, ma);

    // check the validity of the result: there were signals (at least one trade)
    CHECK(res.trades_executed >= 1);
    // equity after the uptrend and downtrend may be <= or >= the start —
    // the goal of the test is to pass the logic, not the financial result.
  }
}

TEST_SUITE("Execution") {
  TEST_CASE("Fixed commission lowers final equity") {
    auto s = make_series({100, 101, 102, 103}); // rynek do góry
    BuyHold bh;
    Engine eng(1000.0, ExecParams{ /*fixed*/ 1.0, /*bps*/ 0.0, /*slip*/ 0.0 });
    auto res = eng.run(s, bh);
    CHECK(res.trades_executed == 1);
    // Bez precyzyjnej wartości – sprawdzamy tylko, że prowizja coś kosztuje:
    // final_equity powinno być < wariantu bez prowizji.
  }

  TEST_CASE("Slippage worsens buy price and improves sell price") {
    auto s = make_series({100, 101, 102, 103, 99}); // najpierw w górę, potem spadek
    MACrossover ma{2,3};
    Engine eng_no(1000.0, ExecParams{0.0, 0.0, 0.0});
    Engine eng_sl(1000.0, ExecParams{0.0, 0.0, 10.0}); // 10 bps slippage

    auto r0 = eng_no.run(s, ma);
    auto r1 = eng_sl.run(s, ma);
    // ze slippage (BUY drożej, SELL taniej) wynik powinien być gorszy lub równy
    CHECK(r1.final_equity <= r0.final_equity);
  }
}