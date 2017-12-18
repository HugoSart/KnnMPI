#include <iostream>
#include <limits>
#include <mpi.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/sysinfo.h>
#include <zconf.h>
#include "../header/Util.h"

#define TEXT_RESET "\33[0m"
#define TEXT_WHITE "\33[47m"
#define TEXT_WHITE_BOLD "\33[1;47m"
#define TEXT_RED "\033[1;31m"
#define TEXT_RED_BOLD "\033[31m"
#define TEXT_YELLOW "\033[33m"
#define TEXT_YELLOW_BOLD "\033[1;33m"
#define TEXT_BLUE "\033[34m"
#define TEXT_BLUE_BOLD "\033[1;34m"

#define CENTRAL_PROCESS 0

#define TEST_ROW_COUNT 2248
#define TRAIN_ROW_COUNT 6746

#define DIVIDED_SIZE(size, qntAttr) ((TEST_ROW_COUNT / (size)) * (qntAttr))

using namespace std;

size_t knn_row(Matrix *mTest, Matrix *mTrain, size_t qntAttr, size_t row) {

    T num = numeric_limits<T>::max();

    size_t r;
    for (size_t i = 0; i < TRAIN_ROW_COUNT; i++) {
        T calc = euclideanDistance(mTest->getRow(row), mTrain->getRow(i), qntAttr);
        if (calc < num) {
            num = calc;
            r = i;
        }
    }

    return r;

}

int main(int argc, char *argv[]) {

    clock_t begin = clock();
    size_t qntAttr = 59;

    int id, size;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &id);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    cout << TEXT_YELLOW_BOLD << "Process " << id << " > " << TEXT_YELLOW << "Initialized." << endl;

    cout << TEXT_YELLOW_BOLD << "Process " << id << " > " << TEXT_YELLOW << "Loading files." << endl;
    Matrix *mTest = nullptr, *mTrain = parseFileToMatrix("/home/hugovs/CLionProjects/KnnMPI/input/train_59.data", TRAIN_ROW_COUNT, qntAttr);
    cout << TEXT_YELLOW_BOLD << "Process " << id << " > " << TEXT_YELLOW << "Train file loaded." << endl;

    T *sendv;
    auto *recv = new T[TEST_ROW_COUNT / size];

    if (id == CENTRAL_PROCESS) {
        mTest = parseFileToMatrix("/home/hugovs/CLionProjects/KnnMPI/input/test_59.data", TEST_ROW_COUNT, qntAttr);
        cout << TEXT_YELLOW_BOLD << "Process " << id << " > " << TEXT_YELLOW << "Test file loaded." << endl;
        sendv = mTest->data;
    }

    cout << static_cast<int>(DIVIDED_SIZE(size, qntAttr));
    MPI_Scatter(sendv, static_cast<int>(DIVIDED_SIZE(size, qntAttr)), MPI_FLOAT, recv,
                static_cast<int>(DIVIDED_SIZE(size, qntAttr)), MPI_FLOAT,
                CENTRAL_PROCESS, MPI_COMM_WORLD);

    /*mTest = new Matrix(static_cast<size_t>(TEST_ROW_COUNT / size), qntAttr, recv);
    cout << mTest->getRowsCount() << " " << mTest->getColumnsCount() << endl;

    if (id == CENTRAL_PROCESS) mTest->print();*/


    /*MPI_Barrier(MPI_COMM_WORLD);

    for (auto i = (size_t) id; i < TEST_ROW_COUNT; i += size) {
        cout << TEXT_YELLOW_BOLD << "Process " << id << " > " << TEXT_BLUE << "Running knn_row for row " << i << "."
             << endl;
        size_t calc = knn_row(mTest, mTrain, qntAttr, i);
    }

    MPI_Barrier(MPI_COMM_WORLD);

    clock_t end = clock();
    auto elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
    cout << TEXT_YELLOW_BOLD << "Process " << id << " > " << TEXT_RESET << "Elapsed time: " << elapsed_secs
         << " seconds." << endl;

    cout << TEXT_RESET;*/

    MPI_Finalize();

    return EXIT_SUCCESS;

}