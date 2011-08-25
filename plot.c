#include "plot.h"
#include <stdio.h>

#include <SDL/SDL.h>
#include <GL/gl.h>

/* TODO: change? */
#define MIN_PLOT_RESOLUTION 0.0000001

int plot_Width = 800;
int plot_Height = 600;
double plot_Resolution = 1;

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

void drawAxes(double minX, double maxX, double minY, double maxY) { /* {{{ */
	double ilen = maxX - minX, ylen = maxY - minY;
	glColor3f(0.2f, 0.2f, 0.8f);
	glBegin(GL_LINES);
	glVertex2f(maxX / ilen * plot_Width, 0);
	glVertex2f(maxX / ilen * plot_Width, plot_Height);
	glVertex2f(0, -minY / ylen * plot_Height);
	glVertex2f(plot_Width, -minY / ylen * plot_Height);
	glEnd();
} /* }}} */

/* TODO a lot of this will be needed for all the plot_f* functions, so we
 * should try to get this split up better */
void plot_fY_X(fYofX f, Interval i) {
	double x = i.start, y, ilen = i.end - i.start, min = 999, max = -999, ylen;
	if(plot_CheckState() != 0)
		return;

	while(x < i.end) {
		y = f(x);
		/*printf("f(%.4f): %.16f\n", x, y);*/
		if(y < min)
			min = y;
		else if(y > max)
			max = y;
		x += plot_Resolution;
	}
	min *= 1.1;
	max *= 1.1;
	ylen = max - min;
	printf("ylen: [%f, %f]\n", min, max);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

	drawAxes(i.start, i.end, min, max);

	x = i.start;
	glBegin(GL_LINE_STRIP);
	glColor3f(0.0f, 0.0f, 0.0f);
	while(x < i.end) {
		y = f(x);
		/* TODO this addressing thing is horrible */
		glVertex2f((x - i.start) / ilen * plot_Width,
				(y - min) / ylen * plot_Height);
		x += plot_Resolution;
	}
	glEnd();
	SDL_GL_SwapBuffers();
}

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
	return plot_Resolution;
}
/* }}} */

void plotDelay(int time) {
	SDL_Delay(time);
}

