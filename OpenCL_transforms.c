#include <stdio.h>
#include <stdlib.h>
#include <OpenCL/opencl.h>


int main() {
    cl_platform_id platform_id = NULL;
    cl_uint ret_num_platforms;
    cl_int ret = clGetPlatformIDs(1, &platform_id, &ret_num_platforms);
    if (ret != CL_SUCCESS) {
        printf("Failed to get platform ID.\n");
        return -1;
    }

    cl_device_id device_id = NULL;
    cl_uint ret_num_devices;
    ret = clGetDeviceIDs(platform_id, CL_DEVICE_TYPE_DEFAULT, 1, &device_id, &ret_num_devices);
    if (ret != CL_SUCCESS) {
        printf("Failed to get device ID.\n");
        return -1;
    }


    cl_context context = clCreateContext(NULL, 1, &device_id, NULL, NULL, &ret);
    if (ret != CL_SUCCESS) {
        printf("Failed to create context.\n");
        return -1;
    }

    return 0;
}
