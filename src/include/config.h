#ifndef config
#define config

typedef struct params_t{
	int resx;
	int resy;
	int n_points;
	int maxiter;
	int n_kernels;
	char checkpoint_filename[256];
	char output_dir[256];
}params_t;

#endif
