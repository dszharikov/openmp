#include <iostream>
#include <vector>
#include <climits>
#include <cstdlib>
#include <fstream>
#include "/opt/homebrew/opt/libomp/include/omp.h"

// Генерация случайных чисел и сохранение в файл
void generateAndSaveRandomData(const std::string& filename, int size) {
    std::ofstream file(filename);

    for (int i = 0; i < size; ++i) {
        file << std::rand() << std::endl;
    }

    file.close();
}

// Загрузка данных из файла
std::vector<int> loadRandomData(const std::string& filename, int size) {
    std::vector<int> vec(size);
    std::ifstream file(filename);

    for (int i = 0; i < size; ++i) {
        file >> vec[i];
    }

    file.close();
    return vec;
}

// Сохранение результата в файл
void saveResult(const std::string& filename, int result) {
    std::ofstream file(filename);
    file << result << std::endl;
    file.close();
}

// Загрузка результата из файла
int loadResult(const std::string& filename) {
    int result;
    std::ifstream file(filename);
    file >> result;
    file.close();
    return result;
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        std::cerr << "Использование: " << argv[0] << " <размер вектора> <потоки>" << std::endl;
        return 1;
    }

    const int size = atoi(argv[1]);
    const int num_threads = atoi(argv[2]);

    if (size <= 0 || num_threads <= 0) {
        std::cerr << "Аргументы должны быть положительными числами." << std::endl;
        return 1;
    }

    const std::string dataFilename = "random_data.txt";
    const std::string resultFilename = "result.txt";

    // Проверка, существуют ли данные, если нет - создание и сохранение
    if (!std::ifstream(dataFilename)) {
        generateAndSaveRandomData(dataFilename, size);
    }

    // Загрузка данных из файла
    std::vector<int> vec = loadRandomData(dataFilename, size);

    int min_val = INT_MAX;

    // Замер времени выполнения с использованием OpenMP
    double start_time = omp_get_wtime();

    #pragma omp parallel for reduction(min:min_val) num_threads(num_threads)
    for (int i = 0; i < size; ++i) {
        if (vec[i] < min_val) {
            min_val = vec[i];
        }
    }

    double end_time = omp_get_wtime();
    double elapsed_time = end_time - start_time;

    // Вывод результата времени выполнения
    std::cout << "Время выполнения: " << elapsed_time << " секунд" << std::endl;

    // Сохранение результата в файл
    if (!std::ifstream(resultFilename)) {
        saveResult(resultFilename, min_val);
    }

    // Загрузка предыдущего результата из файла и сравнение с текущим
    int previousResult = loadResult(resultFilename);
    if (previousResult != min_val)
    {
        std::cout << "Минимальное значение в векторе (предыдущий результат): " << previousResult << std::endl;
    }
    std::cout << "Минимальное значение в векторе (новый результат): " << min_val << std::endl;

    return 0;
}