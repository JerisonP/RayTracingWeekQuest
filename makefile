# Full Makefile Example with src directory
# Variables
CXX = g++
CXXFLAGS = -Wall -Wextra
INCLUDE_DIR = headers
TARGET = my_cpp_program
SRC_DIR = src
SRCS = $(SRC_DIR)/main.cpp
OBJS = main.o  # Build objects in root to avoid path issues
HEADERS = $(wildcard $(INCLUDE_DIR)/*.h)

# Main target to build the executable
$(TARGET): $(OBJS)
	$(CXX) $(OBJS) -o $(TARGET)

# Explicit compilation rule for main.o
main.o: $(SRC_DIR)/main.cpp $(HEADERS)
	$(CXX) $(CXXFLAGS) -I$(INCLUDE_DIR) -c $(SRC_DIR)/main.cpp -o $@

# Clean up generated files
.PHONY: clean
clean:
	rm -f *.o $(TARGET)
