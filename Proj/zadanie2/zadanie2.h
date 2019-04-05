#ifndef zadanie2_h
#define zadanie2_h

#include <queue>
#include <iostream>
#include <thread>
#include <chrono>
#include <tbb/tbb.h>

typedef tbb::mutex mutex_type;

mutex_type mutex;

int counter = 0;

template<class queueType>
class Producer {
private:
    queueType* myQueue;
    tbb::atomic<size_t>* bufferSize;
    size_t maxSize;
public:
    Producer(queueType* setQueue, tbb::atomic<size_t>* setBuffer, size_t setMaxSize) : myQueue(setQueue), bufferSize(setBuffer), maxSize(setMaxSize) { }
    void operator()() const {
        mutex_type::scoped_lock lock;
        while (true) {
            if (*bufferSize < maxSize) {
                (*myQueue).push(counter++);
                (*bufferSize).fetch_and_add(1);
                lock.acquire(mutex);
                std::cout << "Stworzono obiekt \n";
                lock.release();
                std::this_thread::sleep_for(std::chrono::milliseconds(50));
            }
        }
    }
};
template<class queueType>
class Concumer {
private:
    queueType* myQueue;
    tbb::atomic<size_t>* bufferSize;
public:
    Concumer(queueType* setQueue, tbb::atomic<size_t>* setBuffer) : myQueue(setQueue), bufferSize(setBuffer) { }
    void operator()() const {
        int result;
        mutex_type::scoped_lock lock;
        while (true) {
            if ((*myQueue).try_pop(result)) {
                (*bufferSize).fetch_and_add(-1);
                lock.acquire(mutex);
                std::cout << "Pobrano " << result << std::endl;
                lock.release();
                std::this_thread::sleep_for(std::chrono::milliseconds(60));
            }
        }
    }
    
};


#endif /* zadanie2_h */
