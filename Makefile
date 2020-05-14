TARGET = simulator
SOURCES = main.cpp I_type.cpp I_type.h R_type.cpp R_type.h pipeline.cpp pipeline.h parse_code.cpp parse_code.h register.cpp register.h

$(TARGET): $(SOURCES)
	g++ -o $(TARGET) -O3 -Wall $(SOURCES)

clean:
	rm -f *.o

