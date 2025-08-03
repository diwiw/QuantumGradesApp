APP_VERSION = 0.2.1
VERSION_FILE	= include/version.h
VERSION 	= $(shell git describe --tags --always --dirty)
BUILD_DATE	= $(shell date '+%Y-%m-%d %H:%M:%S')

$(VERSION_FILE):
	@echo "@pragma once" > $(VERSION_FILE)
	@echo "#define APP_VERSION \"$(VERSION\"" >> $(VERSION_FILE)
	@echo "#define APP_BUILD_DATE \"$(BUILD_DATE)\"" >> $(VERSION_FILE)

CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++20 -Iinclude -Iexternal

SRC = $(wildcard src/*.cpp)  #src/Grades.cpp src/Logger.cpp src/main.cpp
SRC_NO_MAIN = $(filter-out src/main.cpp, $(SRC))
SRC_OBJ = $(patsubst src/%.cpp, build/%.o, $(SRC))
SRC_OBJ_NO_MAIN = $(patsubst src/%.cpp, build/%.o, $(SRC_NO_MAIN))

TEST_SRC = $(wildcard tests/*.cpp)
TESTS_OBJ = $(patsubst tests/%.cpp, build/tests/%.o, $(TEST_SRC))


APP_BIN = bin/app
TEST_BIN = bin/tests_run

all: $(APP_BIN)

include/version.h:
	@mkdir -p include
	@echo "Generating version.h for QuantumGradesApp v$(APP_VERSION)"
	@echo "#ifndef VERSION_H " >  include/version.h
	@echo "#define VERSION_H " >> include/version.h
        
	@echo "#define APP_VERSION \"$(shell git describe --always --tags --dirty 2>/dev/null || echo 'dev')\"" >> include/version.h
	@echo "#define APP_BUILD_DATE __DATE__ " " __TIME__" >> include/version.h
	@echo "#endif" >> include/version.h

$(APP_BIN): include/version.h $(SRC_OBJ)
	@mkdir -p bin
	@echo "--------------------------------------"
	@echo ">> Building QuantumGradesApp v$(APP_VERSION)"
	@echo ">> Build date: $(shell date '+%Y-%m-%d %H:%M:%S')"
	@echo "--------------------------------------"
	$(CXX) $(CXXFLAGS) -o $@ $(SRC_OBJ)

build/%.o: src/%.cpp include/version.h
	@mkdir -p build
	$(CXX) $(CXXFLAGS) -c $< -o $@

build/tests/%.o: tests/%.cpp include/version.h
	@mkdir -p build/tests
	$(CXX) $(CXXFLAGS) -c $< -o $@

tests: include/version.h $(SRC_OBJ_NO_MAIN) $(TESTS_OBJ)
	@mkdir -p bin
	@echo "-------------------------------------"
	@echo ">> Building QuantumGradesApp v$(APP_VERSION)"
	@echo ">> Build date: $(shell date '+%Y-%m-%d %H:%M:%S')"
	@echo "-------------------------------------"
	$(CXX) $(CXXFLAGS) -o $(TEST_BIN) $^

run_tests: tests
	./$(TEST_BIN)

.PHONY: clean
clean:
	rm -rf build/* logs/* bin/* include/version.h
