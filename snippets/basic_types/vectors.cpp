#include <iostream>
#include <vector>

int main() {
    // dynamic size vector
    std::vector<int> v = {1, 2, 3};

    // iterate vector elements
    for (size_t i = 0; i < v.size(); i++) {
        std::cout << "v[" << i << "]: " << v[i] << '\n';
    }

    // member functions
    std::cout << "v.size(): " << v.size() << '\n';
    std::cout << "v.empty(): " << v.empty() << '\n';
    std::cout << "v.front(): " << v.front() << '\n';
    std::cout << "v.back(): " << v.back() << '\n';

    // access addresses
    std::cout << "&v: " << &v << '\n';
    std::cout << "&v[0]: " << &v[0] << '\n';
    std::cout << "&v[1]: " << &v[1] << '\n';
    std::cout << "&v[2]: " << &v[2] << '\n';

    return 0;
}
