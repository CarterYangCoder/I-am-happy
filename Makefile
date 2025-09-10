# Makefile for the game project
CXX = g++
CXXFLAGS = -g -Wall -std=c++17
TARGET = mud
SRCDIR = .
SOURCES = $(wildcard $(SRCDIR)/*.cpp)
OBJECTS = $(SOURCES:.cpp=.o)
HEADERS = $(wildcard $(SRCDIR)/*.h)

$(TARGET): $(OBJECTS)
	$(CXX) $(OBJECTS) -o $(TARGET)

%.o: %.cpp $(HEADERS)
	$(CXX) $(CXXFLAGS) -c $< -o $@

rebuild: clean $(TARGET)
	@echo "Rebuild completed"

clean:
	rm -f $(OBJECTS) $(TARGET).exe $(TARGET)
	@echo "Cleaned all object files and executables"

.PHONY: clean

# Manual compilation command:
# g++ -g -Wall -std=c++17 *.cpp -o game_cre