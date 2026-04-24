#include <array>
#include <iostream>
#include <string>

int main() {
    // Fixed size array
    std::array<int, 5> a = {1, 2, 3, 4, 5};

    // iterate arrays
    for (size_t i = 0; i < a.size(); ++i) {
        std::cout << "a[" << i << "]: " << a[i] << '\n';
    }

    // member functions
    std::cout << "a.size(): " << a.size() << '\n';
    std::cout << "a.empty(): " << a.empty() << '\n';
    std::cout << "a.front(): " << a.front() << '\n';
    std::cout << "a.back(): " << a.back() << '\n';

    // range-based loop
    for (const auto& el : a) {
        std::cout << "Element " << el << '\n';
    }

    return 0;
}
