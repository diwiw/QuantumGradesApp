#include <iostream>

#include "Version.hpp"
#include "api/ApiServer.hpp"
#include "core/Config.hpp"
#include "utils/LoggerFactory.hpp"

int main()
{
    try
    {
        auto& config = qga::core::Config::getInstance();
        config.loadFromFile("config/config.json");

        auto logger = qga::utils::LoggerFactory::createAsyncRotatingLogger(
            "api", config.logFile().string(), config.logLevel(), config.logMaxSizeBytes(),
            config.logMaxFiles());

        logger->info("QuantGradesApp API starting... version={}", APP_VERSION);

        qga::api::ApiServer server(logger, config);
        server.start();
    }
    catch (const std::exception& e)
    {
        std::cerr << "[FATAL] API crashed: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
