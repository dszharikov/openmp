#include <iostream>
#include <vector>
#include <ctime>
#include <limits>
#include <omp.h>

// Структура для обычной матрицы
struct Matrix {
    std::vector<std::vector<int>> data;

    Matrix(int size) {
        data.resize(size, std::vector<int>(size, 0));
        for (int i = 0; i < size; ++i) {
            for (int j = 0; j < size; ++j) {
                data[i][j] = rand() % 100; // Заполняем матрицу случайными числами
            }
        }
    }

    int getElement(int i, int j) const {
        return data[i][j];
    }
};

// Структура для ленточной матрицы
struct BandMatrix {
    std::vector<std::vector<int>> data;
    int bandwidth;

    BandMatrix(int size, int band) : bandwidth(band) {
        data.resize(size, std::vector<int>(2 * band + 1, 0));
        for (int i = 0; i < size; ++i) {
            for (int j = 0; j < 2 * band + 1; ++j) {
                data[i][j] = rand() % 100; // Заполняем матрицу случайными числами
            }
        }
    }

    int getElement(int i, int j) const {
        int offset = j - i + bandwidth;
        if (offset < 0 || offset >= 2 * bandwidth + 1) {
            return 0; // Вне полосы
        }
        return data[i][offset];
    }
};

// Структура для треугольной матрицы
struct TriangularMatrix {
    std::vector<std::vector<int>> data;

    TriangularMatrix(int size) {
        data.resize(size, std::vector<int>(size, 0));
        for (int i = 0; i < size; ++i) {
            for (int j = 0; j <= i; ++j) {
                data[i][j] = rand() % 100; // Заполняем матрицу случайными числами
            }
        }
    }

    int getElement(int i, int j) const {
        return data[i][j];
    }
};

// Нахождение максимального значения среди минимальных элементов строк матрицы
int findMaxAmongMin(const Matrix& matrix, int size, int num_threads) {
    int maxAmongMin = std::numeric_limits<int>::min();

    #pragma omp parallel for num_threads(num_threads) shared(maxAmongMin)
    for (int i = 0; i < size; ++i) {
        int minInRow = std::numeric_limits<int>::max();
        for (int j = 0; j < size; ++j) {
            int element = matrix.getElement(i, j);
            if (element < minInRow) {
                minInRow = element;
            }
        }

        #pragma omp critical
        {
            if (minInRow > maxAmongMin) {
                maxAmongMin = minInRow;
            }
        }
    }

    return maxAmongMin;
}

// Нахождение максимального значения среди минимальных элементов строк матрицы
int findMaxAmongMin(const BandMatrix& matrix, int size, int num_threads) {
    int maxAmongMin = std::numeric_limits<int>::min();

    #pragma omp parallel for num_threads(num_threads) shared(maxAmongMin)
    for (int i = 0; i < size; ++i) {
        int minInRow = std::numeric_limits<int>::max();
        for (int j = 0; j < size; ++j) {
            int element = matrix.getElement(i, j);
            if (element < minInRow) {
                minInRow = element;
            }
        }

        #pragma omp critical
        {
            if (minInRow > maxAmongMin) {
                maxAmongMin = minInRow;
            }
        }
    }

    return maxAmongMin;
}

// Нахождение максимального значения среди минимальных элементов строк матрицы
int findMaxAmongMin(const TriangularMatrix& matrix, int size, int num_threads) {
    int maxAmongMin = std::numeric_limits<int>::min();

    #pragma omp parallel for num_threads(num_threads) shared(maxAmongMin)
    for (int i = 0; i < size; ++i) {
        int minInRow = std::numeric_limits<int>::max();
        for (int j = 0; j < size; ++j) {
            int element = matrix.getElement(i, j);
            if (element < minInRow) {
                minInRow = element;
            }
        }

        #pragma omp critical
        {
            if (minInRow > maxAmongMin) {
                maxAmongMin = minInRow;
            }
        }
    }

    return maxAmongMin;
}

int main(int argc, char *argv[]) {

    if (argc != 3) {
        std::cerr << "Использование: " << argv[0] << " <размер вектора> <потоки>" << std::endl;
        return 1;
    }

    const int n = atoi(argv[1]);
    const int num_threads = atoi(argv[2]);

    if (n <= 0 || num_threads <= 0) {
        std::cerr << "Аргументы должны быть положительными числами." << std::endl;
        return 1;
    }

    const int size = n;
    const int bandwidth = 100;

    // Обычная матрица
    Matrix matrix(size);

    // Измерение времени выполнения с использованием OpenMP
    double start_time = omp_get_wtime();
    int result = findMaxAmongMin(matrix, size, num_threads);
    double end_time = omp_get_wtime();
    double elapsed_time = end_time - start_time;
    std::cout << "Обычная матрица: " << result << ", Время выполнения: " << elapsed_time << " секунд" << std::endl;

    // Ленточная матрица
    BandMatrix bandMatrix(size, bandwidth);

    start_time = omp_get_wtime();
    result = findMaxAmongMin(bandMatrix, size, num_threads);
    end_time = omp_get_wtime();
    elapsed_time = end_time - start_time;
    std::cout << "Ленточная матрица: " << result << ", Время выполнения: " << elapsed_time << " секунд" << std::endl;

    // Треугольная матрица
    TriangularMatrix triangularMatrix(size);

    start_time = omp_get_wtime();
    result = findMaxAmongMin(triangularMatrix, size, num_threads);
    end_time = omp_get_wtime();
    elapsed_time = end_time - start_time;
    std::cout << "Треугольная матрица: " << result << ", Время выполнения: " << elapsed_time << " секунд" << std::endl;

    return 0;
}
