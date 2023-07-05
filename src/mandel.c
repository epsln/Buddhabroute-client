#include "include/math_utils.h"
#include "include/config.h"
#include "include/bmp.h"
#include "include/readFiles.h"

#include <stdio.h>
#include <math.h>
#include <string.h>
#include <CL/cl.h>

#define MAX_SOURCE_SIZE (0x100000)

void mandelIterOpenCL(t_params* p_params){
	// Allocate memories 
	//We can't pass a 2D array to a kernel, so we'll flatten a 2d array to 1D
	size_t globalItemSize = 32;
	size_t localItemSize = 16; // globalItemSize has to be a multiple of localItemSize. 
	float *trajectoriesA = (float*)malloc(sizeof(float) * p_params.n_kernels * p_params.maxiter);
	float *trajectoriesB = (float*)malloc(sizeof(float) * p_params.n_kernels * p_params.maxiter);

	float *randomPointsA = (float*)malloc(sizeof(float) * p_params.n_kernels);
	float *randomPointsB = (float*)malloc(sizeof(float) * p_params.n_kernels);
	float *randomPointsC = (float*)malloc(sizeof(float) * p_params.n_kernels);
	float *randomPointsD = (float*)malloc(sizeof(float) * p_params.n_kernels);

	float **histogram = malloc(p_params.resx * sizeof(float *));
	for (int i =0; i < p_params.resx; i++) {
		histogram[i] = (float *)malloc(p_params.resy * sizeof(float)); 
	}

	FILE *kernelFile;
	char *kernelSource;
	size_t kernelSize;

	kernelFile = fopen(*p_params.kernelFilename, "r");

	if (!kernelFile) {
		fprintf(stderr, "No file named %s was found !\n", kernelFilename);
		exit(-1);
	}

	kernelSource = (char*)malloc(MAX_SOURCE_SIZE);
	kernelSize = fread(kernelSource, 1, MAX_SOURCE_SIZE, kernelFile);
	fclose(kernelFile);

	// Getting platform and device information
	cl_platform_id platformId = NULL;
	cl_device_id deviceID = NULL;
	cl_uint retNumDevices;
	cl_uint retNumPlatforms;
	cl_int ret = clGetPlatformIDs(1, &platformId, &retNumPlatforms);
	ret = clGetDeviceIDs(platformId, CL_DEVICE_TYPE_DEFAULT, 1, &deviceID, &retNumDevices);

	// Creating context.
	cl_context context = clCreateContext(NULL, 1, &deviceID, NULL, NULL,  &ret);

	// Creating command queue
	cl_command_queue commandQueue = clCreateCommandQueue(context, deviceID, 0, &ret);

	// Memory buffers for each array
	cl_mem randPtsAMemObj = clCreateBuffer(context, CL_MEM_READ_ONLY,  p_params.n_kernels * sizeof(float), NULL, &ret);
	cl_mem randPtsBMemObj = clCreateBuffer(context, CL_MEM_READ_ONLY,  p_params.n_kernels * sizeof(float), NULL, &ret);
	cl_mem randPtsCMemObj = clCreateBuffer(context, CL_MEM_READ_ONLY,  p_params.n_kernels * sizeof(float), NULL, &ret);
	cl_mem randPtsDMemObj = clCreateBuffer(context, CL_MEM_READ_ONLY,  p_params.n_kernels * sizeof(float), NULL, &ret);

	cl_mem trajAMemObj = clCreateBuffer(context, CL_MEM_WRITE_ONLY, p_params.n_kernels * p_params.maxiter * sizeof(float), NULL, &ret);
	cl_mem trajBMemObj = clCreateBuffer(context, CL_MEM_WRITE_ONLY, p_params.n_kernels * p_params.maxiter * sizeof(float), NULL, &ret);

	// Create program from kernel source
	cl_program program = clCreateProgramWithSource(context, 1, (const char **)&kernelSource, (const size_t *)&kernelSize, &ret);	

	// Build program
	ret = clBuildProgram(program, 1, &deviceID, NULL, NULL, NULL);
	if (ret < 0){
		printf("OpenCL Error ! %d\n", ret);
		exit(ret);
	}
	size_t len = 0;
	ret = clGetProgramBuildInfo(program, deviceID, CL_PROGRAM_BUILD_LOG, 0, NULL, &len);
	char *buffer = calloc(len, sizeof(char));
	ret = clGetProgramBuildInfo(program, deviceID, CL_PROGRAM_BUILD_LOG, len, buffer, NULL);

	if(buffer[0] != '\n' && buffer[0] != '\0'){
		printf("\nOPENCL Compilation Error ! Check logs !\n");
		printf("%s", buffer);
		exit(-1);
	}

	// Create kernel
	cl_kernel kernel = clCreateKernel(program, "buddhaTraj", &ret);


	while(1){
		for (int i = 0; i < p_params.n_kernels; i++){
			randomPointsA[i] = float_rand(-2.0, 2.0);
			randomPointsB[i] = float_rand(-2.0, 2.0);
			randomPointsC[i] = float_rand(-2.0, 2.0);
			randomPointsD[i] = float_rand(-2.0, 2.0);
		}
		// Copy lists to memory buffers
		ret = clEnqueueWriteBuffer(commandQueue, randPtsAMemObj, CL_TRUE, 0, p_params.n_kernels * sizeof(float), randomPointsA, 0, NULL, NULL);;
		ret = clEnqueueWriteBuffer(commandQueue, randPtsBMemObj, CL_TRUE, 0, p_params.n_kernels * sizeof(float), randomPointsB, 0, NULL, NULL);;
		ret = clEnqueueWriteBuffer(commandQueue, randPtsCMemObj, CL_TRUE, 0, p_params.n_kernels * sizeof(float), randomPointsC, 0, NULL, NULL);;
		ret = clEnqueueWriteBuffer(commandQueue, randPtsDMemObj, CL_TRUE, 0, p_params.n_kernels * sizeof(float), randomPointsD, 0, NULL, NULL);;
		if (ret < 0){
			printf("OpenCL Error ! %d\n", ret);
			exit(ret);
		}

		int maxiter = p_params.maxiter;
		// Set arguments for kernel
		ret = clSetKernelArg(kernel, 0, sizeof(cl_mem), (void *)&randPtsAMemObj);	
		ret = clSetKernelArg(kernel, 1, sizeof(cl_mem), (void *)&randPtsBMemObj);	
		ret = clSetKernelArg(kernel, 2, sizeof(cl_mem), (void *)&randPtsCMemObj);	
		ret = clSetKernelArg(kernel, 3, sizeof(cl_mem), (void *)&randPtsDMemObj);	
		ret = clSetKernelArg(kernel, 4, sizeof(cl_mem), (void *)&trajAMemObj);	
		ret = clSetKernelArg(kernel, 5, sizeof(cl_mem), (void *)&trajBMemObj);	

		if (ret < 0){
			printf("OpenCL Error in Argument Set! %d\n", ret);
			exit(ret);
		}

		// Execute the kernel
		ret = clEnqueueNDRangeKernel(commandQueue, kernel, 1, NULL, &globalItemSize, &localItemSize, 0, NULL, NULL);	
		if (ret < 0){
			printf("OpenCL Enqueue Error ! %d\n", ret);
			exit(ret);
		}

		// Read from device back to host.
		ret = clEnqueueReadBuffer(commandQueue, trajAMemObj, CL_TRUE, 0, p_params.n_kernels * p_params.maxiter * sizeof(float), trajectoriesA, 0, NULL, NULL);
		ret = clEnqueueReadBuffer(commandQueue, trajBMemObj, CL_TRUE, 0, p_params.n_kernels * p_params.maxiter * sizeof(float), trajectoriesB, 0, NULL, NULL);
		if (ret < 0){
			printf("OpenCL Error ! %d\n", ret);
			exit(ret);
		}

		int x, y;
		for(int i = 0; i < p_params.n_kernels; i++){
			for(int j = 0; j < p_params.maxiter; j++){
				if (trajectoriesA[j + p_params.maxiter * i] == -10) break;
				x = (int)map(trajectoriesB[j + p_params.maxiter * i], -0.5, 0.5, 0, p_params.resx);	
				y = (int)map(trajectoriesA[j + p_params.maxiter * i], -0.35 * sqr2, 0.65 * sqr2, 0, p_params.resy);	
				if (x >= 0 && x < p_params.resx && y >= 0 && y < p_params.resy){
					histogram[x][y]++;
				}
			}
		}
		//TODO: Catch interrupts
		if (iter % 1000 == 1){
			//TODO:
			writeCheckpoint(histogram, p_params);
			drawTrajs(params, histogram);
		}
	}
	ret = clReleaseCommandQueue(commandQueue);
	ret = clReleaseKernel(kernel);
	ret = clReleaseProgram(program);
	ret = clReleaseMemObject(randPtsAMemObj);
	ret = clReleaseMemObject(randPtsBMemObj);
	ret = clReleaseMemObject(randPtsCMemObj);
	ret = clReleaseMemObject(randPtsDMemObj);
	ret = clReleaseMemObject(trajAMemObj);
	ret = clReleaseMemObject(trajBMemObj);
	ret = clReleaseContext(context);
	free(trajectoriesA);
	free(trajectoriesB);
	free(randomPointsA);
	free(randomPointsB);
}

void buddhaCPU(t_params params){
	uint32_t histogram[params.resx][params.resy];
	complex trajs[params.maxit];

	while(1){
		complex r = rand_complex(-2 - 2 * I, 2 + 2 * I);
		trajs[0] = rand_complex(-2 - 2 * I, 2 + 2 * I);
		for (int i = 1; i < params.maxit; i++){
			trajs[i] = r * trajs[i - 1] * (1 - trajs[i - 1]);
			if (cabs(trajs[i]) > 2){trajs[i] = -10; break;}
		}

		drawTrajs(params, trajs);

		for(int i = 0; i < params.maxit; i++){
			int x = (int)map(creal(trajs[i]), -0.5, 0.5, 0, params.resx);
			int y = (int)map(cimag(trajs[i]), -0.35 * sqrt(2), 0.65 * sqrt(2), 0, params.resy);	
			if (x >= 0 && x < params.resx && y >= 0 && y < params.resy){
				histogram[x][y]++;
			}
		}

		if (iter % params.npoint == 1){
			writeCheckpoint(histogram, params);
		}
	}


}
