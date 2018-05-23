DEPENDENCIES := src/main.cpp
TARGET := ising.out

all: $(TARGET)

$(TARGET): $(DEPENDENCIES)
	g++ $(DEPENDENCIES) -o $(TARGET)

%.o: %.cpp
	g++ -std=c++14 -O2 $^ -o $@

clean:
	if [ -e $(TARGET) ]; then rm $(TARGET); fi
	if [ -e src/*.o ]; then rm src/*.o; fi
