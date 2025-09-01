#include "doctest.h"
#include "ingest/DataIngest.hpp"

// Test helper class with friend access
class DataIngestTestAccess {
public:
    static bool validateRow(const std::vector<std::string>& fields) {
        return ingest::DataIngest::validateRow(fields);
    }

    static std::optional<domain::Quote> parseRow(const std::vector<std::string>& fields) {
        return ingest::DataIngest::parseRow(fields);
    }
};

TEST_CASE("validateRow return true for valid field count") {
    std::vector<std::string> valid_row = {
        "1625097600000", "100.0", "105.0", "99.0", "104.0", "12345.67"
    };
    CHECK(DataIngestTestAccess::validateRow(valid_row));
}

TEST_CASE("validateRow return false for invalid field count") {
    std::vector<std::string> invalid_row = {
        "1625097600000", "100.0" // only 2 fields
    };
    CHECK_FALSE(DataIngestTestAccess::validateRow(invalid_row));
}

TEST_CASE("parseRow returns Quote for valid fields") {
    std::vector<std::string> fields = {
        "1669900800000", "100.5", "102.3", "99.0", "101.2", "12345.67"
    };

    auto result = DataIngestTestAccess::parseRow(fields);

    REQUIRE(result.has_value());

    CHECK(result->ts_ == 1669900800000);
    CHECK(result->open_ == doctest::Approx(100.5));
    CHECK(result->high_ == doctest::Approx(102.3));
    CHECK(result->low_ == doctest::Approx(99.0));
    CHECK(result->close_ == doctest::Approx(101.2));
    CHECK(result->volume_ == doctest::Approx(12345.67));
}
