#include <iostream>
#include "matrix.h"
#include "tester/tester.h"

int main() {
    //Tester::execute();

    Matrix<int> m1(2, 2);
    m1.set(0, 0, 1);
    m1.set(0, 1, 2);
    m1.set(1, 0, 3);
    m1.set(1, 1, 4);

    Matrix<int> m2(2, 2);
    m2.set(0, 0, 1);
    m2.set(0, 1, 2);
    m2.set(1, 0, 3);
    m2.set(1, 1, 4);

    cout << m1.operator==(m2);

    Matrix<int> m3(2, 2);
    m3 = m1.operator*(5);
    m3.print();

    return EXIT_SUCCESS;
}

