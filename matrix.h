#ifndef SPARSE_MATRIX_MATRIX_H
#define SPARSE_MATRIX_MATRIX_H

#include <stdexcept>
#include <vector>
#include "node.h"
#include <iomanip>

typedef short unsigned int sui;
using namespace std;

template <typename T>
class Matrix {
private:
    vector<SourceNode<T>* > rowsNodes;
    vector<SourceNode<T>* > columnsNodes;
    sui numberOfRows, numberOfColumns;

    MatrixNode<T>* findAndReturnNode(sui row, sui column) const {
        if (row > numberOfRows or column > numberOfColumns)
            throw invalid_argument("Index out of range");

        MatrixNode<T>* currentNode = rowsNodes[row]->link;
        while (currentNode) {
            if (currentNode->column == column)
                return currentNode;
            currentNode = currentNode->next;
        }
        return nullptr;
    }

public:
    Matrix(sui rows, sui columns) : numberOfRows(rows), numberOfColumns(columns) {
        for (sui i = 0; i < rows; ++i)
            rowsNodes.push_back(new SourceNode<T>(i));

        for (sui i = 0; i < columns; ++i)
            columnsNodes.push_back(new SourceNode<T>(i));
    }

    void set(sui row, sui column, T data) {
        if (row > numberOfRows or column > numberOfColumns)
            throw invalid_argument("Index out of range");


        SourceNode<T>* sourceRowNode = rowsNodes[row];
        SourceNode<T>* sourceColumnNode = columnsNodes[column];

        if (findAndReturnNode(row, column) and data == 0) {
            MatrixNode<T>* nodeToDelete = findAndReturnNode(row, column),
            *onTheRightOfNodeToDelete = nodeToDelete->next,
            *belowNodeToDelete = nodeToDelete->down;
            if (sourceRowNode->link == nodeToDelete)
                sourceRowNode->link = onTheRightOfNodeToDelete;
            else {
                MatrixNode<T>* onTheLeftOfNodeToDelete = sourceRowNode->link;
                while (onTheLeftOfNodeToDelete->next != nodeToDelete)
                    onTheLeftOfNodeToDelete = onTheLeftOfNodeToDelete->next;
                onTheLeftOfNodeToDelete->next = onTheRightOfNodeToDelete;
            }
            if (sourceColumnNode->link == nodeToDelete)
                sourceColumnNode->link = belowNodeToDelete;
            else {
                MatrixNode<T>* aboveNodeToDelete = sourceColumnNode->link;
                while (aboveNodeToDelete->down != nodeToDelete)
                    aboveNodeToDelete = aboveNodeToDelete->down;
                aboveNodeToDelete->down = belowNodeToDelete;
            }
            delete nodeToDelete;
            nodeToDelete = nullptr;
            return;
        }

        if (!findAndReturnNode(row, column)) {
            auto nodeToInsert = new MatrixNode<T>(row, column, data);

            if (!sourceRowNode->link)
                sourceRowNode->link = nodeToInsert;
            else {
                MatrixNode<T>* currentNodeOfRow = sourceRowNode->link;
                if (column < currentNodeOfRow->column) {
                    sourceRowNode->link = nodeToInsert;
                    nodeToInsert->next = currentNodeOfRow;
                } else if (column > currentNodeOfRow->column) {
                    while (currentNodeOfRow->next and currentNodeOfRow->next->column < column)
                        currentNodeOfRow = currentNodeOfRow->next;
                    MatrixNode<T>* onTheLeftOfNodeToInsert = currentNodeOfRow,
                    *onTheRightOfNodeToInsert = currentNodeOfRow->next;
                    onTheLeftOfNodeToInsert->next = nodeToInsert;
                    nodeToInsert->next = onTheRightOfNodeToInsert;
                }
            }
            if (!sourceColumnNode->link)
                sourceColumnNode->link = nodeToInsert;
            else {
                MatrixNode<T>* currentNodeOfColumn = sourceColumnNode->link;
                if (row < currentNodeOfColumn->row) {
                    sourceColumnNode->link = nodeToInsert;
                    nodeToInsert->down = currentNodeOfColumn;
                } else if (row > currentNodeOfColumn->row) {
                    while (currentNodeOfColumn->down and currentNodeOfColumn->down->row < row)
                        currentNodeOfColumn = currentNodeOfColumn->down;
                    MatrixNode<T>* aboveNodeToInsert = currentNodeOfColumn,
                    *belowNodeToInsert = currentNodeOfColumn->down;
                    aboveNodeToInsert->down = nodeToInsert;
                    nodeToInsert->down = belowNodeToInsert;
                }
            }
            return;
        }

        if (findAndReturnNode(row, column) and findAndReturnNode(row, column)->data != data) {
            findAndReturnNode(row, column)->data = data;
            return;
        }

    }

    T operator()(sui row, sui column) const {
        MatrixNode<T>* node = findAndReturnNode(row, column);
        if (node)
            return node->data;
        else
            return 0;
    }

    bool operator==(Matrix<T> other) {
        for (sui r = 0; r < numberOfRows; ++r)
            for (sui c = 0; c < numberOfColumns; ++c)
                if (this->operator()(r, c) != other.operator()(r, c))
                    return false;
        return true;
    }

    const Matrix<T> operator*(T scalar) const {
        Matrix<T> result(numberOfRows, numberOfColumns);
        for (sui r = 0; r < numberOfRows; ++r)
            for (sui c = 0; c < numberOfColumns; ++c)
                result.set(r, c, this->operator()(r, c) * scalar);
        return result;
    }

    const Matrix<T> operator*(Matrix<T> other) const {
        if (numberOfColumns != other.numberOfRows)
            throw invalid_argument("It is not possible to multiply the given matrices");

        Matrix<T> result(numberOfRows, other.numberOfColumns);
        for (sui i = 0; i < numberOfRows; ++i) {
            for (sui j = 0; j < other.numberOfColumns; ++j) {
                T answer = 0;
                for (sui k = 0; k < numberOfColumns; ++k)
                    answer += this->operator()(i, k) * other.operator()(k, j);
                result.set(i, j, answer);
            }
        }
        return result;
    }

    const Matrix<T> operator+(Matrix<T> other) const {
        if (numberOfRows != other.numberOfRows or numberOfColumns != other.numberOfColumns)
            throw invalid_argument("This matrix's rows or/and columns are different from the other matrix");

        Matrix<T> result(numberOfRows, numberOfColumns);
        for (sui r = 0; r < numberOfRows; ++r)
            for (sui c = 0; c < numberOfColumns; ++c)
                result.set(r, c, this->operator()(r, c) + other.operator()(r, c));
        return result;
    }

    const Matrix<T> operator-(Matrix<T> other) const {
        if (numberOfRows != other.numberOfRows or numberOfColumns != other.numberOfColumns)
            throw invalid_argument("This matrix's rows or/and columns are different from the other matrix");

        Matrix<T> result(numberOfRows, numberOfColumns);
        for (sui r = 0; r < numberOfRows; ++r)
            for (sui c = 0; c < numberOfColumns; ++c)
                result.set(r, c, this->operator()(r, c) - other.operator()(r, c));
        return result;
    }
    
    const Matrix<T> transpose() const {
        Matrix<T> result(numberOfColumns, numberOfRows);
        if (numberOfRows > 1 or numberOfColumns > 1) {
            if (numberOfRows == numberOfColumns) {
                for (sui r = 0; r < numberOfRows; ++r) {
                    for (sui c = r; c < numberOfColumns; ++c) {
                        if (r == c) {
                            result.set(r, c, this->operator()(r, c));
                        } else {
                            T temp = this->operator()(r, c);
                            result.set(r, c, this->operator()(c, r));
                            result.set(c, r, temp);
                        }
                    }
                }
                return result;
            }
            else {
                for (sui i = 0; i < numberOfColumns; ++i)
                    for (sui j = 0; j < numberOfRows; ++j)
                        result.set(i, j, this->operator()(j, i));
                return result;
            }
        }
        return result;
    }

    void print() const {
        sui setWidth = 10;
        for (sui i = 0; i < numberOfColumns; ++i) {
            if (i == 0) cout << setw(setWidth) << "R \\ C";
            cout << setw(setWidth) << "Column " << i;
        }

        cout << endl;

        for (sui i = 0; i < numberOfRows; ++i) {
            cout << setw(setWidth) << "Row " << i;
            for (sui j = 0; j < numberOfColumns; ++j)
                cout << setw(setWidth) << this->operator()(i, j);
            cout << endl;
        }
    }

    ~Matrix() {
    };
};

#endif //SPARSE_MATRIX_MATRIX_H