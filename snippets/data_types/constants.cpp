#include <iostream>

int main() {
    // Mutable variable
    int a = 2;
    a = 3;
    std::cout << "a : " << a << '\n';

    // Constants as a promise
    const int b = 3;
    // b = 4; <- this fails. `b` is constant.
    std::cout << "b : " << b << '\n';

    // Constant "more constant than const"
    // `c` can only use values available at compile-time
    // `c` cannot be initialized with a mutable value
    constexpr int c = 4;
    std::cout << "c : " << c << '\n';

    // Creating arrays from compile-time `const` values
    // Sometimes possible, if b was not initialized with a mutable value
    std::array<int, b> v2 = {5, 6, 5};
    std::cout << "v2.size(): " << v2.size() << '\n';

    // Creating arrays from compile-time `constexpr` values
    // Always possible
    std::array<int, c> v3 = {7, 8, 7, 8};
    std::cout << "v3.size(): " << v3.size() << '\n';

    return 0;
}