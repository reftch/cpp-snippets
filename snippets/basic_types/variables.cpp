#include <iostream>

int main() {
    // Printing the size of each data type
    std::cout << "Size of int: " << sizeof(int) << " bytes" << '\n';
    std::cout << "Size of long int: " << sizeof(long int) << " bytes" << '\n';
    std::cout << "Size of long long int: " << sizeof(long long int) << " bytes" << '\n';
    std::cout << "Size of unsigned int: " << sizeof(unsigned int) << " bytes" << '\n';
    std::cout << "Size of signed int: " << sizeof(signed int) << " bytes" << '\n';
    std::cout << "Size of char: " << sizeof(char) << " byte" << '\n';
    std::cout << "Size of float: " << sizeof(float) << " bytes" << '\n';
    std::cout << "Size of double: " << sizeof(double) << " bytes" << '\n';
    std::cout << "Size of long double: " << sizeof(long double) << " bytes" << '\n';

    std::cout << "Size of int32_t: " << sizeof(int32_t) << " bytes" << '\n';
    std::cout << "Size of int64_t: " << sizeof(int64_t) << " bytes" << '\n';
    std::cout << "Size of uint_least64_t: " << sizeof(uint_least64_t) << " bytes" << '\n';

    return 0;
}