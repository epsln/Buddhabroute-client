#include "include/bmp.h"
#include "include/math_utils.h"
#include "include/readFiles.h"
#include "include/mandel.h"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <math.h>
#include <config.h>
#include <CL/cl.h>

#define MAX_SOURCE_SIZE (0x100000)

int main(){
	t_params parameters; 
	parameters.resx = 4960;
	parameters.resx = 7106;
	parameters.n_points = 1000;
	parameters.maxit = 1e6;
	parameters.n_kernels = 32;

	Display *dpy;
	Window root;
	XWindowAttributes wa;
	GC g;

	XColor blackx, blacks;
	XColor whitex, whites;

	XAllocNamedColor(dpy, DefaultColormapOfScreen(DefaultScreenOfDisplay(dpy)), "black", &blacks, &blackx);
	XAllocNamedColor(dpy, DefaultColormapOfScreen(DefaultScreenOfDisplay(dpy)), "white", &whites, &whitex);

	/* open the display (connect to the X server) */
	dpy = XOpenDisplay (getenv ("DISPLAY"));

	/* get the root window */
	root = DefaultRootWindow (dpy);

	/* create a GC for drawing in the window */
	g = XCreateGC (dpy, root, 0, NULL);

	XGetWindowAttributes(dpy, root, &wa);

	XAllocNamedColor(dpy, DefaultColormapOfScreen(DefaultScreenOfDisplay(dpy)), "black", &blacks, &blackx);
	XAllocNamedColor(dpy, DefaultColormapOfScreen(DefaultScreenOfDisplay(dpy)), "white", &whites, &whitex);

	XSetForeground(dpy, g, blacks.pixel);
	XFillRectangle (dpy, root, g, 0, 0, wa.width, wa.height);

	srand(time(NULL));

	buddhaCPU(parameters);
}
