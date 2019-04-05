#include <thread>
#include <chrono>
#include <tbb/tbb.h>
#include <iostream>

int main(int argc, const char * argv[]) {
    tbb::concurrent_queue<int> myQueue;
    tbb::task_group group;
    bool finish = false;
    group.run([&myQueue, &finish](){
        for (int i = 0; i < 10000; i++) {
            myQueue.push(i);
        }
        finish = true;
    });
    group.run([&myQueue, &finish](){
        int result = 0;
        bool poped = false;
        while ( [&myQueue, &result, &poped]() {
            poped = myQueue.try_pop(result);
            return poped;
        }() || !finish) {
            if (poped) {
                std::cout << result << ' ';
            }
            else {
                std::cout << "Nie udało się ściągnąć elementu \n";
            }
        }
    });
    group.wait();
    return 0;
}
