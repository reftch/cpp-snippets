#include <concepts>
#include <iostream>
#include <utility>
#include <vector>

void call_custom_sort();

int main() {
    call_custom_sort();

    return 0;
}

template <std::integral T>
std::vector<T> bubble_sort(std::vector<T> arr) {
    if (arr.size() < 2) return arr;

    for (size_t i = 0; i + 1 < arr.size(); ++i) {
        bool swapped = false;
        for (size_t j = 0; j + 1 < arr.size() - i; ++j) {
            if (arr[j] > arr[j + 1]) {
                std::swap(arr[j], arr[j + 1]);
                swapped = true;
            }
        }
        if (!swapped) break;
    }
    return arr;
}

void print_array(std::vector<int> sorted) {
    for (int num : sorted) {
        std::cout << num << " ";
    }
    std::cout << '\n';
}

void call_custom_sort() {
    std::vector<int> numbers = {64, 34, 25, 12, 22, 11, 90};

    // bubble sorting
    auto sorted = bubble_sort(numbers);
    print_array(sorted);
}