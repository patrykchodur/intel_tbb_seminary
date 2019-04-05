#include <tbb/tbb.h>
#include <iostream>
#include <string>

void printRange (std::string name, tbb::blocked_range<size_t>& range) {
    std::cout << std::boolalpha << name << " range: from " << range.begin() << " to " << range.end() << ", splitable: " << range.is_divisible() << std::endl;
}

int main(int argc, const char * argv[]) {
    ///// Tworzymy obiekt blocked_range o zakresie od 0 do 100
    tbb::blocked_range<size_t> r1 (0, 100, 20);
    printRange("r1", r1);
    tbb::proportional_split split(1, 9);
    
    tbb::blocked_range<size_t>r2 (r1, split);
    printRange("r1", r1);
    printRange("r2", r2);
    tbb::blocked_range<size_t>(r1, tbb::split());
    printRange("r1", r1);
    return 0;
}
