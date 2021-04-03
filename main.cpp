#include <iostream>
#include <chrono>

#define DIMENSION 20
typedef int Matrix[DIMENSION][DIMENSION];

int two_by_two_determinant(const Matrix& matrix) {
    return matrix[0][0] * matrix[1][1] - matrix[1][0] * matrix[0][1];
}

int minus_1_power_n(int n) {
    return n % 2 == 0 ? 1 : -1;
}

int determinantSerial(const Matrix& matrix, int dimension) {

    if(dimension == 2) {
        return two_by_two_determinant(matrix);
    }

    int determinant = 0;

    for(int i=0; i<dimension; i++) {
        Matrix smallMatrix;

        for(int k=0; k<dimension; k++) {
            for(int l=0; l<dimension; l++) {
                if(k != 0 && l != i) {
                    int newK = k-1;
                    int newL = l > i ? l-1 : l;
                    smallMatrix[newK][newL] = matrix[k][l];
                }
            }
        }

        determinant = determinant + (matrix[0][i] * minus_1_power_n(0 + i + 2) *
                                     determinantSerial(smallMatrix, dimension - 1));
    }

    return determinant;
}

int determinantParallel(const Matrix& matrix, int dimension, int threshold) {

    if(dimension == 2) {
        return two_by_two_determinant(matrix);
    }


    int determinant = 0;

    if (dimension >= threshold) {
        #pragma omp parallel
        #pragma omp single nowait
        {
            for(int i=0; i<dimension; i++) {
                #pragma omp task
                {
                    Matrix smallMatrix;

                    for(int k=0; k<dimension; k++) {
                        for(int l=0; l<dimension; l++) {
                            if(k != 0 && l != i) {
                                int newK = k-1;
                                int newL = l > i ? l-1 : l;
                                smallMatrix[newK][newL] = matrix[k][l];
                            }
                        }
                    }

                    int result = matrix[0][i] * minus_1_power_n(0 + i + 2) *
                                                 determinantSerial(smallMatrix, dimension - 1);

                    #pragma omp critical
                    determinant = determinant + result;
                }
            }
        }
    } else {
        for(int i=0; i<dimension; i++) {
            Matrix smallMatrix;

            for(int k=0; k<dimension; k++) {
                for(int l=0; l<dimension; l++) {
                    if(k != 0 && l != i) {
                        int newK = k-1;
                        int newL = l > i ? l-1 : l;
                        smallMatrix[newK][newL] = matrix[k][l];
                    }
                }
            }

            determinant = determinant + (matrix[0][i] * minus_1_power_n(0 + i + 2) *
                                         determinantSerial(smallMatrix, dimension - 1));
        }
    }

    return determinant;
}

void execSerial(const Matrix& matrix, int dimension) {
    auto t1 = std::chrono::high_resolution_clock::now();
    int determinant = determinantSerial(matrix, dimension);
    auto t2 = std::chrono::high_resolution_clock::now();
    printf("determinant serial = %d and took %d ms\n", determinant, std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count());
}

void execParallel(const Matrix& matrix, int dimension, int threshold) {
    auto t1 = std::chrono::high_resolution_clock::now();
    int determinant = determinantParallel(matrix, dimension, threshold);
    auto t2 = std::chrono::high_resolution_clock::now();
    printf("determinant parallel = %d and took %d ms\n", determinant, std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count());
}

int main() {
    int dimension = 11;
    int thresholds[] = {3, 4, 5, 6, 7, 8, 9, 10, 11};

    for(int threshold : thresholds) {
        std::cout << "threshold " << threshold << std::endl;

        Matrix matrix;

        for(int i = 0; i < dimension; i++) {
            for (int j = 0; j < dimension; j++) {
                matrix[i][j] = i*dimension + j + 1;
            }
        }

        // execSerial(matrix, dimension);
        execParallel(matrix, dimension, threshold);
    }

    return 0;
}