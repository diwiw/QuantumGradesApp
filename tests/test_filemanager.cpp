#include "doctest.h"
#include "FileManager.h"
#include <filesystem>
#include <string>
#include <vector>

static const std::string testFile = "test_temp.txt";

TEST_CASE("FileManager basic file operations"){
	std::string testFile = "test_file.txt";

	// Ensure test file does not exist
	if (std::filesystem::exists(testFile)){
		std::filesystem::remove(testFile);
	}

	SUBCASE("File does not exist at start"){
		CHECK_FALSE(FileManager::exists(testFile));
	}

	SUBCASE("WriteAllLines creates and writes file") {
		std::vector<std::string> lines = {"Line 1", "Line 2", "Line 3"};
		REQUIRE(FileManager::writeAllLines(testFile, lines));
		CHECK(FileManager::exists(testFile));

		auto readLines = FileManager::readAllLines(testFile);
		REQUIRE(readLines.has_value());
		CHECK(readLines->size() == 3);
		CHECK((*readLines)[0] == "Line 1");
		CHECK((*readLines)[1] == "Line 2");
		CHECK((*readLines)[2] == "Line 3");

	}

	SUBCASE("AppendLine adds a new line at the end"){
		std::vector<std::string> lines = {"First"};
		REQUIRE(FileManager::writeAllLines(testFile, lines));

		REQUIRE(FileManager::appendLine(testFile, "Second"));
		auto readLines = FileManager::readAllLines(testFile);
		REQUIRE(readLines.has_value());
		CHECK(readLines-> size() == 2);
		CHECK((*readLines)[1] == "Second");

	}

	SUBCASE("RemoveFile deletes existing file") {
		std::vector<std::string> lines = {"Temp"};
		REQUIRE(FileManager::writeAllLines(testFile, lines));

		REQUIRE(FileManager::removeFile(testFile));
		CHECK_FALSE(FileManager::exists(testFile));
	}
}
