#ifndef draw
#define draw
#include "vroot.h"
#include <complex.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xos.h>

typedef struct xStuff_t{
	//Just a struct containing every var necessary to draw on the screen
	Display *dpy;
	Window root;
	XWindowAttributes wa;
	GC g;
}xStuff_t;


void drawTrajs(params_t* p_params, xStuff_t* x, complex* trajs);

void drawHistogram(params_t* p_params, xStuff_t* x, u_int32_t* histo);

#endif
