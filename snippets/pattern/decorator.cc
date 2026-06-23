#include <iostream>
#include <memory>
#include <string>

// Base component interface
class Component {
   public:
    virtual ~Component() = default;
    virtual void draw() const = 0;
};

// Concrete component
class TextComponent : public Component {
   private:
    std::string text_;

   public:
    explicit TextComponent(const std::string& text) : text_(text) {}

    void draw() const override { std::cout << "Text: " << text_ << "\n"; }
};

// Base decorator
class Decorator : public Component {
   protected:
    std::unique_ptr<Component> component_;

   public:
    explicit Decorator(std::unique_ptr<Component> component) : component_(std::move(component)) {}

    void draw() const override { component_->draw(); }
};

// Concrete decorators
class BorderDecorator : public Decorator {
   public:
    using Decorator::Decorator;

    void draw() const override {
        std::cout << "Border: ";
        Decorator::draw();
        std::cout << "Border end\n";
    }
};

class ShadowDecorator : public Decorator {
   public:
    using Decorator::Decorator;

    void draw() const override {
        std::cout << "Shadow effect applied\n";
        Decorator::draw();
    }
};

class ColorDecorator : public Decorator {
   private:
    std::string color_;

   public:
    ColorDecorator(std::unique_ptr<Component> component, const std::string& color)
        : Decorator(std::move(component)), color_(color) {}

    void draw() const override {
        std::cout << "Color: " << color_ << " ";
        Decorator::draw();
    }
};

int main() {
    // Create base component
    auto text = std::make_unique<TextComponent>("Hello World");

    // Apply decorators - no factory needed
    auto borderText = std::make_unique<BorderDecorator>(std::move(text));
    auto shadowText = std::make_unique<ShadowDecorator>(std::move(borderText));
    auto coloredText = std::make_unique<ColorDecorator>(std::move(shadowText), "blue");

    // Use the decorated component
    coloredText->draw();

    return 0;
}