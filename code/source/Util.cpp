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

    for (size_t i = 0; i < rowCount; i++) {
        for (size_t j = 0; j <= qnt_attr; j++) {
            string str = readNext(&file);
            //cout << i << "," << j << " read: " << str;
            if (j == qnt_attr) matrix->setRowIdentifier(i, str);
            else {
                try {
                    matrix->set(i, j, stof(str));
                } catch (invalid_argument e) {
                    //cout << "<- here";
                    cout << "Parsing error in " + path + " at line " << i << endl;
                    exit(0);
                }
            }
            //cout << endl;
        }
        //cout << endl;
    }

    file.close();

    return matrix;

}

T euclideanDistance(T *row1, T *row2, size_t row) {

    T sum = 0.0;
    for (int i = 0; i < row; i++)
        sum += pow(row1[i] - row2[i], 2);

    return sqrt(sum);

}
