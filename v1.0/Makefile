# Makefile for Student Grade Calculator v1.0

CXX = g++
CXXFLAGS = -std=c++11 -O2 -Wall

TARGET = student_grading_v10
SRC = main.cpp Person.cpp

all: $(TARGET)

$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SRC)

clean:
	rm -f $(TARGET)
