#include <iostream>
#include <mutex>
#include <thread>

std::mutex counter_mtx;
int counter = 0;

void increment_counter() {
    counter_mtx.lock();
    for (size_t i = 0; i < 1000000; ++i) {
        ++counter;
    }
    counter_mtx.unlock();
}

int main() {
    std::cout << "start count: " << counter << '\n';
    auto start = std::chrono::high_resolution_clock::now();
    std::thread t1(increment_counter);
    std::thread t2(increment_counter);
    t1.join();
    t2.join();

    auto end = std::chrono::high_resolution_clock::now();
    std::cout << "end count: " << counter << '\n';
    std::chrono::duration<double, std::micro> elapsed = end - start;
    std::cout << "Time took: " << elapsed.count() << " µs" << '\n';

    return 0;
}