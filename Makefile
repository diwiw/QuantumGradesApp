# Compiler and flags
CXX 		:= g++
CXXFLAGS 	:= -Wall -Wextra -Wno-pragma-once-outside-header -std=c++20 -Iinclude -Iexternal

# Directories
SRC_DIR 	:= src
INCLUDE_DIR 	:= include
TEST_DIR 	:= tests
BUILD_DIR 	:= build
BIN_DIR 	:= bin
DATA_DIR 	:= data

# Files
APP 		:= $(BIN_DIR)/app
TEST_BIN 	:= $(BIN_DIR)/run_tests
VERSION_FILE 	:= $(INCLUDE_DIR)/Version.h
APP_VERSION 	:= 0.3.0
APP_BUILD_DATE	:= $(shell git describe --tags --always --dirty)
COMMIT_TAG 	:= $(shell  date '+%Y-%m-%d %H:%M:%S')


SRC := $(filter-out $(SRC_DIR)/main.cpp, $(wildcard $(SRC_DIR)/*.cpp))
SRC_OBJ := $(patsubst $(SRC_DIR)/%.cpp, $(BUILD_DIR)/%.o, $(SRC))

TEST_SRC := $(wildcard $(TEST_DIR)/*.cpp)
TEST_OBJ := $(patsubst $(TEST_DIR)/%.cpp, $(BUILD_DIR)/tests/%.o, $(TEST_SRC))

# Default target
all: $(APP)

# App build
$(APP): $(SRC_OBJ) $(BUILD_DIR)/main.o | $(BIN_DIR) $(DATA_DIR) $(VERSION_FILE)
	@echo "Linking app: $(APP)"
	$(CXX) $(CXXFLAGS) -o $@ $^

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp | $(BUILD_DIR) $(VERSION_FILE)
	@echo "Compiling $<"
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(BUILD_DIR)/main.o: $(SRC_DIR)/main.cpp | $(BUILD_DIR) $(VERSION_FILE)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Tests build
tests: $(TEST_BIN)

$(TEST_BIN): $(SRC_OBJ) $(TEST_OBJ) | $(BIN_DIR) $(VERSION_FILE)
	@echo "Linking tests: $(TEST_BIN)"
	$(CXX) $(CXXFLAGS) -o $@ $^

$(BUILD_DIR)/tests/%.o: $(TEST_DIR)/%.cpp | $(BUILD_DIR) $(VERSION_FILE)
	@echo "Compiling test $<"
	@mkdir -p $(BUILD_DIR)/tests
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Version header generation
$(VERSION_FILE):
	@mkdir -p $(INCLUDE_DIR)
	@echo "Generating version header QuantumGradesApp v$(APP_VERSION)"
	@echo "#pragma once" > $(VERSION_FILE)
	@echo "#define APP_VERSION \"$(APP_VERSION)\"" >> $(VERSION_FILE)
	@echo "#define APP_BUILD_DATE \"$(APP_BUILD_DATE)\"" >> $(VERSION_FILE)
	@echo "#define COMMIT_TAG \"$(COMMIT_TAG)\"" >> $(VERSION_FILE)
	@echo "Generated APP_VERSION \"$(APP_VERSION)\""
	@echo "APP_BUILD_DATE \"$(APP_BUILD_DATE)\""
	@echo "COMMIT_TAG \"$(COMMIT_TAG)\""

# Create directories
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

$(BUILD_DIR)/tests:
	mkdir -p $(BUILD_DIR)/tests

$(BIN_DIR):
	mkdir -p $(BIN_DIR)

$(DATA_DIR):
	mkdir -p $(DATA_DIR)
	@echo "10\n9\n8\n7\n" > $(DATA_DIR)/readGrades.txt

# Run
run: $(APP)
	./$(APP)

run_tests: tests
	./$(TEST_BIN)

# Docs
docs:
	doxygen Doxyfile

# Clean
clean:
	rm -rf $(BUILD_DIR)/* $(BIN_DIR) $(VERSION_FILE) $(DATA_DIR)

.PHONY: all clean run run_tests tests docs
