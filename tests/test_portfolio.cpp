#include "doctest.h"
#include "domain/Instrument.h"
#include "domain/backtest/Trade.h"
#include "domain/backtest/Position.h"
#include "domain/backtest/Portfolio.h"

using namespace domain;
using namespace backtest;

static Instrument AAPL() { return Instrument{"AAPL", AssetClass::Equity, "XNAS", Currency::USD, 0.01, 1, 1.0}; }

TEST_SUITE("Domain/Portfolio") {
  TEST_CASE("Position averages and realizes PnL") {
    Position p{AAPL()};
    p.apply_fill(100.0, 1.0, true);  // buy
    p.apply_fill(110.0, 1.0, true);  // buy -> avg 105
    p.apply_fill(120.0, 1.0, false); // sell 1 -> realized 15
    CHECK(p.qty() == doctest::Approx(1.0));
    CHECK(p.avg_price() == doctest::Approx(105.0));
    CHECK(p.realized_pnl() == doctest::Approx(15.0));
  }

  TEST_CASE("Portfolio cash and nav") {
    Portfolio pf{1000.0};
    Order o_buy{AAPL(), Side::Buy, 1.0};
    Trade t_buy{o_buy, /*execPrice*/100.0, /*execQty*/1.0};
    pf.apply_trade(t_buy);
    CHECK(pf.cash() == doctest::Approx(900.0));
    CHECK(pf.nav_for(AAPL(), 105.0) == doctest::Approx(1005.0));
  }
}
