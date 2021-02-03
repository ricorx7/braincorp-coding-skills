#include <cstdio>
#include <iostream>
#include <vector>

using namespace std; 

/**
 * Create a 2D matrix given a i,j.
 * 
 * :param rows: The number of rows (height)
 * :param columns: The numbers of columns (width)
 * :param starValue: Start value in the matrix.  The values are incremented.
 * :return A 2D matrix with the given width 
 *         and height.  The values are populated with 
 *         a incrementing number. 
 */
int** create2DMatrix(int rows, int columns, int startValue) 
{
    // Initialize the 2D array
    int** matrix = 0;

    // Create the rows
    matrix = new int*[rows];

    int index = startValue;
    // Go through each row to create a column
    for(int m = 0; m < rows; m++)
    {
        // Create the rows based on the number of columns
        matrix[m] = new int[columns];

        // Insert the values
        for(int n = 0; n < columns; n++) 
        {
            matrix[m][n] = index++;
        }
    }

    return matrix;
}

/**
 * Create a 2D matrix given a MxN (Row x Column).
 * 
 * :param rows: The number of rows (height)
 * :param columns: The numbers of columns (width)
 * :return A 2D matrix with the given width and height.  The values are populated with a incrementing number. 
 */
int** create2DEmptyMatrix(int rows, int columns) 
{
    // Initialize the 2D array
    int** matrix = 0;

    // Create the rows
    matrix = new int*[rows];

    // Go through each row to create a column
    for(int m = 0; m < rows; m++)
    {
        // Create the rows based on the number of columns
        matrix[m] = new int[columns];

        // Insert the values
        for(int n = 0; n < columns; n++) 
        {
            matrix[m][n] = 0;
        }
    }

    return matrix;
}

/**
 * Clean up the matrix to prevent memory leaks.
 * 
 * :param matrix: Matrix to delete.
 * :param rows: Number of rows.
 * 
 */
void clean2DMatrix(int** matrix, int rows) 
{
    for(int m = 0; m < rows; m++)
    {
        // Delete each row
        delete [] matrix[m];
    }

    // Delete the entire matrix
    delete [] matrix;
}

/**
 * Print the 2D Matrix.
 * 
 * :param matrix: Matrix to print.
 * :param rows: Number of rows.
 * :param columns: Number of columns.
 * :param printMatrix: If set to false, only the size is printed.
 * 
 */ 
void print2DMatrix(int** matrix, int rows, int columns, bool printMatrix) 
{
    printf("Matrix: [%i,%i] - %i Rows, %i Columns\n", rows, columns, rows, columns);

    // Only print the entire matrix if flag is set
    if(printMatrix)
    {
        // Rows
        for(int m = 0; m < rows; m++)
        {
            // Columns
            for(int n = 0; n < columns; n++) 
            {
                printf("%i,", matrix[m][n]);
            }
            // New Row
            printf("\n");
        }
    }
}