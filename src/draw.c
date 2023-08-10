#include <stdio.h>
#include <complex.h>
#include <math.h>
#include <stdlib.h>
#include <stdint.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xos.h>

#include "include/config.h"
#include "include/math_utils.h"
#include "include/draw.h"

unsigned long _RGB(int r,int g, int b)
{
    return b + (g<<8) + (r<<16);
}

void drawTrajs(params_t* p_params, xStuff_t* x, complex* trajs){

	XSetForeground(x -> dpy, x -> g, _RGB(255, 255, 255));
	for (int i = 0; i < p_params->maxiter; i++){
		if (creal(trajs[i]) == -10) break;
		float x0 = map(creal(trajs[i]), -0.5, 0.5, 0, x->wa.height);
		float y0 = map(cimag(trajs[i]), -0.35 * sqrt(2), 0.65 * sqrt(2) , 0, x->wa.width);

		XDrawPoint(x->dpy, x->root, x->g, x0, y0);
	}
}

void drawHistogram(params_t* p_params, xStuff_t* x, u_int32_t* histogram){
	float maxVal = 0;
	for (int i = 0; i < p_params -> resx; i++){
		for (int j = 0; j < p_params -> resy; j++){
			maxVal = fmax(histogram[i * p_params -> resy + j], maxVal);
		}
	}
	maxVal = logf(maxVal + 1);
	for (int i = 0; i < p_params -> resx; i++){
		for (int j = 0; j < p_params -> resy; j++){
			float x0 = map(i, 0, p_params->resx, 0, x->wa.width);
			float y0 = map(j, 0, p_params->resy, 0, x->wa.height);

			float c = logf(histogram[j * p_params -> resx + i] + 1)/maxVal * 255;

			XSetForeground(x -> dpy, x -> g, _RGB(c, c, c));

			XDrawPoint(x->dpy, x->root, x->g, x0, y0);

		}
	}
}
