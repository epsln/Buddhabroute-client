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
	printf(" ?");
	params_t parameters; 
	parameters.resx = 4960;
	parameters.resy = 7106;
	parameters.n_points = 1000;
	parameters.maxiter = 1e6;
	parameters.n_kernels = 32;
	printf(" ?");
	/*
	XColor blackx, blacks;
	xStuff x;
	x.dpy = XOpenDisplay(getenv("DISPLAY"));
	x.root = DefaultRootWindow(x.dpy);
	x.g = XCreateGC(x.dpy, x.root, 0, NULL);
	printf(" ?");
	XGetWindowAttributes(x.dpy, x.root, &x.wa);
	printf(" ?");

	XAllocNamedColor(x.dpy, DefaultColormapOfScreen(DefaultScreenOfDisplay(x.dpy)), "black", &blacks, &blackx);
	printf(" ?");

	XSetForeground(x.dpy, x.g, blacks.pixel);
	printf(" ?");
	XFillRectangle (x.dpy, x.root, x.g, 0, 0, x.wa.width, x.wa.height);
	printf("??");
	*/
	printf("re %d\n", parameters.resx);
	printf("re %d\n", parameters.resy);
	printf("re %d\n", parameters.n_points);
	printf("re %d\n", parameters.maxiter);
	printf("re %d\n", parameters.n_kernels);

	srand(time(NULL));

	buddhaCPU(parameters);
}
