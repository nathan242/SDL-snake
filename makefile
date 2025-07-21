snake: snake.o graphics.o grid.o
	g++ -o snake snake.o graphics.o grid.o `sdl-config --libs` -lSDL_image

snake.o: snake.cpp
	g++ `sdl-config --cflags` -c snake.cpp

graphics.o: graphics.cpp graphics.h
	g++ `sdl-config --cflags` -c graphics.cpp

grid.o: grid.cpp grid.h
	g++ `sdl-config --cflags` -c grid.cpp

