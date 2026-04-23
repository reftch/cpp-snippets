#include <iostream>
#include <string>

int main() {
    // Strings are dynamic arrays of chars
    constexpr std::string str = "Fahrzeug";

    // print the string
    std::cout << str << std::endl;

    // iterate by string
    for (size_t i = 0; i < str.size(); i++) {
        std::cout << "str[" << i << "]:" << str[i] << std::endl;
    }

    // member functions
    std::cout << "str.size(): " << str.size() << std::endl;
    std::cout << "str.empty(): " << str.empty() << std::endl;
    std::cout << "str.front(): " << str.front() << std::endl;
    std::cout << "str.back(): " << str.back() << std::endl;

    // contiguous addresses (like any array)
    std::cout << "&str: " << &str << std::endl;
    std::cout << "str.data(): " << str.data() << std::endl;

    // the address of a `char` prints from that position
    std::cout << "&str[0]: " << &str[0] << std::endl;
    std::cout << "&str[1]: " << &str[1] << std::endl;
    std::cout << "&str[2]: " << &str[2] << std::endl;

    return 0;
}