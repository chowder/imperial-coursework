CXX = g++
CXXFLAGS = -Wall -MMD -g

BUILD_DIR = ./build
SRC_DIR = ./src
BIN_DIR = ./bin

SOURCE_FILES = $(wildcard $(SRC_DIR)/*.cpp)
OBJ_FILES = $(patsubst $(SRC_DIR)/%.cpp, $(BUILD_DIR)/%.o, $(SOURCE_FILES))
EXE = $(BIN_DIR)/chess

$(EXE): $(OBJ_FILES)
	$(CXX) $^ -o $(EXE)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

-include $(OBJ:.o=.d)

clean:
	rm $(BUILD_DIR)/*

print:
	echo $(OBJ_FILES)

.PHONY: clean
