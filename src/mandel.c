#include "include/math_utils.h"
#include "include/bmp.h"

#include <stdio.h>
#include <math.h>
#include <string.h>
#include <CL/cl.h>

#define MAX_SOURCE_SIZE (0x100000)

void mandelIterOpenCL(char kernelFilename[256], int NKERNELS, int MAXITER, int OPENCL_ITER, int RESX, int RESY){
	// Allocate memories 
	//We can't pass a 2D array to a kernel, so we'll flatten a 2d array to 1D
	int NPOINTS = 10;
		size_t globalItemSize = NKERNELS;
		size_t localItemSize = 64; // globalItemSize has to be a multiple of localItemSize. 
	float *trajectoriesA = (float*)malloc(sizeof(float) * NKERNELS * NPOINTS*MAXITER);
	float *trajectoriesB = (float*)malloc(sizeof(float) * NKERNELS * NPOINTS*MAXITER);

	float *randomPointsA = (float*)malloc(sizeof(float) * NKERNELS * NPOINTS );
	float *randomPointsB = (float*)malloc(sizeof(float) * NKERNELS * NPOINTS );
	float *randomPointsC = (float*)malloc(sizeof(float) * NKERNELS * NPOINTS );
	float *randomPointsD = (float*)malloc(sizeof(float) * NKERNELS * NPOINTS );

	float **bmpArr = malloc(RESX * sizeof(float *));
	float **histogram = malloc(RESX * sizeof(float *));
	for (int i =0; i < RESX; i++) {
		histogram[i] = (float *)malloc(RESY * sizeof(float)); 
		bmpArr[i] = (float *)malloc(RESY * sizeof(float)); 
	}


	// Initialize values for array members.
	// Load kernel from file vecAddKernel.cl

	FILE *kernelFile;
	char *kernelSource;
	size_t kernelSize;

	kernelFile = fopen(kernelFilename, "r");

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
	cl_mem randPtsAMemObj = clCreateBuffer(context, CL_MEM_READ_ONLY,  NKERNELS * NPOINTS * sizeof(float), NULL, &ret);
	cl_mem randPtsBMemObj = clCreateBuffer(context, CL_MEM_READ_ONLY,  NKERNELS * NPOINTS * sizeof(float), NULL, &ret);
	cl_mem randPtsCMemObj = clCreateBuffer(context, CL_MEM_READ_ONLY,  NKERNELS * NPOINTS * sizeof(float), NULL, &ret);
	cl_mem randPtsDMemObj = clCreateBuffer(context, CL_MEM_READ_ONLY,  NKERNELS * NPOINTS * sizeof(float), NULL, &ret);


	cl_mem trajAMemObj = clCreateBuffer(context, CL_MEM_WRITE_ONLY, NKERNELS * MAXITER * NPOINTS * sizeof(float), NULL, &ret);
	cl_mem trajBMemObj = clCreateBuffer(context, CL_MEM_WRITE_ONLY, NKERNELS * MAXITER * NPOINTS * sizeof(float), NULL, &ret);


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


	int stopMark = -1;
	float sqr2 = sqrt(2);
	float maxVal = 0;
	int numImg = 0;
	for (int iter = 0; iter < OPENCL_ITER; iter++){
		for (int i = 0; i < NKERNELS * NPOINTS; i++){
			randomPointsA[i] = float_rand(-2.0, 2.0);
			randomPointsB[i] = float_rand(-2.0, 2.0);
			randomPointsC[i] = float_rand(-2.0, 2.0);
			randomPointsD[i] = float_rand(-2.0, 2.0);
		}
		// Copy lists to memory buffers
		ret = clEnqueueWriteBuffer(commandQueue, randPtsAMemObj, CL_TRUE, 0, NKERNELS * NPOINTS * sizeof(float), randomPointsA, 0, NULL, NULL);;
		ret = clEnqueueWriteBuffer(commandQueue, randPtsBMemObj, CL_TRUE, 0, NKERNELS * NPOINTS * sizeof(float), randomPointsB, 0, NULL, NULL);;
		ret = clEnqueueWriteBuffer(commandQueue, randPtsCMemObj, CL_TRUE, 0, NKERNELS * NPOINTS * sizeof(float), randomPointsC, 0, NULL, NULL);;
		ret = clEnqueueWriteBuffer(commandQueue, randPtsDMemObj, CL_TRUE, 0, NKERNELS * NPOINTS * sizeof(float), randomPointsD, 0, NULL, NULL);;
		if (ret < 0){
			printf("OpenCL Error ! %d\n", ret);
			exit(ret);
		}

		int maxiter = MAXITER;
		int npoints = npoints;
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
		ret = clEnqueueReadBuffer(commandQueue, trajAMemObj, CL_TRUE, 0, NKERNELS * MAXITER * NPOINTS * sizeof(float), trajectoriesA, 0, NULL, NULL);
		ret = clEnqueueReadBuffer(commandQueue, trajBMemObj, CL_TRUE, 0, NKERNELS * MAXITER * NPOINTS * sizeof(float), trajectoriesB, 0, NULL, NULL);
		if (ret < 0){
			printf("OpenCL Error ! %d\n", ret);
			exit(ret);
		}

		int x, y;
		for(int i = 0; i < NKERNELS; i++){
			for(int j = 0; j < NPOINTS; j++){
				for(int k = 0; k < MAXITER; k++){
					if (trajectoriesA[k + MAXITER * (j + NPOINTS * i)] == -10) break;
					x = (int)map(trajectoriesB[k + MAXITER * (j + NPOINTS * i)], -0.5, 0.5, 0, RESX);	
					y = (int)map(trajectoriesA[k + MAXITER * (j + NPOINTS * i)], -0.35 * sqr2, 0.65 * sqr2, 0, RESY);	
					if(x >= 0 && x < RESX && y >= 0 && y < RESY ){
						histogram[x][y]++;
						maxVal = fmax(histogram[x][y], maxVal);
					}
				}
			}
		}
		maxVal = logf(maxVal + 1);
		if (iter % (OPENCL_ITER/(60 * 30)) == 1){

			for(int i = 0; i < RESX; i++){
				for(int j = 0; j < RESY; j++){
					bmpArr[i][j] = logf(histogram[i][j] + 1)/maxVal;
				}
			}
			write2bmp(bmpArr, bmpArr, bmpArr, RESX, RESY, numImg);
			numImg++;
		}

		stopMark = progressBar(iter, OPENCL_ITER, stopMark);
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
