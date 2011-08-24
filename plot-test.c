#include "plot.h"
#include <stdio.h>

double f1(double x) {
	return (x + 3) / (x*x + 5);
}

int main(int argc, char **argv) {
	Interval i = { -5, 5.0001 };

	printf("sizeof(double): %d\n", sizeof(double));
	printf("sizeof(long double): %d\n", sizeof(long double));

	setPlotResolution(1);
	plot_fY_X(&f1, i);
	plotDelay(3000);

	setPlotResolution(0.5);
	plot_fY_X(&f1, i);
	plotDelay(3000);

	setPlotResolution(0.05);
	plot_fY_X(&f1, i);
	plotDelay(3000);

	printf("plot dimensions: [%d, %d]\n", getPlotWidth(), getPlotHeight());
	setPlotWidth(1000);
	setPlotHeight(1000);
	printf("plot dimensions: [%d, %d]\n", getPlotWidth(), getPlotHeight());
	setPlotDimensions(600, 400);
	printf("plot dimensions: [%d, %d]\n", getPlotWidth(), getPlotHeight());

	return 0;
}

