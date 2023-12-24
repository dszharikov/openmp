#!/bin/bash

# Скрипт для запуска программы с различными аргументами

# Компиляция программы
g++ -std=c++17 -Xpreprocessor -fopenmp -lomp -L/opt/homebrew/opt/libomp/lib -I/opt/homebrew/opt/libomp/include program.cpp -o program
g++ -std=c++17 -Xpreprocessor -fopenmp -lomp -L/opt/homebrew/opt/libomp/lib -I/opt/homebrew/opt/libomp/include program1.cpp -o program1

./program
./program1