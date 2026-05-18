#include <iostream>
#include <functional>

int main() {
    // pass argument by value
    std::function<void(int)> increment_by_value = [](int x) { x = x + 1; }; 
    int a = 5;
    std::cout << "Increment by value: " << a << '\n'; // Output: 5
    increment_by_value(a);
    std::cout << "Original value of a: " << a << '\n'; // Output: 5

    // pass argument by reference
    std::function<void(int&)> increment_by_reference = [](int& x) { x = x + 1; }; 
    increment_by_reference(a);
    std::cout << "Value of a after incrementing by reference: " << a << '\n'; // Output: 6  

    // pass argument by pointer (Google recommended)
    std::function<void(int*)> increment_by_pointer = [](int* x) { *x = *x + 1; }; 
    increment_by_pointer(&a);
    std::cout << "Value of a after incrementing by pointer: " << a << '\n'; // Output: 7

    return 0;
}