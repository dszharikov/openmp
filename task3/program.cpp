#include <iostream>
#include <cmath>
#include <fstream>
#include <cstdlib>
#include <omp.h>

// Функция, для которой вычисляется интеграл
double function(double x) {
    // Здесь задайте вашу функцию
    return x * x;
}

// Генерация случайных данных и сохранение в файл
void generateAndSaveRandomData(const std::string& filename, double a, double b, int n) {
    std::ofstream file(filename);

    for (int i = 0; i < n; ++i) {
        double x = a + static_cast<double>(rand()) / RAND_MAX * (b - a);
        file << x << std::endl;
    }

    file.close();
}

// Загрузка данных из файла
std::vector<double> loadVector(const std::string& filename) {
    std::vector<double> vec;
    double value;
    std::ifstream file(filename);

    while (file >> value) {
        vec.push_back(value);
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
    // Задаем границы интегрирования, количество прямоугольников и число потоков
    double a = 0.0;
    double b = 3.0;
    const double epsilon = 1e-9;  // Погрешность

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

    // Генерация случайных данных и сохранение в файл
    const std::string vectorFileName = "random_first_vector.txt";
    const std::string resultFilename = "result.txt";

    if (!std::ifstream(vectorFileName)) {
        generateAndSaveRandomData(vectorFileName, a, b, n);
    }

    // Загрузка данных из файла
    std::vector<double> data = loadVector(vectorFileName);

    // Измерение времени выполнения с использованием OpenMP
    double start_time = omp_get_wtime();

    // Вычисляем интеграл методом прямоугольников
    double sum = 0.0;
    #pragma omp parallel for num_threads(num_threads) reduction(+:sum)
    for (int i = 0; i < n; ++i) {
        double x = data[i];
        sum += function(x) * (b - a) / n; // Площадь прямоугольника идет в сумму
    }

    double end_time = omp_get_wtime();
    double elapsed_time = end_time - start_time;

    // Сохранение результата в файл
    if (!std::ifstream(resultFilename)) {
        saveResult(resultFilename, sum);
    }

    // Загрузка предыдущего результата из файла и сравнение с текущим
    int previousResult = loadResult(resultFilename);
    if (std::abs(previousResult - sum) > epsilon) {
        std::cout << "Значение интеграла (предыдущий результат): " << previousResult << std::endl;
    }

    // Выводим результат
    std::cout << "Значение интеграла: " << sum << std::endl;
    std::cout << "Время выполнения: " << elapsed_time << " секунд" << std::endl;

    return 0;
}
