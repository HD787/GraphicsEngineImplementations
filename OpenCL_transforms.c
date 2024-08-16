#include <stdio.h>
#include <stdlib.h>
#include <OpenCL/opencl.h>

#include "transformKernel.cl"

cl_context createOpenCLContext(){
    cl_platform_id platform_id = NULL;
    cl_uint ret_num_platforms;
    clGetPlatformIDs(1, &platform_id, &ret_num_platforms);

    cl_device_id device_id = NULL;
    cl_uint ret_num_devices;
    clGetDeviceIDs(platform_id, CL_DEVICE_TYPE_DEFAULT, 1, &device_id, &ret_num_devices);
    return clCreateContext(NULL, 1, &device_id, NULL, NULL, NULL);
}

int BuildKernels(cl_context context, transformSpec* ts, scene* sc, vertexBuffer* vb, colorBuffer* cb, normalBuffer* nb) {


    int lenArray[2] = { strlen(vectorKernalSource), strlen(normalVectorSource)}
    cl_program program = clCreateProgramWithSource(context, 1, &kernelSource, lenArray, NULL);
    clBuildProgram(program, 1, &device_id, NULL, NULL, NULL);

    cl_kernel vertexKernal = clCreateKernel(program, "vertexTransforms", NULL);
    cl_kernel normalKernal = clCreateKernal(program, "normalTransforms", NULL);

    cl_command_queue command_queue = clCreateCommandQueue(context, device_id, 0, NULL);

    
    size_t globalVertexWorkSize = vb->length; 
    size_t local_work_size = 4;

    size_t globalNormalWorkSize = nb->length;
    size_t localNormalWorkSize = 4;


    cl_mem vertexBuffer = clCreateBuffer(context, CL_MEM_READ_WRITE, sizeof(float) * vb->length, NULL, NULL);
    cl_mem vertexOutputBuffer = clCreateBuffer(context, CL_MEM_READ_WRITE, sizeof(float) * vb->length, NULL, NULL);

    cl_mem normalBuffer = clCreateBuffer(context, CL_MEM_READ_WRITE, sizeof(float) * nb->length, NULL, NULL);
    cl_mem normalOutputBuffer = clCreateBuffer(context, CL_MEM_READ_WRITE, sizeof(float) * nb->length, NULL, NULL);

    //TODO
    cl_mem matrix_buffer = clCreateBuffer(context, CL_MEM_READ_WRITE, sizeof(float) * , NULL, NULL);

    clEnqueueWriteBuffer(command_queue, vertexBuffer, CL_TRUE, 0, sizeof(float) * vb->length, vb->vertices, 0, NULL, NULL);
    clEnqueueWriteBuffer(command_queue, normalBuffer, CL_TRUE, 0, sizeof(float) * nb->length, nb->normals, 0, NULL, NULL);


    clSetKernelArg(vertexKernal, 0, sizeof(cl_mem), &vertexBuffer);
    clSetKernelArg(vertexKernal, 1, sizeof(cl_mem), &matrixBuffer);
    clSetKernelArg(vertexKernal, 2, sizeof(cl_mem), &vertexOutputBuffer);
    //TODO
    clSetKernelArg(vertexKernal, 3, sizeof(int), &(int){4});

    clEnqueueNDRangeKernel(command_queue, vertexKernal, 1, NULL, &global_work_size, &local_work_size, 0, NULL, NULL);

    clEnqueueReadBuffer(command_queue, vertexOutputBuffer, CL_TRUE, 0, sizeof(result), result, 0, NULL, NULL);

    for(int i = 0; i < 4; i++){
        printf("%f ",result[i]);
    }

    clReleaseCommandQueue(command_queue);
    clReleaseKernel(vertexKernal);
    clReleaseProgram(program);
    clReleaseContext(context);
    return 0;
}
