#include "doctest.h"
#include "domain/backtest/BarSeries.hpp"

using backtest::BarSeries;
using domain::Quote;

TEST_CASE("BarSeries::add and size") {
    BarSeries series;
    CHECK(series.empty());
    CHECK(series.size() == 0);

    Quote q1{1, 100.0, 110.0, 90.0, 105.0, 1234.56};
    series.add(q1);

    CHECK_FALSE(series.empty());
    CHECK(series.size() == 1);
}

TEST_CASE("BarSeries::at valid and invalid access") {
    BarSeries series;
    series.add({1, 100.0, 110.0, 90.0, 105.0, 1234.56});
    series.add({2, 101.0, 111.0, 91.0, 106.0, 2234.56});

    CHECK(series.at(0).ts_ == 1);
    CHECK(series.at(1).ts_ == 2);
    
}

TEST_CASE("BarSeries::back and front") {
    BarSeries series;
    series.add({1, 100.0, 110.0, 90.0, 105.0, 1234.56});
    series.add({2, 101.0, 111.0, 91.0, 106.0, 2234.56});

    CHECK(series.front().ts_ == 1);
    CHECK(series.end().ts_ == 2);
}

TEST_CASE("BarSeries::operator[]") {
    BarSeries series;
    series.add({1, 100.0, 110.0, 90.0, 105.0, 1234.56});

    CHECK(series[0].close_ == doctest::Approx(105.0));
}

TEST_CASE("BarSeries::clear") {
    BarSeries series;
    series.add({1, 100.0, 110.0, 90.0, 105.0, 1234.56});
    series.clear();

    CHECK(series.empty());
    CHECK(series.size() == 0);    
}
