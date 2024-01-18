# Report for Group11
## Ahmad Hejasebazzi; Max Brombach; Shuangyu Zhao


----------
### Part 1: Matrix-matrix Multiplicati
1. Program for matrix multiplication.

See ```Code1_Matrixmulti.cpp``` in ```../code```

2. For a given matrix size N, what is the total number of floating point operations performed by this operator?

$$ 3 \times N \times N \times N $$

explanation:
```c++
for (int i = 0; i < rowsA; ++i) {
    for (int j = 0; j < colsB; ++j) {
        for (int k = 0; k < colsA; ++k) {
            C[i][j] += A[i][k] * B[k][j];
        }
    }
}
```
For the algorithm ``` C[i][j] += A[i][k] * B[k][j]```, there are two load and one store. Because of the three loops, there are total $N^3$ times running the algorithm.

3. Compute the performance in Mflop/s of the matrix-matrix multiply for N=100. Be sure to perform enough repeat calculations of the timing to overcome any statistical noise in the measurement.






-------------
### Part 2: The Roofline Model









