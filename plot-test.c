#include <stdio.h>
#include <stdlib.h>

typedef double(*fYofX)(double);
typedef struct {
	double start, end;
} Interval;

double f1(double x) {
	return (x + 3) / (x*x + 5);
}

int width = 800;
int height = 600;

void plot_yOfX(fYofX f, Interval i, double res) {
	double x = i.start;
	while(x < i.end) {
		printf("f(%f): %f\n", x, f(x));
		x += res;
	}
}

int main(int argc, char **argv) {
	Interval i = { -5, 5 };
	plot_yOfX(&f1, i, 0.01);
	return 0;
}

