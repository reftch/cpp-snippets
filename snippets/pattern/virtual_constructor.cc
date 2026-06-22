#include <memory>

class Base {
   public:
    virtual ~Base() {}
    virtual Base* clone() const = 0;
};

class Derived : public Base {
   public:
    Derived* clone() const override { return new Derived(*this); }
};

void foo(std::unique_ptr<Base> original) {
    // copy
    std::unique_ptr<Base> copy{original->clone()};
}

int main() {
    //  original;

    return 0;
}