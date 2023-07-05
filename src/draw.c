#include <stdio.h>
#include <complex.h>
#include <stdlib.h>
#include <stdint.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xos.h>

#include "include/config.h"

void drawTrajs(params_t params, complex* trajs){
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
	for (int i = 0; i < params.maxiter; i++){
		if (creal(trajs[i]) == -10) break;
		float x = map(cimag(trajs[i]), -0.5, 0.5, 0, wa.width);
		float y = map(creal(trajs[i]), -0.35, 0.65, 0, wa.height);

		XDrawPoint(dpy, root, g, x, y);
	}
}

void drawHisto(params_t params, u_int32_t** histogram){
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
