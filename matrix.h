#ifndef SPARSE_MATRIX_MATRIX_H
#define SPARSE_MATRIX_MATRIX_H

#include <stdexcept>
#include <vector>
#include "node.h"
#include <iomanip>
#include <list>
#include <map>

typedef short unsigned int sui;
using namespace std;

template <typename T>
class Matrix {
private:
    vector<SourceNode<T>* > rowsNodes;
    vector<SourceNode<T>* > columnsNodes;
    sui numberOfRows, numberOfColumns;

    // O(n)
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

    // O(n)
    void set(sui row, sui column, T data) {
        if (row > numberOfRows or column > numberOfColumns or row < 0 or column < 0)
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

    // O(n)
    T operator()(sui row, sui column) const {
        MatrixNode<T>* node = findAndReturnNode(row, column);
        if (node)
            return node->data;
        else
            return 0;
    }

    // O(n^2)
    bool operator==(Matrix<T> other) {
        if (numberOfRows != other.numberOfRows or numberOfColumns != other.numberOfColumns)
            return false;

        for (sui i = 0; i < numberOfRows; ++i) {
            SourceNode<T> *rowOfThis = rowsNodes[i], *rowOfOther = other.rowsNodes[i];

            if ((rowOfThis and !rowOfOther) or (!rowOfThis and rowOfOther))
                return false;

            MatrixNode<T> *currentNodeOfThis = rowOfThis->link, *currentNodeOfOther = rowOfOther->link;

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

    // O(n^2)
    const Matrix<T> operator*(T scalar) const {
        Matrix<T> result(numberOfRows, numberOfColumns);
        map<sui, MatrixNode<T>* > rowsBelow;
        for (int i = numberOfRows - 1; i >= 0; --i) {
            if (rowsNodes[i]->link) {
                list<MatrixNode<T>* > currentRow;
                MatrixNode<T> *currentNode = rowsNodes[i]->link, *currentNodeOfResult = nullptr;
                for (sui j = 0; j < numberOfColumns; ++j) {
                    if (currentNode) {
                        if (j == 0) {
                            result.rowsNodes[i]->link = new MatrixNode<T>(currentNode->row, currentNode->column,
                                    currentNode->data * scalar);
                            currentNodeOfResult = result.rowsNodes[i]->link;
                        } else {
                            currentNodeOfResult->next = new MatrixNode<T>(currentNode->row, currentNode->column,
                                    currentNode->data * scalar);
                            currentNodeOfResult = currentNodeOfResult->next;
                        }
                        if (rowsBelow[currentNodeOfResult->column]) {
                            currentNodeOfResult->down = rowsBelow[currentNodeOfResult->column];
                            rowsBelow[currentNodeOfResult->column] = currentNodeOfResult;
                        }
                        currentNode = currentNode->next;
                        rowsBelow[currentNodeOfResult->column] = currentNodeOfResult;
                    }
                }
            }
        }
        for (sui i = 0; i < numberOfColumns; ++i)
            if (rowsBelow[i])
                result.columnsNodes[i]->link = rowsBelow[i];
        return result;
    }

    // O(n^3)
    const Matrix<T> operator*(Matrix<T> other) const {
        if (numberOfColumns != other.numberOfRows)
            throw invalid_argument("It is not possible to multiply the given matrices");

        Matrix<T> result(numberOfRows, other.numberOfColumns);
        map<sui, MatrixNode<T>* > rowsBelow;
        for (int i = numberOfRows - 1; i >= 0; --i) {
            if (rowsNodes[i]->link) {
                MatrixNode<T> *currentNodeOfResult = nullptr;
                for (sui j = 0; j < result.numberOfColumns; ++j) {
                    if (other.columnsNodes[j]->link) {
                        unsigned int answer = 0;
                        MatrixNode<T> *currentNodeOfThis = rowsNodes[i]->link,
                        *currentNodeOfOther = other.columnsNodes[j]->link;
                        if (currentNodeOfThis and currentNodeOfOther) {
                            while (currentNodeOfThis and currentNodeOfOther) {
                                if (currentNodeOfThis->column == currentNodeOfOther->row) {
                                    answer += currentNodeOfThis->data * currentNodeOfOther->data;
                                    currentNodeOfThis = currentNodeOfThis->next;
                                    currentNodeOfOther = currentNodeOfOther->down;
                                    continue;
                                }
                                if (currentNodeOfThis->column < currentNodeOfOther->row)
                                    currentNodeOfThis = currentNodeOfThis->next;
                                if (currentNodeOfOther->row < currentNodeOfThis->column)
                                    currentNodeOfOther = currentNodeOfOther->down;
                            }
                            if (!result.rowsNodes[i]->link) {
                                result.rowsNodes[i]->link = new MatrixNode<T>(i, j, answer);
                                currentNodeOfResult = result.rowsNodes[i]->link;
                            } else {
                                currentNodeOfResult->next = new MatrixNode<T>(i, j, answer);
                                currentNodeOfResult = currentNodeOfResult->next;
                            }
                            if (rowsBelow[currentNodeOfResult->column])
                                currentNodeOfResult->down = rowsBelow[currentNodeOfResult->column];
                            rowsBelow[currentNodeOfResult->column] = currentNodeOfResult;
                        }
                    }
                }
            }
        }
        for (sui i = 0; i < result.numberOfColumns; ++i)
            if (rowsBelow[i])
                result.columnsNodes[i]->link = rowsBelow[i];
        return result;
    }

    // O(n^2)
    const Matrix<T> operator+(Matrix<T> other) const {
        if (numberOfRows != other.numberOfRows or numberOfColumns != other.numberOfColumns)
            throw invalid_argument("This matrix's rows or/and columns are different from the other matrix");

        Matrix<T> result(numberOfRows, numberOfColumns);
        map<sui, MatrixNode<T>* > rowsBelow;
        for (int i = numberOfRows - 1; i >= 0; --i) {
            MatrixNode<T> *currentNodeOfResult = nullptr;
            if (rowsNodes[i]->link and other.rowsNodes[i]->link) {
                MatrixNode<T> *currentNodeOfThis = rowsNodes[i]->link, *currentNodeOfOther = other.rowsNodes[i]->link;
                while(currentNodeOfThis and currentNodeOfOther) {
                    if (currentNodeOfThis->column == currentNodeOfOther->column) {
                        if (!result.rowsNodes[i]->link) {
                            result.rowsNodes[i]->link = new MatrixNode<T>(i, currentNodeOfThis->column, 
                                    currentNodeOfThis->data + currentNodeOfOther->data);
                            currentNodeOfResult = result.rowsNodes[i]->link;
                        } else {
                            currentNodeOfResult->next = new MatrixNode<T>(i, currentNodeOfThis->column,
                                    currentNodeOfThis->data + currentNodeOfOther->data);
                            currentNodeOfResult = currentNodeOfResult->next;
                        }
                        if (rowsBelow[currentNodeOfResult->column])
                            currentNodeOfResult->down = rowsBelow[currentNodeOfResult->column];
                        rowsBelow[currentNodeOfResult->column] = currentNodeOfResult;
                        currentNodeOfThis = currentNodeOfThis->next;
                        currentNodeOfOther = currentNodeOfOther->next;
                        continue;
                    }
                    if (currentNodeOfThis->column < currentNodeOfOther->column)
                        currentNodeOfThis = currentNodeOfThis->next;
                    if (currentNodeOfOther->column < currentNodeOfThis->column)
                        currentNodeOfOther = currentNodeOfOther->next;
                }
                
                
            }
            if (rowsNodes[i]->link and !other.rowsNodes[i]->link) {
                MatrixNode<T> *currentNodeOfThis = rowsNodes[i]->link;
                while (currentNodeOfThis) {
                    if (!result.rowsNodes[i]->link) {
                        result.rowsNodes[i]->link = new MatrixNode<T>(i, currentNodeOfThis->column, 
                                currentNodeOfThis->data);
                        currentNodeOfResult = result.rowsNodes[i]->link;
                    } else {
                        currentNodeOfResult->next = new MatrixNode<T>(i, currentNodeOfThis->column,
                                currentNodeOfThis->data);
                        currentNodeOfResult = currentNodeOfResult->next;
                    }
                    if (rowsBelow[currentNodeOfResult->column])
                        currentNodeOfResult->down = rowsBelow[currentNodeOfResult->column];
                    rowsBelow[currentNodeOfResult->column] = currentNodeOfResult;
                    currentNodeOfThis = currentNodeOfThis->next;
                }
            }
            if (!rowsNodes[i]->link and other.rowsNodes[i]->link) {
                MatrixNode<T> *currentNodeOfOther = other.rowsNodes[i]->link;
                while (currentNodeOfOther) {
                    if (!result.rowsNodes[i]->link) {
                        result.rowsNodes[i]->link = new MatrixNode<T>(i, currentNodeOfOther->column,
                                                                      currentNodeOfOther->data);
                        currentNodeOfResult = result.rowsNodes[i]->link;
                    } else {
                        currentNodeOfResult->next = new MatrixNode<T>(i, currentNodeOfOther->column,
                                                                      currentNodeOfOther->data);
                        currentNodeOfResult = currentNodeOfResult->next;
                    }
                    if (rowsBelow[currentNodeOfResult->column])
                        currentNodeOfResult->down = rowsBelow[currentNodeOfResult->column];
                    rowsBelow[currentNodeOfResult->column] = currentNodeOfResult;
                    currentNodeOfOther = currentNodeOfOther->next;
                }
            }
        }
        for (sui i = 0; i < result.numberOfColumns; ++i)
            if (rowsBelow[i])
                result.columnsNodes[i]->link = rowsBelow[i];
        return result;
    }

    // O(n^2)
    const Matrix<T> operator-(Matrix<T> other) const {
        if (numberOfRows != other.numberOfRows or numberOfColumns != other.numberOfColumns)
            throw invalid_argument("This matrix's rows or/and columns are different from the other matrix");

        Matrix<T> result(numberOfRows, numberOfColumns);
        map<sui, MatrixNode<T>* > rowsBelow;
        for (int i = numberOfRows - 1; i >= 0; --i) {
            MatrixNode<T> *currentNodeOfResult = nullptr;
            if (rowsNodes[i]->link and other.rowsNodes[i]->link) {
                MatrixNode<T> *currentNodeOfThis = rowsNodes[i]->link, *currentNodeOfOther = other.rowsNodes[i]->link;
                while(currentNodeOfThis and currentNodeOfOther) {
                    if (currentNodeOfThis->column == currentNodeOfOther->column) {
                        if (!result.rowsNodes[i]->link) {
                            result.rowsNodes[i]->link = new MatrixNode<T>(i, currentNodeOfThis->column,
                                    currentNodeOfThis->data - currentNodeOfOther->data);
                            currentNodeOfResult = result.rowsNodes[i]->link;
                        } else {
                            currentNodeOfResult->next = new MatrixNode<T>(i, currentNodeOfThis->column,
                                    currentNodeOfThis->data - currentNodeOfOther->data);
                            currentNodeOfResult = currentNodeOfResult->next;
                        }
                        if (rowsBelow[currentNodeOfResult->column])
                            currentNodeOfResult->down = rowsBelow[currentNodeOfResult->column];
                        rowsBelow[currentNodeOfResult->column] = currentNodeOfResult;
                        currentNodeOfThis = currentNodeOfThis->next;
                        currentNodeOfOther = currentNodeOfOther->next;
                        continue;
                    }
                    if (currentNodeOfThis->column < currentNodeOfOther->column)
                        currentNodeOfThis = currentNodeOfThis->next;
                    if (currentNodeOfOther->column < currentNodeOfThis->column)
                        currentNodeOfOther = currentNodeOfOther->next;
                }


            }
            if (rowsNodes[i]->link and !other.rowsNodes[i]->link) {
                MatrixNode<T> *currentNodeOfThis = rowsNodes[i]->link;
                while (currentNodeOfThis) {
                    if (!result.rowsNodes[i]->link) {
                        result.rowsNodes[i]->link = new MatrixNode<T>(i, currentNodeOfThis->column,
                                                                      currentNodeOfThis->data);
                        currentNodeOfResult = result.rowsNodes[i]->link;
                    } else {
                        currentNodeOfResult->next = new MatrixNode<T>(i, currentNodeOfThis->column,
                                                                      currentNodeOfThis->data);
                        currentNodeOfResult = currentNodeOfResult->next;
                    }
                    if (rowsBelow[currentNodeOfResult->column])
                        currentNodeOfResult->down = rowsBelow[currentNodeOfResult->column];
                    rowsBelow[currentNodeOfResult->column] = currentNodeOfResult;
                    currentNodeOfThis = currentNodeOfThis->next;
                }
            }
            if (!rowsNodes[i]->link and other.rowsNodes[i]->link) {
                MatrixNode<T> *currentNodeOfOther = other.rowsNodes[i]->link;
                while (currentNodeOfOther) {
                    if (!result.rowsNodes[i]->link) {
                        result.rowsNodes[i]->link = new MatrixNode<T>(i, currentNodeOfOther->column,
                                                                      currentNodeOfOther->data * -1);
                        currentNodeOfResult = result.rowsNodes[i]->link;
                    } else {
                        currentNodeOfResult->next = new MatrixNode<T>(i, currentNodeOfOther->column,
                                                                      currentNodeOfOther->data * -1);
                        currentNodeOfResult = currentNodeOfResult->next;
                    }
                    if (rowsBelow[currentNodeOfResult->column])
                        currentNodeOfResult->down = rowsBelow[currentNodeOfResult->column];
                    rowsBelow[currentNodeOfResult->column] = currentNodeOfResult;
                    currentNodeOfOther = currentNodeOfOther->next;
                }
            }
        }
        for (sui i = 0; i < result.numberOfColumns; ++i)
            if (rowsBelow[i])
                result.columnsNodes[i]->link = rowsBelow[i];
        return result;
        return result;
    }

    // O(n^2)
    const Matrix<T> transpose() const {
        Matrix<T> result(numberOfColumns, numberOfRows);
        map<sui, MatrixNode<T>* > columnOntTheRightOfCurrentColumn;
        for (int i = numberOfRows - 1; i >= 0; --i) {
            if (rowsNodes[i]->link) {
                MatrixNode<T> *currentNodeOfThis = rowsNodes[i]->link, *currentNodeOfResult = nullptr;
                result.columnsNodes[i]->link = new MatrixNode<T>(currentNodeOfThis->column, currentNodeOfThis->row,
                        currentNodeOfThis->data);
                currentNodeOfThis = currentNodeOfThis->next;
                currentNodeOfResult = result.columnsNodes[i]->link;
                if (columnOntTheRightOfCurrentColumn[currentNodeOfResult->row])
                    currentNodeOfResult->next = columnOntTheRightOfCurrentColumn[currentNodeOfResult->row];
                columnOntTheRightOfCurrentColumn[currentNodeOfResult->row] = currentNodeOfResult;
                while (currentNodeOfThis) {
                    currentNodeOfResult->down = new MatrixNode<T>(currentNodeOfThis->column, currentNodeOfThis->row,
                            currentNodeOfThis->data);
                    currentNodeOfResult = currentNodeOfResult->down;
                    currentNodeOfThis = currentNodeOfThis->next;
                    if (columnOntTheRightOfCurrentColumn[currentNodeOfResult->row])
                        currentNodeOfResult->next = columnOntTheRightOfCurrentColumn[currentNodeOfResult->row];
                    columnOntTheRightOfCurrentColumn[currentNodeOfResult->row] = currentNodeOfResult;
                }
            }
        }
        for (sui i = 0; i < result.numberOfRows; ++i)
            if (columnOntTheRightOfCurrentColumn[i])
                result.rowsNodes[i]->link = columnOntTheRightOfCurrentColumn[i];
        return result;
    }

    // O(n^2)
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