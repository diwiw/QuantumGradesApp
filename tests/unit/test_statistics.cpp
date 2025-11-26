#include "core/Statistics.hpp"
#include "doctest.h"
// #include "examples/logger_demo/Logger.hpp"
//  TODO: Reactivate test logger after milestone 1.1.5
#include <vector>

using namespace qga::core;

TEST_CASE("Statistics- calculateMean")
{

    std::vector<int> data = {2, 4, 6, 8};
    auto mean = Statistics::calculateMean(data);
    REQUIRE(mean.has_value());
    CHECK(doctest::Approx(mean.value()) == 5.0);

    std::vector<int> empty_data;
    auto mean_empty = Statistics::calculateMean(empty_data);
    CHECK_FALSE(mean_empty.has_value());
}

TEST_CASE("Statistics- calculateMedian")
{
    std::vector<int> odd_data = {1, 3, 5};
    auto median_odd = Statistics::calculateMedian(odd_data);
    REQUIRE(median_odd.has_value());
    CHECK(doctest::Approx(median_odd.value()) == 3.0);

    std::vector<int> even_data = {1, 2, 3, 4};
    auto median_even = Statistics::calculateMedian(even_data);
    REQUIRE(median_even.has_value());
    CHECK(doctest::Approx(median_even.value()) == 2.5);

    std::vector<int> empty_data;
    auto median_empty = Statistics::calculateMedian(empty_data);
    CHECK_FALSE(median_empty.has_value());
}

TEST_CASE("Statistics- calculateStdDev")
{
    std::vector<int> data = {1, 2, 3, 4, 5};
    auto stddev = Statistics::calculateStdDev(data);
    REQUIRE(stddev.has_value());
    CHECK(stddev.value() == doctest::Approx(1.5811).epsilon(0.0001));

    std::vector<int> insufficient_data = {1};
    auto stddev_insufficient = Statistics::calculateStdDev(insufficient_data);
    CHECK_FALSE(stddev_insufficient.has_value());
}

TEST_CASE("Statistics- calculateMax")
{
    std::vector<int> data = {1, 3, 5, 7, 11};
    auto max_value = Statistics::calculateMax(data);
    REQUIRE(max_value.has_value());
    CHECK(doctest::Approx(max_value.value()) == 11);

    std::vector<int> empty_data;
    auto max_empty = Statistics::calculateMax(empty_data);
    CHECK_FALSE(max_empty.has_value());
}
//
// TEST_CASE("Statistics finds max and min values"){
//	std::vector<int> data = {10, 50, 30, 5};
//	CHECK(Statistics::calculateMax(data) == 50);
//	CHECK(Statistics::calculateMin(data) == 5);
//}

// TEST_CASE("Statistics.hppandles empty input"){
//	std::vector<int> empty;
//	CHECK_THROWS_AS(Statistics::calculateMean(empty), std::invalid_argument);
//	CHECK_THROWS_AS(Statistics::calculateMedian(empty), std::invalid_argument);
// }
