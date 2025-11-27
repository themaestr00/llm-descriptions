#include <stdio.h>
#include <stdlib.h>
#include <math.h>

typedef struct {
    int rows;
    int cols;
    double* data;
} Matrix;

/**
 * Creates a new matrix with specified dimensions.
 *
 * @param rows Number of rows.
 * @param cols Number of columns.
 * @return A new Matrix structure.
 */
Matrix create_matrix(int rows, int cols) {
    Matrix m;
    m.rows = rows;
    m.cols = cols;
    m.data = (double*)malloc(rows * cols * sizeof(double));
    if (m.data == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(1);
    }
    return m;
}

/**
 * Frees the memory allocated for a matrix.
 *
 * @param m The matrix to free.
 */
void free_matrix(Matrix m) {
    if (m.data != NULL) {
        free(m.data);
    }
}

/**
 * Sets the value at a specific row and column.
 */
void set_element(Matrix m, int r, int c, double val) {
    if (r >= 0 && r < m.rows && c >= 0 && c < m.cols) {
        m.data[r * m.cols + c] = val;
    }
}

/**
 * Gets the value at a specific row and column.
 */
double get_element(Matrix m, int r, int c) {
    if (r >= 0 && r < m.rows && c >= 0 && c < m.cols) {
        return m.data[r * m.cols + c];
    }
    return 0.0;
}

/**
 * Multiplies two matrices A and B.
 *
 * @param A First matrix.
 * @param B Second matrix.
 * @return The result of A * B. Returns a 0x0 matrix if dimensions mismatch.
 */
Matrix multiply_matrices(Matrix A, Matrix B) {
    if (A.cols != B.rows) {
        fprintf(stderr, "Dimension mismatch for multiplication\n");
        Matrix empty = {0, 0, NULL};
        return empty;
    }

    Matrix C = create_matrix(A.rows, B.cols);

    for (int i = 0; i < C.rows; i++) {
        for (int j = 0; j < C.cols; j++) {
            double sum = 0.0;
            // Optimized loop (naively)
            // Unrolling or tiling could be applied here for optimization
            for (int k = 0; k < A.cols; k++) {
                sum += get_element(A, i, k) * get_element(B, k, j);
            }
            set_element(C, i, j, sum);
        }
    }

    return C;
}

/**
 * Transposes a matrix.
 *
 * @param A The matrix to transpose.
 * @return The transposed matrix.
 */
Matrix transpose_matrix(Matrix A) {
    Matrix T = create_matrix(A.cols, A.rows);
    for (int i = 0; i < A.rows; i++) {
        for (int j = 0; j < A.cols; j++) {
            set_element(T, j, i, get_element(A, i, j));
        }
    }
    return T;
}

/**
 * Prints the matrix to stdout.
 */
void print_matrix(Matrix m) {
    printf("Matrix (%dx%d):\n", m.rows, m.cols);
    for (int i = 0; i < m.rows; i++) {
        for (int j = 0; j < m.cols; j++) {
            printf("%6.2f ", get_element(m, i, j));
        }
        printf("\n");
    }
}

// Helper to calculate determinant (recursive, for demo purposes)
double determinant_recursive(Matrix m) {
    if (m.rows != m.cols) return 0; // Should check earlier
    if (m.rows == 1) return get_element(m, 0, 0);
    if (m.rows == 2) {
        return get_element(m, 0, 0) * get_element(m, 1, 1) -
               get_element(m, 0, 1) * get_element(m, 1, 0);
    }

    double det = 0;
    int sign = 1;
    for (int c = 0; c < m.cols; c++) {
        // Create submatrix
        Matrix sub = create_matrix(m.rows - 1, m.cols - 1);
        for (int i = 1; i < m.rows; i++) {
            int sub_j = 0;
            for (int j = 0; j < m.cols; j++) {
                if (j == c) continue;
                set_element(sub, i - 1, sub_j, get_element(m, i, j));
                sub_j++;
            }
        }
        det += sign * get_element(m, 0, c) * determinant_recursive(sub);
        sign = -sign;
        free_matrix(sub);
    }
    return det;
}

/**
 * Calculates the determinant of a square matrix.
 */
double determinant(Matrix m) {
    if (m.rows != m.cols) {
        fprintf(stderr, "Determinant requires square matrix\n");
        return 0.0;
    }
    return determinant_recursive(m);
}
