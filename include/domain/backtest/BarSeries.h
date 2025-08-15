#pragma once
#include <vector>
#include "domain/Quote.h"

namespace backtest {
    class BarSeries {
    public:
        using Quote = domain::Quote;

        void add(const domain::Quote& q);

        std::size_t size() const noexcept;

        const Quote& at(std::size_t i) const;
        const Quote& back() const;
    private:
        std::vector<Quote> data_;
    };
} // namespace backtest