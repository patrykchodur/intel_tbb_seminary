#include "tbb/tbb.h"
#include <vector>
#include <iostream>

#define size 1000

int main() {
    std::vector<int> myVector;
    myVector.resize(size);
    auto fun1 = [&myVector] (const tbb::blocked_range<size_t>& range) {
        for( size_t iter=range.begin(); iter!=range.end(); iter++ ) {
            myVector.at(iter) += 1;
        }
    };
    
    
    
    auto fun2 = [&myVector] (const tbb::blocked_range<size_t>& range) {
        for( size_t iter=range.begin(); iter!=range.end(); iter++ ) {
            if (myVector.at(iter) == 4) {
                tbb::task::self().cancel_group_execution();
            }
            myVector.at(iter) += 1;
        }
    };
    try {
        tbb::parallel_for( tbb::blocked_range<size_t>(0, 2*size, size*2), fun1);
    } catch( tbb::captured_exception& ex ) {
        std::cout << "captured_exception: " << ex.what() << std::endl;
    } catch( std::out_of_range& ex ) {
        std::cout << "out_of_range: " << ex.what() << std::endl;
    }
    for (auto iter : myVector) {
        std::cout << iter << ' ';
    }
    std::cout << std::endl << std::endl;
    myVector[50] = 4;
    tbb::parallel_for( tbb::blocked_range<size_t>(0, size, 1), fun2, tbb::simple_partitioner());
    for (auto& iter : myVector) {
        std::cout << iter << ' ';
    }
    
    return 0;
}
