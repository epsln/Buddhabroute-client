#include <stdio.h>
#include <complex.h>
#include <stdlib.h>
#include <stdint.h>

#include "include/config.h"

void drawTrajs(params_t params, complex** trajs){
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

	XSetForeground(dpy, g, whites.pixel);
	for (int i = 0; i < params.npoints; i++){
		for (int j = 0; j < params.maxit; j++){
			if (trajs[i][j] == -10) break;
			XDrawPoint(dpy, root, g, creal(trajs[i][j]), cimag(trajs[i][j]) 84, 0);
		}
	}
}

void drawHisto(params_t params, uint32_t** histogram){
	Display *dpy;
	Window root;
	XWindowAttributes wa;
	GC g;

	XColor blackx, blacks;
	XColor whitex, whites;

	/* open the display (connect to the X server) */
	dpy = XOpenDisplay (getenv ("DISPLAY"));


	/* get the root window */
	root = DefaultRootWindow (dpy);


	/* create a GC for drawing in the window */
	g = XCreateGC (dpy, root, 0, NULL);

	XGetWindowAttributes(dpy, root, &wa);

	XAllocNamedColor(dpy, DefaultColormapOfScreen(DefaultScreenOfDisplay(dpy)), "black", &blacks, &blackx);
	XAllocNamedColor(dpy, DefaultColormapOfScreen(DefaultScreenOfDisplay(dpy)), "white", &whites, &whitex);

	//TODO
}
