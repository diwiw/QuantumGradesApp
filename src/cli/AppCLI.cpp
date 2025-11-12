#include "AppCLI.hpp"

#ifndef USE_BOOST
#include <CLI/CLI.hpp>
#else
#include <boost/program_options.hpp>
namespace po = boost::program_options;
#endif

#include "Version.hpp"
#include "common/LogLevel.hpp"
#include "core/Config.hpp"
#include "utils/ILogger.hpp"
#include "utils/LoggerFactory.hpp"
#include "utils/SpdLogger.hpp"
#include <fmt/core.h>
#include <iostream>

namespace qga::cli
{

    AppCLI::AppCLI() = default;
    AppCLI::~AppCLI() = default;

    /**
     * @brief Main CLI execution entry.
     *
     * Handles command-line arguments using CLI11 or Boost.Program_options
     * and coordinates configuration loading, logging, and execution.
     *
     * @param argc The number of arguments.
     * @param argv The argument array.
     * @return Exit code (0 = success).
     */
    int AppCLI::run(int argc, char** argv)
    {
#ifndef USE_BOOST
        CLI::App app{"QuantumGradesApp CLI - run backtests and simulations"};

        std::string config_path;
        std::string input_file;
        std::string output_file;
        bool show_version = false;

        app.add_flag("--version", show_version, "Show version information");
        app.add_option("--config", config_path, "Path to configuration file");
        app.add_option("--input", input_file, "Input data file (CSV)");
        app.add_option("--output", output_file, "Output results file (JSON)");

        CLI11_PARSE(app, argc, argv);

        if (show_version)
        {
            std::cout << "QuantumGradesApp " << APP_VERSION << " (" << APP_BUILD_DATE << ")\n";
            return 0;
        }

        // === Load configuration ===
        auto& config = qga::core::Config::getInstance();
        if (!config_path.empty())
        {
            config.loadFromFile(config_path);
            std::cout << "Loaded configuration: " << config_path << "\n";
        }

        // === Log startup ===
        auto logger = qga::utils::LoggerFactory::createConsoleLogger("CLI", qga::LogLevel::Info);
        logger->info(fmt::format("CLI started with input file: {}", input_file));

        // === Placeholder for core logic ===
        std::cout << "Running backtest using input: " << input_file << "\n";
        std::cout << "Results will be saved to: " << output_file << "\n";

#else
        // === Alternative version using Boost.Program_options ===
        po::options_description desc("QuantumGradesApp CLI options");
        desc.add_options()("help,h", "Show help message")("version,v", "Show version information")(
            "config,c", po::value<std::string>(), "Path to configuration file")(
            "input,i", po::value<std::string>(), "Input data file (CSV)")(
            "output,o", po::value<std::string>(), "Output results file (JSON)");

        po::variables_map vm;
        po::store(po::parse_command_line(argc, argv, desc), vm);
        po::notify(vm);

        if (vm.count("help"))
        {
            std::cout << desc << "\n";
            return 0;
        }

        if (vm.count("version"))
        {
            std::cout << "QuantumGradesApp v0.8.0\n";
            return 0;
        }
#endif

        return 0;
    }

} // namespace qga::cli
