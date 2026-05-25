CXX := clang++
CXXFLAGS := -std=c++20 -O3 -DNDEBUG -flto=thin -march=native -mtune=native -ftemplate-depth=16384 -Wall -Wextra -Wpedantic -I.
BUILD_DIR := build
BIN_DIR := bin

.PHONY: demo test trace clean

demo: $(BIN_DIR)/demo

test: $(BIN_DIR)/tests
	./$(BIN_DIR)/tests

trace: CXXFLAGS += -ftime-trace
trace: clean demo test

$(BIN_DIR)/demo: main.cpp lambda.hpp include/lc/core.hpp include/lc/intrinsics.hpp include/lc/eval.hpp include/lc/std.hpp include/lc/runtime.hpp | $(BIN_DIR)
	$(CXX) $(CXXFLAGS) main.cpp -o $(BIN_DIR)/demo

$(BIN_DIR)/tests: tests.cpp lambda.hpp include/lc/core.hpp include/lc/intrinsics.hpp include/lc/eval.hpp include/lc/std.hpp include/lc/runtime.hpp | $(BIN_DIR)
	$(CXX) $(CXXFLAGS) tests.cpp -o $(BIN_DIR)/tests

$(BIN_DIR):
	mkdir -p $(BIN_DIR)

clean:
	rm -rf $(BUILD_DIR) $(BIN_DIR) *.json
