#include "plot.h"
#include <stdio.h>

double f1(double x) {
	return (x + 3) / (x*x + 5);
}

int main(int argc, char **argv) {
	Interval i = { -5, 5.0001 };

	plot_yOfX(&f1, i);
	plotDelay(3000);

	setPlotResolution(0.01);
	plot_yOfX(&f1, i);
	plotDelay(3000);

	printf("plot dimensions: [%d, %d]\n", getPlotWidth(), getPlotHeight());
	setPlotWidth(1000);
	setPlotHeight(1000);
	printf("plot dimensions: [%d, %d]\n", getPlotWidth(), getPlotHeight());
	setPlotDimensions(600, 400);
	printf("plot dimensions: [%d, %d]\n", getPlotWidth(), getPlotHeight());

	return 0;
}

