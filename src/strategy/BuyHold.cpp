#include "strategy/BuyHold.hpp"


namespace qga::strategy {

  void BuyHold::onStart() { has_bought_ = false; }

    Signal BuyHold::onBar(const domain::Quote&) {
    if (!has_bought_) {
      has_bought_ = true;
      return Signal::Buy;
    }
    return Signal::None;
  }

  void BuyHold::onFinish() {}

} // namespace qga::strategy
