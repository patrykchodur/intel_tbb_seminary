#include <tbb/tbb.h>
#include <iostream>

int main(int argc,const char* argv[]) {
    int threads = tbb::task_scheduler_init::default_num_threads();
    
    tbb::task_scheduler_init init(tbb::task_scheduler_init::deferred);
    std::cout << "is active: " << std::boolalpha << init.is_active() << std::endl;
    init.initialize(threads);
    std::cout << "is active: " << std::boolalpha << init.is_active() << ", number of threads: " << threads << std::endl;
    init.terminate();
    std::cout << "is active: " << std::boolalpha << init.is_active() << std::endl;
    init.~task_scheduler_init();
    
    tbb::task_scheduler_init init2 (tbb::task_scheduler_init::automatic,threads);
    
    tbb::task_scheduler_init init3;

}
