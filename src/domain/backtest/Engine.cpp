#include "domain/backtest/Engine.h"
using namespace backtest;

BacktestResult Engine::run(BarSeries const& s, strategy::IStrategy& strat) {
  BacktestResult r;
  r.initial_equity = initial_equity_;
  r.final_equity   = initial_equity_;

  bool has_pos = false;
  double entry = 0.0;

  strat.onStart();
  for (std::size_t i = 0; i < s.size(); ++i) {
    const auto& q = s.at(i);
    const auto sig = strat.onBar(q);

    if (sig == strategy::Signal::Buy && !has_pos) {
      has_pos = true; entry = q.close; r.trades_executed += 1;
    } else if (sig == strategy::Signal::Sell && has_pos) {
      has_pos = false; r.final_equity *= (q.close / entry);
    }
  }
  strat.onFinish();

  if (has_pos) { // domknięcie na końcu, jeśli trzeba
    r.final_equity *= (s.back().close / entry);
  }
  return r;
}
