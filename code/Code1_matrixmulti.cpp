#include <iostream>
#include <cstdlib>
#include "get_walltime.c"

using namespace std;

double MatrixMultiplication(double ** A, double ** B, int rowsA, int colsA, int rowsB, int colsB) {

    // Initialize matrix C to zeros
    double C[rowsA][colsB];

    // Perform matrix multiplication and calculate time
    double start_time;
    get_walltime(&start_time);
    for (int i = 0; i < rowsA; ++i) {
        for (int j = 0; j < colsB; ++j) {
            for (int k = 0; k < colsA; ++k) {
                C[i][j] += A[i][k] * B[k][j];
            }
        }
    }
    double end_time;
    get_walltime(&end_time);
    double durtime;
    durtime = end_time - start_time;
    return durtime;
   
}

int main() {
    int rowsA, colsA, rowsB, colsB;

    // A
    cout << "Enter the number of rows and columns for matrix A: " << endl;
    cout << "row: ";
    cin >> rowsA;
    cout << "cols: ";
    cin >> colsA;

    // B
    cout << "Enter the number of rows and columns for matrix B: " << endl;
    cout << "row: ";
    cin >> rowsB;
    cout << "cols: ";
    cin >> colsB;

    // Check if matrix multiplication is possible
    if (colsA != rowsB) {
        cout << "Matrix multiplication is not possible. Columns of A must be equal to rows of B." << endl;
        exit(1);
    }

    // initialize matrix A and Matrix B
    double** matrixA = new double*[rowsA];
    double** matrixB = new double*[rowsB];
    for (int i = 0; i < rowsA; ++i) {
        matrixA[i] = new double[colsA];
        for (int j = 0; j < colsA; ++j) {
            matrixA[i][j] = static_cast<float>(rand()) / RAND_MAX;  // Random values between 0 and 1
        }
    }
    for (int i = 0; i < rowsB; ++i) {
        matrixB[i] = new double[colsB];
        for (int j = 0; j < colsB; ++j) {
            matrixB[i][j] = static_cast<float>(rand()) / RAND_MAX;  // Random values between 0 and 1
        }
    }

    // Call the matrix multiplication function and return spending time
    double timespend;
    timespend = MatrixMultiplication(matrixA, matrixB, rowsA, colsA, rowsB, colsB);

    cout << "Time taken for multiplication: " << timespend << " seconds." << endl;
}
