#include <iostream>

int main() {
    //
    int a = 10;
    int* ptr = &a;

    // print value and address
    std::cout << "Value of a: " << a << '\n';
    std::cout << "Address of a: " << &a << '\n';
    std::cout << "Value stored in pointer prt: " << ptr << '\n';
    std::cout << "Value pointed to by prt: " << *ptr << '\n';

    // size of pointer
    // 8 bytes for a 64-bit System
    // 4 bytes for a 32-bit System
    std::cout << "Size of pointer is " << sizeof(ptr) << " bytes" << '\n';

    // A nullptr is a pointer that does not point to any valid memory location. It is often used to initialize a pointer
    // when you do not want it to point to any object.
    int* ptr2 = nullptr;

    // A void pointer (void*) is a special type of pointer in C++ that has no associated data type.
    // void pointer holding address of an int
    void* ptr3 = &a;

    // Error: cannot dereference void pointer
    // cout << *ptr3;

    // Typecast before dereferencing
    std::cout << "Value pointed by void pointer: " << *(static_cast<int*>(ptr3)) << '\n';

    // A dangling pointer is a pointer that refers to memory which has already been freed or is no longer valid.
    // This typically happens when:
    //  - A pointer points to a local variable that has gone out of scope
    //  - Dynamically allocated memory is deallocated using delete, but the pointer still holds the old address
    auto get_pointer = []() {
        int x = 10;
        // returning address of local variable
        return &x;
    };

    int* ptr4 = get_pointer();
    // Undefined behavior
    std::cout << *ptr4 << '\n';

    // double pointer
    int** ptr5 = &ptr;
    std::cout << **ptr5 << '\n';

    return 0;
}