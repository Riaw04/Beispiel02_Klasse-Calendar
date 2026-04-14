CXX := g++
CXXFLAGS := -std=c++17 -Wall -Wextra -pedantic -O2

TARGET := calendar_demo
SRCS := main.cpp Date.cpp Calendar.cpp
OBJS := $(SRCS:.cpp=.o)

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $(OBJS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $<

clean:
	rm -f $(OBJS) $(TARGET)

.PHONY: all clean
