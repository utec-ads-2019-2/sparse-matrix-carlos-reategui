#ifndef SPARSE_MATRIX_MATRIX_H
#define SPARSE_MATRIX_MATRIX_H

#include <stdexcept>
#include <vector>
#include <map>
#include "node.h"
#include <iomanip>

typedef short int si;
using namespace std;

template <typename T>
class Matrix {
private:
    vector<Node<T>* >* rowsNodes;
    vector<Node<T>* >* columnsNodes;
    si numberOfRows, numberOfColumns;

public:
    Matrix(si rows, si columns) : numberOfRows(rows), numberOfColumns(columns) {
        rowsNodes = new vector<Node<T>* >(rows);
        columnsNodes = new vector<Node<T>* >(columns);

        for (si i = 0; i < rows; ++i)
            (*rowsNodes)[i] = new Node<T>();

        for (si i = 0; i < columns; ++i)
            (*columnsNodes)[i] = new Node<T>();

        si rowCount = 0, columnCount = 0;
        for (auto r = rowsNodes->begin(); r != rowsNodes->end(); ++r) {
            (*r)->next = nullptr;
            rowCount == rows - 1 ? (*r)->down = nullptr : (*r)->down = *r + 1;
            (*r)->row = rowCount++;
        }

        for (auto c = columnsNodes->begin(); c != columnsNodes->end(); ++c) {
            columnCount == columns - 1 ? (*c)->next = nullptr : (*c)->next = *c + 1;
            (*c)->down = nullptr;
            (*c)->column = columnCount++;
        }
    }

    void set(si row, si column, T data) {
        if (row > numberOfRows or column > numberOfColumns)
            throw invalid_argument("Index out of range");
        if (!findAndReturnNode(row, column)) {
            auto nodeToInsert = new Node<T>();
            nodeToInsert->data = data;
            nodeToInsert->row = row;
            nodeToInsert->column = column;
            nodeToInsert->next = nullptr;
            nodeToInsert->down = nullptr;

            Node<T>* r = (*rowsNodes)[row];
            Node<T>* c = (*columnsNodes)[column];

            while (r->column < column and r->next)
                r = r->next;

            while (c->row < row and c->down)
                c = c->down;

            Node<T>* onTheLeftOfNodeToInsert = r, *onTheRightOfNodeToInsert = r->next,
            *aboveNodeToInsert = c, *belowNodeToInsert = c->next;

            onTheLeftOfNodeToInsert->next = nodeToInsert;
            nodeToInsert->next = onTheRightOfNodeToInsert;
            aboveNodeToInsert->down = nodeToInsert;
            nodeToInsert->down = belowNodeToInsert;

        } else if (findAndReturnNode(row, column) and findAndReturnNode(row, column)->data != data){
            Node<T>* currentNode = findAndReturnNode(row, column);
            currentNode->data = data;
        }
    }

    Node<T>* findAndReturnNode(si row, si column) {
        for (auto it = rowsNodes->begin(); it != rowsNodes->end(); ++it) {
            Node<T>* currentNode = (*it)->next;
            while (currentNode) {
                if (currentNode->row == row and currentNode->column == column)
                    return currentNode;
                currentNode = currentNode->next;
            }
        }
        return nullptr;
    }

    T operator()(si i, si j) const {
        Node<T>* row = *(rowsNodes)[i];
        while (row) {
            if (row->column == j)
                return row->data;
            row = row->next;
        }
        return 0;
    }

    Matrix<T> operator*(T scalar) const {
        for (auto it = rowsNodes->begin(); it != rowsNodes->end(); ++it) {
            Node<T>* currentNode = *it;
            // if row is empty:
            if (!currentNode->next)
                continue;
            // assigning current node, to the first node of the row
            currentNode = currentNode->next;
            while (currentNode) {
                currentNode->data *= scalar;
                currentNode = currentNode->next;
            }
        }
        return this;
    }
    Matrix<T> operator*(Matrix<T> other) const {
        return this;
    }
    Matrix<T> operator+(Matrix<T> other) const {
        si maxRows, maxColumns;
        this->numberOfRows <= other.numberOfRows ? maxRows = other.numberOfRows : maxRows = this->numberOfRows;
        this->numberOfColumns <= other.numberOfColumns ?  maxColumns = other.numberOfRows : maxColumns = this->numberOfRows;
        Matrix<T> result(maxRows, maxColumns);
        for (si r = 0; r < maxRows; ++r)
            for (si c = 0; c < maxColumns; ++c)
                result.set(r, c, this->operator()(r, c) + other.operator()(r, c));
        return this;
    }
    Matrix<T> operator-(Matrix<T> other) const {
        si maxRows, maxColumns;
        this->numberOfRows <= other.numberOfRows ? maxRows = other.numberOfRows : maxRows = this->numberOfRows;
        this->numberOfColumns <= other.numberOfColumns ?  maxColumns = other.numberOfRows : maxColumns = this->numberOfRows;
        Matrix<T> result(maxRows, maxColumns);
        for (si r = 0; r < maxRows; ++r)
            for (si c = 0; c < maxColumns; ++c)
                result.set(r, c, this->operator()(r, c) - other.operator()(r, c));
        return this;
    }
    Matrix<T> transpose() const {
        // RIP
        return this;
    }
    void print() const {
        /*cout << setw(50);
        for (si i = 1; i <= numberOfColumns; ++i)
            cout << setw(50) << "Column " << i;

        cout << endl;

        for (si i = 1; i <= numberOfRows; ++i) {
            cout << "Row " << i << endl;
        }*/
    }

    ~Matrix();
};

#endif //SPARSE_MATRIX_MATRIX_H