#include <iostream>
#include <iomanip>
#include <locale>
#include <vector>
#include <cstdlib>
#include <fstream>
#include <limits>
#include <omp.h>

void generateAndSaveRandomData(const std::string& filename, int rows, int cols) {
    std::ofstream file(filename);

    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            file << std::rand() % 100 << " ";
        }
        file << std::endl;
    }

    file.close();
}

std::vector<std::vector<int>> loadMatrix(const std::string& filename, int rows, int cols) {
    std::vector<std::vector<int>> matrix(rows, std::vector<int>(cols, 0));
    std::ifstream file(filename);

    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            file >> matrix[i][j];
        }
    }

    file.close();
    return matrix;
}

int findMaxAmongMin(const std::vector<std::vector<int>>& matrix, int rows, int cols, int num_threads) {
    int maxAmongMin = std::numeric_limits<int>::min();

    #pragma omp parallel for num_threads(num_threads) shared(maxAmongMin)
    for (int i = 0; i < rows; ++i) {
        int minInRow = std::numeric_limits<int>::max();
        #pragma omp parallel for reduction(min : minInRow)
        for (int j = 0; j < cols; ++j) {
            if (matrix[i][j] < minInRow) {
                minInRow = matrix[i][j];
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

    int rows = n;
    int cols = n;

    const std::string matrixFileName = "random_matrix.txt";

    if (!std::ifstream(matrixFileName)) {
        generateAndSaveRandomData(matrixFileName, rows, cols);
    }

    std::vector<std::vector<int>> matrix = loadMatrix(matrixFileName, rows, cols);

    double start_time = omp_get_wtime();

    int result = findMaxAmongMin(matrix, rows, cols, num_threads);

    double end_time = omp_get_wtime();
    double elapsed_time = end_time - start_time;

    std::cout << "Максимальное значение среди минимальных элементов строк матрицы: " << result << std::endl;
    std::cout << "Время выполнения: " << std::fixed << elapsed_time << " секунд" << std::endl;

    return 0;
}