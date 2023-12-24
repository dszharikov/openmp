#include <iostream>
#include <vector>
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
std::vector<int> loadVector(const std::string& filename, int size) {
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

    const std::string firstVectorFileName = "random_first_vector.txt";
    const std::string secondVectorFileName = "random_second_vector.txt";
    const std::string resultFilename = "result.txt";

    // Проверка, существуют ли данные, если нет - создание и сохранение
    if (!std::ifstream(firstVectorFileName)) {
        generateAndSaveRandomData(firstVectorFileName, size);
        generateAndSaveRandomData(secondVectorFileName, size);
    }

    // Загрузка данных из файла
    std::vector<int> vec = loadVector(firstVectorFileName, size);
    std::vector<int> vec2 = loadVector(secondVectorFileName, size);

    int scalar_product = 0;

    // Замер времени выполнения с использованием OpenMP
    double start_time = omp_get_wtime();

    #pragma omp parallel for reduction(+:scalar_product) num_threads(num_threads)
    for (int i = 0; i < size; ++i) {
        scalar_product += vec[i] * vec2[i];
    }

    double end_time = omp_get_wtime();
    double elapsed_time = end_time - start_time;

    // Вывод результата времени выполнения
    std::cout << "Время выполнения: " << elapsed_time << " секунд" << std::endl;

    // Сохранение результата в файл
    if (!std::ifstream(resultFilename)) {
        saveResult(resultFilename, scalar_product);
    }

    // Загрузка предыдущего результата из файла и сравнение с текущим
    int previousResult = loadResult(resultFilename);
    if (previousResult != scalar_product) {
        std::cout << "Скалярное произведение (предыдущий результат): " << previousResult << std::endl;
    }
    std::cout << "Скалярное произведение (новый результат): " << scalar_product << std::endl;

    return 0;
}
