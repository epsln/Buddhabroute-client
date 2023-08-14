#include "include/math_utils.h"
#include "include/bmp.h"
#include "include/vroot.h"
#include "include/readFiles.h"
#include "include/draw.h"

#include <stdio.h>
#include <math.h>
#include <string.h>
#include <signal.h>
#include <complex.h>

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xos.h>

#define MAX_SOURCE_SIZE (0x100000)

char checkpoint_filename[256];
char output_dir[256];

void sig_handler(int signo){
	char buffer_filename[256];

	sprintf(buffer_filename, "%s/%s", output_dir, "checkpoint_buffer.csv");
	printf("%s %s\n", buffer_filename, checkpoint_filename);
	if (signo == SIGINT)
		rename(buffer_filename, checkpoint_filename);
	if (signo == SIGKILL)
		rename(buffer_filename, checkpoint_filename);
	exit(1);
}

void buddhaCPU(params_t* p_params, xStuff_t* x){
	u_int32_t *histogram = (u_int32_t*) malloc(p_params->resx * p_params->resy * sizeof(u_int32_t));
	complex* trajs = (complex*) malloc(p_params->maxiter * sizeof(complex));
	int iter = 0;
	float invSqr2 = 1/sqrt(2);

	int animation_state = 0;
	XColor blackx, blacks;
	XAllocNamedColor(x->dpy, DefaultColormapOfScreen(DefaultScreenOfDisplay(x->dpy)), "black", &blacks, &blackx);

	strcpy(checkpoint_filename, p_params -> checkpoint_filename);
	strcpy(output_dir, p_params -> output_dir);

	while(1){
		complex r = rand_complex(-4 - 4 * I, 4 + 4 * I);
		trajs[0] = rand_complex(-4 - 4 * I, 4 + 4 * I);
		for (int i = 1; i < p_params->maxiter; i++){
			trajs[i] = r * trajs[i - 1] * (1 - trajs[i - 1]);
			//if (cabs(trajs[i] - trajs[i - 1]) < 1e-5){trajs[i] = -10; break;};
			if (cabs(trajs[i]) > 2){trajs[i] = -10; break;}
			if (i == p_params->maxiter - 1){trajs[0] = -10; break;}
		}


		for(int i = 0; i < p_params->maxiter; i++){
			if (trajs[i] == -10) break;
			int x = (int)map(creal(trajs[i]), -0.5, 0.5, 0, p_params->resx);
			int y = (int)map(cimag(trajs[i]), -0.5 * invSqr2, 0.5 * invSqr2, 0, p_params->resy);
			if (x >= 0 && x < p_params->resx && y >= 0 && y < p_params->resy){
				histogram[x + y * p_params->resx]++;
			}
		}
		float f = 0;
		switch (animation_state) {
			case 0:
				drawTrajs(p_params, x, trajs);

				if (iter % p_params -> n_points == p_params->n_points - 1) animation_state = 1;
				break;
			case 1:
				drawHistogram(p_params, x, histogram);
				animation_state = 2;
				break;
			case 2:
				if (iter % p_params-> n_points == 0) {
					animation_state = 0;
					XFillRectangle(x->dpy, x->root, x->g, 0, 0, x->wa.width, x->wa.height);
					XFlush(x->dpy);
				}
				break;
		}
		if (iter % p_params -> n_points == 0)
			writeCheckpoint(p_params, histogram);
		signal(SIGINT, sig_handler);
		iter++;
	}
}
