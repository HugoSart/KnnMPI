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
#define TEXT_YELLOW "\033[33m"
#define TEXT_YELLOW_BOLD "\033[1;33m"
#define TEXT_BLUE "\033[34m"

#define CENTRAL_PROCESS 0

#define TEST_ROW_COUNT 2248
#define TRAIN_ROW_COUNT 6746

#define DIVIDED_SIZE(size, qntAttr) ((TEST_ROW_COUNT / (size)) * (qntAttr))
#define REST_ROW_COUNT(size, qntAttr) (TEST_ROW_COUNT - (DIVIDED_SIZE(size, qntAttr) / qntAttr) * size)

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

    string base = argv[1];

    if (base != "59" && base != "161" && base != "256" && base != "1380" && base != "1601") {
        cout << "Invalid argument." << endl;
        return 0;
    }

    auto qntAttr = stoi(base);

    int id, size;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &id);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    cout << TEXT_YELLOW_BOLD << "Process " << id << " > " << TEXT_YELLOW << "Initialized." << endl;
    cout << TEXT_YELLOW_BOLD << "Process " << id << " > " << TEXT_YELLOW << "Loading files." << endl;

    // Carrega o arquivo de treino para uma matriz
    Matrix *mTrain = parseFileToMatrix("bases/train_" + base + ".data",
                                       TRAIN_ROW_COUNT, qntAttr);
    Matrix *mTest = nullptr, *mTestMain = nullptr;

    cout << TEXT_YELLOW_BOLD << "Process " << id << " > " << TEXT_YELLOW << "Train matrix created." << endl;

    T *sendv;
    auto *recv = new T[DIVIDED_SIZE(size, qntAttr)];

    // Se é o processo principal, arrega o arquivo de teste para a matriz
    if (id == CENTRAL_PROCESS) {
        mTestMain = mTest = parseFileToMatrix("bases/test_" + base + ".data",
                                              TEST_ROW_COUNT, qntAttr);
        cout << TEXT_YELLOW_BOLD << "Process " << id << " > " << TEXT_YELLOW << "Test file loaded." << endl;
        sendv = mTest->data;
    }

    // Envia o teste para os demais processos
    MPI_Scatter(sendv, DIVIDED_SIZE(size, qntAttr), MPI_FLOAT, recv,
                DIVIDED_SIZE(size, qntAttr), MPI_FLOAT,
                CENTRAL_PROCESS, MPI_COMM_WORLD);

    mTest = new Matrix(TEST_ROW_COUNT / size, qntAttr, recv);

    cout << TEXT_YELLOW_BOLD << "Process " << id << " > " << TEXT_YELLOW << "Test matrix created." << endl;

    int ret[DIVIDED_SIZE(size, qntAttr) / qntAttr];
    for (auto i = 0; i < DIVIDED_SIZE(size, qntAttr) / qntAttr; i++) {
        cout << TEXT_YELLOW_BOLD << "Process " << id << " > " << TEXT_BLUE << "Running knn_row for row " << i << " ("
             << id * (DIVIDED_SIZE(size, qntAttr) / qntAttr) + i << ")."
             << endl;
        ret[i] = knn_row(mTest, mTrain, qntAttr, i);
    }

    int *gret;
    if (id == CENTRAL_PROCESS)
        gret = new int[TEST_ROW_COUNT];

    MPI_Barrier(MPI_COMM_WORLD);

    MPI_Gather(ret, DIVIDED_SIZE(size, qntAttr) / qntAttr, MPI_INT,
               gret, DIVIDED_SIZE(size, qntAttr) / qntAttr, MPI_INT,
               CENTRAL_PROCESS, MPI_COMM_WORLD);

    MPI_Barrier(MPI_COMM_WORLD);

    // Calcula o que sobrou
    if (id == CENTRAL_PROCESS)
        for (int i = TEST_ROW_COUNT - REST_ROW_COUNT(size, qntAttr) - 1; i < TEST_ROW_COUNT; i++) {
            cout << "i: " << i << endl;
            gret[i] = knn_row(mTestMain, mTrain, qntAttr, i);
        }

    if (id == CENTRAL_PROCESS) {
        for (int i = 0; i < TEST_ROW_COUNT; i++) {
            cout << TEXT_YELLOW_BOLD << "Process " << id << " > " << TEXT_RESET;
            cout << "Result[" << i << "] -> " << mTrain->getRowIdentifier(gret[i]) << endl;
        }
    }

    delete mTest, mTestMain, mTrain, gret, recv;

    clock_t end = clock();
    auto elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;

    cout << TEXT_YELLOW_BOLD << "Process " << id << " > " << TEXT_RESET << "Elapsed time: " << elapsed_secs
         << " seconds." << endl;

    cout << TEXT_RESET;

    MPI_Finalize();

    return EXIT_SUCCESS;

}
