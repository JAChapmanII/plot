#include "plot.h"
#include <stdio.h>
#include <float.h>

#include <SDL/SDL.h>
#include <GL/gl.h>

/* TODO: change? */
#define MIN_PLOT_RESOLUTION 0.0000001

/* TODO: handle better? Probably should be relative to window size and
 * interval size. I mean, do we really need to plot from -11 to 11 for a 
 * -1 -> 1 window with a resolution of 1? */
/* We multiply this number by the plot resolution, then graph that much more
 * off the left and right ends of the x interval */
#define PLOT_OVERFLOW 10

int plot_Width = 800;
int plot_Height = 600;
double plot_Resolution = 1;
double plot_Overflow = 10;
Interval plot_XInterval = { -10, 10 };
Interval plot_YInterval = { -10, 10 };

enum PlotState { PS_NINIT, PS_NRSIZE, PS_GOOD, PS_FAIL } plot_State;

SDL_Surface *plot_Screen;

/* TODO: less exit-y */
int plot_Init() {
	const SDL_VideoInfo *videoInfo;
	int videoFlags;
	if(SDL_Init(SDL_INIT_VIDEO) < 0) {
		fprintf(stderr, "Unable to init SDL: %s\n", SDL_GetError());
		exit(1);
	}
	atexit(SDL_Quit);

	videoInfo = SDL_GetVideoInfo();
	if(!videoInfo) {
		fprintf(stderr, "Can't query video info: %s\n", SDL_GetError());
		exit(1);
	}

	videoFlags = SDL_OPENGL;
	videoFlags |= SDL_GL_DOUBLEBUFFER;
	videoFlags |= SDL_HWPALETTE;
	/*videoFlags |= SDL_RESIZABLE; TODO */

	if(videoInfo->hw_available)
		videoFlags |= SDL_HWSURFACE;
	else
		videoFlags |= SDL_SWSURFACE;

	if(videoInfo->blit_hw)
		videoFlags |= SDL_HWACCEL;

	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

	plot_Screen = SDL_SetVideoMode(plot_Width, plot_Height, 32, videoFlags);
	if(plot_Screen == NULL) {
		fprintf(stderr, "Unable to create plot screen: %s\n", SDL_GetError());
		exit(1);
	}

	/* initialize OpenGL */
	glClearColor(1.0f, 1.0f, 1.0f, 0.0f);

	if(plot_Resize() != 0)
		return 1;

	return 0;
}

/* TODO change this to have 0,0 in the center? */
int plot_Resize() {
	double ratio = plot_Width / plot_Height;

	glViewport(0, 0, plot_Width, plot_Height);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	glOrtho(0, plot_Width, 0, plot_Height, 0, 1);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	return 0;
}

int plot_CheckState() {
	switch(plot_State) {
		case PS_NINIT:
			return plot_Init();
		case PS_NRSIZE:
			return plot_Resize();
		case PS_GOOD:
			return 0;
		case PS_FAIL: default:
			return 1;
	}
}

void clearPlot() {
	if(plot_CheckState() != 0)
		return;

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
}

void drawAxes() { /* {{{ */
	double xLen = plot_XInterval.end - plot_XInterval.start,
			yLen = plot_YInterval.end - plot_YInterval.start;
	glColor3f(0.2f, 0.2f, 0.8f);
	glBegin(GL_LINES);

	/* x axis */
	glVertex2f(plot_XInterval.end / xLen * plot_Width, 0);
	glVertex2f(plot_XInterval.end / xLen * plot_Width, plot_Height);

	/* y axis */
	glVertex2f(0, -plot_YInterval.start / yLen * plot_Height);
	glVertex2f(plot_Width, -plot_YInterval.start / yLen * plot_Height);

	glEnd();
} /* }}} */

/* TODO a lot of this will be needed for all the plot_f* functions, so we
 * should try to get this split up better */
void plot_fYofX(fYofX f) {
	double x, y, xLen, yLen, rStart, rEnd;
	if(plot_CheckState() != 0)
		return;

	xLen = plot_XInterval.end - plot_XInterval.start;
	yLen = plot_YInterval.end - plot_YInterval.start;

	/*
	rStart = (plot_XInterval.start + xLen / 2) - (plot_Resolution / 2) -
		(xLen / plot_Resolution / 2);
	rEnd = (plot_XInterval.start + xLen / 2) + (plot_Resolution / 2) +
		(xLen / plot_Resolution / 2);
	*/
	rStart = plot_XInterval.start - plot_Overflow;
	rEnd = plot_XInterval.end + plot_Overflow;

	x = rStart;

	glBegin(GL_LINE_STRIP);
	glColor3f(0.0f, 0.0f, 0.0f);
	while(x < rEnd) {
		y = f(x);
		/* TODO could this addressing be better? */
		glVertex2f((x - plot_XInterval.start) / xLen * plot_Width,
				(y - plot_YInterval.start) / yLen * plot_Height);
		x += plot_Resolution;
	}
	glEnd();
	SDL_GL_SwapBuffers();
}

void drawDot(double x, double y) {
	double xLen, yLen;
	if(plot_CheckState() != 0)
		return;
	xLen = plot_XInterval.end - plot_XInterval.start;
	yLen = plot_YInterval.end - plot_YInterval.start;

	glColor3f(0.0f, 0.0f, 1.0f);
	glPointSize(9);
	glBegin(GL_POINTS);
	/*glBegin(GL_LINE_STRIP);*/
		glVertex2f((x - plot_XInterval.start) / xLen * plot_Width,
				(y - plot_YInterval.start) / yLen * plot_Height);
		/*
		glVertex2f((x - 0.02 - plot_XInterval.start) / xLen * plot_Width,
				(y - plot_YInterval.start) / yLen * plot_Height);
		glVertex2f((x - plot_XInterval.start) / xLen * plot_Width,
				(y + 0.02 - plot_YInterval.start) / yLen * plot_Height);
		glVertex2f((x + 0.02 - plot_XInterval.start) / xLen * plot_Width,
				(y - plot_YInterval.start) / yLen * plot_Height);
		glVertex2f((x - plot_XInterval.start) / xLen * plot_Width,
				(y - 0.02 - plot_YInterval.start) / yLen * plot_Height);
				*/
	glEnd();
	SDL_GL_SwapBuffers();
}

/* Calculate and return an appropriate plot_YInterval for some function f over a
 * particular x interval */
Interval getYInterval_fYofX(fYofX f) { /* {{{ */
	Interval plot_YInterval = { DBL_MAX, -DBL_MAX };
	double x, y;

	plot_XInterval.start -= plot_Overflow;
	plot_XInterval.end += plot_Overflow;

	x = plot_XInterval.start;
	while(x < plot_XInterval.end) {
		y = f(x);
		/*printf("f(%.4f): %.16f\n", x, y);*/
		if(y < plot_YInterval.start)
			plot_YInterval.start = y;
		if(y > plot_YInterval.end)
			plot_YInterval.end = y;
		x += plot_Resolution;
	}
	/* TODO: this expansion fails for 0, and when min = max */
	plot_YInterval.start *= 1.1;
	plot_YInterval.end *= 1.1;
	/*ylen = max - min;
	printf("ylen: [%f, %f]\n", min, max);*/

	return plot_YInterval;
} /* }}} */

/* Plot dimension (width, height, resolution setters and getters {{{ */
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
void setPlotXInterval(double start, double end) {
	plot_XInterval.start = start;
	plot_XInterval.end = end;
}
void setPlotYInterval(double start, double end) {
	plot_YInterval.start = start;
	plot_YInterval.end = end;
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
	plot_Overflow = pResolution * PLOT_OVERFLOW;
}
double getPlotResolution() {
	return plot_Resolution;
}
Interval getXInterval() {
	return plot_XInterval;
}
Interval getYInterval() {
	return plot_YInterval;
}
/* }}} */

void plotDelay(int time) {
	SDL_Delay(time);
}

void pause() {
	SDL_Delay(3000);
}

