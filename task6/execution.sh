#!/bin/bash

# Скрипт для запуска программы с различными аргументами

# Компиляция программы
g++ -std=c++17 -Xpreprocessor -fopenmp -lomp -L/opt/homebrew/opt/libomp/lib -I/opt/homebrew/opt/libomp/include program.cpp -o program

for threads in 1 2 4 8; do
    for size in 1000000 2000000; do
        echo "Running with $threads threads and vector size $size"
        ./program $size $threads
    done
done
