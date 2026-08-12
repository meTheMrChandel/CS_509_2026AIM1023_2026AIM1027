CXX = g++
CXXFLAGS = -std=c++14 -O3 -Wall -Wextra

SRCS = driver/main.cpp src/gradient_descent.cpp src/maxflow_mincut.cpp
OBJS = $(SRCS:.cpp=.o)
TARGET = main.exe

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

driver/main.o: driver/main.cpp src/gradient_descent.h src/maxflow_mincut.h
src/gradient_descent.o: src/gradient_descent.cpp src/gradient_descent.h
src/maxflow_mincut.o: src/maxflow_mincut.cpp src/maxflow_mincut.h

clean:
	@del /Q driver\main.o src\gradient_descent.o src\maxflow_mincut.o $(TARGET) 2>nul || rm -f driver/main.o src/gradient_descent.o src/maxflow_mincut.o $(TARGET)

.PHONY: all clean
