#include <array>
#include <iostream>
#include <string>

int main() {
    // Fixed size array
    std::array<int, 5> a = {1, 2, 3, 4, 5};

    // iterate arrays
    for (size_t i = 0; i < a.size(); ++i) {
        std::cout << "a[" << i << "]: " << a[i] << std::endl;
    }

    // member functions
    std::cout << "a.size(): " << a.size() << std::endl;
    std::cout << "a.empty(): " << a.empty() << std::endl;
    std::cout << "a.front(): " << a.front() << std::endl;
    std::cout << "a.back(): " << a.back() << std::endl;

    // range-based loop
    for (const auto& el : a) {
        std::cout << "Element " << el << '\n';
    }

    return 0;
}
