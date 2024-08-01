#include <OpenCL/opencl.h>
int main() {
    cl_platform_id platform;
    cl_device_id device;
    cl_context context;
    cl_command_queue queue;
    cl_int err;
    
    // Get platform and device information
    err = clGetPlatformIDs(1, &platform, NULL);
    if (err != CL_SUCCESS) {
        printf("Unable to get platform ID\n");
        return 1;
    }
    
    err = clGetDeviceIDs(platform, CL_DEVICE_TYPE_GPU, 1, &device, NULL);
    if (err != CL_SUCCESS) {
        printf("Unable to get device ID\n");
        return 1;
    }
    
    // Create an OpenCL context
    context = clCreateContext(NULL, 1, &device, NULL, NULL, &err);
    if (!context) {
        printf("Unable to create context\n");
        return 1;
    }
    
    // Create a command queue
    queue = clCreateCommandQueueWithProperties(context, device, 0, &err);
    if (!queue) {
        printf("Unable to create command queue\n");
        clReleaseContext(context);
        return 1;
    }
    return 0;
}