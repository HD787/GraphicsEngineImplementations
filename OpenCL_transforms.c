#include <stdio.h>
#include <stdlib.h>
#include <OpenCL/opencl.h>

#include "vertexTransformKernel.cl"

openClResources createOpenClResources(){
    cl_platform_id platform_id = NULL;
    cl_uint ret_num_platforms;
    clGetPlatformIDs(1, &platform_id, &ret_num_platforms);

    cl_device_id device_id = NULL;
    cl_uint ret_num_devices;
    clGetDeviceIDs(platform_id, CL_DEVICE_TYPE_DEFAULT, 1, &device_id, &ret_num_devices);
    clCreateContext(NULL, 1, &device_id, NULL, NULL, NULL);
    createOpenClResources clr;
    clr.context = clCreateContext(NULL, 1, &device_id, NULL, NULL, NULL);
    return clr;
}

int BuildKernels(createOpenClResources* clr, transformSpec* ts, scene* sc, vertexBuffer* vb, colorBuffer* cb, normalBuffer* nb, float* mb) {
    int lenArray[2] = { strlen(vectorKernalSource), strlen(normalVectorSource)}
    clr->program = clCreateProgramWithSource(context, 1, &kernelSource, lenArray, NULL);
    clBuildProgram(clr->program, 1, &device_id, NULL, NULL, NULL);
    clr->kernelCount = 2
    clr->kernels = malloc(sizeof(cl_kernel) * clr->kernelCount);
    cl_kernel vertexKernal = clCreateKernel(clr->program, "vertexTransforms", NULL);
    cl_kernel normalKernal = clCreateKernal(clr->program, "normalTransforms", NULL);

    clr->commandQueue = clCreateCommandQueue(clr->context, device_id, 0, NULL);

    size_t globalVertexWorkSize = vb->length; 
    size_t local_work_size = 4;

    size_t globalNormalWorkSize = nb->length;
    size_t localNormalWorkSize = 4;


    cl_mem vertexBuffer = clCreateBuffer(clr->context, CL_MEM_READ_WRITE, sizeof(float) * vb->length, NULL, NULL);
    cl_mem vertexOutputBuffer = clCreateBuffer(clr->context, CL_MEM_READ_WRITE, sizeof(float) * vb->length, NULL, NULL);

    cl_mem normalBuffer = clCreateBuffer(clr->context, CL_MEM_READ_WRITE, sizeof(float) * nb->length, NULL, NULL);
    cl_mem normalOutputBuffer = clCreateBuffer(clr->context, CL_MEM_READ_WRITE, sizeof(float) * nb->length, NULL, NULL);

    cl_mem matrix_buffer = clCreateBuffer(clr->context, CL_MEM_READ_WRITE, sizeof(float) * , NULL, NULL);

    clEnqueueWriteBuffer(clr->commandQueue, vertexBuffer, CL_TRUE, 0, sizeof(float) * vb->length, vb->vertices, 0, NULL, NULL);
    clEnqueueWriteBuffer(clr->commandQueue, normalBuffer, CL_TRUE, 0, sizeof(float) * nb->length, nb->normals, 0, NULL, NULL);


    clSetKernelArg(vertexKernal, 0, sizeof(cl_mem), &vertexBuffer);
    clSetKernelArg(vertexKernal, 1, sizeof(cl_mem), &matrixBuffer);
    clSetKernelArg(vertexKernal, 2, sizeof(cl_mem), &vertexOutputBuffer);

    clSetKernelArg(vertexKernal, 3, sizeof(int), &(int){4});

    return 0;
}

void callKernels(openClResources* clr, vertexbuffer* vb, normalBuffer* nb, transformSpec* ts){
    //how am i gonna handle this
    size_t globalVertexWorkSize = vb->length; 
    size_t local_work_size = 4;

    size_t globalNormalWorkSize = nb->length;
    size_t localNormalWorkSize = 4;
    clEnqueueNDRangeKernel(clr->commandQueue, clr->vertexKernel, NULL, &vb->length, &local_work_size, 0, NULL, NULL);
    clEnqueueNDRangeKernel(clr->commandQueue, clr->normalKernel, NULL, &nb->length, &local_work_size, 0, NULL, NULL);
}

void readData(openClResources clr, vertexBuffer* vb, normalBuffer* nb){
    clEnqueueReadBuffer(clr->commandQueue, vb->vertexOutputBuffer, CL_TRUE, 0, sizeof(result), result, 0, NULL, NULL); 
    clEnqueueReadBuffer(clr->commandQueue, vb->normalOuputBuffer, CL_TRUE, 0, sizeof(results), result, 0, NULL, NULL);
}

void deleteClContext(openClResources* clr){
    clReleaseCommandQueue(clr->command_queue);
    clReleaseProgram(clr->program);
    clReleaseContext(clr->context);
    for(int i = 0; i < clr->kernelCount; i++){
        clReleaseKernel(clr->kernels[i]);
    }
}
