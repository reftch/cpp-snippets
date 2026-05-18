#include <functional>
#include <iostream>

int main() {
    // recursive lambda
    std::function<int(int)> fibonacci = [&](int n) -> int {
        if (n <= 1) return n;
        return fibonacci(n - 1) + fibonacci(n - 2);
    };

    std::cout << "F(10) = " << fibonacci(10) << '\n';

    return 0;
}