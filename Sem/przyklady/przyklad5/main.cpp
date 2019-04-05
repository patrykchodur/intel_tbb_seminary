#include <tbb/tbb.h>
#include <iostream>
#include <vector>

#define size 10000

typedef tbb::spin_mutex mutex;

class Sum {
private:
    const std::vector<int>* vec;
    mutex* mu;
public:
    int sum;
    Sum (Sum& other, tbb::split) : vec(other.vec), mu(other.mu), sum(0) {
        (*mu).lock();
        std::cout << "\nWywolanie split konstructora, Sum" << std::endl;
        (*mu).unlock();
    }
    Sum (const Sum& other) : vec(other.vec), mu(other.mu), sum(other.sum){
        (*mu).lock();
        std::cout << "\nWywolanie konstruktora kopiujacego Sum" << std::endl;
        (*mu).unlock();
    }
    Sum (std::vector<int>* setVec, mutex* setMu) : vec(setVec), mu(setMu), sum(0) {    }
    void join (const Sum& other) {
        sum += other.sum;
    }
    void operator() (const tbb::blocked_range<size_t>& r) {
        for (auto iter = r.begin(); iter != r.end(); iter++) {
            sum += (*vec)[iter];
        }
    }
};


int main(int argc, const char * argv[]) {
    tbb::task_scheduler_init init;
    std::vector<int> myVector(size);
    mutex myMutex;
    for (auto& iter : myVector) {
        iter = 1;
    }
    
    tbb::simple_partitioner partitioner;
    
    Sum myFun(&myVector, &myMutex);
    tbb::parallel_reduce(tbb::blocked_range<size_t>(0, size, 20), myFun, partitioner);
    std::cout << "Sum parallel_reduce = " << myFun.sum;
    
    
    
    Sum myFun2(&myVector, &myMutex);
    tbb::parallel_deterministic_reduce(tbb::blocked_range<size_t>(0, size, 20), myFun2, partitioner);
    std::cout << "Sum parallel_deterministic_reduce = " << myFun2.sum;
    
    
    
    auto function = [&myVector](const tbb::blocked_range<size_t>& range, int init) {
        for (auto iter = range.begin(); iter != range.end(); iter++) {
            init += myVector[iter];
        }
        return init;
    };
    auto reduction = [](int value1, int value2) {
        return value1+value2;
    };
    
    
    int result = tbb::parallel_reduce(tbb::blocked_range<size_t>(0, size, 20), 0, function, reduction);
    std::cout << "Sum parallel_reduce lambda = " << result;

    
    return 0;
}
