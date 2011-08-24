LDFLAGS=-lm
CFLAGS=-pedantic -ansi -W -Wextra

plot-test: plot-test.o plot.o
	$(CC) -o $@ $^ $(LDFLAGS)

clean:
	rm -f *.o plot-test

