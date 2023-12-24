#include <iostream>
#include <omp.h>
#include <chrono>

int main() {
    int outer_sum = 0;

    double start_time = omp_get_wtime();

    #pragma omp parallel for reduction(+:outer_sum)
    for (int i = 0; i < 10000000; ++i) {
        outer_sum += 1;
    }

    double end_time = omp_get_wtime();
    double duration = end_time - start_time;

    std::cout << "Outer sum: " << outer_sum << std::endl;
    std::cout << "Time: " << duration << " seconds" << std::endl;

    return 0;
}
