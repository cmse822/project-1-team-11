# Report for Group11
## Ahmad Hejasebazzi; Max Brombach; Shuangyu Zhao


----------
### Part 1: Matrix-matrix Multiplication
1. Program for matrix multiplication.

See ```Code1_Matrixmulti.cpp``` in ```../code```

2. For a given matrix size N, what is the total number of <b>floating point operations</b> performed by this operator?

$$ 2 \times N \times N \times N $$

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
For the algorithm ``` C[i][j] += A[i][k] * B[k][j]```, there are three load,  one store and two operations. Because of the three loops, there are total $N^3$ times running the algorithm.

3. Compute the performance in Mflop/s of the matrix-matrix multiply for N=100. Be sure to perform enough repeat calculations of the timing to overcome any statistical noise in the measurement.

* MacBook Pro 2017, 2.3 GHz Dual-Core Intel Core i5: 

$$ \frac{2 \times 100^3 Mflop}{0.00486708 s} = 410924003.715 Mflop/s $$
* 
* 

4. 












-------------
### Part 2: The Roofline Model









