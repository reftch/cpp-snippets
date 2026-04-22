#include <iostream>

template <std::integral T>
T compute(T n) {
    switch (n) {
        case 0:
        case 1:
            return n;
        default:
            return compute(n - 1) + compute(n - 2);
    }
}

int main() {
    unsigned long n = 45;

    std::cout << "Running fibonacci calculation..." << std::endl;
    unsigned long result = compute<unsigned long>(n);

    std::cout << "Result fibonacci of " << n << " is " << result << std::endl;
    return 0;
}
