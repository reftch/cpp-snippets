#include <concepts>
#include <iostream>

// Define concept from a compile-time boolean predicate
template <typename T>
concept Number = std::is_arithmetic_v<T>;

// Define concept from a another concepts as a boolean predicate
template <typename T>
concept NotNumber = !Number<T>;

// Number, but nor SignedNumber neither UnsignedNumber
template <Number T>
T max_value(T a, T b) {
    return a > b ? a : b;
}

// NotNumber == !Number<T>
template <NotNumber T>
T max_value(T a, T b) {
    long l1 = 0, l2 = 0;
    for (char e : std::string(a)) {
        l1 += (int)e;
    }
    for (char e : std::string(b)) {
        l2 += (int)e;
    }
    return max_value(l1, l2) == l1 ? a : b;
}

int main() {
    // compare number
    std::cout << max_value(5, 7) << '\n';
    std::cout << max_value(10, 7) << '\n';
    // compare not number
    std::cout << max_value("foo", "bar") << '\n';

    return 0;
}