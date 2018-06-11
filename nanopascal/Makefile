OUTPUT= $(BIN_DIR)/sample
SRC_DIR= ./src
BIN_DIR= ./bin
HDR_DIR= $(SRC_DIR)/headers
BIN= bin
GCC= g++
GCC-FLAGS= -std=c++11 -g -c -I $(HDR_DIR) -o
TEST-FILES_DIR= ./test

TEST-FILES= $(wildcard $(TEST-FILES_DIR)/*.pas)
SRC_FILES= $(wildcard $(SRC_DIR)/*.cpp)
OBJ_FILES= $(patsubst $(SRC_DIR)/%.cpp, $(BIN_DIR)/%.o,$(SRC_FILES))
.PHONY: clean run
.SUFFIXES:

all: $(BIN) $(OUTPUT) debug

$(BIN):
	mkdir -p $(BIN);           \

$(OUTPUT): $(OBJ_FILES)
	$(GCC) -g -o $@ $^

$(BIN_DIR)/%.o :$(SRC_DIR)/%.cpp $(HDR_DIR)/%.hpp
	$(GCC) $(GCC-FLAGS) $@ $<

$(BIN_DIR)/main.o :$(SRC_DIR)/main.cpp
	$(GCC) $(GCC-FLAGS) $@ $<

debug:

run: $(OUTPUT)
	$(OUTPUT) ./test.pas

run_all: $(OUTPUT)
	$(OUTPUT) $(TEST-FILES) -d random hola adios

clean:
	rm -f *.o
	rm -f $(BIN_DIR)/*
	rm -f $(OUTPUT)
