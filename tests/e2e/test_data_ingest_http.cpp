#include "doctest.h"
#include "ingest/DataIngest.hpp"
#include "testHttpServer.hpp"
#include "utils/MockLogger.hpp"

using namespace qga::ingest;

TEST_CASE("DataIngest::fromHttpUrl loads valid CSV data over HTTP")
{

    // Start local HTTP server
    TestHttpServer server(8000, QGA_DATA_DIR);

    const std::string URL = "http://localhost:8000/test_http.csv";

    auto ingest = DataIngest(std::make_shared<qga::utils::MockLogger>());
    auto result = ingest.fromHttpUrl(URL);

    REQUIRE_MESSAGE(result.has_value(), "Failed to load data from HTTP URL");

    const auto& series = result.value();
    CHECK(series.size() == 2);

    const auto& first = series.at(0);
    CHECK(first.ts_ == 1669900800000);

    const auto& second = series.at(1);
    CHECK(second.volume_ == doctest::Approx(14500.00));
}
