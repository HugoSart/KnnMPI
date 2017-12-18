//
// Created by hugovs on 17/12/17.
//
#include <cstddef>
#include <stdexcept>
#include <utility>
#include <iostream>
#include "../header/Matrix.h"

using namespace std;
Matrix::Matrix(size_t rowCount, size_t columnCount) : rowCount(rowCount), columnCount(columnCount) {
    this->data = new T[rowCount * columnCount];
    this->rowIdentifier = new string[rowCount];
}

size_t Matrix::getRowsCount() {
    return this->rowCount;
}

size_t Matrix::getColumnsCount() {
    return this->columnCount;
}

T Matrix::get(size_t rowCount, size_t columnCount) {
    validateArgument(rowCount, columnCount);
    return this->data[this->columnCount * rowCount + columnCount];
}

T *Matrix::getRow(size_t row) {
    return data + row * columnCount;
}

void Matrix::set(size_t rowCount, size_t columnCount, T data) {
    validateArgument(rowCount, columnCount);
    this->data[this->columnCount * rowCount + columnCount] = data;
}

bool Matrix::validateArgument(size_t rowCount, size_t columnCount) {
    if ((rowCount < 0 || rowCount >= this->rowCount) || (columnCount < 0 || columnCount >= this->columnCount))
        throw std::invalid_argument("Expected m and n values in rage of matrix.");
    return true;
}

string Matrix::getRowIdentifier(size_t rowCount) {
    return rowIdentifier[rowCount];
}

void Matrix::setRowIdentifier(size_t rowCount, string str) {
    rowIdentifier[rowCount] = std::move(str);
}

void Matrix::print() {

    for (size_t i = 0; i < rowCount; i++) {
        cout << this->getRowIdentifier(i) << ": ";
        for (size_t j = 0; j < columnCount; j++) {
            cout << this->get(i, j) << " ";
        }
        cout << endl;
    }

}
