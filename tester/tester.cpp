#include "tester.h"

void Tester::execute() {
    Mocker mocker;

    for (int i = 0; i < NUMBER_OF_TESTS; ++i) {
        cout << "-------------------"<< endl;
        cout << setw(10) << i << endl;
        cout << "-------------------" << endl;
        unsigned int rows = mocker.generateRandomInt(10);
        unsigned int columns = mocker.generateRandomInt(10);
        testMatrix<int>(rows, columns);
    }
}

template <typename T>
void Tester::testMatrix(unsigned int rows, unsigned int columns) {
    Mocker mocker;

    T **matrix1 = buildMatrix<T>(rows, columns);
    Matrix<T> test1 = setMatrix<T>(matrix1, rows, columns);

    unsigned int scalar = mocker.generateRandomInt(10);
    Matrix<T> result = test1 * scalar;
    cout << "\tMULTIPLICATION\n";
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < columns; ++j) {
            cout << "result(" << i << ", " << j << "): " << result(i, j) << endl;
            cout << "matrix[" << i << "]" << "[" << j << "] * " << scalar << ": " << matrix1[i][j] * scalar << endl;
            ASSERT(result(i, j) == matrix1[i][j] * scalar, "There is a problem with the scalar multiplication");
        }
    }

    int **matrix2 = buildMatrix<T>(rows, columns);
    Matrix<T> test2 = setMatrix<T>(matrix2, rows, columns);
    result = test1 + test2;
    cout << "\tADDITION\n";
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < columns; ++j) {
            cout << "result(" << i << ", " << j << "): " << result(i, j) << endl;
            cout << "matrix1[" << i << "]" << "[" << j << "]: " << matrix1[i][j] << " + "
            << "matrix2[" << i << "]" << "[" << j << "]: " << matrix2[i][j] << " = " <<
            matrix1[i][j] + matrix2[i][j] << endl;
            ASSERT(result(i, j) == matrix1[i][j] + matrix2[i][j], "There is a problem with the addition");
        }
    }

    result = test1 - test2;
    cout << "\tSUBTRACTION\n";
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < columns; ++j) {
            cout << "result(" << i << ", " << j << "): " << result(i, j) << endl;
            cout << "matrix1[" << i << "]" << "[" << j << "]: " << matrix1[i][j] << " - "
                 << "matrix2[" << i << "]" << "[" << j << "]: " << matrix2[i][j] << " = " <<
                 matrix1[i][j] - matrix2[i][j] << endl;
            ASSERT(result(i, j) == matrix1[i][j] - matrix2[i][j], "There is a problem with the subtraction");
        }
    }
}

template <typename T>
T** Tester::buildMatrix(unsigned int rows, unsigned int columns) {
    Mocker mocker;

    T **matrix = new int*[rows];
    for (int i = 0; i < rows; ++i) {
        matrix[i] = mocker.generateRandomArray<T>(columns);
    }

    return matrix;
}

template <typename T>
Matrix<T> Tester::setMatrix(T **&matrix, unsigned int rows, unsigned int columns) {
    Matrix<T> result(rows, columns);
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < columns; ++j) {
            result.set(i, j, matrix[i][j]);
            ASSERT(result(i, j) == matrix[i][j], "There is a problem with the set or operator()");
        }
    }

    return result;
}