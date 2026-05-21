#include <iostream>

union A {
    int x;
    char y;
};

union B {
    int arr[10];
    char y;
};

int main() {
    // Finding size using sizeof() operator
    std::cout << "Sizeof A: " << sizeof(A) << '\n';
    std::cout << "Sizeof B: " << sizeof(B) << '\n';

    return 0;
}