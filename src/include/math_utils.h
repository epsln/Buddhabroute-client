#ifndef math_utils
#define math_utils

#include <complex.h>

float map(double var1, double min1, double max1, double min2, double max2);

float float_rand(float min, float max );
complex rand_complex(complex min, complex max );

int progressBar(int i, int iMax, int oldMark);

#endif
