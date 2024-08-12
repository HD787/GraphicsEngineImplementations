#include <stdio.h>
#include <stdlib.h>
#include <OpenCL/opencl.h>

#include "tranformKernal.cl"


int main() {

    float matrix[16] = { 1.0f, 0.0f, 0.0f, 5.0f,
                         0.0f, 1.0f, 0.0f, 4.0f,
                         0.0f, 0.0f, 1.0f, 3.0f,
                         0.0f, 0.0f, 0.0f, 1.0f
                       };
    float vector[4] = { 6.0f, 7.0f, 8.0f, 9.0f };

    float result[4]

    cl_platform_id platform_id = NULL;
    cl_uint ret_num_platforms;
    clGetPlatformIDs(1, &platform_id, &ret_num_platforms);

    cl_device_id device_id = NULL;
    cl_uint ret_num_devices;
    clGetDeviceIDs(platform_id, CL_DEVICE_TYPE_DEFAULT, 1, &device_id, &ret_num_devices);


    cl_context context = clCreateContext(NULL, 1, &device_id, NULL, NULL, NULL);
    strlen(kernalSource);
    cl_program program = clCreateProgramWithSource(context, 1, &kernalSource, &source_size, NULL);
    clBuildProgram(program, 1, &device_id, NULL, NULL, NULL);

    cl_kernel kernel = clCreateKernel(program, "vector_matrix_multiply", NULL);

    cl_command_queue command_queue = clCreateCommandQueue(context, device_id, 0, NULL);

    
    size_t global_work_size = 4; // Total number of work items
    size_t local_work_size = 4;  // Number of work items per work-group (optional)

    clEnqueueNDRangeKernel(command_queue, kernel, 1, NULL, &global_work_size, &local_work_size, 0, NULL, NULL);

    vector_buffer = clCreateBuffer(context, CL_MEM_READ_WRITE, sizeof(vector), NULL, NULL);
    matrix_buffer = clCreateBuffer(context, CL_MEM_READ_WRITE, sizeof(matrix), NULL, NULL);
    result_buffer = clCreateBuffer(context, CL_MEM_READ_WRITE, sizeof(result), NULL, NULL);

    clEnqueueWriteBuffer(command_queue, matrix_buffer, CL_TRUE, 0, sizeof(matrix), matrix, 0, NULL, NULL);
    clEnqueueWriteBuffer(command_queue, vector_buffer, CL_TRUE, 0, sizeof(vector), vector 0, NULL, NULL);

    clEnqueueReadBuffer(command_queue, result_buffer, CL_TRUE, 0, sizeof(result), result, 0, NULL, NULL);

    clSetKernelArg(kernal, 0, sizeof(vector), vector_buffer);
    clSetKernelArg(kernal, 1, sizeof(matrix), matrix_buffer);
    clSetKernelArg(kernal, 2, sizeof(result), result_buffer);
    clSetKernelArg(kernal, 3, sizeof(vector), &(int){4});

    clReleaseCommandQueue(command_queue);
    clReleaseKernel(kernel);
    clReleaseProgram(program);
    clReleaseContext(context);
    return 0;
}
