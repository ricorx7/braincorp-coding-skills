# Brain Corp Coding Skill Test
You’ve been tasked with writing a high-performance, portable linear algebra library for a client who has an awful case of not-invented-here syndrome. This means you must only use standard libraries (and miss out on the decades of optimizations in various BLAS implementations). As a start, the client has asked for just transpose and multiplication of MxN matrices.

Your solution should be implemented in standard C++11 (g++ or clang++ on Linux). To keep things simple, the library may be header-only. You may include a Makefile/CMakeLists.txt, or you may specify the compiler command used to compile your source file: g++ main.cpp -std=c++11 -lthread


# Compile
```bash
g++ -g main.cpp -o main.out -std=c++11 -lpthread
```

# Run Test Application
```bash
./main.out
```

If you want to adjust the initial matrices used for calculations, there are 8 optional parameters that will change the matrices.

## Application Parameters
8 Arguments that are optional:

`[M1_Rows] [M1_Columns] [M2_Rows] [M2_Columns] [NumThreads] [M1_StartValue] [M2_StartValue]`

```
M1_Rows: Number of rows in matrix 1. [DEFAULT: 2]
M1_Columns: Number of columns in matrix 1. [DEFAULT: 3]
M2_Rows: Number of rows in matrix 2. [DEFAULT: 3]
M2_Columns: Number of columns in matrix 2.  [DEFAULT: 2]
NumThreads: Number threads to utilize.  [DEFAULT: 2]
M1_StartValue: The first value in matrix 1.  The values are incremented in the matrix. [DEFAULT: 0]
M2_StartValue: The first value in matrix 2.  The values are incremented in the matrix. [DEFAULT: 5]
```

### Example Using Parameters
```bash
./main.out 4 4 4 4 3 2 7
```
Matrix 1: 4x4
Matrix 2: 4x4
3 Threads
Matrix 1 starts with 2
Matrix 2 starts with 7

Results:
```bash
---------------------------
Matrix Multiplication
---------------------------
Original Matrices: 
Matrix: [4,4] - 4 Rows, 4 Columns
2,3,4,5,
6,7,8,9,
10,11,12,13,
14,15,16,17,

Matrix: [4,4] - 4 Rows, 4 Columns
7,8,9,10,
11,12,13,14,
15,16,17,18,
19,20,21,22,

Result: 
Matrix Multiply 3 Thread Duration: 223 microseconds
Matrix: [4,4] - 4 Rows, 4 Columns
202,216,230,244,
820,880,940,1000,
1236,1328,1420,1512,
826,888,950,1012,

---------------------------
Matrix Transpose
---------------------------
Original Matrix: 
Matrix: [4,4] - 4 Rows, 4 Columns
2,3,4,5,
6,7,8,9,
10,11,12,13,
14,15,16,17,

Result: 
Transpose 3 Thread Duration: 3011 microseconds
Matrix: [4,4] - 4 Rows, 4 Columns
2,6,10,14,
3,7,11,15,
4,8,12,16,
5,9,13,17,
```


# Explaination
main.cpp will utilize matrix.h and common.h.  The code allows for many parameters in the command line to adjust the size of the matrices used for testing and the values within the matrices.  It also allows you to play with the number of threads to optimize for speed.

When running this code, you can determine how many threads you want to use.  No threads
is very efficient for very small matrix (less than 1000 x 1000).  But as you start to increase
in size, you will want to start to utilize threads.  This will allow you to determine the optimal
number of threads by trial.  At least 2 threads will improve things. You typically do not need to exceed
4 or 5 threads for very large matrices. 

The reason the threaded version are not good for small matrices is the because of the overhead
of creating and disposing of the threads.  The time involved takes longer than just doing the calculation.
After this, the number of threads can improved based on the matrix size.  Sometimes it is better to use
more than 2 threads.  You could plot this out to determine the optimal number of threads based on matrix 
size.
 
I left the timing code in the file so the timing can be tested to show improvement.  You can comment
out the `#define TIMING` line in matrix.h to remove the timing.
 
I might be able to pull out a little more speed if I go from a 2D to a 1D array to represent the 
matrix.  But I did not test this.
 
I am forcing the sizes of the matrix to be given.  This way the size does not
need to be calculated.


# Functions
`matrixMultiply()` will determine based on the number of threads given which function to use to multiple 2 matrices.

`transpose()` will determine based on the number of threads given which function to use to Transpose the matrix.


# Files
## common.h
This file contains functions that do not do actual calculations and are used to create matrices and print them.  I wanted to leave the actual calculations in another file.

## matrix.h
This contains the matrix multiplication and transpose functions.  There are basically 2 types of functions, one that does NOT use threads and one that allows the user to select how many threads to use.  Sometimes it is better to use no  threads.

## main.cpp
Runs all the tests to display the functionality of the code.  This will display the text matrix and the results.  within this file is the MAIN function.  You can set all the different parameters to adjust the initial matrices and the number of threads.


# Environment
Testing was done on Ubuntu 18.04 Desktop.  
Coding was done in Visual Studio Code.

Packages installed:
```bash
sudo apt-get install build-essential gdb
```