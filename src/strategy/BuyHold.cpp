#include "strategy/BuyHold.h"

using namespace strategy;

void BuyHold::onStart() { has_bought_ = false; }

Signal BuyHold::onBar(const domain::Quote&) {
  if (!has_bought_) {
    has_bought_ = true;
    return Signal::Buy;
  }
  return Signal::None;
}

void BuyHold::onFinish() {}
