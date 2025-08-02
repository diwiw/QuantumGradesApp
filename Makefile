CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++20 -Iinclude -Iexternal

SRC = $(wildcard src/*.cpp)  #src/Grades.cpp src/Logger.cpp src/main.cpp
SRC_NO_MAIN = $(filter-out src/main.cpp, $(SRC))
SRC_OBJ = $(patsubst src/%.cpp, build/%.o, $(SRC))
SRC_OBJ_NO_MAIN = $(patsubst src/%.cpp, build/%.o, $(SRC_NO_MAIN))

TEST_SRC = $(wildcard tests/*.cpp)
TESTS_OBJ = $(patsubst tests/%.cpp, build/tests/%.o, $(TEST_SRC))
TEST_BIN = build/tests_run

all: app

APP = build/app

app: $(SRC_OBJ)
	$(CXX) $(CXXFLAGS) -o $@ $^

build/%.o: src/%.cpp
	@mkdir -p build
	$(CXX) $(CXXFLAGS) -c $< -o $@

build/tests/%.o: tests/%.cpp
	@mkdir -p build/tests
	$(CXX) $(CXXFLAGS) -c $< -o $@

tests: $(SRC_OBJ_NO_MAIN) $(TESTS_OBJ)
	@mkdir -p bin
	$(CXX) $(CXXFLAGS) -o $(TEST_BIN) $^

run_tests: tests
	./$(TEST_BIN)

.PHONY: clean
clean:
	rm -rf build/* logs/* bin/*
