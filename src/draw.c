#include <stdio.h>
#include <complex.h>
#include <stdlib.h>
#include <stdint.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xos.h>

#include "include/config.h"
#include "include/math_utils.h"
#include "include/draw.h"

void drawTrajs(params_t* p_params, xStuff_t* x, complex* trajs){

	for (int i = 0; i < p_params->maxiter; i++){
		if (creal(trajs[i]) == -10) break;
		float x0 = map(creal(trajs[i]), -0.5, 0.5, 0, x->wa.height);
		float y0 = map(cimag(trajs[i]), -0.35 * 16/9., 0.65 * 16/9. , 0, x->wa.width);

		XDrawPoint(x->dpy, x->root, x->g, x0, y0);
	}
}

void drawHisto(params_t* p_params, u_int32_t** histogram){
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
