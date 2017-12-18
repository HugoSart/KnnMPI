#include <iostream>
#include <limits>
#include <mpi.h>
#include "../header/Util.h"

#define CENTRAL_PROCESS 0

#define TEST_ROW_COUNT 2248
#define TRAIN_ROW_COUNT 6746

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
    size_t qntAttr = 154;

    int id, size;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &id);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (id == CENTRAL_PROCESS)
        cout << "World size: " << size << "." << endl;
    cout << "Process " << id << " > Initialized." << endl;

    Matrix *mTest, *mTrain;
    mTest = parseFileToMatrix("/home/hugovs/CLionProjects/KnnMPI/input/test_154.data", TEST_ROW_COUNT, qntAttr);
    mTrain = parseFileToMatrix("/home/hugovs/CLionProjects/KnnMPI/input/train_154.data", TRAIN_ROW_COUNT, qntAttr);

    for (auto i = (size_t)id; i < TEST_ROW_COUNT; i += size) {
        cout << "Process " << id << " > Running knn_row for row " << i << "." << endl;
        size_t calc = knn_row(mTest, mTrain, qntAttr, i);
    }

    MPI_Barrier(MPI_COMM_WORLD);

    clock_t end = clock();
    auto elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
    cout << "Process " << id << " > Elapsed time: " << elapsed_secs << " seconds." << endl;

    MPI_Finalize();

    return EXIT_SUCCESS;

}