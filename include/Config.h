#pragma once
#include <toml++/toml.h>
#include <string>

struct Config {
    std::string logLevel = "INFO";
    std::string logDir = "logs";
    std::string dataDir = "data";
    int threads = 4;

    static Config load(const std::string& path) {
        Config c;
        try {
            auto t = toml::parse_file(path);
            if (auto v = t["logging"]["level"].value<std::string>()) {
                c.logLevel = *v;
            }
            if (auto v = t["logging"]["dir"].value<std::string>()) {
                c.logDir = *v;                
            }
            if (auto v = t["paths"]["data_dir"].value<std::string>()) {
                c.dataDir = *v;
            }
            if (auto v = t["engine"]["threads"].value<int>()) {
                c.threads = *v;
            }

        } catch (const toml::parse_error& e) {
            // Handle error (e.g., log it)
        }
        return c
    }
}