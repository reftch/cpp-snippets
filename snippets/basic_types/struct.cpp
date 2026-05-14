#include <iostream>
#include <string>

struct car {
    std::string name;
    std::string mark;
    int year;
    double price;
};

int main() {
    car myCar = {"120i", "BMW", 2024, 40000.0};

    std::cout << myCar.name << "\n";
    std::cout << myCar.mark << "\n";
    std::cout << myCar.year << "\n";
    std::cout << myCar.price << "\n";

    car myCar2{};
    std::cout << myCar2.name << "\n";
    std::cout << myCar2.mark << "\n";
    std::cout << myCar2.year << "\n";
    std::cout << myCar2.price << "\n";

    int x;
    std::cout << x << "\n";

    return 0;
}