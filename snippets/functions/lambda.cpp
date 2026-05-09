/**
 * https://www.geeksforgeeks.org/cpp/lambda-expression-in-c/
 *
 */
#include <algorithm>
#include <iostream>
#include <vector>

int main() {
    // Defining a lambda
    auto res = [](int x) { return x + x; };

    // Using the lambda
    std::cout << res(5) << '\n';

    // define vectors
    std::vector<int> v1, v2;

    auto print = [](const std::vector<int>& v) {
        for (auto x : v) {
            std::cout << x << " ";
        }
        std::cout << '\n';
    };

    //  Capture all by reference
    auto byRef = [&](int m) {
        v1.push_back(m);
        v2.push_back(m);
    };

    //  Capture all by value
    auto byVal = [=](int m) mutable {
        v1.push_back(m);
        v2.push_back(m);
    };

    //  Capture v1 by reference and v2 by value
    auto mixed = [&v1, v2](int m) mutable {
        v1.push_back(m);
        v2.push_back(m);
    };

    // Case 1: byRef — modifies both v1 and v2
    byRef(20);

    // Case 2: byVal — modifies only copies (originals unchanged)
    byVal(234);

    // Case 3: mixed — modifies only v1 (since v2 is captured by value)
    mixed(10);

    print(v1);
    print(v2);

    // sorting
    std::vector<int> v3 = {5, 1, 8, 3, 9, 2, 5, 1, 10};

    // Sort in descending order
    sort(v3.begin(), v3.end(), [](const int& a, const int& b) { return a > b; });

    print(v3);

    // Find first element divisible by 3
    auto it = find_if(v3.begin(), v3.end(), [](const int& a) { return a % 3 == 0; });

    if (it != v3.end()) {
        std::cout << *it;
    } else {
        std::cout << "No such element";
    }
    std::cout << '\n';

    auto succeeded = []() { return true; };
    std::cout << (succeeded() ? "True" : "False") << '\n';

    return 0;
}