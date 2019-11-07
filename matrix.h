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
    vector<SourceRowNode<T>* > rowsNodes;
    vector<SourceColumnNode<T>* > columnsNodes;
    sui numberOfRows, numberOfColumns;

    MatrixNode<T>* findAndReturnNode(sui row, sui column) {
        if (row > numberOfRows or column > numberOfColumns)
            throw invalid_argument("Index out of range");

        MatrixNode<T>* currentNode = rowsNodes[row]->next;
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
            rowsNodes.push_back(new SourceRowNode<T>(i));

        for (sui i = 0; i < columns; ++i)
            columnsNodes.push_back(new SourceColumnNode<T>(i));
    }

    void set(sui row, sui column, T data) {
        if (row > numberOfRows or column > numberOfColumns)
            throw invalid_argument("Index out of range");

        if (!findAndReturnNode(row, column)) {
            auto nodeToInsert = new MatrixNode<T>(row, column, data);
            SourceRowNode<T>* sourceRowNode = rowsNodes[row];
            SourceColumnNode<T>* sourceColumnNode = columnsNodes[column];

            if (!sourceRowNode->next)
                sourceRowNode->next = nodeToInsert;
            else {
                MatrixNode<T>* currentNodeOfRow = sourceRowNode->next;
                if (column < currentNodeOfRow->column) {
                    sourceRowNode->next = nodeToInsert;
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
            if (!sourceColumnNode->down)
                sourceColumnNode->down = nodeToInsert;
            else {
                MatrixNode<T>* currentNodeOfColumn = sourceColumnNode->down;
                if (row < currentNodeOfColumn->row) {
                    sourceColumnNode->down = nodeToInsert;
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

        } else if (findAndReturnNode(row, column) and findAndReturnNode(row, column)->data != data) {
            MatrixNode<T>* node = findAndReturnNode(row, column);
            node->data = data;
        }
    }

    T operator()(sui row, sui column) const {
        if (row > numberOfRows or column > numberOfColumns)
            throw invalid_argument("Index out of range");

        MatrixNode<T>* currentNode = rowsNodes[row]->next;
        while (currentNode) {
            if (currentNode->column == column)
                return currentNode->data;
            currentNode = currentNode->next;
        }
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
        for (sui i = 0; i < numberOfColumns; ++i) {
            vector<T> numbersOfRow(numberOfColumns);
            for (sui j = 0; j < numberOfColumns; ++j)
                numbersOfRow[j] = this->operator()(i, j);
            for (sui j = 0; j < other.numberOfRows; ++j) {
                T answer = 0;
                vector<T> numbersOfColumn(other.numberOfRows);
                for (sui k = 0; k < other.numberOfRows; ++k)
                    numbersOfColumn[k] = other.operator()(k, j);
                for (sui k = 0; k < numberOfColumns; ++k)
                    answer += numbersOfRow[k] * numbersOfColumn[k];
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
        Matrix<T> result(numberOfRows, numberOfColumns);
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
                // ROWS != COLUMNS
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
        /*while(!rowsNodes->empty()) {
            if (rowsNodes->back()->next) {
                MatrixNode<T>* currentNode = rowsNodes->back()->next, *temp = nullptr;
                while (currentNode) {
                    temp = currentNode;
                    currentNode = currentNode->next;
                    delete temp;
                    temp = nullptr;
                }
            }
            SourceRowNode<T>* temp = rowsNodes->back();
            delete temp;
            temp = nullptr;
            rowsNodes->pop_back();
        }
        while(!columnsNodes->empty()) {
            if (columnsNodes->back()->down) {
                MatrixNode<T>* currentNode = columnsNodes->back()->down, *temp = nullptr;
                while (currentNode) {
                    temp = currentNode;
                    currentNode = currentNode->down;
                    delete temp;
                    temp = nullptr;
                }
            }
            SourceColumnNode<T>* temp = columnsNodes->back();
            delete temp;
            temp = nullptr;
            columnsNodes->pop_back();
        }*/
    };
};

#endif //SPARSE_MATRIX_MATRIX_H