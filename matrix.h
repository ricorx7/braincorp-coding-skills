#include <cstdio>
#include <chrono> 
#include <iostream>
#include <thread>
#include <vector>
#include "common.h"

using namespace std;
using namespace std::chrono; 

class MatrixAlgebra {

    private:
        /**
         * Matrix Multiplication and Transpose
         * 
         * When running this code, you can determine how many threads you want to use.  No threads
         * is very efficient for very small matrix (less than 1000 x 1000).  But as you start to increase
         * in size, you will want to start to utilize threads.  This will allow you to determine the optimal
         * number of threads by trial.  At least 2 threads will improve things. You typically do not need to exceed
         * 4 or 5 threads for very large matrices. 
         * 
         * The reason the threaded version are not good for small matrices is the because of the overhead
         * of creating and disposing of the threads.  The time involved takes longer than just doing the calculation.
         * After this, the number of threads can improved based on the matrix size.  Sometimes it is better to use
         * more than 2 threads.  You could plot this out to determine the optimal number of threads based on matrix 
         * size.
         * 
         * I left the timing code in the file so the timing can be tested to show improvement.  You can comment
         * out the define line to remove the timing.
         * 
         * I might be able to pull out a little more speed if I go from a 2D to a 1D array to represent the 
         * matrix.  But I did not test this.
         * 
         * I am forcing the sizes of the matrix to be given.  This way the size does not
         * need to be calculated.
         * 
         */

        // Display the timing information for the code
        // Comment out this line to remove the timing information
        #define TIMING

        /**
         * Transpose the matrix.  This will create a new matrix and swap the
         * rows in the original matrix as the column in the new matrix.
         * 
         * :param origMatrix: Original matrix to transpose.
         * :param rows: Number of rows in the original matrix.
         * :param columns: Number of columns in the original matrix.
         * :return: Transposed matrix.
         */
        double** transpose2D(double** origMatrix, int rows, int columns)
        {
            // Create a new matrix based on the size of this matrix
            MatrixCommon mc;
            double** newMatrix = mc.create2DEmptyMatrix(columns, rows);

        #ifdef TIMING
            // Used to Time the transpose process
            auto start = high_resolution_clock::now(); 
        #endif

            // Rows for original matrix
            for(int m = 0; m < rows; m++)
            {
                // Columns for original matrix
                for(int n = 0; n < columns; n++)
                {
                    // Transpose the values
                    newMatrix[n][m] = origMatrix[m][n];
                }
            }

        #ifdef TIMING
            // Used to calculate the transpose time
            auto stop = high_resolution_clock::now();
            auto duration = duration_cast<microseconds>(stop - start); 
            cout << "Transpose Duration: " <<  duration.count() << " microseconds" << endl; 
        #endif

            return newMatrix;
        }

        /**
         * WORKER THREAD FUNCTION
         * Transpose a row in the matrix.  This will transpose the number of rows given in numRowsCompute.
         * This is used to breakup the work into rows.
         * 
         * :param newMatrix: The transposed matrix.
         * :param origMatrix: The original matrix.
         * :param rowStart: The row number to start with.
         * :param numRowsCompute: The number of rows to compute.
         * :param columns: The number of columns.
         */ 
        static void workerTransposeThreadN(double** newMatrix, double** origMatrix, int rowStart, int numRowsCompute, int columns)
        {
            for(int m = rowStart; m < numRowsCompute + rowStart; m++)
            {
                // Columns for original matrix
                for(int n = 0; n < columns; n++)
                {
                    // Because this only going to touch very unique spots in the 
                    // matrix, a lock is not needed
                    newMatrix[n][m] = origMatrix[m][n];
                }
            }
        }

        /**
         * This will reduce the number of threads created.  This will only use
         * numThreads threads.  Each thread will do 1/nth the work.
         * 
         * At very large matrix sizes, more than 10000 rows, you start to see
         * some improvements where threads could help.  But for smaller size
         * matrix, the overhead in creating and disposing of the threads makes
         * this function call slower.  Only use this for very large matrix.
         * 
         * :param origMatrix: Original Matrix to transpose.
         * :param rows: Number of rows.
         * :param columns: Number of columns.
         * :param numThreads: Number of threads to use.
         * :return Transposed Matrix.
         */
        double** transpose2DThreadN(double** origMatrix, int rows, int columns, int numThreads)
        {
            // Create a new matrix based on the size of this matrix
            MatrixCommon mc;
            double** newMatrix = mc.create2DEmptyMatrix(columns, rows);

        #ifdef TIMING
            // Used to Time the transpose process
            auto start = high_resolution_clock::now(); 
        #endif

            // Create an vector to hold all the threads
            // I chose a vector here to just use something other than
            // an array.  I would suggest an array
            // A thread will be created for each row
            vector<thread> threadHolder;

            int rowsPerThread = rows / numThreads;

            // Rows for original matrix
            for(int m = 0; m < rows; m+=rowsPerThread)
            {
                // Determine how many rows to compute for each thread
                // The last thread may not need to do a full work load
                int numRowsCompute = rowsPerThread;
                // Ensure the last value is not too large to go past the end of the array
                if(rowsPerThread + m > rows)
                {
                    numRowsCompute = rows - m;
                }

                // Create a thread to transpose this row
                threadHolder.emplace_back(workerTransposeThreadN, newMatrix, origMatrix, m, numRowsCompute, columns);
            }

            // Wait for all the threads to complete
            for(auto& t: threadHolder)
            {
                t.join();
            }

        #ifdef TIMING
            // Used to calculate the transpose time
            auto stop = high_resolution_clock::now();
            auto duration = duration_cast<microseconds>(stop - start); 
            cout << "Transpose " << numThreads <<  " Thread Duration: " <<  duration.count() << " microseconds" << endl; 
        #endif

            return newMatrix;
        }

        /**
         * Mutliply two matrices. Return the result.
         * 
         * It is assumed that the matrices are the correct size to allow multiplication
         * to be done.  This will not check the sizes.  The error checking of the
         * 2 matrices should be done before calling this method.
         * 
         * :param m1: First matrix to multiply.
         * :param m2: Second matrix to multiply.
         * :param m1Rows: Number of rows in the first matrix.
         * :param m1Columns: Number of columns in the first matrix and the number of rows in the second matrix.
         * :param m2Columsn: Number of columns in the second matrix.
         * :return: Solution the multiplication of the 2 matrices.
         */ 
        double** matrixMultiply2D(double** m1, double** m2, int m1Rows, int m1Columns, int m2Columns)
        {
            // Initialize the result
            MatrixCommon mc;
            double** resultMaxtrix = mc.create2DEmptyMatrix(m1Rows, m2Columns);

        #ifdef TIMING
            // Used to Time the transpose process
            auto start = high_resolution_clock::now(); 
        #endif

            // Preform the matrix multiplication
            for(int i = 0; i < m1Rows; i++)
            {
                for(int j = 0; j < m2Columns; j++)
                {
                    for(int k = 0; k < m1Columns; k++)
                    {
                        resultMaxtrix[i][j] += m1[i][k] * m2[k][j];
                    }
                }
            }

        #ifdef TIMING
            // Used to calculate the transpose time
            auto stop = high_resolution_clock::now();
            auto duration = duration_cast<microseconds>(stop - start); 
            cout << "Matrix Multiply Duration: " <<  duration.count() << " microseconds" << endl; 
        #endif

            // Return the result
            return resultMaxtrix;
        }

        /**
         * The thread worker that does the actual work in the thread.  This will multiply a small
         * portion of the 2 matrices given and set them in the result matrix.  
         * 
         * Based on the input, this will determine which thread it is to know which section it needs
         * to calculate.
         * 
         * :param resultMatrix: The matrix to set the results.
         * :param m1: First matrix to multiply.
         * :param m2: Second matrix to multiply.
         * :param m1Rows: Number of rows in the first matrix.
         * :param m1Columns: Number of columns in the first matrix and number of rows in the second column.
         * :param m2Columns: Number of columns in the second matrix.
         * :param numThreads: Number of threads used to do the calculations.
         * :param threadIndex: The index of the thread to know which chunck to work on.
         */ 
        static void multiplyThreadWorker(double** resultMaxtrix, double** m1, double** m2, int m1Rows, int m1Columns, int m2Columns, int numThreads, int threadIndex)
        {

            // Calculate how many elements will be done for each thread
            int elementsPerThread = m1Rows / numThreads;

            // Because we are dividing, we may have a remainder
            // Account for the remainder
            int remainder = m1Rows % numThreads;
            
            // If we have a remainder, we need to make one of the thread do more work
            // Use the first thread since it will be running the longest because
            // int starts before the rest of the threads
            int start, end;
            if(threadIndex == 0)
            {
                // Where to start the thread
                start = elementsPerThread * threadIndex;

                // Where to end the thread, just after the elementsPerThread
                // Also include the remainder in the first thread
                end = (elementsPerThread * (threadIndex + 1)) + remainder;
            }
            else
            {
                // Where to start in this thread
                // Take into acount the remainder that the first thread did
                start = elementsPerThread * threadIndex; + remainder;

                // Where to end this thread
                end = (elementsPerThread * (threadIndex + 1)) + remainder;
            }

            // Go through and do the multiplcation based on the
            // batch size calculated for the thread
            for(int i = start; i < end; i++)
            {
                for(int j = 0; j < m2Columns; j++)
                {
                    for(int k = 0; k < m1Columns; k++)
                    {
                        resultMaxtrix[i][j] += m1[i][k] * m2[k][j];
                    }
                }
            }
        }

        /**
         * Do a matrix multiplication using threads.  This will breakup the
         * multiplication steps in parts based on the number of threads that
         * are given to use.  Typically 4-5 threads will help for very large matrix.
         * Using any threads will be slower if used for smaller matrices.  The overhead
         * in creating the threads and joining will be longer then the calculation.
         * 
         * It is assumed that the matrix are the correct size to allow multiplication
         * to be done.  This will not check the sizes.  There error checking of the
         * 2 matrices should be done before calling this method.
         * 
         * :param m1: First matrix to multiply.
         * :param m2: Second matrix to multiply.
         * :param m1Rows: Number of rows in first matrix.
         * :param m1Columns: Number of columns in first matrix and number of rows in second matrix.
         * :param m2Columns: Number of columns in the second matrix.
         * :param numThreads: Number of threads to use to do the calculation. Look above for suggested sizes.
         * :return: The solution to multiplying the two matrices.
         * 
         * 
         */ 
        double** matrixMultiplyThread(double** m1, double** m2, int m1Rows, int m1Columns, int m2Columns, int numThreads)
        {
            // Initialize a thread for the results
            MatrixCommon mc;
            double** resultMaxtrix = mc.create2DEmptyMatrix(m1Rows, m2Columns);

        #ifdef TIMING
            // Used to Time the transpose process
            auto start = high_resolution_clock::now(); 
        #endif

            // Create an array to hold all the threads
            // A thread will be created based on the input
            thread threadHolder[numThreads];

            // Create a thread and breakup the matrix calculations
            for(int threadCtr = 0; threadCtr < numThreads; threadCtr++)
            {
                // Create a thread to transpose this row
                threadHolder[threadCtr] = thread(multiplyThreadWorker, resultMaxtrix, m1, m2, m1Rows, m1Columns, m2Columns, numThreads, threadCtr);
            }

            // Wait for all the threads to complete
            for(int x = 0; x < numThreads; x++)
            {
                threadHolder[x].join();
            }

        #ifdef TIMING
            // Used to calculate the transpose time
            auto stop = high_resolution_clock::now();
            auto duration = duration_cast<microseconds>(stop - start); 
            cout << "Matrix Multiply " << numThreads <<  " Thread Duration: " <<  duration.count() << " microseconds" << endl; 
        #endif

            return resultMaxtrix;
        }

    public:
        /**
         * Transpose the matrix.
         * 
         * Based on the number of threads, determine which is the best method
         * to use.  When the matrix is small or only 1 thread is needed, 
         * then do not use any threading.  It will be faster since you will not
         * have to create a thread and wait for it to complete.
         * 
         * :param origMatrix: Original matrix to transpose.
         * :param rows: Number of rows in the original matrix.
         * :param columns: Number of columns in the original matrix.
         * :param numThreads: Number of threads to use.
         * :return: Transposed matrix.
         */ 
        double** transpose(double** origMatrix, int rows, int columns, int numThreads)
        {
            if(numThreads <= 1)
            {
                // No threads used
                return transpose2D(origMatrix, rows, columns);
            }
            else
            {
                // Use the given number of threads
                return transpose2DThreadN(origMatrix, rows, columns, numThreads);
            }
        }

        /**
         * Matrix Multiplication.  
         * 
         * Based on the number of threads, determine which is the best method
         * to use.  When the matrix is small or only 1 thread is needed, 
         * then do not use any threading.  It will be faster since you will not
         * have to create a thread and wait for it to complete.
         * 
         * It is assumed that the matrix are the correct size to allow multiplication
         * to be done.  This will not check the sizes.  There error checking of the
         * 2 matrices should be done before calling this method.
         * 
         * :param m1: First matrix to multiply.
         * :param m2: Second matrix to multiply.
         * :param m1Rows: Number of rows in first matrix.
         * :param m1Columns: Number of columns in first matrix and number of rows in second matrix.
         * :param m2Columns: Number of columns in the second matrix.
         * :param numThreads: Number of threads to use to do the calculation. Look above for suggested sizes.
         * :return: The solution to multiplying the two matrices.
         */ 
        double** matrixMultiply(double** m1, double** m2, int m1Rows, int m1Columns, int m2Columns, int numThreads)
        {
            if(numThreads <= 1)
            {
                // No threads used
                return matrixMultiply2D(m1, m2, m1Rows, m1Columns, m2Columns);
            }
            else
            {
                // Use the given number of threads
                return matrixMultiplyThread(m1, m2, m1Rows, m1Columns, m2Columns, numThreads);
            }
        }
};