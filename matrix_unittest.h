#include <iostream>
#include <assert.h>
#include <cmath>

using namespace std;

// To prevent circular dependencies
class MatrixAlgebra;
class MatrixCommon;

class TestMatrix {

    public:
        void test_matrix_create() 
        {
            MatrixCommon mc;
            double** test1M = mc.create2DMatrix(2, 2, 2.0);

            assert(test1M[0][0] == 2.0);
            assert(test1M[0][1] == 3.0);
            assert(test1M[1][0] == 4.0);
            assert(test1M[1][1] == 5.0);

            mc.clean2DMatrix(test1M, 2);

            cout << "Test Matrix Create PASS" << endl;
        }

        void test_matrix_create_empty() 
        {
            MatrixCommon mc;
            double** test1M = mc.create2DEmptyMatrix(2, 2);

            assert(test1M[0][0] == 0.0);
            assert(test1M[0][1] == 0.0);
            assert(test1M[1][0] == 0.0);
            assert(test1M[1][1] == 0.0);

            mc.clean2DMatrix(test1M, 2);

            cout << "Test Matrix Creation Empty PASS" << endl;
        }

        void test_transpose() 
        {
            MatrixCommon mc;
            double** test1M = mc.create2DMatrix(2, 2, 2.0);

            // 2 3
            // 4 5
            assert(test1M[0][0] == 2.0);
            assert(test1M[0][1] == 3.0);
            assert(test1M[1][0] == 4.0);
            assert(test1M[1][1] == 5.0);

            MatrixAlgebra ma;
            double** result = ma.transpose(test1M, 2, 2, 1);

            // 2 4
            // 3 5
            assert(result[0][0] == 2.0);
            assert(result[1][0] == 3.0);
            assert(result[0][1] == 4.0);
            assert(result[1][1] == 5.0);

            mc.clean2DMatrix(test1M, 2);
            mc.clean2DMatrix(result, 2);

            cout << "Test Matrix Transpose PASS" << endl;
        }

        void test_matrix_multiply() 
        {
            MatrixCommon mc;
            double** test1M = mc.create2DMatrix(3, 2, 2.15);
            double** test2M = mc.create2DMatrix(2, 3, 1.65);

            assert(test1M[0][0] == 2.15);
            assert(test1M[0][1] == 3.15);
            assert(test1M[1][0] == 4.15);
            assert(test1M[1][1] == 5.15);
            assert(test1M[2][0] == 6.15);
            assert(test1M[2][1] == 7.15);

            assert(test2M[0][0] == 1.65);
            assert(test2M[0][1] == 2.65);
            assert(test2M[0][2] == 3.65);
            assert(test2M[1][0] == 4.65);
            assert(test2M[1][1] == 5.65);
            assert(test2M[1][2] == 6.65);

            MatrixAlgebra ma;
            double** result = ma.matrixMultiply(test1M, test2M, 3, 2, 3, 1);

            assert(fabs(result[0][0] - 18.195) < 0.01f);
            assert(fabs(result[0][1] - 23.495) < 0.01f);
            assert(fabs(result[0][2] - 28.795) < 0.01f);
            assert(fabs(result[1][0] - 30.795) < 0.01f);
            assert(fabs(result[1][1] - 40.095) < 0.01f);
            assert(fabs(result[1][2] - 49.395) < 0.01f);
            assert(fabs(result[2][0] - 43.395) < 0.01f);
            assert(fabs(result[2][1] - 56.695) < 0.01f);
            assert(fabs(result[2][2] - 69.995) < 0.01f);

            mc.clean2DMatrix(test1M, 2);
            mc.clean2DMatrix(test2M, 2);
            mc.clean2DMatrix(result, 2);

            cout << "Test Matrix Multiply PASS" << endl;
        }



        void test_all()
        {
            test_matrix_create();
            test_matrix_create_empty();
            test_transpose();
            test_matrix_multiply();
        }
};
