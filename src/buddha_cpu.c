#include "include/math_utils.h"
#include "include/bmp.h"
#include "include/vroot.h"
#include "include/readFiles.h"
#include "include/draw.h"

#include <stdio.h>
#include <math.h>
#include <string.h>
#include <complex.h>
#include <CL/cl.h>

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xos.h>

#define MAX_SOURCE_SIZE (0x100000)

void buddhaCPU(params_t* p_params){
	u_int32_t *histogram = (u_int32_t*) malloc(p_params->resx * p_params->resy * sizeof(u_int32_t));
	//complex* trajs = (complex*) malloc(p_params->maxiter * sizeof(complex));
	complex trajs[p_params->maxiter];
	int iter = 0;

	//TODO:
	//readCheckpoint(histogram);

	while(1){
		complex r = rand_complex(-2 - 2 * I, 2 + 2 * I);
		trajs[0] = rand_complex(-2 - 2 * I, 2 + 2 * I);
		for (int i = 1; i < p_params->maxiter; i++){
			trajs[i] = r * trajs[i - 1] * (1 - trajs[i - 1]);
			printf("%f %f ", creal(trajs[i - 1]), cimag(trajs[i - 1]));
			printf("%f %f ", creal(trajs[i]), cimag(trajs[i]));
			printf("%f %f\n", creal(r), cimag(r));
			if (cabs(trajs[i] - trajs[i - 1]) < 1e-5){trajs[i] = -10; break;};
			if (cabs(trajs[i]) > 2){trajs[i] = -10; break;}
			if (i == p_params->maxiter - 1){trajs[0] = -10; break;}
		}

		//drawTrajs(p_params, trajs);

		for(int i = 0; i < p_params->maxiter; i++){
			int x = (int)map(creal(trajs[i]), -0.5, 0.5, 0, p_params->resx);
			int y = (int)map(cimag(trajs[i]), -0.35 * sqrt(2), 0.65 * sqrt(2), 0, p_params->resy);
			if (x >= 0 && x < p_params->resx && y >= 0 && y < p_params->resy){
				histogram[x + y * p_params->resx]++;
			}
		}

		if (iter % p_params->n_points == 1){
			//writeCheckpoint(p_params, histogram);
		}
		iter++;
	}
}
