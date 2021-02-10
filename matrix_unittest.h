#include <iostream>
#include <assert.h>

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


        void test_all()
        {
            test_matrix_cleanup();
            test_matrix_create();
            test_matrix_create_empty();
        }
};
