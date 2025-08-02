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
	@echo "#ifndef PROJECT_VERSION " >  include/version.h
	@echo "#define PROJECT_VERSION \"$(shell git describe --always --tags --dirty 2>/dev/null || echo 'dev')\"" >> include/version.h
	@echo "#endif" >> include/version.h

$(APP_BIN): include/version.h $(SRC_OBJ)
	@mkdir -p bin
	$(CXX) $(CXXFLAGS) -o $@ $(SRC_OBJ)

build/%.o: src/%.cpp include/version.h
	@mkdir -p build
	$(CXX) $(CXXFLAGS) -c $< -o $@

build/tests/%.o: tests/%.cpp include/version.h
	@mkdir -p build/tests
	$(CXX) $(CXXFLAGS) -c $< -o $@

tests: include/version.h $(SRC_OBJ_NO_MAIN) $(TESTS_OBJ)
	@mkdir -p bin
	$(CXX) $(CXXFLAGS) -o $(TEST_BIN) $^

run_tests: tests
	./$(TEST_BIN)

.PHONY: clean
clean:
	rm -rf build/* logs/* bin/* include/version.h
