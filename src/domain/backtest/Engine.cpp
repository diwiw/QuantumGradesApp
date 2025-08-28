#include "domain/backtest/Engine.h"

namespace backtest{

  BacktestResult Engine::run(BarSeries const& s, strategy::IStrategy& strat) {
    BacktestResult r;
    r.initial_equity_ = initial_equity_;
    r.final_equity_   = initial_equity_;

    // Simple account state: all-in 1 item; without leverage
    bool has_pos  = false;
    double entry  = 0.0;
    double cash   = initial_equity_;
    double qty    = 0.0;

    strat.onStart();

    for (std::size_t i = 0; i < s.size(); ++i) {
      const auto& q = s.at(i);
      const auto SIG = strat.onBar(q);

      if (SIG == strategy::Signal::Buy && !has_pos) {
        // Execution with delay
        const double PX_EXEC  = applySlippage(q.close_, exec_.slippage_bps_, /*is_buy=*/true);
        const double FEE      = commissionCost(PX_EXEC, 1.0, exec_.commission_fixed_, exec_.commission_bps_);

        if(PX_EXEC > 0.0 && cash >= (PX_EXEC + FEE)) {
          has_pos = true; 
          entry   = PX_EXEC;
          qty     = 1.0;
          cash    -= (PX_EXEC + FEE);
          r.trades_executed_ += 1;
        }
        
      } else if (SIG == strategy::Signal::Sell && has_pos) {
        const double PX_EXEC  = applySlippage(q.close_, exec_.slippage_bps_, /*is_buy=*/false);
        const double FEE      = commissionCost(PX_EXEC, qty, exec_.commission_fixed_, exec_.commission_bps_);

        has_pos = false; 
        cash    += PX_EXEC * qty;   // income from sell
        cash    -= FEE;             // minus commission
        qty     = 0.0;
      }

      r.final_equity_ = cash + (has_pos ? q.close_ * qty : 0.0);
    }
    strat.onFinish();

    if (has_pos) { // 
      const auto& last      = s.back();
      const double PX_EXEC  = applySlippage(last.close_, exec_.slippage_bps_, /*is_buy=*/false);
      const double FEE      = commissionCost(PX_EXEC, qty, exec_.commission_fixed_, exec_.commission_bps_);
      cash                  += PX_EXEC * qty;
      cash                  -= FEE;
      qty                   = 0.0;
      has_pos               = false;
      r.final_equity_ = cash;
    }
    
    return r;
  }

} // namespace backtest