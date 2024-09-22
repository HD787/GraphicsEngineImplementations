#include <stdio.h>
#include <stdlib.h>
#include <OpenCL/opencl.h>

#include "transformKernel.cl"

typedef struct{
    cl_context context;
    cl_device_id deviceId;
    cl_program program;
    cl_command_queue commandQueue;
    cl_kernel kernel;
    cl_mem vertexOutput;
    cl_mem colorOutput;
}openClResources;


openClResources createOpenClResources(){
    openClResources clr;
    cl_platform_id platform_id = NULL;
    cl_uint ret_num_platforms;
    clGetPlatformIDs(1, &platform_id, &ret_num_platforms);

    clr.deviceId = NULL;
    cl_uint ret_num_devices;
    clGetDeviceIDs(platform_id, CL_DEVICE_TYPE_DEFAULT, 1, &clr.deviceId, &ret_num_devices);
    clCreateContext(NULL, 1, &clr.deviceId, NULL, NULL, NULL);
    clr.context = clCreateContext(NULL, 1, &clr.deviceId, NULL, NULL, NULL);
    return clr;
}

int buildKernels(openClResources* clr, transformSpec* ts, renderContext* rc, vertexBuffer* vb, colorBuffer* cb, normalBuffer* nb, float* mb) {
    size_t size = strlen(kernelSource);
    clr->program = clCreateProgramWithSource(clr->context, 1, &kernelSource, &size, NULL);
    clBuildProgram(clr->program, 1, &clr->deviceId, NULL, NULL, NULL);
    clr->kernel = clCreateKernel(clr->program, "transforms", NULL);

    clr->commandQueue = clCreateCommandQueue(clr->context, clr->deviceId, 0, NULL);

    size_t globalVertexWorkSize = vb->length; 
    size_t local_work_size = 4;

    cl_mem vertexBuffer = clCreateBuffer(clr->context, CL_MEM_READ_WRITE, sizeof(float) * vb->length, NULL, NULL);
    cl_mem vertexOutputBuffer = clCreateBuffer(clr->context, CL_MEM_READ_WRITE, sizeof(float) * vb->length, NULL, NULL);

    cl_mem colorBuffer = clCreateBuffer(clr->context, CL_MEM_READ_WRITE, sizeof(float) * cb->length, NULL, NULL);
    cl_mem colorOutputBuffer = clCreateBuffer(clr->context, CL_MEM_READ_WRITE, sizeof(float) * cb->length, NULL, NULL);
    

    cl_mem normalBuffer = clCreateBuffer(clr->context, CL_MEM_READ_WRITE, sizeof(float) * nb->length, NULL, NULL);

    clEnqueueWriteBuffer(clr->commandQueue, vertexBuffer, CL_TRUE, 0, sizeof(float) * vb->length, vb->vertices, 0, NULL, NULL);
    clEnqueueWriteBuffer(clr->commandQueue, normalBuffer, CL_TRUE, 0, sizeof(float) * nb->length, nb->normals, 0, NULL, NULL);
    clEnqueueWriteBuffer(clr->commandQueue, colorBuffer, CL_TRUE, 0, sizeof(float) * cb->length, cb->colors, 0, NULL, NULL);

    clSetKernelArg(clr->kernel, 0, sizeof(cl_mem), &vertexBuffer);
    clSetKernelArg(clr->kernel, 1, sizeof(cl_mem), &colorBuffer);
    clSetKernelArg(clr->kernel, 2, sizeof(cl_mem), &normalBuffer);

    clSetKernelArg(clr->kernel, 4, sizeof(cl_mem), &vertexOutputBuffer);
    clSetKernelArg(clr->kernel, 5, sizeof(cl_mem), &colorOutputBuffer); 
    int matrixCount = 5;
    clSetKernelArg(clr->kernel, 6, sizeof(cl_int), &matrixCount);
    clSetKernelArg(clr->kernel, 7, sizeof(cl_int), &rc->height);
    clSetKernelArg(clr->kernel, 8, sizeof(cl_int), &rc->width);
    
    clr->vertexOutput = vertexOutputBuffer;
    clr->colorOutput = colorOutputBuffer;
    return 0;
}

void setKernelArgs(openClResources* clr, transformSpec* ts, float* mb){
    float *rotationMatrixX, *rotationMatrixY, *rotationMatrixZ, *translationMatrix, *perspectiveProjectionMatrix;
    rotationMatrixX = mb;
    rotationMatrixY = mb + 16;
    rotationMatrixZ = mb + 32;
    translationMatrix = mb + 48;
    perspectiveProjectionMatrix = mb + 64;

    createRotationMatrixX(ts->rotateX, rotationMatrixX);
    createRotationMatrixY(ts->rotateY, rotationMatrixY);
    createRotationMatrixZ(ts->rotateZ, rotationMatrixZ);
    createTranslationMatrix(ts->translateX, ts->translateY, ts->translateZ, translationMatrix);
    createPerspectiveProjectionMatrix(45.0, 1.0, 10.0, 1000.0/700.0, perspectiveProjectionMatrix);

    cl_mem matrixBuffer = clCreateBuffer(clr->context, CL_MEM_READ_WRITE, sizeof(float) * 16 * 5 , NULL, NULL);
    clSetKernelArg(clr->kernel, 3, sizeof(cl_mem), &matrixBuffer);
}

void callKernels(openClResources* clr, vertexBuffer* vb){
    size_t globalWorkSize = vb->length; 
    size_t localWorkSize = 4;
    clEnqueueNDRangeKernel(clr->commandQueue, clr->kernel, 1, NULL, &globalWorkSize, &localWorkSize, 0, NULL, NULL);
}

void readData(openClResources* clr, vertexBuffer* vb, colorBuffer* cb){
    clEnqueueReadBuffer(clr->commandQueue, clr->vertexOutput, CL_TRUE, 0, sizeof(cl_mem), vb->vertices, 0, NULL, NULL); 
    clEnqueueReadBuffer(clr->commandQueue, clr->colorOutput, CL_TRUE, 0, sizeof(cl_mem), cb->colors, 0, NULL, NULL);
}

void deleteClContext(openClResources* clr){
    clReleaseCommandQueue(clr->commandQueue);
    clReleaseProgram(clr->program);
    clReleaseContext(clr->context);
    clReleaseKernel(clr->kernel);
}
