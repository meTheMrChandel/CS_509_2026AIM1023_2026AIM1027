CXX = g++
CXXFLAGS = -std=c++17 -Wall

TARGET = graph_simulator

SRC = src/graph.cpp \
      src/connected_components.cpp \
      src/triangle_counting.cpp \
      src/betweenness_centrality.cpp \
      src/timer.cpp \
      driver/driver.cpp

INC = -Iinclude

$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) $(SRC) $(INC) -o $(TARGET)

clean:
	del /Q $(TARGET).exe 2>nul || exit 0
