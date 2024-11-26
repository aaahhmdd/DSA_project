# Define compiler and flags
CXX = g++
CXXFLAGS = -std=c++11 -Wall

# Define the target executable and object files
TARGET = main
OBJS = main.o xml_parser.o

# Rule to build the target
$(TARGET): $(OBJS)
	$(CXX) $(OBJS) -o $(TARGET)

# Rule to build main.o
main.o: main.cpp xml_parser.h
	$(CXX) $(CXXFLAGS) -c main.cpp -o main.o

# Rule to build xml_parser.o
xml_parser.o: xml_parser.cpp xml_parser.h
	$(CXX) $(CXXFLAGS) -c xml_parser.cpp -o xml_parser.o

# Rule to clean up generated files
clean:
	rm -f $(OBJS) $(TARGET)
