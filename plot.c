#include "plot.h"
#include <stdio.h>

#define MIN_PLOT_RESOLUTION 0.0000001

int plot_Width = 800;
int plot_Height = 600;
double plot_Resolution = 1;

void plot_yOfX(fYofX f, Interval i) {
	double x = i.start;
	while(x < i.end) {
		printf("f(%f): %f\n", x, f(x));
		x += plot_Resolution;
	}
}

void setPlotWidth(int pWidth) {
	if(pWidth < 1) return;
	plot_Width = pWidth;
}
void setPlotHeight(int pHeight) {
	if(pHeight < 1) return;
	plot_Height = pHeight;
}
void setPlotDimensions(int pWidth, int pHeight) {
	if((pWidth < 1) || (pHeight < 1)) return;
	plot_Width = pWidth;
	plot_Height = pHeight;
}

int getPlotWidth() {
	return plot_Width;
}
int getPlotHeight() {
	return plot_Height;
}

void setPlotResolution(double pResolution) {
	if(pResolution < 0)
		pResolution = -pResolution;
	if(pResolution < MIN_PLOT_RESOLUTION)
		pResolution = MIN_PLOT_RESOLUTION;
	plot_Resolution = pResolution;
}
double getPlotResolution() {
}
