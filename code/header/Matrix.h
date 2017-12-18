#ifndef KNNMPI_MATRIX_H
#define KNNMPI_MATRIX_H

typedef float T;

class Matrix {

private:
    const size_t rowCount, columnCount;
    std::string *rowIdentifier;

    bool validateArgument(size_t rowCount, size_t columnCount);

public:
    T *data;
    size_t getRowsCount();
    size_t getColumnsCount();
    T get(size_t rowCount, size_t columnCount);
    T *getRow(size_t rowCount);
    std::string getRowIdentifier(size_t rowCount);
    T *splitAt(size_t i);
    size_t size();
    void setRowIdentifier(size_t rowCount, std::string str);
    void set(size_t rowCount, size_t columnCount, T data);
    void print();

    Matrix(size_t rowCount, size_t columnCount);
    Matrix(size_t rowCount, size_t columnCount, T *data);

};



#endif //KNNMPI_MATRIX_H
