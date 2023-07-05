#ifndef mandel 
#define mandel 
#include "config.h"

void buddhaCPU(params_t params);
void mandelIterOpenCL(char kernelFilename[256], int NPOINTS, int MAXITER, int OPENCL_ITER, int RESX, int RESY, long long int **histogram);
#endif
