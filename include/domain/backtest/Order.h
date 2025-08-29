/**
 * @file Order.h
 * @brief Order abstraction used in backtests: side, type, quantity, timestamp.
 *
 * Represents a simplified financial order object for use in the backtest engine.
 * Supports only market orders for now, but can be extended for limit/stop/etc.
 */
#pragma once

#include "domain/Instrument.h"
#include <string>
#include <chrono>

namespace backtest {
    /**
     * @enum Side
     * @brief Order direction.
     */
    enum class Side { Buy, Sell };

    /**
     * @enum OrderType
     * @brief Currently supports only market orders.
     */
    enum class OrderType { Market };
    /**
     * @brief Represents a single order in the backtest system.
     *
     * Contains full metadata required for trade simulation:
     * - Instrument
     * - Direction (side)
     * - Quantity
     * - Timestamp
     * - Type (currently only market orders)
     */
    class Order {
        public:
        /**
         * @brief Constructs a new order.
         *
         * @param instrument Financial instrument metadata (symbol, etc.).
         * @param side Buy or sell.
         * @param quantity Positive trade quantity.
         * @param type Order type (default: Market).
         * @param ts Optional timestamp (default: now).
         */
        Order(domain::Instrument instrument,
            Side side,
            double quantity,
            OrderType type = OrderType::Market,
            std::chrono::system_clock::time_point ts = std::chrono::system_clock::now());
        /**
         * @brief Returns instrument metadata.
        */
        const domain::Instrument& instrument() const noexcept { return instrument_; }
        /**
         * @brief Returns trade side (Buy/Sell).
         */
        Side side() const noexcept { return side_; }
        /**
        * @brief Returns order type.
        */
        OrderType type() const noexcept { return type_; }
        /**
         * @brief Returns quantity of the order.
         */
        double quantity() const noexcept { return quantity_ ; }
        /**
         * @brief Returns timestamp of the order.
         */
        std::chrono::system_clock::time_point timestamp() const noexcept { return ts_; }
        /**
         * @brief Checks if the order is valid (quantity > 0).
        */
        bool valid() const noexcept { return quantity_ > 0.0; }

        private:
        domain::Instrument instrument_;         ///< Instrument metadata (symbol, etc.).
        Side side_{Side::Buy};                  ///< Buy or sell side.
        OrderType type_{OrderType::Market};     ///< Order type.
        double quantity_{0.0};                  ///< Quantity of asset to trade.
        std::chrono::system_clock::time_point ts_{};    ///< Timestamp of order.
    };
}   // namespace backtest