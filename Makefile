CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++17 -Iinclude

SRC = $(wildcard src/*.cpp)
OBJ = $(SRC:.cpp=.o)
EXEC = build/app
TESTS = $(wildcard tests/*.cpp)
TEST_BIN = build/test_runner

$(shell mkdir -p build)

all: $(EXEC)

$(EXEC): $(OBJ)
	$(CXX) $(CXXFLAGS) $^ -o $@ 

build/%.o: src/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

tests: $(TESTS) src/Grades.cpp
	$(CXX) $(CXXFLAGS) -o $(TEST_BIN) $(TESTS) src/Grades.cpp
	
.PHONY: tests run_tests clean

run_tests: tests
	./$(TEST_BIN)

clean:
	rm -rf build/*
