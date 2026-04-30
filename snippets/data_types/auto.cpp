#include <iostream>
#include <unordered_map>

int main() {
    // auto for fundamental data types
    auto dont_do_that = 87;
    std::cout << "dont_do_that: " << dont_do_that << '\n';

    // creating a hash table for the next snippet
    std::unordered_map<std::string, double> t;
    t["zero"] = 0.0;
    t["pi"] = 3.14;
    t["ten"] = 10.0;

    // accessing container with `auto`
    auto it2 = t.find("zero");
    if (it2 != t.end()) {
        std::cout << it2->first << ": " << it2->second << '\n';
    }

    // `auto` in generic functions
    auto print_map_container = [](const auto& m) {
        for (auto&& item : m) {
            std::cout << item.first << ": " << item.second << '\n';
        }
    };

    print_map_container(t);

    return 0;
}