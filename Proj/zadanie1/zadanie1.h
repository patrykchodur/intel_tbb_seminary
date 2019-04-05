#ifndef zadanie1_h
#define zadanie1_h

#include <tbb/tbb.h>
#include <iostream>
#include <utility>

template <size_t size>
void fill (int arrayToFill[][size], tbb::blocked_range2d<size_t> range, int value = 1) {
    tbb::parallel_for(range, [&](const tbb::blocked_range2d<size_t>& r) {
        for (size_t iter1 = r.cols().begin() ; iter1 != r.cols().end(); iter1++) {
            for (size_t iter2 = r.rows().begin() ; iter2 != r.rows().end(); iter2++) {
                arrayToFill [iter1] [iter2] = (iter1*size + iter2)*value;
            }
        }
    });
}

template <size_t size>
int sum (int arrayToSum[][size], tbb::blocked_range2d<size_t> range) {
    return tbb::parallel_reduce(range, 0 ,[&](const tbb::blocked_range2d<size_t>& r, int init) {
        for (size_t iter1 = r.cols().begin() ; iter1 != r.cols().end(); iter1++) {
            for (size_t iter2 = r.rows().begin() ; iter2 != r.rows().end(); iter2++) {
                init += arrayToSum [iter1] [iter2];
            }
        }
        return init;
    }, [&](int val1, int val2) {
        return val1+val2;
    });
}

template <size_t size>
void square (int arrayToSquare[][size]) {
    int index= 0;
    tbb::filter_t<void, int> readFilter (tbb::filter::serial_in_order, [&](tbb::flow_control& control) {
        if (index == size*size) {
            control.stop();
            return 0;
        }
        index++;
        return arrayToSquare [(index-1)/size] [(index-1)%size];
    });
    tbb::filter_t<int, int> squareFilter (tbb::filter::parallel, [&](int& toSquare) {
        return toSquare*toSquare;
    });
    int index2 = 0;
    tbb::filter_t<int, void> saveInArray (tbb::filter::serial_in_order, [&](int& number) {
        arrayToSquare [index2/size] [index2%size] = number;
        index2++;
        return;
    });
    tbb::parallel_pipeline(10, readFilter & squareFilter & saveInArray);
}

#endif /* zadanie1_h */
