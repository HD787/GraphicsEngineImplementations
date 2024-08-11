__kernel void vector_matrix_multiply(__global const float* matrix, 
                                     __global const float* vector, 
                                     __global float* result, 
                                     const int N) {
    // Get the row index of the work-item
    int row = get_global_id(0);

    // Initialize the result for this row
    float sum = 0.0f;

    // Perform the dot product of the row in the matrix with the vector
    for (int col = 0; col < N; col++) {
        sum += matrix[row * N + col] * vector[col];
    }

    // Store the result
    result[row] = sum;
}