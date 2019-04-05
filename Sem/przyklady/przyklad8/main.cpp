#include <tbb/tbb.h>
#include <list>
#include <iostream>

#define size 1000


int main(int argc, const char * argv[]) {
    std::list<int> myList;
    for (size_t iter = 0; iter < size; iter++) {
        myList.emplace_back(1);
    }
    auto fun1 = [](int& number) {
        number = number*2;
    };
    auto fun2 = [fun1](int& number, tbb::parallel_do_feeder<int>& feeder) {
        fun1(number);
        if (true)
            feeder.add(number);
    };
    tbb::parallel_do(myList.begin(), myList.end(), fun1);
    tbb::parallel_do(myList.begin(), myList.end(), fun2);
    for (auto& iter : myList) {
        std::cout << iter << ' ';
    }
    
    return 0;
}
