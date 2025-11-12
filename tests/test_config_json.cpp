#include "core/Config.hpp"
#include "doctest.h"
#include <filesystem>
#include <fstream>
#include <thread>

namespace fs = std::filesystem;
using namespace qga::core;

static fs::path tmpdir()
{
    fs::path d{"test_tmp_cfg_json"};
    fs::create_directories(d);
    return d;
}

TEST_SUITE("Config/JSON")
{
    TEST_CASE("Load full JSON and sets fields")
    {
        auto d = tmpdir();
        auto f = d / "full.json";
        std::ofstream out(f);
        out << R"({
            "logging": {"level": "DEBUG", "file": "logs/test_log.txt" },
            "paths": { "data_dir": "dataset" },
            "engine": { "threads": 8 }
        })";
        out.close();

        Config& cfg = Config::getInstance();
        cfg.loadDefaults();
        cfg.loadFromFile(f);

        CHECK(qga::toString(cfg.logLevel()) == qga::toString(qga::LogLevel::Debug));
        CHECK(cfg.logFile() == fs::path("logs/test_log.txt"));
        CHECK(cfg.dataDir() == fs::path("dataset"));
        CHECK(cfg.threads() == 8);
    }

    TEST_CASE("Missing keys keep defaults")
    {
        auto d = tmpdir();
        auto f = d / "missing_keys.json";
        std::ofstream out(f);
        out << R"({            
            "paths": { "data_dir": "data2" }
        })";
        out.close();

        Config& cfg = Config::getInstance();
        cfg.loadDefaults();
        cfg.loadFromFile(f);

        CHECK(qga::toString(cfg.logLevel()) == qga::toString(qga::LogLevel::Info));
        CHECK(cfg.logFile() == fs::path("app.log")); // Default value
        CHECK(cfg.dataDir() == fs::path("data2"));
        CHECK(cfg.threads() == 4); // Default value
    }

    TEST_CASE("Invalid threads fallback to safe value")
    {
        auto d = tmpdir();
        auto f = d / "bad_threads.json";
        std::ofstream out(f);
        out << R"({ "engine": { "threads": -5 } })";
        out.close();

        Config& cfg = Config::getInstance();
        cfg.loadDefaults();
        std::vector<std::string> warnings;
        cfg.loadFromFile(f, &warnings);

        CHECK(cfg.threads() == 1);
        REQUIRE_FALSE(warnings.empty());
        CHECK(warnings[0].find("engine.threads < 1") != std::string::npos);
    }

    TEST_CASE("Invalid log level falls back to INFO")
    {
        auto d = tmpdir();
        auto f = d / "bad_loglevel.json";
        std::ofstream out(f);
        out << R"({ "logging": { "level": "VERBOSE" } })";
        out.close();

        Config& cfg = Config::getInstance();
        cfg.loadDefaults();
        std::vector<std::string> warnings;
        cfg.loadFromFile(f, &warnings);

        CHECK(qga::toString(cfg.logLevel()) == qga::toString(qga::LogLevel::Info));
        REQUIRE_FALSE(warnings.empty());
        CHECK(warnings[0].find("logging.level: unknown value") != std::string::npos);
    }

    TEST_CASE("Malformed JSON keeps defaults and adds warning")
    {
        auto d = tmpdir();
        auto f = d / "malformed.json";
        std::ofstream out(f);
        out << R"({ "logging": { "level": "DEBUG", )"; // incomplete JSON
        out.close();

        auto& cfg = Config::getInstance();
        cfg.loadDefaults();
        std::vector<std::string> warnings;
        cfg.loadFromFile(f, &warnings);

        // Defaults should remain
        CHECK(qga::toString(cfg.logLevel()) == qga::toString(qga::LogLevel::Info));
        CHECK(cfg.logFile() == fs::path("app.log"));
        CHECK(cfg.dataDir() == fs::path("data"));
        CHECK(cfg.threads() == 4);

        REQUIRE_FALSE(warnings.empty());
        CHECK(warnings[0].find("Invalid JSON") != std::string::npos);
    }

    TEST_CASE("Threads above hardware concurrency capped")
    {
        auto d = tmpdir();
        auto f = d / "too_many_threads.json";
        std::ofstream out(f);
        out << R"({ "engine": { "threads": 9999 } })";
        out.close();

        Config& cfg = Config::getInstance();
        cfg.loadDefaults();
        std::vector<std::string> warnings;
        cfg.loadFromFile(f, &warnings);

        unsigned hw = std::max(1u, std::thread::hardware_concurrency());
        CHECK(cfg.threads() == static_cast<int>(hw));
        REQUIRE_FALSE(warnings.empty());
        CHECK(warnings[0].find("engine.threads > CPU cores") != std::string::npos);
    }

    TEST_CASE("Empty string values fall back to defaults")
    {
        auto d = tmpdir();
        auto f = d / "empty_strings.json";
        std::ofstream out(f);
        out << R"({
            "paths": { "data_dir": "" },
            "logging": { "file": "" }
        })";
        out.close();

        Config& cfg = Config::getInstance();
        cfg.loadDefaults();
        std::vector<std::string> warnings;
        cfg.loadFromFile(f, &warnings);

        CHECK(cfg.dataDir() == fs::path("data"));    // fallback
        CHECK(cfg.logFile() == fs::path("app.log")); // fallback
        REQUIRE(warnings.size() >= 2);
        CHECK_MESSAGE((warnings[0].find("paths.data_dir is empty") != std::string::npos ||
                       warnings[1].find("paths.data_dir is empty") != std::string::npos),
                      "Expected warning about empty data_dir");

        CHECK_MESSAGE((warnings[0].find("logging.file is empty") != std::string::npos ||
                       warnings[1].find("logging.file is empty") != std::string::npos),
                      "Expected warning about empty log file");
    }

    TEST_CASE("Missing config file keeps defaults and warns")
    {
        auto d = tmpdir();
        auto f = d / "nonexistent.json"; // do not create this file

        Config& cfg = Config::getInstance();
        cfg.loadDefaults();
        std::vector<std::string> warnings;
        cfg.loadFromFile(f, &warnings);

        CHECK(cfg.dataDir() == fs::path("data"));
        CHECK(cfg.logFile() == fs::path("app.log"));
        CHECK(cfg.threads() == 4);
        CHECK(qga::toString(cfg.logLevel()) == qga::toString(qga::LogLevel::Info));

        REQUIRE_FALSE(warnings.empty());
        CHECK(warnings[0].find("Config file not found") != std::string::npos);
    }
}