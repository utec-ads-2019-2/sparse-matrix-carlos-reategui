#include <iostream>
#include "matrix.h"
#include "tester/tester.h"

template <typename T>
ostream& operator<<(ostream& os, Matrix<T> &matrix) {
    matrix.print();
    return os;
}

int main() {
    Tester::execute();

    Matrix<int> m1(2, 2);
    m1.set(0, 0, 1);
    //m1.set(0, 1, 2);
    m1.set(1, 0, 3);
    m1.set(1, 1, 4);

    Matrix<int> m2(2, 2);
    m2.set(0, 0, 1);
    m2.set(0, 1, 2);
    m2.set(1, 0, 3);
    m2.set(1, 1, 4);

    Matrix<int> m3(2, 2);
    m3 = m1.operator*(5);
    cout << "\tMULTIPLICATION\n" << m3 << endl;
    cout << "\tMULTIPLICATION 2\n" << m1 << endl;

    Matrix<int> m4(3, 3);
    m4.set(0, 0, 1);
    m4.set(0, 1, 2);
    m4.set(0, 2, 3);
    m4.set(1, 0, 4);
    m4.set(1, 1, 5);
    m4.set(1, 2, 6);
    m4.set(2, 0, 7);
    m4.set(2, 1, 8);
    m4.set(2, 2, 9);

    cout << "\t3x3 MATRIX\n" << m4 << endl;

    Matrix<int> m5(2, 2);
    m5.set(0, 0, 1);
    m5.set(0, 1, 2);
    m5.set(1, 0, 3);
    m5.set(1, 1, 4);

    Matrix<int> m6(2, 2);
    m6.set(0, 0, 1);
    m6.set(0, 1, 2);
    m6.set(1, 0, 3);
    m6.set(1, 1, 4);

    Matrix<int> resultOfSum = m5 + m6;
    cout << "\tSUM\n" << resultOfSum << endl;

    Matrix<int> m7(2, 2);
    m7.set(0, 0, 2);
    m7.set(0, 1, 3);
    m7.set(1, 0, 4);
    m7.set(1, 1, 5);

    Matrix<int> m8(2, 2);
    m8.set(0, 0, 1);
    m8.set(0, 1, 2);
    m8.set(1, 0, 3);
    m8.set(1, 1, 4);

    Matrix<int> resultOfSubtraction = m7.operator-(m8);
    cout << "\tSUBTRACTION\n" << resultOfSubtraction << endl;

    Matrix<int> m9(2, 3);
    //m9.set(0, 0, 1);
    m9.set(0, 1, 2);
    m9.set(0, 2, 3);
    m9.set(1, 0, 4);
    m9.set(1, 1, 5);
    m9.set(1, 2, 6);
    cout << m9 << endl;

    Matrix<int> transpose = m9.transpose();
    cout << "\tTRANSPOSE\n" << transpose << endl;
    
    Matrix<int> m10(2, 2);
    m10.set(0, 0, 1);
    m10.set(0, 1, 2);
    m10.set(1, 0, 3);
    m10.set(1, 1, 4);
    
    Matrix<int> m11(2, 2);
    m11.set(0, 0, 1);
    m11.set(0, 1, 2);
    m11.set(1, 0, 3);
    m11.set(1, 1, 4);

    Matrix<int> multiplication = m10.operator*(m11);
    cout << "\tMULTIPLICATION\n" << multiplication << endl;

    return EXIT_SUCCESS;
}

