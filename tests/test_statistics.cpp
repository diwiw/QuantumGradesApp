#include "doctest.h"
#include "Statistics.h"
#include "Logger.h"
#include <vector>

TEST_CASE("Statistics- calculateMean") {
	
	std::vector<int> data = {2, 4, 6, 8};
	auto mean = Statistics::calculateMean(data);
	REQUIRE(mean.has_value());
	CHECK(doctest::Approx(mean.value()) == 5.0);

	std::vector<int> emptyData;
	auto meanEmpty = Statistics::calculateMean(emptyData);
	CHECK_FALSE(meanEmpty.has_value());
}

TEST_CASE("Statistics- calculateMedian"){
	std::vector<int> oddData = {1, 3, 5};
	auto medianOdd = Statistics::calculateMedian(oddData);
	REQUIRE(medianOdd.has_value());
	CHECK(doctest::Approx(medianOdd.value()) == 3.0);

	std::vector<int> evenData = {1, 2, 3, 4};
	auto medianEven = Statistics::calculateMedian(evenData);
	REQUIRE(medianEven.has_value());
	CHECK(doctest::Approx(medianEven.value()) == 2.5);

	std::vector<int> emptyData;
	auto medianEmpty = Statistics::calculateMedian(emptyData);
	CHECK_FALSE(medianEmpty.has_value());
}

TEST_CASE("Statistics- calculateStdDev"){
	std::vector<int> data = {1, 2, 3, 4, 5};
	auto stddev = Statistics::calculateStdDev(data);
	REQUIRE(stddev.has_value());
	CHECK(stddev.value() == doctest::Approx(1.5811).epsilon(0.0001));

	std::vector<int> insufficientData = {1};
	auto stddevInsufficient = Statistics::calculateStdDev(insufficientData);
	CHECK_FALSE(stddevInsufficient.has_value());
}

TEST_CASE("Statistics- calculateMax"){
	std::vector<int> data = {1, 3, 5, 7, 11};
	auto maxValue = Statistics::calculateMax(data);
	REQUIRE(maxValue.has_value());
	CHECK(doctest::Approx(maxValue.value()) == 11);

	std::vector<int> emptyData;
	auto maxEmpty = Statistics::calculateMax(emptyData);
	CHECK_FALSE(maxEmpty.has_value());
} 
//
//TEST_CASE("Statistics finds max and min values"){
//	std::vector<int> data = {10, 50, 30, 5};
//	CHECK(Statistics::calculateMax(data) == 50);
//	CHECK(Statistics::calculateMin(data) == 5);
//}

//TEST_CASE("Statistics handles empty input"){
//	std::vector<int> empty;
//	CHECK_THROWS_AS(Statistics::calculateMean(empty), std::invalid_argument);
//	CHECK_THROWS_AS(Statistics::calculateMedian(empty), std::invalid_argument);
//}
