#ifndef SPARSE_MATRIX_NODE_H
#define SPARSE_MATRIX_NODE_H

template <typename T>
class Matrix;

template <class T>
class MatrixNode {
protected:
    T data;
    unsigned int row, column;
    MatrixNode<T>* next, *down;
public:
    MatrixNode(unsigned int row, unsigned int column, T data) : row(row), column(column), data(data), next(nullptr),
    down(nullptr) {}

    ~MatrixNode() {}

    friend class Matrix<T>;
};

template <class T>
class SourceRowNode {
protected:
    unsigned int row;
    MatrixNode<T>* next;
public:
    explicit SourceRowNode(unsigned int row) : row(row), next(nullptr) {}
    friend class Matrix<T>;
};


template <class T>
class SourceColumnNode {
protected:
    unsigned int column;
    MatrixNode<T>* down;
public:
    explicit SourceColumnNode(unsigned int column) : column(column), down(nullptr) {}
    friend class Matrix<T>;
};




#endif //SPARSE_MATRIX_NODE_H
