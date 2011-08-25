#include "plot.h"
#include <stdio.h>

double f1(double x) {
	return (x + 3) / (x*x + 5);
}

int main(int argc, char **argv) {
	Interval i = { -5, 5.0001 }, yi = getYInterval_fYofX(&f1, i);
	double res = 1;

	printf("sizeof(double): %d\n", sizeof(double));
	printf("sizeof(long double): %d\n", sizeof(long double));

	for(; res >= 0.01; res -= 0.01) {
		setPlotResolution(res);
		clearPlot();
		drawAxes(i, yi);
		plot_fYofX(f1, i, yi);
		plotDelay(50);
	}

	printf("plot dimensions: [%d, %d]\n", getPlotWidth(), getPlotHeight());
	setPlotWidth(1000);
	setPlotHeight(1000);
	printf("plot dimensions: [%d, %d]\n", getPlotWidth(), getPlotHeight());
	setPlotDimensions(600, 400);
	printf("plot dimensions: [%d, %d]\n", getPlotWidth(), getPlotHeight());

	return 0;
}

