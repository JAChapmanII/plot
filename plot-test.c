#include "plot.h"
#include <stdio.h>

double f1(double x) {
	return x*x;
}

double f(double x) {
	return (x + 3) / (x*x + 5);
}

int main(int argc, char **argv) {
	int n;
	double x[13], y[13], h = 0.1;

	setWriteTicks(1);
	setPlotXInterval(-5.0, 5);
	setPlotYInterval(getYInterval_fYofX(f).start, getYInterval_fYofX(f).end);

	setPlotResolution(1);
	plot_fYofX(f);
	pause();

	setPlotXInterval(0.0, 5.0);
	setPlotYInterval(0.0, 25.0);

	setPlotResolution(1);
	plot_fYofX(f1);
	pause();

	printf("res: 0.5\n");
	setPlotResolution(0.5);
	resetPlot();
	plot_fYofX(f1);
	pause();

	printf("res: 0.1\n");
	setPlotResolution(0.1);
	resetPlot();
	plot_fYofX(f1);
	pause();

	printf("res: 0.01\n");
	setPlotResolution(0.01);
	resetPlot();
	plot_fYofX(f1);
	pause();

	printf("approx vs exact\n");
	setPlotXInterval(0.0, 1.0);
	setPlotYInterval(0.0, 1.0);

	setPlotResolution(0.01);
	plot_fYofX(f1);

	x[0] = 0; y[0] = 0;
	for(n = 0; n < 13; ++n) {
		x[n+1] = n * h;
		y[n+1] = y[n] + h * 2 * x[n];
	}
	drawDots(x, y, 13);
	pause();

	return 0;
}

