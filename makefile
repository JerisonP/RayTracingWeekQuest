# Full Makefile Example with src directory
# Variables
CXX = g++
CXXFLAGS = -Wall -Wextra
INCLUDE_DIR = headers
TARGET = my_cpp_program
SRC_DIR = src
SRCS = $(SRC_DIR)/main.cpp
OBJS = $(SRCS:.cpp=.o)
HEADERS = $(wildcard $(INCLUDE_DIR)/*.h)

# Tell 'make' to look in the src directory for source files
VPATH = $(SRC_DIR)

# Main target to build the executable
$(TARGET): $(OBJS)
	$(CXX) $(OBJS) -o $(TARGET)

# Compilation rule: uses the VPATH setting to find the .cpp file
%.o: %.cpp $(HEADERS)
	$(CXX) $(CXXFLAGS) -I$(INCLUDE_DIR) -c $< -o $@

# Clean up generated files
.PHONY: clean
clean:
	rm -f $(OBJS) $(TARGET)
