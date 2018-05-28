DEPENDENCIES := src/main.o src/mylib.o src/problem.o src/ising_solver.cpp
TARGET := ising.out

all: $(TARGET)

$(TARGET): $(DEPENDENCIES)
	g++ $^ -o $@

%.o: %.cpp
	g++ -std=c++14 -O2 -c $^ -o $@

clean:
	if [ -e $(TARGET) ]; then rm $(TARGET); fi
	if [ -e src/*.o ]; then rm src/*.o; fi
