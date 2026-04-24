#include <iostream>
#include <string>

int main() {
    // Strings are dynamic arrays of chars
    std::string str = "Fahrzeug";

    // print the string
    std::cout << str << '\n';

    // iterate by string
    for (size_t i = 0; i < str.size(); i++) {
        std::cout << "str[" << i << "]:" << str[i] << '\n';
    }

    // member functions
    std::cout << "str.size(): " << str.size() << '\n';
    std::cout << "str.empty(): " << str.empty() << '\n';
    std::cout << "str.front(): " << str.front() << '\n';
    std::cout << "str.back(): " << str.back() << '\n';

    // contiguous addresses (like any array)
    std::cout << "&str: " << &str << '\n';
    std::cout << "str.data(): " << str.data() << '\n';

    // the address of a `char` prints from that position
    std::cout << "&str[0]: " << &str[0] << '\n';
    std::cout << "&str[1]: " << &str[1] << '\n';
    std::cout << "&str[2]: " << &str[2] << '\n';

    return 0;
}