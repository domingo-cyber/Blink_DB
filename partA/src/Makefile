# Makefile for Part-A: Storage Engine REPL

CXX = g++
CXXFLAGS = -std=c++17 -pthread -Wall -Wextra
TARGET = blinkdb_repl

SRCS = main.cpp storage_engine.cpp
OBJS = $(SRCS:.cpp=.o)

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(TARGET) $(OBJS)
