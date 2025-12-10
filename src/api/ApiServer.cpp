#include "ApiServer.hpp"
#include <fmt/format.h>

using namespace qga;
using namespace qga::api;

ApiServer::ApiServer(std::shared_ptr<utils::ILogger> logger,
                     const core::Config& config)
    : logger_(std::move(logger)),
      config_(config)
{
    logger_->info("API Server created [port={}, version={}].",
                  config_.apiPort(),
                  config_.version());
}

void ApiServer::start()
{
    registerEndpoints();

    logger_->info("Starting API server on port {}", config_.apiPort());

    bool ok = server_.listen("0.0.0.0", config_.apiPort());

    if (!ok) {
        logger_->error("Failed to start API server on port {}", config_.apiPort());
        throw std::runtime_error("API server failed to bind to port");
    }
}

void ApiServer::stop()
{
    logger_->info("Stopping API server...");
    server_.stop();
}

void ApiServer::registerEndpoints()
{
    logger_->info("Registering API endpoints...");

    // ------------------------------------------------------------
    // GET /health
    // ------------------------------------------------------------
    server_.Get("/health",
        [&](const httplib::Request&, httplib::Response& res)
    {
        res.set_content(R"({"status":"ok"})", "application/json");
    });

    // ------------------------------------------------------------
    // GET /version
    // ------------------------------------------------------------
    server_.Get("/version",
        [&](const httplib::Request&, httplib::Response& res)
    {
        std::string json = fmt::format(
            R"({{"version":"{}"}})",
            config_.version()
        );
        res.set_content(json, "application/json");
    });

    // ------------------------------------------------------------
    // POST /ingest/csv
    // (stub — real ingest lands in milestone 1.1)
    // ------------------------------------------------------------
    server_.Post("/ingest/csv",
        [&](const httplib::Request&,
            httplib::Response& res)
    {
        logger_->info("API: Triggering CSV ingest request");

        // TODO milestone 1.1:
        // DataIngest::fromCsv(config_.dataDir() / "input.csv");

        res.set_content(R"({"status":"ingest_started"})",
                        "application/json");
    });

    // ------------------------------------------------------------
    // GET /grades/report
    // (stub — real implementation in milestone 1.2)
    // ------------------------------------------------------------
    server_.Get("/grades/report",
        [&](const httplib::Request&, httplib::Response& res)
    {
        // TODO milestone 1.2:
        // auto stats = Reporter::generate();
        // res.set_content(stats.toJson(), "application/json");

        res.set_content(R"({"grades":[]})", "application/json");
    });
}

std::string ApiServer::makeErrorJson(const std::string& code,
                                     const std::string& message)
{
    return fmt::format(
        R"({{"error":{{"code":"{}","message":"{}"}}}})",
        code, message
    );
}
