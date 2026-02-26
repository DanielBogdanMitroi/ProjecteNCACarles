	CC = g++
CFLAGS = -Wall -Wextra -I include
LDFLAGS =

ifeq ($(OS),Windows_NT)
EXE_EXT = .exe
MKDIRS_CMD = if not exist $(OBJ_DIR) mkdir $(OBJ_DIR) & if not exist $(BIN_DIR) mkdir $(BIN_DIR) & if not exist $(DATA_DIR) mkdir $(DATA_DIR)
CLEAN_CMD = if exist $(OBJ_DIR) rmdir /s /q $(OBJ_DIR) & if exist $(BIN_DIR) rmdir /s /q $(BIN_DIR)
RUN_CMD = .\\$(EXECUTABLE)
else
EXE_EXT =
MKDIRS_CMD = mkdir -p $(OBJ_DIR) $(BIN_DIR) $(DATA_DIR)
CLEAN_CMD = rm -rf $(OBJ_DIR) $(BIN_DIR)
RUN_CMD = ./$(EXECUTABLE)
endif

SRC_DIR = src
INC_DIR = include
OBJ_DIR = obj
BIN_DIR = bin
DATA_DIR = datos

SOURCES = $(wildcard $(SRC_DIR)/*.cpp)
OBJECTS = $(SOURCES:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o)
EXECUTABLE = $(BIN_DIR)/escuela$(EXE_EXT)

all: dirs $(EXECUTABLE)

dirs:
	$(MKDIRS_CMD)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(OBJECTS) -o $@ $(LDFLAGS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CC) $(CFLAGS) -c $< -o $@

run: all
	$(RUN_CMD)

clean:
	$(CLEAN_CMD)

.PHONY: all dirs run clean
