#include <iostream>
#include <omp.h>

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

    int data[size];
    int result_atomic = 0;
    int result_critical = 0;
    int result_lock = 0;
    int result_reduction = 0;

    // Заполняем массив случайными числами
    for (int i = 0; i < size; ++i) {
        data[i] = rand() % 100;
    }

    // Используем атомарные операции для редукции
    double start_time = omp_get_wtime();
    #pragma omp parallel for num_threads(num_threads) reduction(+:result_atomic)
    for (int i = 0; i < size; ++i) {
        result_atomic += data[i];
    }
    double end_time = omp_get_wtime();
    std::cout << "Result (atomic): " << result_atomic << ", Time: " << end_time - start_time << " seconds" << std::endl;

    // Используем критические секции для редукции
    start_time = omp_get_wtime();
    #pragma omp parallel num_threads(num_threads)
    {
        int local_result = 0;

        #pragma omp for
        for (int i = 0; i < size; ++i) {
            local_result += data[i];
        }

        #pragma omp critical
        result_critical += local_result;
    }
    end_time = omp_get_wtime();
    std::cout << "Result (critical): " << result_critical << ", Time: " << end_time - start_time << " seconds" << std::endl;

    // Используем синхронизацию при помощи замков для редукции
    start_time = omp_get_wtime();
    omp_lock_t lock;
    omp_init_lock(&lock);

    #pragma omp parallel for num_threads(num_threads)
    for (int i = 0; i < size; ++i) {
        omp_set_lock(&lock);
        result_lock += data[i];
        omp_unset_lock(&lock);
    }

    omp_destroy_lock(&lock);
    end_time = omp_get_wtime();
    std::cout << "Result (lock): " << result_lock << ", Time: " << end_time - start_time << " seconds" << std::endl;

    // Используем директиву reduction для редукции
    start_time = omp_get_wtime();
    #pragma omp parallel for num_threads(num_threads) reduction(+:result_reduction)
    for (int i = 0; i < size; ++i) {
        result_reduction += data[i];
    }
    end_time = omp_get_wtime();
    std::cout << "Result (reduction): " << result_reduction << ", Time: " << end_time - start_time << " seconds" << std::endl;

    return 0;
}
