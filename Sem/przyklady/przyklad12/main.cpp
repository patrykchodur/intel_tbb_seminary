#include <tbb/tbb.h>
#include <iostream>

#define repetitions 10000

int main(int argc, const char * argv[]) {
    tbb::task_group group;
    tbb::atomic<int> x;
    x = 0;
    group.run([&](){
        for (size_t iter = 0; iter < repetitions; iter++)
            x.compare_and_swap(x+1, x);
    });
    group.run([&](){
        for (size_t iter = 0; iter < repetitions; iter++)
            x.compare_and_swap(-9999999, x+1);
    });
    group.wait();
    std::cout << "x = " << x;
    return 0;
}
