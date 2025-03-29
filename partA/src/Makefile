# Compiler and Flags
CXX = g++
CXXFLAGS = -std=c++17 -pthread -Wall -Wextra

# Target Executable
TARGET = blinkdb_repl

# Source and Object Files
SRCS = main.cpp storage_engine.cpp
OBJ_DIR = o
OBJS = $(addprefix $(OBJ_DIR)/, $(SRCS:.cpp=.o))

# Default target (compiles and runs)
all: $(TARGET)
	./$(TARGET)   # Automatically run the program after compilation

# Link the final executable
$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJS)

# Compile source files into object files
$(OBJ_DIR)/%.o: %.cpp | $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Create the object directory if it doesn't exist
$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

# Clean up build files
clean:
	rm -rf $(TARGET) $(OBJ_DIR)
