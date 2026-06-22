#include <concepts>
#include <iostream>
#include <ranges>
#include <string>
#include <vector>

// Concept for pattern elements
template <typename T>
concept PatternElement = requires(T t) {
    { t.build() } -> std::convertible_to<std::string>;
};

// Base pattern builder class
class PatternBuilder {
   private:
    std::vector<std::string> parts;

   public:
    template <PatternElement T>
    auto add(const T& element) -> PatternBuilder& {
        parts.push_back(element.build());
        return *this;
    }

    auto build() const -> std::string {
        std::string result;
        for (const auto& part : parts) {
            result += part;
        }
        return result;
    }
};

// Concrete pattern elements
struct LiteralElement {
    std::string text;

    auto build() const -> std::string { return text; }
};

struct NumberElement {
    int value;

    auto build() const -> std::string { return std::to_string(value); }
};

struct ConditionalElement {
    bool condition;
    std::string true_value;
    std::string false_value;

    auto build() const -> std::string { return condition ? true_value : false_value; }
};

// Usage example
int main() {
    auto pattern = PatternBuilder{}
                       .add(LiteralElement{"Hello "})
                       .add(NumberElement{42})
                       .add(ConditionalElement{true, " world!", " universe!"});

    std::cout << pattern.build() << '\n';  // Output: Hello 42 world!

    return 0;
}