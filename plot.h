#ifndef PLOT_H
#define PLOT_H

typedef struct {
	double start, end;
} Interval;

typedef double(*fYofX)(double);

void clearPlot();
void resetPlot();
void drawAxes();

void plot_fYofX(fYofX f);

void drawDot(double x, double y);
void drawDots(double *x, double *y, int count);

Interval getYInterval_fYofX(fYofX f);

void setPlotWidth(int pWidth);
void setPlotHeight(int pHeight);
void setPlotDimensions(int pWidth, int pHeight);
void setPlotXInterval(double start, double end);
void setPlotYInterval(double start, double end);

int getPlotWidth();
int getPlotHeight();
Interval getXInterval();
Interval getYInterval();

void setPlotResolution(double pResolution);
double getPlotResolution();

void setWriteTicks(int pWriteTicks);
int getWriteTicks();

void plotDelay(int time);
void pause();

#endif /* PLOT_H */
