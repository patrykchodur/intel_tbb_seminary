//  zadanie polega na stworzeniu klas konsumenta i producenta. Bufferem jest
//  tbb::concurrent_queue, ktore nie moze przekraczac rozmiaru maxBufferSize.
//  Jako semafora nalezy uzyc bufferSize uzywajac atomic expressions
//  Pliku main.cpp nie mozna modyfikowac. Do uspienia rdzenia mozna wykozystac
//  std::this_thread::sleep_for, jest ono wspierane przez biblioteke

#include "zadanie2.h"
#include <tbb/tbb.h>
#include <queue>
#include <iostream>

#define maxBufferSize 100

int main(int argc, const char * argv[]) {
    tbb::task_scheduler_init init;
    tbb::concurrent_queue<int> myQueue;
    tbb::atomic<size_t> bufferSize = 0;
    tbb::task_group g;
    g.run(Producer(&myQueue, &bufferSize, maxBufferSize));
    g.run(Concumer(&myQueue, &bufferSize));
    g.wait();
    
    return 0;
}


/*
 Stworzono obiekt
 Pobrano 0
 Stworzono obiekt
 Pobrano 1
 Stworzono obiekt
 Pobrano 2
 Stworzono obiekt
 Pobrano 3
 Stworzono obiekt
 Pobrano 4
 Stworzono obiekt
 Stworzono obiekt
 Pobrano 5
 Stworzono obiekt
 Pobrano 6
 Stworzono obiekt
 Pobrano 7
 Pobrano 8
 Stworzono obiekt
 Pobrano 9
 
 
 
 */
