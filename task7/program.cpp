#include <iostream>
#include <fstream>
#include <vector>
#include <omp.h>


int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <size>" << std::endl;
        return 1;
    }

    const int n = atoi(argv[1]);

    if (n <= 0) {
        std::cerr << "Аргументы должны быть положительными числами." << std::endl;
        return 1;
    }

    const int size = n;

    std::vector<int> vector_data(size);

    // Генерация случайного вектора и сохранение в файл
    std::ofstream file("vector.txt");
    for (int i = 0; i < size; ++i) {
        vector_data[i] = rand() % 100;
        file << vector_data[i] << "\n";
    }
    file.close();

    int dotProduct = 0;

    // Замер времени выполнения
    double start_time = omp_get_wtime();

    // Параллельное считывание из файла и перемножение векторов
    #pragma omp parallel sections num_threads(2) shared(dotProduct, vector_data)
    {
        // Секция 1: Считывание из файла
        #pragma omp section
        {
            std::ifstream infile("vector.txt");
            int value;
            // #pragma omp for
            for (int i = 0; i < size; ++i) {
                infile >> value;
                vector_data[i] = value;
            }
            infile.close();
        }

        // Секция 2: Перемножение векторов
        #pragma omp section
        {
            // #pragma omp for reduction(+:dotProduct)
            for (int i = 0; i < size; ++i) {
                dotProduct += vector_data[i] * vector_data[i];
            }
        }
    }

    double end_time = omp_get_wtime();
    std::cout << "Result (dot product): " << dotProduct << std::endl;
    std::cout << "Time: " << end_time - start_time << " seconds" << std::endl;

    return 0;
}