#include "doctest.h"
#include <filesystem>
#include <fstream>
#include <string>

#include "Config.h"

namespace fs = std::filesystem;

static fs::path tmp_dir()
{
  fs::path d = fs::path("test_tmp_toml");
  fs::create_directories(d);
  return d;
}

static fs::path write_file(const fs::path& p, std::string_view content)
{
  std::ofstream out(p, std::ios::binary);
  out.write(content.data(), static_cast<std::streamsize>(content.size()));
  out.flush();
  return p;
}

TEST_SUITE("Config/TOML")
{
  TEST_CASE("valid config: all fields parsed")
  {
    auto d = tmp_dir();
    auto f = write_file(d / "valid.toml", R"(# minimal valid
[logging]
level = "DEBUG"
dir  = "logs"
file = "app.log"

[paths]
data_dir = "data"

[engine]
threads = 8
)");

    std::string err;
    auto cfg = Config::load(f.string(), &err);
    CHECK(err.empty());         // brak błędów
    CHECK(cfg.logLevel == "DEBUG");
    CHECK(cfg.logDir  == "logs");
    CHECK(cfg.logFile  == "app.log");
    CHECK(cfg.dataDir  == "data");
    CHECK(cfg.threads  == 8);

    // walidacja końcowa (jeśli masz)
    CHECK(cfg.validate(&err) == true);
    CHECK(err.empty());
  }

  TEST_CASE("missing optional: defaults are kept")
  {
    auto d = tmp_dir();
    auto f = write_file(d / "missing_optional.toml", R"(
[paths]
data_dir = "data"
)");

    std::string err;
    auto cfg = Config::load(f.string(), &err);
    CHECK(err.empty());
    // spodziewane domyślne
    CHECK(cfg.logLevel == "INFO");
    CHECK(cfg.logDir  == "logs");
    CHECK(cfg.logFile == "app.log");
    CHECK(cfg.dataDir == "data");
    CHECK(cfg.threads == 4);
    CHECK(cfg.validate(&err) == true);
    CHECK(err.empty());
  }

  TEST_CASE("wrong type: threads as string -> reject or keep default")
  {
    auto d = tmp_dir();
    auto f = write_file(d / "wrong_type.toml", R"(
[paths]
data_dir = "data"
[engine]
threads = "eight"
)");

    std::string err;
    auto cfg = Config::load(f.string(), &err);
    // nasza implementacja load() powinna zostawić domyślną wartość
    CHECK(cfg.threads == 4);
    // walidacja powinna przejść (bo mamy default) lub zwrócić czytelny błąd — akceptujemy obie strategie:
    bool ok = cfg.validate(&err);
    if (!ok) {
      CHECK_MESSAGE(err.find("threads") != std::string::npos, "Expected error message to mention 'threads'");
    }
  }

  TEST_CASE("out of range: threads < 1 or > 64")
  {
    auto d = tmp_dir();
    auto f1 = write_file(d / "threads_low.toml", R"(
[paths]
data_dir = "data"
[engine]
threads = 0
)");
    auto f2 = write_file(d / "threads_high.toml", R"(
[paths]
data_dir = "data"
[engine]
threads = 1000
)");

    std::string err;

    auto cfg1 = Config::load(f1.string(), &err);
    CHECK(cfg1.validate(&err) == false);
    CHECK_MESSAGE(err.find("threads") != std::string::npos, err.c_str());

    err.clear();
    auto cfg2 = Config::load(f2.string(), &err);
    CHECK(cfg2.validate(&err) == false);
    CHECK_MESSAGE(err.find("threads") != std::string::npos, err.c_str());
  }

  TEST_CASE("missing required: data_dir")
  {
    auto d = tmp_dir();
    auto f = write_file(d / "missing_required.toml", R"(
[logging]
level = "INFO"
)");

    std::string err;
    auto cfg = Config::load(f.string(), &err);
    // brak data_dir → walidacja powinna zwrócić false z komunikatem
    CHECK(cfg.validate(&err) == false);
    CHECK_MESSAGE(err.find("data_dir") != std::string::npos, err.c_str());
  }

  TEST_CASE("malformed TOML: parser error -> defaults, validation decides")
  {
    auto d = tmp_dir();
    auto f = write_file(d / "malformed.toml", R"(
[logging
level = "INFO"
)"); // brakujący nawias, błąd składni

    std::string err;
    auto cfg = Config::load(f.string(), &err);
    // nasz load() łapie wyjątek i zostawia domyślne — err może zawierać opis
    // Wystarczy, że domyślne są sensowne i walidacja to powie:
    bool ok = cfg.validate(&err);
    // Nie narzucamy tu wyniku (zależnie od Twojej polityki), ale jeżeli false, to komunikat powinien być czytelny.
    if (!ok) {
      CHECK_MESSAGE(!err.empty(), "Expected a readable validation error");
    }
  }
}

