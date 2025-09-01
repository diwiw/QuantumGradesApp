#include "doctest.h"
#include "FileManager.hpp"
#include <filesystem>
#include <string>
#include <vector>

static const std::string TEST_FILE = "test_temp.txt";

TEST_CASE("FileManager basic file operations"){
	std::string test_file = "test_file.txt";

	// Ensure test file does not exist
	if (std::filesystem::exists(test_file)){
		std::filesystem::remove(test_file);
	}

	SUBCASE("File does not exist at start"){
		CHECK_FALSE(FileManager::exists(test_file));
	}

	SUBCASE("WriteAllLines creates and writes file") {
		std::vector<std::string> lines = {"Line 1", "Line 2", "Line 3"};
		REQUIRE(FileManager::writeAllLines(test_file, lines));
		CHECK(FileManager::exists(test_file));

		auto read_lines = FileManager::readAllLines(test_file);
		REQUIRE(read_lines.has_value());
		CHECK(read_lines->size() == 3);
		CHECK((*read_lines)[0] == "Line 1");
		CHECK((*read_lines)[1] == "Line 2");
		CHECK((*read_lines)[2] == "Line 3");

	}

	SUBCASE("AppendLine adds a new line at the end"){
		std::vector<std::string> lines = {"First"};
		REQUIRE(FileManager::writeAllLines(test_file, lines));

		REQUIRE(FileManager::appendLine(test_file, "Second"));
		auto read_lines = FileManager::readAllLines(test_file);
		REQUIRE(read_lines.has_value());
		CHECK(read_lines-> size() == 2);
		CHECK((*read_lines)[1] == "Second");

	}

	SUBCASE("RemoveFile deletes existing file") {
		std::vector<std::string> lines = {"Temp"};
		REQUIRE(FileManager::writeAllLines(test_file, lines));

		REQUIRE(FileManager::removeFile(test_file));
		CHECK_FALSE(FileManager::exists(test_file));
	}
}
