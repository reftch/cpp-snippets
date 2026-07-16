#include <iostream>
#include <string>
#include <variant>

struct FirstVaraint {
    int sockfd;
    void send(const std::string& msg) { std::cout << "FirstVariant sending: " << msg << '\n'; }
};

struct SecondVaraint {
    double v;
    void send(const std::string& msg) { std::cout << "SecondVariant sending: " << msg << '\n'; }
};

int main() {
    std::variant<FirstVaraint, SecondVaraint> var;

    var = FirstVaraint{10};

    std::visit([](auto&& v) { v.send("1"); }, var);

    var = SecondVaraint{3.14};

    std::visit([](auto&& v) { v.send("2"); }, var);

    return 0;
}