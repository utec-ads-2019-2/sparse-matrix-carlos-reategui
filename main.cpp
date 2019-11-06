#include <iostream>
#include "matrix.h"
#include "tester/tester.h"

int main() {
    //Tester::execute();
    auto sparseMatrix = new Matrix<int>(3, 2);
    sparseMatrix->set(0, 0, 3);
    sparseMatrix->set(0, 1, 4);
    sparseMatrix->set(1, 0, 7);
    sparseMatrix->set(1, 1, 2);
    sparseMatrix->set(2, 0, 5);
    sparseMatrix->set(2, 1, 9);
    auto sparseMatrix2 = new Matrix<int>(2, 3);
    sparseMatrix2->set(0, 0, 3);
    sparseMatrix2->set(0, 1, 1);
    sparseMatrix2->set(0, 2, 5);
    sparseMatrix2->set(1, 0, 6);
    sparseMatrix2->set(1, 1, 9);
    sparseMatrix2->set(1, 2, 7);

    auto sparseMatrix3 = sparseMatrix->operator*(*sparseMatrix2);
    sparseMatrix3.print();


    return EXIT_SUCCESS;
}

