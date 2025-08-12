#pragma once
#include <string>
#include <fstream>
#include <nlohmann/json.hpp>


struct Config {
// Default values
    std::string dataDir = "data";
int         threads = 4;    // [1..64]
std::string logLevel = "INFO";
std::string logFile = "app.log";

    static Config load(const std::string& path, std::string* err = nullptr) {
        Config c{};
        std::ifstream in (path);
        if (!in) {
            if(err) *err = "config file not found: " + path + " (using defaults)";
            return c; // Return default config if file cannot be opened
        }
        try {
            nlohmann::json j; in >> j;
            
            if (auto it = j.find("paths"); it != j.end() && it->contains("data_dir"))
                c.dataDir = (*it)["data_dir"].get<std::string>();

            if(auto it = j.find("engine"); it != j.end() && it->contains("threads"))
                c.threads = (*it)["threads"].get<int>();

            if (auto it = j.find("logging"); it != j.end()) {
                if(it->contains("level"))
                    c.logLevel = (*it)["level"].get<std::string>();
                if(it->contains("file"))
                    c.logFile = (*it)["file"].get<std::string>();
            }

        } catch (const nlohmann::json::exception& e) {
            if (err) *err = std::string("invalid JSON: ") + e.what();
            // Handle JSON parsing errors
        }
        return c;
    }
};
