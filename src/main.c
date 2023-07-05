#include "include/bmp.h"
#include "include/math_utils.h"
#include "include/readFiles.h"
#include "include/mandel.h"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <math.h>
#include <config.h>
#include <CL/cl.h>

#define MAX_SOURCE_SIZE (0x100000)

int main(){
	t_params parameters; 
	parameters.resx = 4960;
	parameters.resx = 7106;
	parameters.npoints = 1000;
	parameters.maxit = 1e6;
	parameters.n_kernels = 32;


	srand(time(NULL));

	buddhaCPU(parameters);
}
