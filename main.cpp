#include <iostream>
#include "matrix.h"
#include "tester/tester.h"

int main() {
    //Tester::execute();
    auto* sparseMatrix = new Matrix<si>(2, 2);
    sparseMatrix->set(0, 0, 4);
    sparseMatrix->set(0, 1, 5);


    return EXIT_SUCCESS;
}

