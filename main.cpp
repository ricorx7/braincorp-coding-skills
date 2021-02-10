#include "matrix.h"
#include "matrix_unittest.h"

/**
 * Test Matrix Multiplication and Matrix Transpose.
 * 
 * 8 Arguments that are optional:
 * [M1_Rows] [M1_Columns] [M2_Rows] [M2_Columns] [NumThreads] [M1_StartValue] [M2_StartValue]
 * 
 * :param M1_Rows: Number of rows in matrix 1. [DEFAULT: 2]
 * :param M1_Columns: Number of columns in matrix 1. [DEFAULT: 3]
 * :param M2_Rows: Number of rows in matrix 2. [DEFAULT: 3]
 * :param M2_Columns: Number of columns in matrix 2.  [DEFAULT: 2]
 * :param NumThreads: Number threads to utilize.  [DEFAULT: 2]
 * :param M1_StartValue: The first value in matrix 1.  The values are incremented in the matrix. [DEFAULT: 0]
 * :param M2_StartValue: The first value in matrix 2.  The values are incremented in the matrix. [DEFAULT: 5]
 *  
 * 
 * This application will utilize Matrix Multiplication and Matrix Transpose
 * defined in matrix.h.  The code has been optimized to utilize threads to increase
 * speed.  But sometimes multiple threads is not always needed.  
 * Typically 4-5 threads will help for very large matrix (10000 x 10000).
 * Using any threads will be slower if used for smaller matrices.  The overhead
 * in creating the threads and joining will be longer then the calculation.
 * 
 * The matrix are created based on the start value.  The matrix will have the values incremeted.
 * The full matrix will NOT be displayed if the matrix exceeds 10 rows or columns.
 * 
 */ 
int main(int argc, char** argv)
{
    /** Handle setting up the user input **/

    int iM1Rows = 2;          // DEFAULT Number of rows (Height/rows)
    int iM1Columns = 3;       // DEFAULT Number of columns (Width/columns)
    int iM2Rows = 3;          // DEFAULT Number of rows (Height/rows)
    int iM2Columns = 2;       // DEFAULT Number of columns (Width/columns)

    int iNumThreads = 2;      // DEFAULT Number of threads to utilize threads efficiently
    double iM1StartValue = 0;    // DEFAULT Start value in the matrix 1.
    double iM2StartValue = 5;    // DEFAULT start value for matrix 2

    // Handle any arguments
    if(argc >= 5)
    {
        try 
        {
            // Convert the arguments to integers
            iM1Rows = stoi(argv[1]);
            iM1Columns = stoi(argv[2]);
            iM2Rows = stoi(argv[3]);
            iM2Columns = stoi(argv[4]);
            
            // Check if number of threads is given
            if(argc >= 6)
            {
                iNumThreads = stoi(argv[5]);
            }

            // Check if M1 start value is given
            if(argc >= 7)
            {
                iM1StartValue = stod(argv[6]);
            }

            // Check if M2 start value is given
            if(argc >= 8)
            {
                iM2StartValue = stod(argv[7]);
            }
        }
        catch(exception &ex)
        {
            cerr << "Error processing input arguments [M1_Rows] [M1_Columns] [M2_Rows] [M2_Columns] [NumThreads] [M1_StartValue] [M2_StartValue]" << endl;
        }
    }

    // Decide wheter to print the matrix completely
    // based on the size
    bool bPrintMatrix = true;
    if(iM1Rows > 10 || iM1Columns > 10 || iM2Rows > 10 || iM2Columns > 10)
    {
        // Do not print the matrix
        bPrintMatrix = false;
    }

    // Verify the number of threads is not large for small matrices
    // Threshold for now is 3x3 or smaller, but should be even larger
    // But for testing purpose, the number is left low to allow to see the difference
    int maxMatrix = max(max(max(iM1Rows, iM2Columns), iM2Rows), iM2Columns);
    if(maxMatrix <= 3)
    {
        // Only use 1 thread
        iNumThreads = 1;
    }

    // Verify good values are given for the matrices
    if(iM1Rows <= 0 || iM2Rows <= 0 || iM1Columns <= 0 || iM2Columns <= 0)
    {
        cerr << "The given matrix sizes were zero or less then zero, please use a correct size." << endl;
        return -1;
    }

    // Verify for matrix multiplication, matrix 1 and 2 can be multiplied together
    if(iM1Columns != iM2Rows)
    {
        cerr << "Matrix 1 Columns must equal Matrix 2 Rows." << endl;
        return -2;
    }

    /******************************************************/
    /** ACTUAL Program **/
    /******************************************************/
    cout << "---------------------------" << endl;
    cout << "Matrix Multiplication" << endl;
    cout << "---------------------------" << endl;

    MatrixAlgebra ma;
    MatrixCommon mc;

    // Create the initial thread
    double** matrix1 = mc.create2DMatrix(iM1Rows, iM1Columns, iM1StartValue);
    double** matrix2 = mc.create2DMatrix(iM2Rows, iM2Columns, iM2StartValue);

    cout << "Original Matrices: " << endl;
    mc.print2DMatrix(matrix1, iM1Rows, iM1Columns, bPrintMatrix);
    cout << endl;
    mc.print2DMatrix(matrix2, iM2Rows, iM2Columns, bPrintMatrix);
    cout << endl;

    // Matrix Multiply
    cout << "Result: " << endl;
    double** resultT = ma.matrixMultiply(matrix1, matrix2, iM1Rows, iM1Columns, iM2Columns, iNumThreads);
    mc.print2DMatrix(resultT, iM1Rows, iM2Columns, bPrintMatrix);
    mc.clean2DMatrix(resultT, iM1Rows);

    /******************************************************/

    /******************************************************/
    /** Transpose Example 
    /******************************************************/
    cout << endl;
    cout << "---------------------------" << endl;
    cout << "Matrix Transpose" << endl;
    cout << "---------------------------" << endl;
    // Create the initial matrix
    cout << "Original Matrix: " << endl;
    mc.print2DMatrix(matrix1, iM1Rows, iM1Columns, bPrintMatrix);

    cout << endl;

    // Transpose the matrix
    cout << "Result: " << endl;
    double** matrixT = ma.transpose(matrix1, iM1Rows, iM1Columns, iNumThreads);
    mc.print2DMatrix(matrixT, iM1Columns, iM1Rows, bPrintMatrix);
    mc.clean2DMatrix(matrixT, iM1Columns);

    mc.clean2DMatrix(matrix1, iM1Rows);
    mc.clean2DMatrix(matrix2, iM2Rows);

    TestMatrix tm;
    tm.test_all();
}