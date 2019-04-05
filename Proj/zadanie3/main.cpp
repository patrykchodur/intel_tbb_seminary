//  Zadanie polega na napisaniu algorytmu quicksort w oparciu o wielozadaniowosc.
//  Kazde rekurencyjne wykonanie powinno tworzyc 2 nowe klasy sort alokowane
//  jako zadania tak jak ma to miejsce w mainie. Funktory maja kozystac z
//  tbb::blocked_range, a gdy blocked_range nie jest podzielny (is_divisible()) powinny
//  sortowac iteracyjnie jakims prostym algorytmem typu bubble sort lub insert sort
//  pliku main.cpp nie mozna modyfikowac
#include "zadanie3.h"
#include <tbb/tbb.h>
#include <iostream>
#include <random>
#include <vector>

#define size 200

int main(int argc, const char * argv[]) {
    std::vector<int> myVector;
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distibution(0,99);
    myVector.resize(size);
    for (int& iter : myVector) {
        iter = distibution(gen);
    }
    for (auto& iter : myVector) {
        std::cout << iter << ' ';
    }
    mySort* sort = new (tbb::task::allocate_root()) mySort(&myVector, tbb::blocked_range<int> (0, size, 20));
    tbb::task::spawn_root_and_wait(*sort);
    std::cout << std::endl << std::endl;
    for (auto& iter : myVector) {
        std::cout << iter << ' ';
    }
    return 0;
}

/*
 przykladowy output:
 62 72 43 2 67 63 71 88 89 23 57 98 84 43 55 82 20 34 90 42 19 59 75 19 29 52 12 38 1 67 14 15 28 13 11 22 25 81 12 14 82 97 70 94 93 15 14 72 50 24 29 30 30 73 44 80 69 3 58 27 59 13 76 49 94 4 62 73 43 18 28 24 54 83 5 65 66 25 32 62 26 33 27 43 30 71 47 65 53 6 68 26 14 31 89 41 76 1 76 94 0 72 77 43 87 37 1 28 51 71 43 47 37 93 42 68 41 1 21 35 90 84 84 59 83 95 2 79 57 59 84 15 84 72 96 40 13 15 33 21 54 80 7 19 19 62 3 50 82 90 74 72 58 75 17 65 22 43 84 77 90 85 44 74 6 70 0 20 15 6 55 49 29 17 13 98 93 9 19 55 13 71 24 7 63 30 3 91 12 88 84 77 26 90 16 50 89 9 44 17
 
 0 0 1 1 1 1 2 2 3 3 3 4 5 6 6 6 7 7 9 9 11 12 12 12 13 13 13 13 13 14 14 14 14 15 15 15 15 15 16 17 17 17 18 19 19 19 19 19 20 20 21 21 22 22 23 24 24 24 25 25 26 26 26 27 27 28 28 28 29 29 29 30 30 30 30 31 32 33 33 34 35 37 37 38 40 41 41 42 42 43 43 43 43 43 43 43 44 44 44 47 47 49 49 50 50 50 51 52 53 54 54 55 55 55 57 57 58 58 59 59 59 59 62 62 62 62 63 63 65 65 65 66 67 67 68 68 69 70 70 71 71 71 71 72 72 72 72 72 73 73 74 74 75 75 76 76 76 77 77 77 79 80 80 81 82 82 82 83 83 84 84 84 84 84 84 84 85 87 88 88 89 89 89 90 90 90 90 90 91 93 93 93 94 94 94 95 96 97 98 98
 */
