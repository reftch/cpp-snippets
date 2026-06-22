#include <iostream>
#include <optional>

void foo(int i, std::optional<double> f, std::optional<bool> b) {
    if (f.has_value()) {
        std::cout << "Second parameter is " << f.value() << '\n';
    }
    if (b.has_value()) {
        std::cout << "Third parameter is " << b.value() << '\n';
    }
}

// since c++17
// Allow argument values to be omitted when calling a function.

int main() {
    foo(5, 1.0, true);
    foo(5, std::nullopt, true);
    foo(5, 1.0, std::nullopt);
    foo(5, std::nullopt, std::nullopt);
}
