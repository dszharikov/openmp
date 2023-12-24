#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <omp.h>


int main(int argc, char *argv[]) {
    if (argc != 2) {
        std::cerr << "Использование: " << argv[0] << " <размер вектора>" << std::endl;
        return 1;
    }

    const int n = atoi(argv[1]);

    if (n <= 0) {
        std::cerr << "Аргументы должны быть положительными числами." << std::endl;
        return 1;
    }

    
    const int N = n;
    std::vector<int> data(N);
    std::srand(std::time(0));

    // Инициализация вектора случайными числами
    for (int i = 0; i < N; ++i) {
        data[i] = std::rand() % 100;
    }

    double start_time, end_time;

    // Режим static
    start_time = omp_get_wtime();
    #pragma omp parallel for schedule(static)
    for (int i = 0; i < N; ++i) {
        // Неравномерная нагрузка: выполнение более сложных вычислений на некоторых итерациях
        if (i % 1000 == 0) {
            data[i] = data[i] * 2 + i;
        } else {
            data[i] = data[i] * 2;
        }
    }
    end_time = omp_get_wtime();
    std::cout << "Time (static): " << end_time - start_time << " seconds" << std::endl;

    // Режим dynamic
    start_time = omp_get_wtime();
    #pragma omp parallel for schedule(dynamic)
    for (int i = 0; i < N; ++i) {
        // Неравномерная нагрузка: выполнение более сложных вычислений на некоторых итерациях
        if (i % 1000 == 0) {
            data[i] = data[i] * 2 + i;
        } else {
            data[i] = data[i] * 2;
        }
    }
    end_time = omp_get_wtime();
    std::cout << "Time (dynamic): " << end_time - start_time << " seconds" << std::endl;

    // Режим guided
    start_time = omp_get_wtime();
    #pragma omp parallel for schedule(guided)
    for (int i = 0; i < N; ++i) {
        // Неравномерная нагрузка: выполнение более сложных вычислений на некоторых итерациях
        if (i % 1000 == 0) {
            data[i] = data[i] * 2 + i;
        } else {
            data[i] = data[i] * 2;
        }
    }
    end_time = omp_get_wtime();
    std::cout << "Time (guided): " << end_time - start_time << " seconds" << std::endl;

    return 0;
}
