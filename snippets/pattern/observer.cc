#include <functional>
#include <iostream>
#include <vector>

// Observer interface - defines the contract for objects that want to be notified
class observer {
   public:
    // Pure virtual function that must be implemented by derived classes
    virtual void notify() = 0;

    // Virtual destructor for proper cleanup of derived objects
    virtual ~observer() = default;
};

// Concrete implementation of the observer interface
class observer_concrete : public observer {
   public:
    // Implementation of the notification method
    void notify() override { std::cout << "Notified" << '\n'; }
};

// Subject class that manages a collection of observers
class subject {
   public:
    // Register an observer with the subject
    void registerObserver(observer& o) {
        // Store a reference_wrapper to avoid copying the observer object
        observers.push_back(std::ref(o));
    }

    // Notify all registered observers
    void notifyObservers() {
        // Iterate through all registered observers and call their notify method
        for (auto& o : observers) {
            // Use .get() to access the referenced object and call notify()
            o.get().notify();
        }
    }

   private:
    // Vector to store references to observers (using reference_wrapper)
    std::vector<std::reference_wrapper<observer>> observers;
};

int main() {
    // Create a concrete observer instance
    observer_concrete c;

    // Create a subject instance
    subject s;

    // Register the observer with the subject
    s.registerObserver(c);

    // Notify all registered observers
    s.notifyObservers();

    return 0;
}