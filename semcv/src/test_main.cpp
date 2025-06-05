#include "semcv.h"
#include <iostream>

int main() {
    std::cout << "Running semcv library tests..." << std::endl;

    bool success = semcv::runTests();

    if (success) {
        std::cout << "All semcv tests passed!" << std::endl;
        return 0;
    } else {
        std::cout << "Some semcv tests failed!" << std::endl;
        return 1;
    }
}
