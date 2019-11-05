#ifndef SPARSE_MATRIX_MATRIX_H
#define SPARSE_MATRIX_MATRIX_H

#include <stdexcept>
#include <vector>
#include <map>
#include "node.h"
#include <iomanip>

typedef short unsigned int sui;
using namespace std;

template <typename T>
class Matrix {
private:
    vector<Node<T>* > * rowsSourceNodes;
    vector<Node<T>* > * columnSourceNodes;
    map<pair<sui, sui>, Node<T>* > * sparseMatrix;
    sui rows, columns;

public:
    Matrix(sui rows, sui columns) : rows(rows), columns(columns) {
        rowsSourceNodes = new vector<Node<T>* >(rows);
        columnSourceNodes = new vector<Node<T>* >(columns);
        sparseMatrix = new map<pair<sui, sui>, Node<T>* >;

        for (sui i = 0; i < rows; ++i)
            (*rowsSourceNodes)[i] = new Node<T>();

        for (sui i = 0; i < columns; ++i)
            (*columnSourceNodes)[i] = new Node<T>();

        sui rowCount = 1, columnCount = 1;
        for (auto r = rowsSourceNodes->begin(); r != rowsSourceNodes->end(); ++r) {
            (*r)->next = nullptr;
            rowCount == rows ? (*r)->down = nullptr : (*r)->down = *r + 1;
            (*r)->row = rowCount++;
            (*r)->column = 0;
        }

        for (auto c = columnSourceNodes->begin(); c != columnSourceNodes->end(); ++c) {
            columnCount == columns ? (*c)->next = nullptr : (*c)->next = *c + 1;
            (*c)->down = nullptr;
            (*c)->row = 0;
            (*c)->column = columnCount++;
        }
    }

    void set(sui row, sui column, T data) {

        // CREATE NODE TO INSERT

        auto nodeToInsert = new Node<T>();
        nodeToInsert->data = data;
        nodeToInsert-row = row;
        nodeToInsert->column = column;
        nodeToInsert->next = nullptr;
        nodeToInsert->down = nullptr;


        // CHECK IN SPARSE MATRIX MAP

        if (!sparseMatrix->find(make_pair(row, column))) {
            // CHECK IF ROW AND COLUMNS NODES ARE NULLPTR
            // IF THEY'RE, CREATE THE SOURCE ROW OR COLUMN

            // CREATE ROW NODE
            if (!rowsSourceNodes->find(row)) {
                auto rowNode = new Node<T>();
                rowNode->row = row;
                rowNode->column = 0;
                rowNode->next = nodeToInsert;
                rowNode->down = nullptr;
                (*rowsSourceNodes)[row] = rowNode;
            } else {
                Node<T>* currentNode = (*rowsSourceNodes)[row];
                while (currentNode->column < column and currentNode->next)
                    currentNode = currentNode->next;
                Node<T>* prevOfNodeToInsert = currentNode;
                Node<T>* nextOfNodeToInsert = currentNode->next;
                prevOfNodeToInsert->next = nodeToInsert;
                nodeToInsert->next = nextOfNodeToInsert;
            }

                // CREATE COLUMN NODE
            if (!columnSourceNodes->find(column)) {
                auto columnNode = new Node<T>();
                columnNode->row = 0;
                columnNode->column = column;
                columnNode->next = nullptr;
                columnNode->down = nodeToInsert;
                (*columnSourceNodes)[column] = columnNode;
            }



        }
    }

    T operator()(unsigned, unsigned) const;
    Matrix<T> operator*(T scalar) const;
    Matrix<T> operator*(Matrix<T> other) const;
    Matrix<T> operator+(Matrix<T> other) const;
    Matrix<T> operator-(Matrix<T> other) const;
    Matrix<T> transpose() const;
    void print() const {
        cout << setw(50);
        for (sui i = 1; i <= columns; ++i)
            cout << setw(50) << "Column " << i;

        cout << endl;

        for (sui i = 1; i <= columns; ++i) {
            cout << "Row " << i << endl;
        }
    }

    ~Matrix();
};

#endif //SPARSE_MATRIX_MATRIX_H