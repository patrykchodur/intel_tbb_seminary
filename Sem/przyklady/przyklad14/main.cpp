#include <tbb/tbb.h>
#include <iostream>

class Fibonacci : public tbb::task {
private:
    unsigned* result;
    int depth;
public:
    Fibonacci(unsigned* setResult, int setDepth) : result(setResult), depth(setDepth) {  }
    tbb::task* execute () {
        if (depth == 1 || depth == 2) {
            *result = 1;
            return NULL;
        }
        unsigned x, y;
        x = 0;
        y = 0;
        Fibonacci* a = new(allocate_child()) Fibonacci(&x, depth-1);
        Fibonacci* b = new(allocate_child()) Fibonacci(&y, depth-2);
        set_ref_count(3);
        spawn(*a);
        spawn(*b);
        wait_for_all();
        *result = x+y;
        return NULL;
    }
};


int main(int argc, const char * argv[]) {
    unsigned result;
    Fibonacci* task = new(tbb::task::allocate_root()) Fibonacci(&result, 40);
    tbb::task::spawn_root_and_wait(*task);
    std::cout << result;
    return 0;
}
