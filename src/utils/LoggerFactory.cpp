#include "utils/LoggerFactory.hpp"

#include "common/LogLevel.hpp"
#include "utils/SpdLogger.hpp"

namespace qga::utils
{

    std::shared_ptr<qga::utils::ILogger> LoggerFactory::createLogger(const std::string& name,
                                                                     const std::string& filename,
                                                                     qga::LogLevel level)
    {
        size_t max_size = 10 * 1024 * 1024; // 10 MB
        size_t max_files = 5;
        return createAsyncRotatingLogger(name, filename, level, max_size, max_files);
    }

    std::shared_ptr<qga::utils::ILogger>
    LoggerFactory::createAsyncRotatingLogger(const std::string& name, const std::string& filename,
                                             qga::LogLevel level, size_t max_size, size_t max_files)
    {
        if (!spdlog::thread_pool())
        {
            spdlog::init_thread_pool(8192, 1);
        }
        auto sink =
            std::make_shared<spdlog::sinks::rotating_file_sink_mt>(filename, max_size, max_files);
        auto logger = std::make_shared<qga::utils::SpdLogger>(
            name, std::vector<spdlog::sink_ptr>{sink}, true);
        logger->setLevel(level);
        return logger;
    }

    std::shared_ptr<qga::utils::ILogger> LoggerFactory::createConsoleLogger(const std::string& name,
                                                                            qga::LogLevel level)
    {
        auto sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
        auto logger =
            std::make_shared<qga::utils::SpdLogger>(name, std::vector<spdlog::sink_ptr>{sink});
        logger->setLevel(level);
        return logger;
    }

    std::shared_ptr<ILogger> LoggerFactory::createNullLogger()
    {
        auto null_sink = std::make_shared<spdlog::sinks::null_sink_mt>();
        return std::make_shared<SpdLogger>("null", std::vector<spdlog::sink_ptr>{null_sink});
    }

} // namespace qga::utils
