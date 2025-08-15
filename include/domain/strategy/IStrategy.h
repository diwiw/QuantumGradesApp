#pragma once
#include "domain/Quote.h"

namespace strategy {

    enum class Signal { None, Buy, Sell };

    class IStrategy {
    public:
        virtual ~IStrategy() = default;

        virtual void onStart() {}
        virtual Signal on_bar(const domain::Quote& q) = 0;

        virtual void on_finish() {}
    };

} // namespace strategy