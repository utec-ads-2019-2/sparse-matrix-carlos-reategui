#ifndef SPARSE_MATRIX_NODE_H
#define SPARSE_MATRIX_NODE_H

template <typename T>
class Matrix;

template <typename T>
class Node {
protected:
    T data;
    Node<T> *next, *down;
    unsigned int row, column;
public:
    explicit Node() {
        next = down = nullptr;
        row = column = 0;
    };

    friend class Matrix<T>;
};

#endif //SPARSE_MATRIX_NODE_H
