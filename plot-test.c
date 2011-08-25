#include "plot.h"
#include <stdio.h>

double f1(double x) {
	return (x + 3) / (x*x + 5);
}

int main(int argc, char **argv) {
	Interval xi = { -5, 5 }, yi = getYInterval_fYofX(f1, xi);
	double res = 3;

	for(; res >= 0.01; res -= .01) {
		setPlotResolution(res);
		clearPlot();
		drawAxes(xi, yi);
		plot_fYofX(f1, xi, yi);
		plotDelay(50);
	}

	return 0;
}

