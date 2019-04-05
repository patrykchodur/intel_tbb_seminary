#include <tbb/tbb.h>
#include <string>
#include <iostream>


class readFromString {
    std::string::const_iterator* iter;
    std::string::const_iterator end;
public:
    readFromString (std::string::const_iterator* setIter, std::string::const_iterator setEnd) : iter(setIter), end(setEnd) { };
    char operator()(tbb::flow_control& control) const {
        if (*iter == end) {
            control.stop();
            return NULL;
        }
        return *((++(*iter))-1);
    }
};

class addToString {
    std::string* result;
public:
    addToString (std::string* setString) : result(setString) { };
    char operator ()(char letter) const {
        *result += letter;
        return letter;
    }
};

class swapFunctor {
public:
    char operator ()(char input) const{
        if (int(input) > 64 && int(input) < 91) {
            return input+32;
        }
        if (int(input) > 96 && int(input) < 123) {
            return input-32;
        }
        return input;
    }
};

size_t count;
class counter {
private:
public:
    counter() {
        count = 0;
    }
    void operator()(char letter) const{
        if (int(letter) > 64 && int(letter) < 91)
            count++;
    }
};


int main(int argc, const char * argv[]) {
    std::string text = "Lorem ipsum dolor sit amet, consectetur adipiscing elit. Proin nibh augue, suscipit a, scelerisque sed, lacinia in, mi. Cras vel lorem. Etiam pellentesque aliquet tellus. Phasellus pharetra nulla ac diam. Quisque semper justo at risus. Donec venenatis, turpis vel hendrerit interdum, dui ligula ultricies purus, sed posuere libero dui id orci. Nam congue, pede vitae dapibus aliquet, elit magna vulputate arcu, vel tempus metus leo non est. Etiam sit amet lectus quis est congue mollis. Phasellus congue lacus eget neque. Phasellus ornare, ante vitae consectetuer consequat, purus sapien ultricies dolor, et mollis pede metus eget nisi. Praesent sodales velit quis augue. Cras suscipit, urna at aliquam rhoncus, urna quam viverra nisi, in interdum massa nibh nec erat.";
    std::string result;
    result.reserve(700);
    std::string::const_iterator iter = text.begin();
    tbb::filter_t<void, char> f0 (tbb::filter::serial_in_order, readFromString(&iter, text.end()));
    tbb::filter_t<char, char> f1 (tbb::filter::parallel, swapFunctor());
    tbb::filter_t<char, char> f2 (tbb::filter::serial_in_order, addToString(&result));
    tbb::filter_t<char, void> f3 (tbb::filter::serial_out_of_order, counter());
    tbb::parallel_pipeline (4, f0 & f1 & f2 & f3);
    std::cout << result << std::endl << "liczba duzych liter: " << count;
    return 0;
}
