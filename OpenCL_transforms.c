#include <stdio.h>
#include <stdlib.h>
#include <OpenCL/opencl.h>

#include "vertexTransformKernel.cl"
#include "normalTransformKernel.cl"


int vertexKernal(renderContext* rc, transformSpec* ts, scene* sc, vertexBuffer* vb, colorBuffer* cb, normalBuffer* nb ) {

    cl_platform_id platform_id = NULL;
    cl_uint ret_num_platforms;
    clGetPlatformIDs(1, &platform_id, &ret_num_platforms);

    cl_device_id device_id = NULL;
    cl_uint ret_num_devices;
    clGetDeviceIDs(platform_id, CL_DEVICE_TYPE_DEFAULT, 1, &device_id, &ret_num_devices);

    cl_context context = clCreateContext(NULL, 1, &device_id, NULL, NULL, NULL);

    int lenArray[2] = { strlen(vertexKernelSource), strlen(normalKernelSource) };
    char* kernelSources[2] = { vertexKernelSource, normalKernelSource };
    cl_program program = clCreateProgramWithSource(context, 1, &kernelSources, lenArray, NULL);
    clBuildProgram(program, 1, &device_id, NULL, NULL, NULL);

    cl_kernel vectorKernal = clCreateKernel(program, "vector_matrix_multiply", NULL);
    cl_kernel normalKernal = clCreateKernal()

    cl_command_queue command_queue = clCreateCommandQueue(context, device_id, 0, NULL);

    
    size_t global_work_size = 4;
    size_t local_work_size = 4;


    cl_mem vertexBuffer = clCreateBuffer(context, CL_MEM_READ_WRITE, vb->length, NULL, NULL);
    cl_mem matrixBuffer = clCreateBuffer(context, CL_MEM_READ_WRITE, sizeof(matrix), NULL, NULL);
    cl_mem vertexResultBuffer = clCreateBuffer(context, CL_MEM_READ_WRITE, sizeof(result), NULL, NULL);
    cl_mem normalBuffer = clCreateBuffer(context, CL_MEM_READ_WRITE, sizeof(result), NULL, NULL);
    cl_mem normalResultBuffer = clCreateBuffer(context, CL_MEM_READ_WRITE, sizeof(result), NULL, NULL);

    //NOT DONE
    clEnqueueWriteBuffer(command_queue, matrix_buffer, CL_TRUE, 0, sizeof(matrix), matrix, 0, NULL, NULL);
    clEnqueueWriteBuffer(command_queue, vector_buffer, CL_TRUE, 0, sizeof(vector), vector, 0, NULL, NULL);
    clEnqueueWriteBuffer(command_queue, vector_buffer, CL_TRUE, 0, sizeof(vector), vector, 0, NULL, NULL);



    clSetKernelArg(vectorKernal, 0, sizeof(cl_mem), &vertexBuffer);
    clSetKernelArg(vectorKernal, 1, sizeof(cl_mem), &matrixBuffer);
    clSetKernelArg(vectorKernal, 2, sizeof(cl_mem), &vertexResultBuffer);
    //ADD MORE
    clSetKernelArg(vectorKernal, 3, sizeof(int), &(int){4});
    //NOT DONE
    clEnqueueNDRangeKernel(command_queue, vectorKernal, 1, NULL, &global_work_size, &local_work_size, 0, NULL, NULL);

    clEnqueueReadBuffer(command_queue, result_buffer, CL_TRUE, 0, sizeof(result), result, 0, NULL, NULL);

    for(int i=0;i < 4; i++){
        printf("%f ",result[i]);
    }

    clReleaseCommandQueue(command_queue);
    clReleaseKernel(vectorKernal);
    clReleaseProgram(program);
    clReleaseContext(context);
    return 0;
}
