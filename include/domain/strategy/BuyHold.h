#pragma once
#include "strategy/IStrategy.h"

namespace strategy {

    class BuyHold : public IStrategy {
    public:
        void onStart() override {
            // Initialization code here
        }

        Signal on_bar(const domain::Quote& q) override;

        void on_finish() override;
        
        private:
            bool has_bought_ = false;  ///< Flag to track if a buy has been made.
    };
}   // namespace strategy