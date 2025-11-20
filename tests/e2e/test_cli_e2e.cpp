#include "doctest.h"
#include <cstdlib>
#include <cstring>
#include <filesystem>
#include <fstream>
#include <string>
#include <vector>

namespace fs = std::filesystem;

// -----------------------------------------
// Utility helpers
// -----------------------------------------
static std::string runCliAndCapture(const std::string& command, int& exit_code)
{
#ifdef _WIN32
    std::string wrapped = command + " > tmp_cli_out.txt 2>&1";
    exit_code = std::system(wrapped.c_str());
    std::ifstream ifs("tmp_cli_out.txt");
#else
    std::string wrapped = command + " > tmp_cli_out.txt 2>&1";
    exit_code = std::system(wrapped.c_str());
    std::ifstream ifs("tmp_cli_out.txt");
#endif

    std::string result((std::istreambuf_iterator<char>(ifs)), std::istreambuf_iterator<char>());
    return result;
}

static void writeFile(const fs::path& path, const std::string& content)
{
    std::ofstream ofs(path);
    REQUIRE_MESSAGE(ofs.good(), "Failed to create fixture file: " << path);
    ofs << content;
}

// -----------------------------------------
// Paths to fixture resources
// (adjust if your directory structure differs)
// -----------------------------------------
static fs::path projectRoot()
{
    // CTest runs binary from build/.../tests/e2e
    // This climbs up to project root deterministically.
    fs::path p = fs::current_path();
    while (!p.empty())
    {
        if (fs::exists(p / "CMakeLists.txt"))
        {
            return p;
        }
        p = p.parent_path();
    }
    REQUIRE_MESSAGE(false, "Project root not found.");
    return {};
}

static fs::path cliBinaryPath()
{
#ifdef _WIN32
    return projectRoot() / "build" / "Debug" / "bin" / "qga_cli.exe";
#else
    return projectRoot() / "build" / "Debug" / "bin" / "qga_cli";
#endif
}

static fs::path fixturesPath() { return projectRoot() / "tests" / "fixtures" / "e2e"; }

// -----------------------------------------
// E2E tests
// -----------------------------------------
TEST_CASE("E2E: happy path – CLI executes on CSV + config")
{

    const auto ROOT = projectRoot();
    const auto CLI = cliBinaryPath();
    const auto FX = fixturesPath();

    REQUIRE_MESSAGE(fs::exists(CLI), "CLI binary not found: " << CLI);

    // -------------------------------------
    // 1. Prepare temp directory for output
    // -------------------------------------
    fs::path temp_dir = ROOT / "build" / "e2e_tmp";
    fs::remove_all(temp_dir);
    fs::create_directories(temp_dir);

    fs::path csv_in = FX / "sample_short.csv";
    fs::path config_in = temp_dir / "config_e2e.json";
    fs::path output_out = temp_dir / "result_output.csv";

    REQUIRE_MESSAGE(fs::exists(csv_in), "CSV fixture missing: " << csv_in);

    // -------------------------------------
    // 2. Generate config (deterministic)
    // -------------------------------------
    std::string config_json = R"({
        "input": {
            "format": "csv",
            "path": "PLACEHOLDER_CSV"
        },
        "output": {
            "format": "csv",
            "path": "PLACEHOLDER_OUTPUT"
        }
    })";

    // Replace placeholders manually (simple, deterministic)
    {
        std::string replaced = config_json;
        size_t pos;

        pos = replaced.find("PLACEHOLDER_CSV");
        REQUIRE(pos != std::string::npos);
        replaced.replace(pos, std::string("PLACEHOLDER_CSV").size(), csv_in.string());

        pos = replaced.find("PLACEHOLDER_OUTPUT");
        REQUIRE(pos != std::string::npos);
        replaced.replace(pos, std::string("PLACEHOLDER_OUTPUT").size(), output_out.string());

        writeFile(config_in, replaced);
    }

    // -------------------------------------
    // 3. Run CLI
    // -------------------------------------
    std::string command = "\"" + CLI.string() + "\" --config \"" + config_in.string() + "\"";
    int exit_code = 0;
    std::string stdout_and_err = runCliAndCapture(command, exit_code);

    // -------------------------------------
    // 4. Asserts
    // -------------------------------------
#ifdef _WIN32
    CHECK(exit_code == 0); // Windows returns exactly exit code
#else
    CHECK(WEXITSTATUS(exit_code) == 0);
#endif

    CHECK_MESSAGE(fs::exists(output_out), "Output file was not created: " << output_out
                                                                          << "\nCLI output:\n"
                                                                          << stdout_and_err);

    // Minimal correctness check: file non-empty
    {
        std::ifstream ofs(output_out);
        std::string line;
        std::getline(ofs, line);
        CHECK(!line.empty());
    }
}

TEST_CASE("E2E: negative case – missing CSV results in error")
{

    const auto ROOT = projectRoot();
    const auto CLI = cliBinaryPath();
    const auto FX = fixturesPath();

    REQUIRE_MESSAGE(fs::exists(CLI), "CLI binary not found: " << CLI);

    fs::path temp_dir = ROOT / "build" / "e2e_tmp_neg";
    fs::remove_all(temp_dir);
    fs::create_directories(temp_dir);

    // Non-existing CSV
    fs::path csv_in = temp_dir / "non_existing_source.csv";
    fs::path config_in = temp_dir / "config_neg.json";
    fs::path output_out = temp_dir / "result_neg.csv";

    // Generate invalid config
    std::string config_json = R"({
        "input": {
            "format": "csv",
            "path": "PLACEHOLDER_CSV"
        },
        "output": {
            "format": "csv",
            "path": "PLACEHOLDER_OUTPUT"
        }
    })";

    {
        std::string replaced = config_json;
        size_t pos;

        pos = replaced.find("PLACEHOLDER_CSV");
        replaced.replace(pos, strlen("PLACEHOLDER_CSV"), csv_in.string());

        pos = replaced.find("PLACEHOLDER_OUTPUT");
        replaced.replace(pos, strlen("PLACEHOLDER_OUTPUT"), output_out.string());

        writeFile(config_in, replaced);
    }

    // -------------------------------------
    // Run CLI
    // -------------------------------------
    std::string command = "\"" + CLI.string() + "\" --config \"" + config_in.string() + "\"";
    int exit_code = 0;
    std::string stdout_and_err = runCliAndCapture(command, exit_code);

    // -------------------------------------
    // Assertions
    // -------------------------------------
#ifdef _WIN32
    CHECK(exit_code != 0);
#else
    CHECK(WEXITSTATUS(exit_code) != 0);
#endif

    bool contains_error = stdout_and_err.find("error") != std::string::npos ||
                          stdout_and_err.find("Error") != std::string::npos ||
                          stdout_and_err.find("ERROR") != std::string::npos;

    CHECK_MESSAGE(contains_error, "Expected some error message, got:\n" << stdout_and_err);

    CHECK(!fs::exists(output_out));
}
