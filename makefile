snake: snake.o graphics.o grid.o
	g++ -g -o snake -L/usr/local/lib/ -lSDL snake.o graphics.o grid.o

snake.o: snake.cpp
	g++ -g `sdl-config --cflags` -c snake.cpp

graphics.o: graphics.cpp graphics.h
	g++ -g `sdl-config --cflags` -c graphics.cpp

grid.o: grid.cpp grid.h
	g++ -g `sdl-config --cflags` -c grid.cpp

