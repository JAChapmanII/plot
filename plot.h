#ifndef PLOT_H
#define PLOT_H

typedef struct {
	double start, end;
} Interval;

typedef double(*fYofX)(double);

void clearPlot();
void drawAxes(Interval xInterval, Interval yInterval);

void plot_fYofX(fYofX f, Interval xInterval, Interval yInterval);

Interval getYInterval_fYofX(fYofX f, Interval xInterval);

void setPlotWidth(int pWidth);
void setPlotHeight(int pHeight);
void setPlotDimensions(int pWidth, int pHeight);

int getPlotWidth();
int getPlotHeight();

void setPlotResolution(double pResolution);
double getPlotResolution();

void plotDelay(int time);

#endif /* PLOT_H */
