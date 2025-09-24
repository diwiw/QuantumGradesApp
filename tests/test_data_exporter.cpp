#include "doctest.h"
#include "io/DataExporter.hpp"
#include "domain/backtest/BarSeries.hpp"

#include <fstream>
#include <filesystem>
#include <string>

using qga::domain::backtest::BarSeries;
using qga::domain::Quote;
using qga::io::DataExporter;

namespace fs = std::filesystem;

static const std::string TEST_FILE = "test_export.csv";

TEST_SUITE("DataExporter") {

    TEST_CASE("exportSeries writes entire series to file") {
        BarSeries series;
        series.add({1, 100.0, 110.0, 90.0, 105.0, 1000.0});
        series.add({2, 101.0, 111.0, 91.0, 106.0, 2000.0});

        DataExporter exporter(TEST_FILE, false); // overwrite
        exporter.exportSeries(series);

        std::ifstream in(TEST_FILE);
        REQUIRE(in.is_open());

        std::string line;
        std::getline(in, line);
        CHECK(line == "timestamp,open,high,low,close,volume");

        std::getline(in, line);
        CHECK(line == "1,100,110,90,105,1000");

        std::getline(in, line);
        CHECK(line == "2,101,111,91,106,2000");

        in.close();
        fs::remove(TEST_FILE);
    }

    TEST_CASE("exportRange writes subset of series") {
        BarSeries series;
        series.add({1, 100.0, 110.0, 90.0, 105.0, 1000.0});
        series.add({2, 101.0, 111.0, 91.0, 106.0, 2000.0});
        series.add({3, 102.0, 112.0, 92.0, 107.0, 3000.0});

        DataExporter exporter(TEST_FILE, false);
        exporter.exportRange(series, 1, 3);

        std::ifstream in(TEST_FILE);
        REQUIRE(in.is_open());

        std::string line;
        std::getline(in, line);
        CHECK(line == "timestamp,open,high,low,close,volume");

        std::getline(in, line);
        CHECK(line == "2,101,111,91,106,2000");

        std::getline(in, line);
        CHECK(line == "3,102,112,92,107,3000");

        in.close();
        fs::remove(TEST_FILE);
    }

    TEST_CASE("exportRange throws on invalid indices") {
        BarSeries series;
        series.add({1, 100.0, 110.0, 90.0, 105.0, 1000.0});

        DataExporter exporter(TEST_FILE, false);
        CHECK_THROWS_AS(exporter.exportRange(series, 1, 2), std::invalid_argument);
    }

    TEST_CASE("append=true appends new data to file") {
        BarSeries s1;
        s1.add({1, 100.0, 110.0, 90.0, 105.0, 1000.0});
        s1.add({2, 101.0, 111.0, 91.0, 106.0, 2000.0});

        {
            DataExporter exporter1(TEST_FILE, false); // overwrite
            exporter1.exportSeries(s1);
        }
        BarSeries s2;
        s2.add({3, 102.0, 112.0, 92.0, 107.0, 3000.0});

        {
            DataExporter exporter2(TEST_FILE, true); // append
            exporter2.exportSeries(s2);
        }

        std::ifstream in(TEST_FILE);
        REQUIRE(in.is_open());

        std::vector<std::string> lines;
        std::string line;
        while (std::getline(in, line)) {
            lines.push_back(line);
        }

        // Should contain one header and three data lines
        //REQUIRE(lines.size() == 3);
        REQUIRE(lines.size() == 4);

        CHECK(lines[0] == "timestamp,open,high,low,close,volume");
        CHECK(lines[1] == "1,100,110,90,105,1000");
        CHECK(lines[2] == "2,101,111,91,106,2000");
        CHECK(lines[3] == "3,102,112,92,107,3000");

        in.close();
        fs::remove(TEST_FILE);
    }
}
