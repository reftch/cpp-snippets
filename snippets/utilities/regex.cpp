#include <iostream>
#include <regex>

int main() {
    // match single expression
    if (std::regex_match("subject", std::regex("(sub)(.*)"))) {
        std::cout << "subject matches expression (sub)(.*)" << '\n';
    }

    return 0;
}