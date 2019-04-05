#include <tbb/tbb.h>
#include <iostream>
#include <ctime>
#include <utility>

#define N 40

auto fibonacciParalell (unsigned value) {
    if (value < 2)
        return value;
    unsigned x, y;
    tbb::task_group group;
    group.run([&]{x = fibonacciParalell(value-1);});
    group.run([&]{y = fibonacciParalell(value-2);});
    group.wait();
    return x+y;
}

auto fibonacci (unsigned value) {
    if (value < 2)
        return value;
    return fibonacci(value-1) + fibonacci(value-2);
}


int main(int argc, const char * argv[]) {
    tbb::task_scheduler_init init;
    unsigned result1, result2;
    std::clock_t start1, stop1, start2, stop2;
    
    start1 = std::clock();
    result1 = fibonacciParalell(N);
    stop1 = std::clock();
    std::cout << "result paralell = " << result1 << " time: " << stop1 - start1 << std::endl;
    
    start2 = std::clock();
    result2 = fibonacci(N);
    stop2 = std::clock();
    std::cout << "result2 = " << result2 << " time: " << stop2 - start2 << std::endl;
    
    
    
    return 0;
}
