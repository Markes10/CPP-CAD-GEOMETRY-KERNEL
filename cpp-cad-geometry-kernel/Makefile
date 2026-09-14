CXX ?= g++
CXXFLAGS ?= -std=c++20 -O3 -Wall -Wextra -Isrc

TARGET = cad_kernel
SRCS = src/main.cpp src/cad_kernel.cpp
OBJS = $(SRCS:.cpp=.o)

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $(OBJS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

test: $(TARGET)
	./$(TARGET)

clean:
	rm -f src/*.o $(TARGET)

.PHONY: all test clean
