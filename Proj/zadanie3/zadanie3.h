#ifndef zadanie3_h
#define zadanie3_h

#include <tbb/tbb.h>
#include <iostream>
#include <vector>


int partition (const tbb::blocked_range<int>& originalRange, std::vector<int>& vec) {
    int left = originalRange.begin();
    int right = originalRange.end()-1;
    auto key = vec[left];
    do {
        while (vec[left] < key) {
            left++;
        }
        while (vec[right] > key) {
            right--;
        }
        if (left <= right) {
            auto temp = vec[left];
            vec[left] = vec[right];
            vec[right] = temp;
            left++;
            right--;
        }
    } while (left <= right);
    return left;
}

class mySort : public tbb::task {
private:
    std::vector<int>* myVec;
    tbb::blocked_range<int> range;
public:
    mySort (std::vector<int>* setVec,const tbb::blocked_range<int>& setRange) : myVec(setVec), range(setRange) {  }
    tbb::task* execute() {
        if (!range.is_divisible()) {
            for(auto i = range.begin() + 1; i != range.end(); ++i)
            {
                for (auto j = i; j > range.begin() && (*myVec)[j] < (*myVec)[j-1]; --j) {
                    auto temp = (*myVec)[j];
                    (*myVec)[j] = (*myVec)[j-1];
                    (*myVec)[j-1] = temp;

                }
            }
            return NULL;
        } else {
            int temp = partition(range, *myVec);
            tbb::blocked_range<int> subRange1 (range.begin(), temp, range.grainsize());
            tbb::blocked_range<int> subRange2 (temp, range.end(), range.grainsize());
            mySort* left = new(allocate_child()) mySort(myVec, subRange1);
            mySort* right = new(allocate_child()) mySort(myVec, subRange2);
            set_ref_count(3);
            spawn(*left);
            spawn(*right);
            wait_for_all();
            return NULL;
        }
    }
    
    
};

#endif /* zadanie3_h */
