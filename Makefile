CXX = g++
CXXFLAGS = -O3 -march=native -std=c++17

TARGET = macro_engine
SRC = src/macro_engine_v7.cpp

all:
	$(CXX) $(CXXFLAGS) $(SRC) -o build/$(TARGET)

run: all
	./build/$(TARGET)

clean:
	rm -f build/$(TARGET)
