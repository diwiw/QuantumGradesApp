/**
 * @file Instrument.h
 * @brief Domain model: security/instrument metadata (symbol, venue, asset class, trading params).
 * 
 * Minimal, robust Value Object representing a tradable instrument.
 * Validates construction and exposes read-only accessors.
 */

#pragma once
#include <string>
#include <stdexcept>
#include <functional>


namespace domain {

    /**
     * @enum AssetClass
     * @brief High-Level classification of the instrument.
     */
    enum class AssetClass { 
        Equity, 
        ETF, 
        Future, 
        Option, 
        FX, 
        Bond, 
        Crypto, 
        Unknown 
    };

    /**
     * @enum Currency
     * @brief Trading/settlement currency (subset of ISO 4217).
     */

    enum class Currency { 
        USD,    ///< US Dollar
        EUR,    ///< Euro
        PLN,    ///< Polish Zloty
        GBP,    ///< British Pound
        JPY,    ///< Japanese Yen
        Unknown ///< Unspecified/unknown currency
    };

    class Instrument {
    public:
    /**
     * @brief Construct on Instrument with validation.
     * @param symbol        Ticker/identifier (e.g. "AAPL"). Must be non-empty.
     * @param asset_class   Asset class classification.
     * @param exchange_mic  MIC code of the primary trading venu (e.g. "XNAS). Must by non-empty.
     * @param currency      Trading/settlement currency (default: USD).
     * @param tick_size     
     */
        Instrument(std::string symbol,
            AssetClass asset_class,
            std::string exchange_mic,
            Currency currency = Currency::USD,
            double tick_size = 0.01,
            int lot_size = 1,
            double multiplier = 1.0)
        : symbol_(std::move(symbol)),
          exchange_mic_(std::move(exchange_mic)),
          asset_class_(asset_class),
          currency_(currency),
          tick_size_(tick_size),
          lot_size_(lot_size),
          multiplier_(multiplier)
        {
            if(symbol_.empty())         throw std::invalid_argument("Instrument: symbol must not be empty");
            if(exchange_mic_.empty())  throw std::invalid_argument("Instrument: exchange MIC must not be empty");
            if(tick_size_ <= 0.0)  throw std::invalid_argument("Instrument: tick size must be positive");
            if(lot_size_ <= 0)     throw std::invalid_argument("Instrument: lot size must be positive");
            if(multiplier_ <= 0.0) throw std::invalid_argument("Instrument: multiplier must be positive");
        }
        
        /// @name Read-only accessors
        /// @{
        /// @return Ticker/identifier (e.g. "AAPL").
        const std::string& symbol() const noexcept { return symbol_; }
        /// @return MIC code of the trading venu (e.g. "XNAS").
        const std::string& exchangeMic() const noexcept { return exchange_mic_; }
        /// @return Asset class classification.
        AssetClass assetClass() const noexcept { return asset_class_; }
        /// @return Trading/settlement currency.
        Currency currency() const noexcept { return currency_; }
        /// @return Minimum price increment (tick size).
        double tickSize() const noexcept { return tick_size_; }
        /// @return Minimum tradeable quantinty.
        int lotSize() const noexcept { return lot_size_; }
        /// @return Contract multiplier (e.g. 100 for options).
        double multiplier() const noexcept { return multiplier_; }
        /// @}
        
        /// Equality comparison (useful in tests and containers).
        friend bool operator==(const Instrument& lhs, const Instrument& rhs) {
            return lhs.symbol_ == rhs.symbol_ &&
                   lhs.exchange_mic_ == rhs.exchange_mic_ &&
                   lhs.asset_class_ == rhs.asset_class_ &&
                   lhs.currency_ == rhs.currency_ &&
                   lhs.tick_size_ == rhs.tick_size_ &&
                   lhs.lot_size_ == rhs.lot_size_ &&
                   lhs.multiplier_ == rhs.multiplier_;
        }

        /// Inequality comparison.
        friend bool operator!=(const Instrument& lhs, const Instrument& rhs) noexcept {
            return !(lhs == rhs);
        }

        private:
        // Stored as private to keep the object immutable after construction.
            std::string symbol_;         ///< Ticker/identifier (e.g. "AAPL").
            std::string exchange_mic_;   ///< MIC code of the primary trading venue (e.g. "XNAS").
            AssetClass asset_class_;     ///< High-level classification of the instrument.
            Currency currency_;          ///< Trading/settlement currency.
            double tick_size_;           ///< Minimum price increment (tick size).
            int lot_size_;               ///< Minimum tradeable quantity.
            double multiplier_;          ///< Contract multiplier (e.g. 100 for options).
    };
}   // namespace domain

/// @cond DOXYGEN_IGNORE
// Optional hash for unordered containers keyed by Instrument (based on symbol+MIC).
namespace std {
    template <> struct hash<domain::Instrument> {
        size_t operator()(const domain::Instrument& inst) const noexcept {
            return std::hash<std::string>()(inst.symbol()) ^ 
                   std::hash<std::string>()(inst.exchangeMic());
        }
    };
}
/// @endcond