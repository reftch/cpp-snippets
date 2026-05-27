#include <iostream>

int main() {
    union A {
        int x;
        char y;
    };

    union B {
        int arr[10];
        char y;
    };

    // Finding size using sizeof() operator
    std::cout << "Sizeof A: " << sizeof(A) << '\n';
    std::cout << "Sizeof B: " << sizeof(B) << '\n';

    union Student {
        int rollNo;
        float height;
        char firstLetter;
    };

    Student data;

    data.rollNo = 21;
    std::cout << data.rollNo << '\n';

    data.height = 5.2;
    std::cout << data.height << '\n';

    data.firstLetter = 'N';
    std::cout << data.firstLetter << '\n';

    // nested union
    struct Employee {
        char name[50];
        int id;

        // Nested union
        union Pay {
            float hourlyRate;
            float salary;
        } payment;
    };

    Employee e1;
    e1.id = 101;

    // Access nested union member using dot operator
    e1.payment.hourlyRate = 300.0;

    std::cout << "Employee ID: " << e1.id << '\n';
    std::cout << "Hourly Rate: Rs " << e1.payment.hourlyRate << '\n';

    // You can also assign salary if needed
    e1.payment.salary = 50000.0;
    std::cout << "Salary: Rs " << e1.payment.salary << '\n';

    return 0;
}