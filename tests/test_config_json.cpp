/*
#include "doctest.h"
#include <filesystem>
#include <fstream>
#include "Config.h"

namespace fs = std::filesystem;

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

        auto cfg = Config::load(f.string());
        CHECK(cfg.logLevel == "DEBUG");
        CHECK(cfg.logFile == "logs/test_log.txt");
        CHECK(cfg.dataDir == "dataset");
        CHECK(cfg.threads == 8);
    }

    TEST_CASE("Missing keys keep defaults") {
        auto d = tmpdir();
        auto f = d / "missing_keys.json";
        std::ofstream out(f);
        out << R"({            
            "paths": { "data_dir": "data2" }
        })";
        out.close();

        Config& cfg = Config::getInstance(f.string());
        CHECK(cfg.logLevel == "INFO");
        CHECK(cfg.logFile == "app.log"); // Default value
        CHECK(cfg.dataDir == "data2");
        CHECK(cfg.threads == 4); // Default value
    }

}*/