#include <iostream>
#include <vector>

int main() {
    // copy a vector
    std::vector<double> v1(30, 1.0);
    std::vector<double> v2 = v1;

    std::cout << "v1: ";
    for (size_t i = 0; i < v1.size(); i++) {
        std::cout << v1[i] << " ";
    }
    std::cout << '\n';

    std::cout << "v2: ";
    for (size_t i = 0; i < v2.size(); i++) {
        std::cout << v2[i] << " ";
    }
    std::cout << '\n';

    // moving a vector
    std::vector<double> v3 = std::move(v1);

    //[iterate_2 The vector moved-from is invalidated
    std::cout << "v1: ";
    for (double x : v1) {
        std::cout << x << " ";  // prints nothing
    }
    std::cout << '\n';

    // iterate v3
    std::cout << "v3: ";
    for (size_t i = 0; i < v3.size(); i++) {
        std::cout << v3[i] << " ";
    }
    std::cout << '\n';

    return 0;
}