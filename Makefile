myChip8: main.o chip8.o
	$(CXX) $^ -lglut -lGL -lGLU -o $@

main.o: main.cpp
	$(CXX) -c $^

chip8.o: chip8.cpp chip8.h
	$(CXX) -c $^

clean:
	rm *.o myChip8
