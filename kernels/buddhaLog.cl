__kernel void buddhaTraj(__global const float *initPointsA,
				         __global const float *initPointsB, 
				       	 __global const float *initPointsC, 
				       	 __global const float *initPointsD,
				       	 __global float *trajsA,
				       	 __global float *trajsB){

	int gid = get_global_id(0);
	float2 r,z;
	int maxit = 1000000;
	r = (float2)(initPointsA[gid], initPointsB[gid]);
	z = (float2)(initPointsC[gid], initPointsD[gid]);
	float xtemp = 0;
	int escaped = 0;
	int compt = 0;
	for (int i = 0; i < maxit; i++){

		xtemp = r.x * z.x - r.x * z.x * z.x - r.y * z.y + 2 * r.y * z.x * z.y + r.x * z.y * z.y;
		z.y =   r.y * z.x - r.y * z.x * z.x + r.x * z.y - 2 * r.x * z.x * z.y + r.y * z.y * z.y;
		z.x = xtemp;

		trajsA[i + maxit * (idx + npoints * gid)] = z.x;
		trajsB[i + maxit * (idx + npoints * gid)] = z.y;
		if (z.x*z.x+z.y*z.y > 4 ){
			escaped = 1;
			trajsA[i + maxit * (idx + npoints * gid)] = -10.;
			trajsB[i + maxit * (idx + npoints * gid)] = -10.;
			break;
		}

		if (escaped == 0 && compt == maxit){
			for (int j = 0; j < maxit; j++){
				trajsA[j + maxit * (idx + npoints * gid)] = -10.;
				trajsB[j + maxit * (idx + npoints * gid)] = -10.;
			}
		}
		compt++;
	}
}
