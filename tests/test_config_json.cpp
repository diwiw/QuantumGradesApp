#include "doctest.h"
#include <filesystem>
#include <fstream>
#include "Config.hpp"

namespace fs = std::filesystem;
using namespace qga;

static fs::path tmpdir() {
    fs::path d{"test_tmp_cfg_json"};
    fs::create_directories(d);
    return d;
}

TEST_SUITE("Config/JSON") {
    TEST_CASE("Load full JSON and sets fields"){
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

        CHECK(cfg.logLevel() == LogLevel::Debug);
        CHECK(cfg.logFile() == fs::path("logs/test_log.txt"));
        CHECK(cfg.dataDir() == fs::path("dataset")); 
        CHECK(cfg.threads() == 8);
    }

    TEST_CASE("Missing keys keep defaults") {
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

        CHECK(cfg.logLevel() == LogLevel::Info);
        CHECK(cfg.logFile() == fs::path("app.log")); // Default value
        CHECK(cfg.dataDir() == fs::path("data2"));
        CHECK(cfg.threads() == 4); // Default value
    }

}