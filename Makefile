DEPENDENCIES := src/main.o src/mylib.o src/problem.o src/ising_solver.o src/random_selector.o
TARGET := ising

all: $(TARGET)

$(TARGET): $(DEPENDENCIES)
	g++ $^ -o $@

%.o: %.cpp
	g++ -std=c++14 -Wall -O2 -c $^ -o $@

clean:
	rm -f $(TARGET)
	rm -f src/*.o
