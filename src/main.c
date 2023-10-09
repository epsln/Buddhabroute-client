#include "include/bmp.h"
#include "include/math_utils.h"
#include "include/ini.h"
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
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xos.h>

static int handler(void* params, const char* section, const char* name, const char* value){
    params_t* pconfig = (params_t*)params;

    #define MATCH(s, n) strcmp(section, s) == 0 && strcmp(name, n) == 0
    if (MATCH("IMAGE", "resx")) {
        pconfig->resx= atoi(value);
    } else if (MATCH("IMAGE", "resy")) {
        pconfig->resy = atoi(value);
    } else if (MATCH("IMAGE", "resy")) {
        pconfig->maxiter = atoi(value);
    } else if (MATCH("EXPORT", "n_points")) {
        pconfig->n_points = atoi(value);
    } else if (MATCH("EXPORT", "sleep_time")) {
        pconfig->sleep_time = atoi(value);
    } else {
        return 0;  /* unknown section/name, error */
    }
    return 1;
}

int main(int argc, char *argv[]){
	srand(time(NULL));

	params_t parameters;
	params_t* p_parameters = &parameters;
	if (argc >= 2 && strcmp(argv[1], "--no-output") == 0)
		p_parameters->plot= 0;
	else
		p_parameters->plot= 1;

	if (ini_parse("config.ini", handler, &parameters) < 0) {
		printf("Can't load 'test.ini'\n");
		return 1;
	}

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

	xStuff_t* p_x = &x;

	buddhaCPU(p_parameters, p_x);
}
