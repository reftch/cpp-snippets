#include <iostream>

class IFibonacci {
public:
    virtual ~IFibonacci() = default;
    // virtual unsigned long compute(unsigned long n) = 0;
};

class Fibonacci : public IFibonacci {
public:
    unsigned long compute(unsigned long n) {
        if (n < 2)
            return n;
        return compute(n - 1) + compute(n - 2);
    }
};

int main() {
    Fibonacci fib;
    unsigned long n = 45;
    unsigned long result = fib.compute(n);

    std::cout << "Result fibonacci of " << n << " is " << result << std::endl;
    return 0;
}
