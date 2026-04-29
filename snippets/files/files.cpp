#include <algorithm>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <vector>

int main() {
    // create vector of numbers
    std::vector<int> numbers;
    constexpr size_t n = 15;
    numbers.resize(n);
    for (size_t i = 0; i < n; ++i) {
        numbers[i] = i * 1000 + 1;
    }

    // save numbers in text file
    std::ofstream fout("numbers.txt");
    for (int number : numbers) {
        fout << number << " ";
    }
    fout.close();

    // load numbers from text file
    std::vector<int> m_numbers;
    std::ifstream fin("numbers.txt");
    do {
        int x;
        fin >> x;
        if (fin) {
            m_numbers.emplace_back(x);
        }
    } while (fin);
    fin.close();

    // Print numbers from file
    for (const auto& number : numbers) {
        std::cout << number << " ";
    }
    std::cout << '\n';
    for (const auto& number : m_numbers) {
        std::cout << number << " ";
    }
    std::cout << '\n';

    // save numbers in binary file
    fout.open("numbers.bin", std::ios::binary);
    for (int& number : numbers) {
        char* pointer_to_number = (char*)&number;
        size_t bytes_per_number = sizeof(number);
        fout.write(pointer_to_number, bytes_per_number);
    }
    fout.close();

    m_numbers.clear();
    fin.open("numbers.bin", std::ios::binary);
    for (size_t i = 0; i < numbers.size(); ++i) {
        int x;
        char* pointer_to_number = (char*)&x;
        size_t bytes_per_number = sizeof(x);
        fin.read(pointer_to_number, bytes_per_number);
        m_numbers.emplace_back(x);
    }
    fin.close();

    // print numbers from binary file
    for_each(numbers.begin(), numbers.end(), [](auto x) { std::cout << x << " "; });
    std::cout << '\n';
    for_each(m_numbers.begin(), m_numbers.end(), [](auto x) { std::cout << x << " "; });
    std::cout << '\n';

    std::cout << "File size (text): " << std::filesystem::file_size("numbers.txt") << " bytes\n";
    std::cout << "File size (binary): " << std::filesystem::file_size("numbers.bin") << " bytes\n";

    return 0;
}