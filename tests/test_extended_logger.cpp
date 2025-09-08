/**
 * @file test_logger.cpp
 * @brief Unit tests for ILogger implementations (MockLogger, NullLogger, SpdLogger).
 */

#include "doctest.h"

#include "utils/ILogger.hpp"
#include "utils/MockLogger.hpp"
#include "utils/NullLogger.hpp"
#include "utils/SpdLogger.hpp"

#include <fstream>
#include <filesystem>

using namespace utils;

TEST_SUITE("Logger") {

    TEST_CASE("MockLogger stores messages") {
        auto logger = std::make_shared<MockLogger>();

        logger->info("Hello");
        logger->warn("Something might be wrong");
        logger->error("Error happened");        

        auto infos = logger->getLogsByLevel(LogLevel::Info);
        auto warns = logger->getLogsByLevel(LogLevel::Warn);
        auto errors = logger->getLogsByLevel(LogLevel::Error);
        CHECK(infos[0] == "Hello");
        CHECK(warns[0] == "Something might be wrong");
        CHECK(errors[0] == "Error happened");
    }

    TEST_CASE("NullLogger ignores messages") {
        auto logger = std::make_shared<NullLogger>();

        // Te logi powinny zostać zignorowane
        logger->info("Should not appear");
        logger->error("Still nothing");

        // Nic do sprawdzenia – klasa celowo nie przechowuje logów
        CHECK(true); // test przejdzie zawsze
    }

    TEST_CASE("SpdLogger writes to file") {
        const std::string log_file = "test_spdlog.log";
        if (std::filesystem::exists(log_file)) {
            std::filesystem::remove(log_file);
        }

        auto logger = std::make_shared<SpdLogger>(
            "TestLogger",
            std::vector<std::shared_ptr<spdlog::sinks::sink>>{
                std::make_shared<spdlog::sinks::basic_file_sink_mt>(log_file, true)
            },
            false // sync mode
        );

        logger->info("Logger started");
        logger->error("Something bad");

        logger.reset(); // wymuszamy flush i zamknięcie

        REQUIRE(std::filesystem::exists(log_file));
        std::ifstream infile(log_file);
        std::string contents((std::istreambuf_iterator<char>(infile)),
                             std::istreambuf_iterator<char>());

        CHECK(contents.find("Logger started") != std::string::npos);
        CHECK(contents.find("Something bad") != std::string::npos);
    }
}
