#include <iostream>
#include "matrix.h"
#include "tester/tester.h"

int main() {
    //Tester::execute();

    /*auto sparseMatrix2 = new Matrix<string>(2, 3);
    sparseMatrix2->set(0, 0, "(0, 0)");
    sparseMatrix2->set(0, 1, "(0, 1)");
    sparseMatrix2->set(0, 2, "(0, 2)");
    sparseMatrix2->set(1, 0, "(1, 0)");
    sparseMatrix2->set(1, 1, "(1, 1)");
    sparseMatrix2->set(1, 2, "(1, 2)");
    sparseMatrix2->print();*/

    auto m1 = new Matrix<int>(2, 2);
    m1->set(0, 0, 1);
    m1->set(0, 1, 2);
    m1->set(1, 0, 3);
    m1->set(1, 1, 4);
    m1->print();

    auto m2 = new Matrix<int>(3, 3);
    m2->set(0, 0, 10);
    m2->set(0, 1, 20);
    m2->set(0, 2, 30);
    m2->set(1, 0, 40);
    m2->set(1, 1, 50);
    m2->set(1, 2, 60);
    m2->set(2, 0, 70);
    m2->set(2, 1, 80);
    m2->set(2, 2, 90);
    m2->print();

    auto m3 = new Matrix<int>(2, 2);
    m3->set(0, 0, 10);
    m3->set(0, 1, 20);
    m3->set(1, 0, 30);
    m3->set(1, 1, 40);
    m3->print();

    auto trans = m2->transpose();
    trans.print();

    cout << m2->operator==(*m3) << endl;

    auto mul = m1->operator*(*m3);
    mul.print();

    return EXIT_SUCCESS;
}

