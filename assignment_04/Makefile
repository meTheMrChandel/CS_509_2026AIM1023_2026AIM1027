CXX = g++
CXXFLAGS = -O3 -Wall -std=c++17 -Iinclude
SRCS = src/graph.cpp src/csr.cpp src/graph_coloring.cpp src/pagerank.cpp src/kmeans.cpp src/timer.cpp driver/main.cpp
OBJS = $(SRCS:.cpp=.o)
TARGET = main.exe

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f src/*.o driver/*.o $(TARGET)
