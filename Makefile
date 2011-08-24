LDFLAGS=-lm `sdl-config --libs` -lGL
CFLAGS=-pedantic -ansi -W -Wextra `sdl-config --cflags`

plot-test: plot-test.o plot.o
	$(CC) -o $@ $^ $(LDFLAGS)

clean:
	rm -f *.o plot-test

