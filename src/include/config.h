#ifndef config
#define config

typedef struct params_t{
	int resx;
	int resy;
	int n_points;
	int maxiter;
	int n_kernels;
	char kernel_filename[256];
}params_t;

#endif
