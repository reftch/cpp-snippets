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

    return 0;
}