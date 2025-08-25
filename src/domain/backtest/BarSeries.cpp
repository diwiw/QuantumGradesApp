#include "domain/backtest/BarSeries.h"
#include <stdexcept>

namespace backtest{

  void BarSeries::add(const domain::Quote& q) {
    // (opcjonalnie) weryfikacja danych wejściowych
    // if (!(q.high >= q.low && q.high >= q.open && q.high >= q.close)) { ... }
    data_.push_back(q);
  }

  std::size_t BarSeries::size() const noexcept { return data_.size(); }

  const domain::Quote& BarSeries::at(std::size_t i) const {
    if (i >= data_.size()) throw std::out_of_range("BarSeries::at index out of range");
    return data_[i];
  }

  const domain::Quote& BarSeries::back() const {
    if (data_.empty()) throw std::out_of_range("BarSeries::back on empty series");
    return data_.back();
  }

} // namespace backtest