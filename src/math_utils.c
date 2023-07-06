#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <complex.h>

float map(double var1, double min1, double max1, double min2, double max2){
	return min2+(max2-min2)*((var1-min1)/(max1-min1));
}

float float_rand(float min, float max){
	float scale = rand() / (float) RAND_MAX; /* [0, 1.0] */
	return min + scale * ( max - min );      /* [min, max] */
}

complex rand_complex(complex min, complex max){
	float realPart = (creal(max - min)) * ((((float) rand()) / (float) RAND_MAX)) + creal(min);
	float imagPart = (cimag(max - min)) * ((((float) rand()) / (float) RAND_MAX)) + cimag(min);

	return realPart + I * imagPart;
}
