#include <chrono>
#include <future>
#include <iostream>
#include <thread>
#include <vector>

int main() {
    constexpr int pool_size = 10;
    std::vector<std::future<int>> tasks;

    // the lambda expression will be executed in a separate threads
    auto lambda = [](int a) {
        std::this_thread::sleep_for(std::chrono::milliseconds(300));
        return a * a;
    };

    // "Async pool" of std::async
    std::cout << "Submitting async tasks...\n";
    for (int i = 1; i <= pool_size; ++i) {
        tasks.push_back(std::async(std::launch::async, lambda, i + 8));
    }

    // do something while async tasks run
    std::cout << "Async tasks currently running; do other work..." << '\n';

    // collect results from the pool
    for (auto& f : tasks) {
        std::cout << "result is " << f.get() << '\n';
    }

    return 0;
}
