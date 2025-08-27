CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -Werror -O2
SRCS = $(wildcard src/*.cpp src/*/*.cpp)
OBJS = $(SRCS:.cpp=.o)
BIN = bin/Practica1

all: $(BIN)
$(BIN): $(OBJS)
	@mkdir -p bin
	$(CXX) $(CXXFLAGS) -o $@ $(OBJS)

clean:
	rm -f $(OBJS) $(BIN)