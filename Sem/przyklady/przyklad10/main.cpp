#include <tbb/tbb.h>
#include <iostream>

template <typename T>
std::ostream& operator << (std::ostream& stream, const tbb::concurrent_vector<T>& vec) {
    if (vec.capacity() == 0) {
        stream << "Vector jest pusty" << std::endl;
        return stream;
    }
    for (auto iter : vec) {
        stream << iter << ' ';
    }
    stream << std::endl;
    return stream;
}

int main(int argc, const char * argv[]) {
    tbb::concurrent_vector<int> myVector;
    
    for (int iter = 0; iter < 16; iter++) {
        if (iter) myVector.push_back(iter);
        std::cout << myVector << "capacity: "<< myVector.capacity() << std::endl;
    }
    
    std::cout << myVector;
    myVector.emplace_back(1);
    std::cout << myVector;
    myVector.grow_to_at_least(20, 10);
    std::cout << myVector;
    for (int i = 1; i < myVector.size(); i++)
        std::cout << &myVector[i] - &myVector[i-1] << ' ';
    std::cout << std::endl << std::endl <<std::endl;
    myVector.shrink_to_fit();
    for (int i = 1; i < myVector.size(); i++)
        std::cout << &myVector[i] - &myVector[i-1] << ' ';
    std::cout << std::endl;
    tbb::parallel_for(tbb::blocked_range<size_t> (0, 100), [&myVector](const tbb::blocked_range<size_t>& range) {
        for (auto it = range.begin(); it != range.end(); it++) {
            myVector.push_back(it);
        }
    });
    std::cout << myVector;

    return 0;
}
