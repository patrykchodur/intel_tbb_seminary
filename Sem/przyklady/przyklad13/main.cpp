#include <tbb/tbb.h>
#include <iostream>

int Fib(int n) {
    if( n<2 ) {
        return n;
    } else {
        int x, y;
        tbb::task_group g;
        g.run([&]{x=Fib(n-1);});
        g.run_and_wait([&]{y=Fib(n-2);});
        auto res = g.wait();
        if (res == tbb::task_group_status::complete)
            return x+y;
        return 0;
    }
}

int main(int argc, const char * argv[]) {
    
    std::cout << Fib(37);
    return 0;
}
