#include "doctest.h"
#include "ingest/DataIngest.hpp"
#include "utils/MockLogger.hpp"

namespace qga::ingest {

// Test helper with friend access
class DataIngestTestAccess {
public:
    static bool validateRow(DataIngest& ingest, const std::vector<std::string>& fields) {
        return ingest.validateRow(fields);
    }

    static std::optional<qga::domain::Quote> parseRow(DataIngest& ingest, const std::vector<std::string>& fields) {
        return ingest.parseRow(fields);
    }
};

TEST_SUITE("DataIngest - unit tests") {

    TEST_CASE("validateRow returns true for valid field count") {
        std::vector<std::string> valid_row = {
            "1625097600000", "100.0", "105.0", "99.0", "104.0", "12345.67"
        };

        auto ingest = DataIngest(std::make_shared<qga::utils::MockLogger>());
        CHECK(DataIngestTestAccess::validateRow(ingest, valid_row));
    }

    TEST_CASE("validateRow returns false for invalid field count") {
        std::vector<std::string> invalid_row = {
            "1625097600000", "100.0" // only 2 fields
        };

        auto ingest = DataIngest(std::make_shared<qga::utils::MockLogger>());
        CHECK_FALSE(DataIngestTestAccess::validateRow(ingest, invalid_row));
    }

    TEST_CASE("parseRow returns Quote for valid fields") {
        std::vector<std::string> fields = {
            "1669900800000", "100.5", "102.3", "99.0", "101.2", "12345.67"
        };

        auto ingest = DataIngest(std::make_shared<qga::utils::MockLogger>());
        auto result = DataIngestTestAccess::parseRow(ingest, fields);

        REQUIRE(result.has_value());
        CHECK(result->ts_ == 1669900800000);
        CHECK(result->open_ == doctest::Approx(100.5));
        CHECK(result->high_ == doctest::Approx(102.3));
        CHECK(result->low_ == doctest::Approx(99.0));
        CHECK(result->close_ == doctest::Approx(101.2));
        CHECK(result->volume_ == doctest::Approx(12345.67));
    }
}

/**
 * @brief Integration test for ingesting market data via HTTP.
 *
 * Requires a running local HTTP server with a valid CSV file.
 * Example:
 *   cd tests/data
 *   python3 -m http.server 8000
 *
 * CSV must match expected format:
 * timestamp, open, high, low, close, volume
 */
TEST_CASE("DataIngest::fromHttpUrl loads valid CSV data over HTTP") {
    const std::string URL = "http://localhost:8000/test_http.csv";

    auto ingest = DataIngest(std::make_shared<qga::utils::MockLogger>());
    auto result = ingest.fromHttpUrl(URL);

    REQUIRE_MESSAGE(result.has_value(), "Failed to load data from HTTP URL");

    const auto& series = result.value();
    CHECK(series.size() == 2);

    const auto& first = series.at(0);
    CHECK(first.ts_ == 1669900800000);
    CHECK(first.open_ == doctest::Approx(100.5));
    CHECK(first.high_ == doctest::Approx(102.3));
    CHECK(first.low_ == doctest::Approx(99.0));
    CHECK(first.close_ == doctest::Approx(101.2));
    CHECK(first.volume_ == doctest::Approx(12345.67));

    const auto& second = series.at(1);
    CHECK(second.ts_ == 1669987200000);
    CHECK(second.open_ == doctest::Approx(101.2));
    CHECK(second.high_ == doctest::Approx(103.0));
    CHECK(second.low_ == doctest::Approx(100.0));
    CHECK(second.close_ == doctest::Approx(102.5));
    CHECK(second.volume_ == doctest::Approx(14500.00));
}

} // namespace qga::ingest
