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

src/main.o: src/main.cpp src/gradient_descent.h src/maxflow_mincut.h
src/gradient_descent.o: src/gradient_descent.cpp src/gradient_descent.h
src/maxflow_mincut.o: src/maxflow_mincut.cpp src/maxflow_mincut.h

clean:
	@del /Q src\*.o $(TARGET) 2>nul || rm -f src/*.o $(TARGET)

.PHONY: all clean
