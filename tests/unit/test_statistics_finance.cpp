#include "doctest.h"
#include "core/Statistics.hpp"

#include <limits>
#include <vector>

using namespace qga::core;

TEST_SUITE("Statistics - Financial Metrics")
{
    // ---------------------------------------------------------
    // MAX DRAWDOWN
    // ---------------------------------------------------------
    TEST_CASE("maxDrawdown() - simple downward trend")
    {
        std::vector<double> eq = {100, 90, 80, 70};
        CHECK(Statistics::maxDrawdown(eq) == doctest::Approx(0.30)); // 100 -> 70
    }

    TEST_CASE("maxDrawdown() - no drawdown")
    {
        std::vector<double> eq = {100, 101, 102, 103};
        CHECK(Statistics::maxDrawdown(eq) == doctest::Approx(0.0));
    }

    TEST_CASE("maxDrawdown() - empty and too short input")
    {
        CHECK(Statistics::maxDrawdown({}) == doctest::Approx(0.0));
        CHECK(Statistics::maxDrawdown({100}) == doctest::Approx(0.0));
    }

    // ---------------------------------------------------------
    // CAGR
    // ---------------------------------------------------------
    TEST_CASE("cagr() - valid input gives correct CAGR")
    {
        std::vector<double> eq = {100, 110}; // 1 period
        double res = Statistics::cagr(eq, 1.0);
        CHECK(res == doctest::Approx(0.10)); // 10%
    }

    TEST_CASE("cagr() - invalid periodsPerYear returns 0")
    {
        std::vector<double> eq = {100, 110};

        CHECK(Statistics::cagr(eq, 0.0) == doctest::Approx(0.0));
        CHECK(Statistics::cagr(eq, -5.0) == doctest::Approx(0.0));
        CHECK(Statistics::cagr(eq, std::numeric_limits<double>::quiet_NaN()) == doctest::Approx(0.0));
    }

    TEST_CASE("cagr() - invalid equity values")
    {
        std::vector<double> eq1 = {};
        std::vector<double> eq2 = {100};
        std::vector<double> eq3 = {0, 110}; // invalid start value

        CHECK(Statistics::cagr(eq1, 1.0) == doctest::Approx(0.0));
        CHECK(Statistics::cagr(eq2, 1.0) == doctest::Approx(0.0));
        CHECK(Statistics::cagr(eq3, 1.0) == doctest::Approx(0.0));
    }

    // ---------------------------------------------------------
    // SHARPE RATIO
    // ---------------------------------------------------------
    TEST_CASE("sharpeRatio() - valid input")
    {
        std::vector<double> returns = {0.1, 0.2, 0.0, -0.1};
        double sr = Statistics::sharpeRatio(returns, 0.02, 1.0);
        CHECK(sr != 0.0); // not strict test, only sanity check
    }

    TEST_CASE("sharpeRatio() - invalid periodsPerYear returns 0")
    {
        std::vector<double> returns = {0.1, 0.2};

        CHECK(Statistics::sharpeRatio(returns, 0.01, 0.0) == doctest::Approx(0.0));
        CHECK(Statistics::sharpeRatio(returns, 0.01, -3.0) == doctest::Approx(0.0));
    }

    TEST_CASE("sharpeRatio() - too few returns")
    {
        CHECK(Statistics::sharpeRatio({}, 0.01, 1.0) == doctest::Approx(0.0));
        CHECK(Statistics::sharpeRatio({0.1}, 0.01, 1.0) == doctest::Approx(0.0));
    }

    // ---------------------------------------------------------
    // SORTINO RATIO
    // ---------------------------------------------------------
    TEST_CASE("sortinoRatio() - valid input")
    {
        std::vector<double> returns = {0.1, -0.2, 0.05};
        double sr = Statistics::sortinoRatio(returns, 0.02, 1.0);
        CHECK(sr != 0.0);
    }

    TEST_CASE("sortinoRatio() - no downside returns → ratio = 0")
    {
        std::vector<double> returns = {0.1, 0.2, 0.3};
        CHECK(Statistics::sortinoRatio(returns, 0.0, 1.0) == doctest::Approx(0.0));
    }

    TEST_CASE("sortinoRatio() - invalid periodsPerYear")
    {
        std::vector<double> returns = {0.1, -0.2};

        CHECK(Statistics::sortinoRatio(returns, 0.01, 0.0) == doctest::Approx(0.0));
    }

    // ---------------------------------------------------------
    // HIT RATIO
    // ---------------------------------------------------------
    TEST_CASE("hitRatio() - mixed returns")
    {
        std::vector<double> r = {0.1, -0.1, 0.2, -0.2, 0.05};
        CHECK(Statistics::hitRatio(r) == doctest::Approx(3.0 / 5.0));
    }

    TEST_CASE("hitRatio() - all losing")
    {
        std::vector<double> r = {-1.0, -0.5, -0.01};
        CHECK(Statistics::hitRatio(r) == doctest::Approx(0.0));
    }

    TEST_CASE("hitRatio() - empty returns 0")
    {
        CHECK(Statistics::hitRatio({}) == doctest::Approx(0.0));
    }
}
