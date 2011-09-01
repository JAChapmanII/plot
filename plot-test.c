#include "plot.h"
#include <stdio.h>

double f1(double x) {
	return x*x;
}

int main(int argc, char **argv) {
	int n;
	double x[13], y[13], h = 0.1;

	setPlotXInterval(0.0, 5.0);
	setPlotYInterval(0.0, 25.0);

	setPlotResolution(1);
	clearPlot();
	drawAxes();
	plot_fYofX(f1);
	pause();

	printf("res: 0.5\n");
	setPlotResolution(0.5);
	clearPlot();
	drawAxes();
	plot_fYofX(f1);
	pause();

	printf("res: 0.1\n");
	setPlotResolution(0.1);
	clearPlot();
	drawAxes();
	plot_fYofX(f1);
	pause();

	printf("res: 0.01\n");
	setPlotResolution(0.01);
	clearPlot();
	drawAxes();
	plot_fYofX(f1);
	pause();

	printf("approx vs exact\n");
	setPlotXInterval(0.0, 1.0);
	setPlotYInterval(0.0, 1.0);

	setPlotResolution(0.01);
	clearPlot();
	drawAxes();
	plot_fYofX(f1);

	x[0] = 0; y[0] = 0;
	for(n = 0; n < 13; ++n) {
		x[n+1] = n * h;
		y[n+1] = y[n] + h * 2 * x[n];
		drawDot(x[n], y[n]);
	}
	pause();

	return 0;
}

