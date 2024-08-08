#include <stdio.h>
#include <stdlib.h>
#include <OpenCL/opencl.h>

#define MAX_PLATFORMS 5
#define MAX_DEVICES 5

void checkError(cl_int err, const char* operation) {
    if (err != CL_SUCCESS) {
        fprintf(stderr, "Error during operation '%s': %d\n", operation, err);
        exit(1);
    }
}

int main() {
    cl_platform_id platforms[MAX_PLATFORMS];
    cl_uint num_platforms;
    cl_int err;

    // Get the number of platforms
    err = clGetPlatformIDs(MAX_PLATFORMS, platforms, &num_platforms);
    checkError(err, "clGetPlatformIDs");

    printf("Found %u platforms\n", num_platforms);

    for (cl_uint i = 0; i < num_platforms; i++) {
        char platform_name[128];
        char platform_vendor[128];
        char platform_version[128];

        err = clGetPlatformInfo(platforms[i], CL_PLATFORM_NAME, sizeof(platform_name), platform_name, NULL);
        checkError(err, "clGetPlatformInfo: CL_PLATFORM_NAME");

        err = clGetPlatformInfo(platforms[i], CL_PLATFORM_VENDOR, sizeof(platform_vendor), platform_vendor, NULL);
        checkError(err, "clGetPlatformInfo: CL_PLATFORM_VENDOR");

        err = clGetPlatformInfo(platforms[i], CL_PLATFORM_VERSION, sizeof(platform_version), platform_version, NULL);
        checkError(err, "clGetPlatformInfo: CL_PLATFORM_VERSION");

        printf("\nPlatform %u:\n", i+1);
        printf("  Name:    %s\n", platform_name);
        printf("  Vendor:  %s\n", platform_vendor);
        printf("  Version: %s\n", platform_version);

        cl_device_id devices[MAX_DEVICES];
        cl_uint num_devices;

        err = clGetDeviceIDs(platforms[i], CL_DEVICE_TYPE_ALL, MAX_DEVICES, devices, &num_devices);
        checkError(err, "clGetDeviceIDs");

        printf("  Found %u devices\n", num_devices);

        for (cl_uint j = 0; j < num_devices; j++) {
            char device_name[128];
            cl_device_type device_type;
            cl_uint compute_units;
            cl_ulong global_mem_size;

            err = clGetDeviceInfo(devices[j], CL_DEVICE_NAME, sizeof(device_name), device_name, NULL);
            checkError(err, "clGetDeviceInfo: CL_DEVICE_NAME");

            err = clGetDeviceInfo(devices[j], CL_DEVICE_TYPE, sizeof(device_type), &device_type, NULL);
            checkError(err, "clGetDeviceInfo: CL_DEVICE_TYPE");

            err = clGetDeviceInfo(devices[j], CL_DEVICE_MAX_COMPUTE_UNITS, sizeof(compute_units), &compute_units, NULL);
            checkError(err, "clGetDeviceInfo: CL_DEVICE_MAX_COMPUTE_UNITS");

            err = clGetDeviceInfo(devices[j], CL_DEVICE_GLOBAL_MEM_SIZE, sizeof(global_mem_size), &global_mem_size, NULL);
            checkError(err, "clGetDeviceInfo: CL_DEVICE_GLOBAL_MEM_SIZE");

            printf("  Device %u:\n", j+1);
            printf("    Name:            %s\n", device_name);
            printf("    Type:            %s\n", (device_type == CL_DEVICE_TYPE_CPU) ? "CPU" :
                                                (device_type == CL_DEVICE_TYPE_GPU) ? "GPU" :
                                                (device_type == CL_DEVICE_TYPE_ACCELERATOR) ? "Accelerator" : "Other");
            printf("    Compute Units:   %u\n", compute_units);
            printf("    Global Memory:   %llu MB\n", global_mem_size / (1024 * 1024));
        }
    }

    return 0;
}
