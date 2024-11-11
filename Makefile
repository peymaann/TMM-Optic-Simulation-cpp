CXX = g++

CXXFLAGS = -std=c++17 -lstdc++fs -fopenmp -Wall -Wextra -O0 -Iinclude -MMD

TARGET = bin/tmm

SRC_DIR = src
OBJ_DIR = obj

SRCS = $(wildcard $(SRC_DIR)/*.cpp)
OBJS = $(patsubst $(SRC_DIR)/%.cpp, $(OBJ_DIR)/%.o, $(SRCS))
DEPS = $(patsubst $(SRC_DIR)/%.cpp, $(OBJ_DIR)/%.d, $(SRCS))

.PHONY: all
all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp | $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

-include $(DEPS)

.PHONY: clear
clear:
	rm -rf $(OBJ_DIR)/*.o $(OBJ_DIR)/*.d

.PHONY: clean
clean: clear
	rm -f $(TARGET)
