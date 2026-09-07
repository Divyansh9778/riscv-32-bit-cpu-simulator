CXX = g++
CXXFLAGS = -O2 -std=c++17

all: CacheController TraceGenerator

CacheController: CacheController.cpp
	$(CXX) $(CXXFLAGS) CacheController.cpp -o CacheController

TraceGenerator: TraceGenerator.cpp
	$(CXX) $(CXXFLAGS) TraceGenerator.cpp -o TraceGenerator

clean:
	rm -f CacheController TraceGenerator
