#include "doctest.h"
#include "domain/Instrument.hpp"
#include "domain/backtest/Trade.hpp"
#include "domain/backtest/Position.hpp"
#include "domain/backtest/Portfolio.hpp"

using namespace domain;
using namespace backtest;

static Instrument aapl() { return Instrument{"AAPL", AssetClass::Equity, "XNAS", Currency::USD, 0.01, 1, 1.0}; }

TEST_SUITE("Domain/Portfolio") {
  TEST_CASE("Position averages and realizes PnL") {
    Position p{aapl()};
    p.applyFill(100.0, 1.0, true);  // buy
    p.applyFill(110.0, 1.0, true);  // buy -> avg 105
    p.applyFill(120.0, 1.0, false); // sell 1 -> realized 15
    CHECK(p.qty() == doctest::Approx(1.0));
    CHECK(p.avgPrice() == doctest::Approx(105.0));
    CHECK(p.realizedPnl() == doctest::Approx(15.0));
  }

  TEST_CASE("Portfolio cash and nav") {
    Portfolio pf{1000.0};
    Order o_buy{aapl(), Side::Buy, 1.0};
    Trade t_buy{o_buy, /*execPrice*/100.0, /*execQty*/1.0};
    pf.applyTrade(t_buy);
    CHECK(pf.cash() == doctest::Approx(900.0));
    CHECK(pf.navFor(aapl(), 105.0) == doctest::Approx(1005.0));
  }
}
