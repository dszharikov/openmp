#!/bin/bash

# Скрипт для запуска программы с различными аргументами

# Компиляция программы
g++ -std=c++17 -Xpreprocessor -fopenmp -lomp -L/opt/homebrew/opt/libomp/lib -I/opt/homebrew/opt/libomp/include bandmatrix.cpp -o bandmatrix

for threads in 1 2 4 8; do
    for size in 1000 2000 3000 4000; do
        echo "Running with $threads threads and vector size $size"
        ./bandmatrix $size $threads
    done
done
