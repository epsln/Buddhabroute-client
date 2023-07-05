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

void buddhaCPU(params_t params){
	u_int32_t histogram[params.resx][params.resy];
	complex* trajs = (complex*) malloc(params.maxiter * sizeof(complex));
	int iter = 0;

	//TODO:
	//readCheckpoint(histogram);

	while(1){
		complex r = rand_complex(-2 - 2 * I, 2 + 2 * I);
		trajs[0] = rand_complex(-2 - 2 * I, 2 + 2 * I);
		for (int i = 1; i < params.maxiter; i++){
			trajs[i] = r * trajs[i - 1] * (1 - trajs[i - 1]);
			if (cabs(trajs[i]) > 2){trajs[i] = -10; break;}
		}

		drawTrajs(params, trajs);

		for(int i = 0; i < params.maxiter; i++){
			int x = (int)map(creal(trajs[i]), -0.5, 0.5, 0, params.resx);
			int y = (int)map(cimag(trajs[i]), -0.35 * sqrt(2), 0.65 * sqrt(2), 0, params.resy);	
			if (x >= 0 && x < params.resx && y >= 0 && y < params.resy){
				histogram[x][y]++;
			}
		}

		if (iter % params.n_points == 1){
			writeCheckpoint(params, histogram);
		}
		iter++;
	}
}
