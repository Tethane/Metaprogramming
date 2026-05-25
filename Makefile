CXX := clang++
CXXFLAGS := -std=c++20 -O3 -DNDEBUG -flto=thin -march=native -mtune=native -ftemplate-depth=16384 -Wall -Wextra -Wpedantic -I.
BUILD_DIR := build
BIN_DIR := bin
SCRIPTS_DIR := scripts
GENERATED_DIR := generated/lc
SCRIPT_GENERATOR := tools/generate_script_headers.sh
SCRIPT_SOURCES := $(wildcard $(SCRIPTS_DIR)/*.lisp) $(wildcard $(SCRIPTS_DIR)/*.hs)
GENERATED_SCRIPT_INDEX := $(GENERATED_DIR)/scripts.hpp
GENERATED_HASKELL_INDEX := $(GENERATED_DIR)/haskell_scripts.hpp
GENERATED_RESOURCES := $(GENERATED_DIR)/resources.hpp
LIB_HEADERS := lambda.hpp include/lc/core.hpp include/lc/intrinsics.hpp include/lc/eval.hpp include/lc/lisp.hpp include/lc/reader.hpp include/lc/haskell.hpp include/lc/haskell_reader.hpp include/lc/std.hpp include/lc/runtime.hpp include/lc/pretty.hpp

.PHONY: demo test trace clean

demo: $(BIN_DIR)/demo

test: $(BIN_DIR)/tests
	./$(BIN_DIR)/tests

trace: CXXFLAGS += -ftime-trace
trace: clean demo test

$(GENERATED_SCRIPT_INDEX) $(GENERATED_HASKELL_INDEX) $(GENERATED_RESOURCES): $(SCRIPT_SOURCES) $(SCRIPT_GENERATOR) | $(GENERATED_DIR)
	bash $(SCRIPT_GENERATOR) $(SCRIPTS_DIR) $(GENERATED_DIR)

$(BIN_DIR)/demo: main.cpp demo/demo.cpp demo/demo.hpp $(LIB_HEADERS) $(GENERATED_SCRIPT_INDEX) $(GENERATED_HASKELL_INDEX) $(GENERATED_RESOURCES) | $(BIN_DIR)
	$(CXX) $(CXXFLAGS) main.cpp demo/demo.cpp -o $(BIN_DIR)/demo

$(BIN_DIR)/tests: tests.cpp $(LIB_HEADERS) $(GENERATED_SCRIPT_INDEX) $(GENERATED_HASKELL_INDEX) $(GENERATED_RESOURCES) | $(BIN_DIR)
	$(CXX) $(CXXFLAGS) tests.cpp -o $(BIN_DIR)/tests

$(BIN_DIR):
	mkdir -p $(BIN_DIR)

$(GENERATED_DIR):
	mkdir -p $(GENERATED_DIR)

clean:
	rm -rf $(BUILD_DIR) $(BIN_DIR) generated *.json
