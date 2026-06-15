#include <iostream>
#include <cassert>

double divide(double numerator, double denominator) {
    assert(denominator != 0 && "Denominator cannot be zero.");

    if (denominator == 0) {
        throw std::invalid_argument("Denominator cannot be zero.");
    }
    return numerator / denominator;
}

int main() {
    try {
        double result = divide(10, 0);
        std::cout << "Result: " << result << std::endl;
    } catch (const std::invalid_argument& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
    return 0;
}