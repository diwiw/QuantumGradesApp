#include "strategy/MACrossover.hpp"
#include <limits>
#include <cmath>

namespace strategy {

  static double updateWindow(std::deque<double>& w, double& sum, int win, double px) {
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
    const double CLOSE = q.close_;

    const double SMA_F = updateWindow(w_fast_, sum_fast_, fast_period_, CLOSE);
    const double SMA_S = updateWindow(w_slow_, sum_slow_, slow_period_, CLOSE);

    if (!std::isfinite(SMA_F) || !std::isfinite(SMA_S)) return Signal::None;

    if (!ready_) {
      ready_ = true;
      prev_fast_ = SMA_F;
      prev_slow_ = SMA_S;
      return Signal::None;
    }

    const bool CROSS_UP   = (prev_fast_ <= prev_slow_) && (SMA_F >  SMA_S);
    const bool CROSS_DOWN = (prev_fast_ >= prev_slow_) && (SMA_F <  SMA_S);

    prev_fast_ = SMA_F;
    prev_slow_ = SMA_S;

    if (CROSS_UP)   return Signal::Buy;
    if (CROSS_DOWN) return Signal::Sell;
    return Signal::None;
  }

  void MACrossover::onFinish() {}

} // namespace strategy