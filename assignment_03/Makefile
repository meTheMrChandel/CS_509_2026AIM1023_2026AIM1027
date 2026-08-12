CXX = g++
CXXFLAGS = -std=c++14 -O3 -Wall -Wextra

SRCS = src/main.cpp src/gradient_descent.cpp src/maxflow_mincut.cpp
OBJS = $(SRCS:.cpp=.o)
TARGET = main.exe

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	@del /Q src\*.o $(TARGET) 2>nul || rm -f src/*.o $(TARGET)

.PHONY: all clean
