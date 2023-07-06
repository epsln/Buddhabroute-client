#include "include/bmp.h"
#include "include/math_utils.h"
#include "include/readFiles.h"
#include "include/config.h"
#include "include/buddha_cpu.h"
#include "include/vroot.h"
#include "include/draw.h"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <math.h>
#include <config.h>
#include <CL/cl.h>

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xos.h>

int main(){
	params_t parameters;
	params_t* p_parameters = &parameters;
	p_parameters->resx = 4960;
	p_parameters->resy = 7106;
	p_parameters->n_points = 1000;
	p_parameters->maxiter = 1e6;
	p_parameters->n_kernels = 32;

	XColor blackx, blacks;
	xStuff_t x;
	x.dpy = XOpenDisplay(getenv("DISPLAY"));
	x.root = DefaultRootWindow(x.dpy);
	x.g = XCreateGC(x.dpy, x.root, 0, NULL);
	XGetWindowAttributes(x.dpy, x.root, &x.wa);

	XAllocNamedColor(x.dpy, DefaultColormapOfScreen(DefaultScreenOfDisplay(x.dpy)), "black", &blacks, &blackx);
	XFillRectangle(x.dpy, x.root, x.g, 0, 0, x.wa.width, x.wa.height);
	XFlush(x.dpy);

	XColor whitex, whites;
	XAllocNamedColor(x.dpy, DefaultColormapOfScreen(DefaultScreenOfDisplay(x.dpy)), "white", &whites, &whitex);
	XSetForeground(x.dpy,x.g,whites.pixel);

	srand(time(NULL));
	xStuff_t* p_x = &x;

	buddhaCPU(p_parameters, p_x);
}
