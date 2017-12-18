#ifndef KNNMPI_UTIL_H
#define KNNMPI_UTIL_H

#include <array>
#include <fstream>
#include "Matrix.h"

Matrix *parseFileToMatrix(std::string path, size_t qnt_attr, size_t qnt_line);
T euclideanDistance(T *row1, T *row2, size_t row);

#endif //KNNMPI_UTIL_H
