#ifndef PLOT_H
#define PLOT_H

typedef struct {
	double start, end;
} Interval;

typedef double(*fYofX)(double);

void plot_yOfX(fYofX f, Interval i);

void setPlotWidth(int pWidth);
void setPlotHeight(int pHeight);
void setPlotDimensions(int pWidth, int pHeight);

int getPlotWidth();
int getPlotHeight();

void setPlotResolution(double pResolution);
double getPlotResolution();

#endif /* PLOT_H */
