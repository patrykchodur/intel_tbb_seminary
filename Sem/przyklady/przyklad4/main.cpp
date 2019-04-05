#include <tbb/tbb.h>
#include <iostream>
#include <vector>

#define grainSize 20
#define vecSize 100


typedef tbb::spin_mutex mutex;

class printVecInt {
private:
    std::vector<int>* vec;
    mutex* mu;
public:
    static size_t counter;
    printVecInt(std::vector<int>* setVec, mutex* setMu) : vec(setVec), mu(setMu) { }
    
    /////       Konstruktor kopiujacy       /////
    printVecInt(const printVecInt& other) : vec(other.vec), mu(other.mu) {
        mutex::scoped_lock lock;
        lock.acquire(*mu);
        std::cout << "Konstruktor kopiujacy" << std::endl;
        counter++;
        lock.release();
    }
    printVecInt& operator=(const printVecInt& other) {
        vec = other.vec;
        mu = other.mu;
        mutex::scoped_lock lock;
        lock.acquire(*mu);
        std::cout << "Kopiujacy operator przypisania" << std::endl;
        lock.release();
        return *this;
    }
    
    /////       Operator () wykorzystujacy blocked_range      /////
    void operator ()(const tbb::blocked_range<size_t>& range) const{
        mutex::scoped_lock lock;
        lock.acquire(*mu);
        std::cout << "Rozpoczynam wykonywanie funkcji, zakres od " << range.begin() << " do " << range.end() << std::endl;
        lock.release();
        for (size_t iter = range.begin(); iter < range.end(); iter++) {
            lock.acquire(*mu);
            std::cout << (*vec)[iter] << ' ';
            lock.release();
        }
    }
    
    /////       Operator () wykorzystujacy size_t           /////
    void operator()(size_t outerIter) const {
        mutex::scoped_lock lock;
        for (size_t iter = outerIter*grainSize; iter!= (outerIter+1)*grainSize; iter++) {
            lock.acquire(*mu);
            std::cout << (*vec)[iter] << ' ';
            lock.release();
        }
    }
};

size_t printVecInt::counter = 0;


int main(int argc, const char * argv[]) {
    tbb::task_scheduler_init init;
    mutex mu;
    std::vector<int> vec;
    int iter = 0;
    while (iter < vecSize) {
        vec.emplace_back(iter);
        iter++;
    }
    
    
    /////       Wyswietlenie tablicy w standardowy sposob            /////
    for (auto& iter2 : vec) {
        std::cout << iter2 << ' ';
    }
    std::cout << std::endl;
    
    tbb::affinity_partitioner partitioner;
    
    
    /////       Wyswietlenie tablicy z wykorzystaniem parallel_for  /////
    tbb::parallel_for(0, vecSize/grainSize, 1, printVecInt(&vec, &mu), partitioner);
    std::cout << std::endl;
    
    
    /////   Wyswietlenie tablicy z wykorzystaniem paralell_for oraz blocked_range   /////
    tbb::parallel_for(tbb::blocked_range<size_t>(0, vecSize, grainSize), printVecInt(&vec, &mu));

    std::cout << "\nLiczba wywołań kostruktora kopiujacego: " << printVecInt::counter;
    
    return 0;
}
