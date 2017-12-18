#include <iostream>
#include <cmath>
#include "../header/Util.h"

using namespace std;
string readNext(ifstream *file) {

    string buffer;

    while (true) {
        char c;
        file->get(c);
        if (c == ',' || c == '\n') break;
        buffer += c;
    }

    return buffer;

}

Matrix *parseFileToMatrix(string path, const size_t rowCount, const size_t qnt_attr) {

    ifstream file(path, ios::in);

    auto *matrix = new Matrix(rowCount, qnt_attr);

    for (size_t i = 0; i < rowCount; i++)
        for (size_t j = 0; j <= qnt_attr; j++)
            if (j == qnt_attr) matrix->setRowIdentifier(i, readNext(&file));
            else matrix->set(i, j, stof(readNext(&file)));

    file.close();

    return matrix;

}

T euclideanDistance(T *row1, T *row2, size_t row) {

    T sum = 0.0;
    for (int i = 0; i < row; i++)
        sum += pow(row1[i] - row2[i], 2);

    return sqrt(sum);

}