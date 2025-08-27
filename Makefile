CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -Werror -O2
SRCS = $(wildcard src/*.cpp src/*/*.cpp)
OBJS = $(SRCS:.cpp=.o)
BIN = bin/Practica1.exe

all: $(BIN)
$(BIN): $(OBJS)
	@mkdir -p bin
	$(CXX) $(CXXFLAGS) -o $@ $(OBJS)

clean:
	-del /Q $(OBJS) $(BIN)