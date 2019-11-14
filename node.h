#ifndef SPARSE_MATRIX_NODE_H
#define SPARSE_MATRIX_NODE_H

template <typename T>
class Matrix;

template <class T>
class MatrixNode {
protected:
    T data;
    unsigned int row, column;
    MatrixNode<T> *next, *down;
public:
    MatrixNode(unsigned int row, unsigned int column, T data) : row(row), column(column), data(data), next(nullptr),
    down(nullptr) {}

    ~MatrixNode() {
        next = nullptr;
        down = nullptr;
        row = column = 0;
    }

    friend class Matrix<T>;
};

template <class T>
class SourceNode {
protected:
    unsigned int index;
    MatrixNode<T>* link;
public:
    explicit SourceNode(unsigned int index) : index(index), link(nullptr) {}

    ~SourceNode() {
        link = nullptr;
        index = 0;
    }

    friend class Matrix<T>;
};
#endif //SPARSE_MATRIX_NODE_H
