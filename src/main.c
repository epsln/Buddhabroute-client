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
	//Hyperparameters read from the config file
	t_params parameters; 
	t_params* p_parameters;

	readConf(p_parameters);


	srand(time(NULL));

	mandelIterOpenCL(params);
}
