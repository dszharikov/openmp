#include <iostream>
#include <iomanip> // Добавляем заголовочный файл для std::setlocale
#include <locale>  // Добавляем заголовочный файл для std::locale
#include <vector>
#include <cstdlib>
#include <fstream>
#include <limits>
#include <omp.h>

// Генерация случайных данных и сохранение в файл
void generateAndSaveRandomData(const std::string& filename, int rows, int cols) {
    std::ofstream file(filename);

    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            file << std::rand() % 100 << " "; // Заполняем матрицу случайными числами
        }
        file << std::endl;
    }

    file.close();
}

// Загрузка матрицы из файла
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

// Нахождение максимального значения среди минимальных элементов строк матрицы
int findMaxAmongMin(const std::vector<std::vector<int>>& matrix, int rows, int cols, int num_threads) {
    int maxAmongMin = std::numeric_limits<int>::min();

    #pragma omp parallel for num_threads(num_threads) shared(maxAmongMin)
    for (int i = 0; i < rows; ++i) {
        int minInRow = std::numeric_limits<int>::max();
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

    // Генерация случайных данных и сохранение в файл
    const std::string matrixFileName = "random_matrix.txt";

    if (!std::ifstream(matrixFileName)) {
        generateAndSaveRandomData(matrixFileName, rows, cols);
    }

    

    // Загрузка данных из файла
    std::vector<std::vector<int>> matrix = loadMatrix(matrixFileName, rows, cols);

    // Измерение времени выполнения с использованием OpenMP
    double start_time = omp_get_wtime();

    // Находим максимальное значение среди минимальных элементов строк матрицы
    int result = findMaxAmongMin(matrix, rows, cols, num_threads);

    double end_time = omp_get_wtime();
    double elapsed_time = end_time - start_time;

    // Выводим результат
    std::cout << "Максимальное значение среди минимальных элементов строк матрицы: " << result << std::endl;
    std::cout << "Время выполнения: " << std::fixed << elapsed_time << " секунд" << std::endl;

    return 0;
}