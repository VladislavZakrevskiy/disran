CXX ?= c++
CXXFLAGS ?= -std=c++17 -Wall -Wextra -pedantic -O2

TARGET := kmp_demo
SRC := kmp_demo.cpp

.PHONY: all run demo clean

all: $(TARGET)

$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) -o $@ $<

run: $(TARGET)
	./$(TARGET) all abacaba aba

demo: run

clean:
	rm -f $(TARGET)
