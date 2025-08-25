#include "strategy/MACrossover.h"
#include <limits>
#include <cmath>

namespace strategy {

  static double update_window(std::deque<double>& w, double& sum, int win, double px) {
    w.push_back(px);
    sum += px;
    if ((int)w.size() > win) { sum -= w.front(); w.pop_front(); }
    if ((int)w.size() == win) return sum / win;
    return std::numeric_limits<double>::quiet_NaN();
  }

  MACrossover::MACrossover(int fast, int slow)
    : fast_period_(fast), slow_period_(slow) {}

  void MACrossover::onStart() {
    w_fast_.clear(); w_slow_.clear();
    sum_fast_ = sum_slow_ = 0.0;
    prev_fast_ = prev_slow_ = 0.0;
    ready_ = false;
  }

  Signal MACrossover::onBar(const domain::Quote& q) {
    const double close = q.close;

    const double sma_f = update_window(w_fast_, sum_fast_, fast_period_, close);
    const double sma_s = update_window(w_slow_, sum_slow_, slow_period_, close);

    if (!std::isfinite(sma_f) || !std::isfinite(sma_s)) return Signal::None;

    if (!ready_) {
      ready_ = true;
      prev_fast_ = sma_f;
      prev_slow_ = sma_s;
      return Signal::None;
    }

    const bool cross_up   = (prev_fast_ <= prev_slow_) && (sma_f >  sma_s);
    const bool cross_down = (prev_fast_ >= prev_slow_) && (sma_f <  sma_s);

    prev_fast_ = sma_f;
    prev_slow_ = sma_s;

    if (cross_up)   return Signal::Buy;
    if (cross_down) return Signal::Sell;
    return Signal::None;
  }

  void MACrossover::onFinish() {}

} // namespace strategy