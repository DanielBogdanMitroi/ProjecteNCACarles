CC = g++
CFLAGS = -Wall -Wextra -I include
LDFLAGS =

SRC_DIR = src
INC_DIR = include
OBJ_DIR = obj
BIN_DIR = bin
DATA_DIR = datos

SOURCES = $(wildcard $(SRC_DIR)/*.cpp)
OBJECTS = $(SOURCES:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o)
EXECUTABLE = $(BIN_DIR)/escuela

all: dirs $(EXECUTABLE)

dirs:
	mkdir -p $(OBJ_DIR) $(BIN_DIR) $(DATA_DIR)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(OBJECTS) -o $@ $(LDFLAGS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CC) $(CFLAGS) -c $< -o $@

run: all
	./$(EXECUTABLE)

clean:
	rm -rf $(OBJ_DIR) $(BIN_DIR)

.PHONY: all dirs run clean
