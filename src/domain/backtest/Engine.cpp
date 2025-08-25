#include "domain/backtest/Engine.h"

namespace backtest{

  BacktestResult Engine::run(BarSeries const& s, strategy::IStrategy& strat) {
    BacktestResult r;
    r.initial_equity = initial_equity_;
    r.final_equity   = initial_equity_;

    // Simple account state: all-in 1 item; without leverage
    bool has_pos  = false;
    double entry  = 0.0;
    double cash   = initial_equity_;
    double qty    = 0.0;

    strat.onStart();

    for (std::size_t i = 0; i < s.size(); ++i) {
      const auto& q = s.at(i);
      const auto sig = strat.onBar(q);

      if (sig == strategy::Signal::Buy && !has_pos) {
        // Execution with delay
        const double px_exec  = applySlippage(q.close, exec_.slippage_bps, /*is_buy=*/true);
        const double fee      = commissionCost(px_exec, 1.0, exec_.commission_fixed, exec_.commission_bps);

        if(px_exec > 0.0 && cash >= (px_exec + fee)) {
          has_pos = true; 
          entry   = px_exec;
          qty     = 1.0;
          cash    -= (px_exec + fee);
          r.trades_executed += 1;
        }
        
      } else if (sig == strategy::Signal::Sell && has_pos) {
        const double px_exec  = applySlippage(q.close, exec_.slippage_bps, /*is_buy=*/false);
        const double fee      = commissionCost(px_exec, qty, exec_.commission_fixed, exec_.commission_bps);

        has_pos = false; 
        cash    += px_exec * qty;   // income from sell
        cash    -= fee;             // minus commission
        qty     = 0.0;
      }

      r.final_equity = cash + (has_pos ? q.close * qty : 0.0);
    }
    strat.onFinish();

  if (has_pos) { 
    r.final_equity = (s.back().close / entry);
  }
  return r;
}

} // namespace backtest