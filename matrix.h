#ifndef SPARSE_MATRIX_MATRIX_H
#define SPARSE_MATRIX_MATRIX_H

#include <stdexcept>
#include <vector>
#include "node.h"
#include <iomanip>
#include <list>

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

    void operationSumSetRowOfResultIfOneRowIsNullptr(Matrix<T> &result, sui i, const SourceNode<T> *currentRow) const {
        MatrixNode<T> *currentNode = currentRow->link;
        for (sui j = 0; j < numberOfColumns; ++j) {
            if (currentNode->column == j) {
                result.set(i, j, currentNode->data);
                currentNode = currentNode->next;
            }
        }
    }

    void operationSubtractionSetRowOfResultIfOneRowIsNullptr(Matrix<T> &result, sui i, const SourceNode<T> *currentRow, const int sign) const {
        MatrixNode<T> *currentNode = currentRow->link;
        for (sui j = 0; j < numberOfColumns; ++j) {
            if (currentNode->column == j) {
                result.set(i, j, currentNode->data * sign);
                currentNode = currentNode->next;
            }
        }
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

        bool dataEqualsZero = false;

        if (is_same<T, int>::value or is_same<T, float>::value)
            if (data == 0)
                dataEqualsZero = true;
        if (is_same<T, char>::value)
            if (data == '0')
                dataEqualsZero = true;

        if (findAndReturnNode(row, column) and dataEqualsZero) {
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

        if (findAndReturnNode(row, column) and findAndReturnNode(row, column)->data != data) {
            findAndReturnNode(row, column)->data = data;
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
    }

    /*const Matrix<T> operator=(Matrix<T> other) {
        Matrix<T> result(other.numberOfRows, other.numberOfColumns);
        for (sui i = 0; i < numberOfRows; ++i) {
            SourceNode<T>* currentRowOfOther = other.rowsNodes[i];
            if (currentRowOfOther->link) {
                for (sui j = 0; j < numberOfColumns; ++i) {
                    SourceNode<T>* currentNodeOfOther
                }
            }
        }
    }*/

    T operator()(sui row, sui column) const {
        MatrixNode<T>* node = findAndReturnNode(row, column);
        if (node)
            return node->data;
        else
            return 0;
    }

    bool operator==(Matrix<T> other) {
        if (numberOfRows != other.numberOfRows or numberOfColumns != other.numberOfColumns)
            return false;

        for (sui i = 0; i < numberOfRows; ++i) {
            SourceNode<T>* rowOfThis = rowsNodes[i], *rowOfOther = other.rowsNodes[i];

            if ((rowOfThis and !rowOfOther) or (!rowOfThis and rowOfOther))
                return false;

            MatrixNode<T>* currentNodeOfThis = rowOfThis->link, *currentNodeOfOther = rowOfOther->link;

            while (currentNodeOfThis and currentNodeOfOther) {
                if (currentNodeOfThis->data != currentNodeOfOther->data or
                currentNodeOfThis->column != currentNodeOfOther->column)
                    return false;
                currentNodeOfThis = currentNodeOfThis->next;
                currentNodeOfOther = currentNodeOfOther->next;
            }
        }
        return true;
    }

    const Matrix<T> operator*(T scalar) const {
        Matrix<T> result(numberOfRows, numberOfColumns);
        list<MatrixNode<T>* > previousRow;
        for (int i = numberOfRows - 1; i >= 0; --i) {
            if (rowsNodes[i]->link) {
                list<MatrixNode<T>* > currentRow;
                MatrixNode<T> *currentNode = rowsNodes[i]->link, *prevNode = nullptr;
                for (sui j = 0; j < numberOfColumns; ++j) {
                    if (currentNode) {
                        if (j == 0) {
                            result.rowsNodes[i]->link = new MatrixNode<T>(currentNode->row, currentNode->column,
                                    currentNode->data * scalar);
                            prevNode = result.rowsNodes[i]->link;
                            currentRow.push_back(prevNode);
                        } else {
                            prevNode->next = new MatrixNode<T>(currentNode->row, currentNode->column,
                                    currentNode->data * scalar);
                            prevNode = prevNode->next;
                            currentRow.push_back(prevNode);
                        }
                        if (i != numberOfRows - 1) {
                            if (prevNode->column == previousRow.front()->column) {
                                prevNode->down = previousRow.front();
                                previousRow.pop_front();
                            }
                        }
                        currentNode = currentNode->next;
                        previousRow = currentRow;
                    }
                }
            } else
                for (auto it = previousRow.begin(); it != previousRow.end(); ++it)
                    *it = nullptr;
        }

        return result;
    }

    // FALTA OPTIMIZAR
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
        for (sui i = 0; i < numberOfRows; ++i) {
            SourceNode<T> *currentRowOfThis = rowsNodes[i], *currentRowOfOther = other.rowsNodes[i];

            if (currentRowOfThis->link and !currentRowOfOther->link)
                operationSumSetRowOfResultIfOneRowIsNullptr(result, i, currentRowOfThis);
            if (!currentRowOfThis->link and currentRowOfOther->link)
                operationSumSetRowOfResultIfOneRowIsNullptr(result, i, currentRowOfOther);

            MatrixNode<T> *currentNodeOfThis = currentRowOfThis->link, *currentNodeOfOther = currentRowOfOther->link;
            for (sui j = 0; j < numberOfColumns; ++j) {
                if (currentNodeOfThis and currentNodeOfOther) {
                    if (currentNodeOfThis->column == j and currentNodeOfOther->column == j) {
                        result.set(i , j, currentNodeOfThis->data + currentNodeOfOther->data);
                        currentNodeOfThis = currentNodeOfThis->next;
                        currentNodeOfOther = currentNodeOfOther->next;
                    } else if (currentNodeOfThis->column == j and currentNodeOfOther->column != j) {
                        result.set(i , j, currentNodeOfThis->data);
                        currentNodeOfThis = currentNodeOfThis->next;
                    } else if (currentNodeOfThis->column != j and currentNodeOfOther->column == j) {
                        result.set(i, j, currentNodeOfOther->data);
                        currentNodeOfOther = currentNodeOfOther->next;
                    }
                }
                if (currentNodeOfThis and !currentNodeOfOther) {
                    if (currentNodeOfThis->column == j) {
                        result.set(i, j, currentNodeOfThis->data);
                        currentNodeOfThis = currentNodeOfThis->next;
                    }
                }
                if (!currentNodeOfThis and currentNodeOfOther) {
                    if (currentNodeOfOther->column == j) {
                        result.set(i, j, currentNodeOfOther->data);
                        currentNodeOfOther = currentNodeOfOther->next;
                    }
                }
                if (!currentNodeOfThis and !currentNodeOfOther)
                    break;
            }
        }
        return result;
    }

    const Matrix<T> operator-(Matrix<T> other) const {
        if (numberOfRows != other.numberOfRows or numberOfColumns != other.numberOfColumns)
            throw invalid_argument("This matrix's rows or/and columns are different from the other matrix");

        Matrix<T> result(numberOfRows, numberOfColumns);
        for (sui i = 0; i < numberOfRows; ++i) {
            SourceNode<T> *currentRowOfThis = rowsNodes[i], *currentRowOfOther = other.rowsNodes[i];

            if (currentRowOfThis->link and !currentRowOfOther->link)
                operationSubtractionSetRowOfResultIfOneRowIsNullptr(result, i, currentRowOfThis, 1);
            if (!currentRowOfThis->link and currentRowOfOther->link)
                operationSubtractionSetRowOfResultIfOneRowIsNullptr(result, i, currentRowOfOther, - 1);

            MatrixNode<T> *currentNodeOfThis = currentRowOfThis->link, *currentNodeOfOther = currentRowOfOther->link;
            for (sui j = 0; j < numberOfColumns; ++j) {
                if (currentNodeOfThis and currentNodeOfOther) {
                    if (currentNodeOfThis->column == j and currentNodeOfOther->column == j) {
                        result.set(i , j, currentNodeOfThis->data - currentNodeOfOther->data);
                        currentNodeOfThis = currentNodeOfThis->next;
                        currentNodeOfOther = currentNodeOfOther->next;
                    } else if (currentNodeOfThis->column == j and currentNodeOfOther->column != j) {
                        result.set(i , j, currentNodeOfThis->data);
                        currentNodeOfThis = currentNodeOfThis->next;
                    } else if (currentNodeOfThis->column != j and currentNodeOfOther->column == j) {
                        result.set(i, j, currentNodeOfOther->data);
                        currentNodeOfOther = currentNodeOfOther->next;
                    }
                }
                if (currentNodeOfThis and !currentNodeOfOther) {
                    if (currentNodeOfThis->column == j) {
                        result.set(i, j, currentNodeOfThis->data);
                        currentNodeOfThis = currentNodeOfThis->next;
                    }
                }
                if (!currentNodeOfThis and currentNodeOfOther) {
                    if (currentNodeOfOther->column == j) {
                        result.set(i, j, currentNodeOfOther->data * -1);
                        currentNodeOfOther = currentNodeOfOther->next;
                    }
                }
                if (!currentNodeOfThis and !currentNodeOfOther)
                    break;
            }
        }
        return result;
    }

    // FALTA OPTIMIZAR
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
            MatrixNode<T> *currentNode = rowsNodes[i]->link;
            if (currentNode) {
                for (sui j = 0; j < numberOfColumns; ++j) {
                    cout << setw(setWidth);
                    if (currentNode) {
                        if (currentNode->column == j) {
                            cout << currentNode->data;
                            currentNode = currentNode->next;
                        }
                        else
                            cout << 0;
                    } else
                        cout << 0;
                }
            } else
                for (sui j = 0; j < numberOfColumns; ++j)
                    cout << setw(setWidth) << 0;
            cout << endl;
        }
    }

    ~Matrix() {
        /*while (!rowsNodes.empty()) {
            MatrixNode<T>* currentNode = rowsNodes.back()->link;
            while (currentNode) {
                MatrixNode<T>* temp = currentNode;
                currentNode = currentNode->next;
                delete temp;
                temp = nullptr;
            }
            SourceNode<T>* temp = rowsNodes.back();
            rowsNodes.pop_back();
            delete temp;
            temp = nullptr;
        }
        while (!columnsNodes.empty()) {
            SourceNode<T>* temp = columnsNodes.back();
            columnsNodes.pop_back();
            delete temp;
            temp = nullptr;
        }*/
    }
};

#endif //SPARSE_MATRIX_MATRIX_H