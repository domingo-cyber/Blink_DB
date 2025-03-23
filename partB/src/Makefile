# Makefile for BLINK DB Part B (TCP Server)

CXX = g++
CXXFLAGS = -std=c++17 -pthread -Wall -Wextra
TARGET = blinkdb_server

SRCS = main.cpp tcp_server.cpp storage_engine.cpp resp_protocol.cpp
OBJS = $(SRCS:.cpp=.o)

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(TARGET) $(OBJS)
