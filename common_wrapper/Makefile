CXX = g++
CXXFLAGS = -std=c++17 -Wall

TARGET = wrapper

$(TARGET): wrapper.cpp
	$(CXX) $(CXXFLAGS) wrapper.cpp -o $(TARGET)

clean:
	del /Q $(TARGET).exe 2>nul || rm -f $(TARGET)

.PHONY: clean
