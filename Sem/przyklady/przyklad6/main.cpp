#include <tbb/tbb.h>
#include <climits> 
#include <iostream>
#include <ctime>
#include <random>
#include <vector>

#define size 100
#define range 100000


class findMin {
private:
    std::vector<int>::iterator min;
    std::vector<int>* vec;
public:
    findMin (std::vector<int>* setVec) : vec(setVec), min((*setVec).begin()) {    }
    findMin (findMin& other, tbb::split) : vec(other.vec), min((*other.vec).begin()) {    }
    void operator()(const tbb::blocked_range<size_t>& r) {
        for (auto iter = r.begin(); iter != r.end(); iter++) {
            if ((*vec)[iter] < *min) {
                min = (*vec).begin() + iter;
            }
        }
    }
    void join (findMin& other) {
        if (*min > *other.min)
            min = other.min;
    }
    int returnValue () {
        return *min;
    }
};

int main(int argc, const char * argv[]) {
    tbb::task_scheduler_init init;
    tbb::spin_mutex mutex;
    std::vector<int> myVec(size);
    std::default_random_engine engine;
    engine.seed(std::time(nullptr));
    std::uniform_int_distribution<int> distribution(0,range-1);
    auto dice = std::bind(distribution, engine);
    
    
    for (auto& iter : myVec) {
        iter = dice();
    }
    for (auto& iter : myVec) {
        std::cout << iter << ' ';
    }
    
    
    findMin min(&myVec);
    tbb::parallel_reduce(tbb::blocked_range<size_t>(0, size), min);
    std::cout << "\nMinimum parallel_reduce = " << min.returnValue() << std::endl;
    
    findMin minDeterministic(&myVec);
    tbb::parallel_deterministic_reduce(tbb::blocked_range<size_t>(0, size, 10), minDeterministic);
    std::cout << "\nMinimum parallel_reduce = " << min.returnValue() << std::endl;
    return 0;
}
