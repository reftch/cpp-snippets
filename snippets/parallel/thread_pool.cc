#include <atomic>
#include <chrono>
#include <condition_variable>
#include <functional>
#include <future>
#include <iostream>
#include <mutex>
#include <queue>
#include <thread>
#include <vector>

#ifndef POOL_SIZE
#define POOL_SIZE 14
#endif

class ThreadPool {
   public:
    // Constructor: Create worker threads
    explicit ThreadPool(size_t threads) : stop(false) {
        for (size_t i = 0; i < threads; ++i) {
            workers.emplace_back([this] {
                for (;;) {
                    std::function<void()> task;
                    {
                        std::unique_lock<std::mutex> lock(this->queue_mutex);
                        this->condition.wait(lock, [this] { return this->stop || !this->tasks.empty(); });
                        if (this->stop && this->tasks.empty()) return;
                        task = std::move(this->tasks.front());
                        this->tasks.pop();
                    }
                    task();
                }
            });
        }
    }

    // Submit a task to the thread pool
    template <class F>
    auto submit(F&& f) -> std::future<typename std::invoke_result<F>::type> {
        using return_type = typename std::invoke_result<F>::type;

        auto task = std::make_shared<std::packaged_task<return_type()>>(std::forward<F>(f));

        std::future<return_type> res = task->get_future();
        {
            std::unique_lock<std::mutex> lock(queue_mutex);
            if (stop) throw std::runtime_error("submit on stopped ThreadPool");
            tasks.emplace([task]() { (*task)(); });
        }
        condition.notify_one();
        return res;
    }

    // Destructor: Clean up threads
    ~ThreadPool() {
        stop = true;
        condition.notify_all();
        for (std::thread& worker : workers) {
            worker.join();
        }
    }

   private:
    std::vector<std::thread> workers;
    std::queue<std::function<void()>> tasks;
    std::mutex queue_mutex;
    std::condition_variable condition;
    std::atomic<bool> stop;
};

int main() {
    // Create a thread pool with 4 threads
    ThreadPool pool(POOL_SIZE);

    // Submit some tasks
    std::vector<std::future<int>> results;

    // Submit tasks that return values
    for (int i = 0; i < 20; ++i) {
        results.emplace_back(pool.submit([i] {
            // Simulate some work
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
            return i * i;
        }));
    }

    // Collect results
    for (auto& result : results) {
        std::cout << "Result: " << result.get() << '\n';
    }

    return 0;
}