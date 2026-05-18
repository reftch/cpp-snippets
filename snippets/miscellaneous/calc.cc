#include <iostream>
#include <sstream>
#include <string>
#include <vector>

// Function to perform the calculation based on tokens
double calculate(const std::vector<std::string>& tokens) {
    if (tokens.empty()) {
        return 0.0;
    }

    // Start with the first number
    double result = std::stod(tokens[0]);

    // Iterate through the rest of the tokens, performing the operation
    for (size_t i = 1; i < tokens.size(); i += 2) {
        char op = tokens[i][0];
        double next_num = std::stod(tokens[i + 1]);

        switch (op) {
            case '+':
                result += next_num;
                break;
            case '-':
                result -= next_num;
                break;
            case '*':
                result *= next_num;
                break;
            case '/':
                if (next_num != 0) {
                    result /= next_num;
                } else {
                    // Handle division by zero error
                    std::cerr << "Error: Division by zero!" << '\n';
                    return NAN;  // Not a Number
                }
                break;
            default:
                std::cerr << "Error: Invalid operator " << op << '\n';
                return NAN;
        }
    }
    return result;
}

std::vector<std::string> parse_expression(const std::string& expression) {
    std::vector<std::string> tokens;

    for (size_t i = 0; i < expression.length(); ++i) {
        char current_char = expression[i];

        // check if the character is a number (digit or decimal)
        if (std::isdigit(current_char) || current_char == '.') {
            std::string number;
            // Read the entire contiguous number
            while (i < expression.length() && (std::isdigit(expression[i]) || expression[i] == '.')) {
                number += expression[i];
                i++;
            }
            tokens.push_back(number);
            i--;  // Backtrack one step because the outer loop will increment i again
        }
        // check if the character is an operator
        else if (current_char == '+' || current_char == '-' || current_char == '*' || current_char == '/') {
            tokens.push_back(std::string(1, current_char));
        }
        // ignore spaces (this handles the input "4*5" correctly by ignoring the '*')
        // We implicitly ignore spaces here, allowing the parser to stitch numbers and operators together.
    }
    return tokens;
}

int main() {
    std::cout << "calculator v1.0.0" << '\n';

    std::string expression;

    // Main loop to continuously read input
    while (true) {
        std::cout << "> ";
        // Use std::getline to read the entire line, including spaces
        if (std::getline(std::cin, expression)) {
            // empty input
            if (expression.empty()) {
                return 0;
            }

            std::vector<std::string> tokens = parse_expression(expression);

            if (tokens.empty()) {
                std::cout << "Could not parse the expression. Check your format." << '\n';
                continue;
            }

            // 2. Calculate the result
            double result = calculate(tokens);
            // process valid input
            std::cout << "Result: " << result << '\n';
        }
        // Handle potential stream errors (optional but good practice)
        else {
            std::cerr << "\nError reading input or EOF detected." << '\n';
            break;
        }
    }

    return 0;
}