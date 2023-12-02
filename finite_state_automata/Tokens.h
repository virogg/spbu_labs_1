#ifndef FINITE_STATE_AUTOMATA_TOKENS_H
#define FINITE_STATE_AUTOMATA_TOKENS_H

// Primary template for tokenize_input
template <typename SymbolType>
std::vector<SymbolType> tokenize_input(const std::string& input, const std::unordered_set<SymbolType>& alphabet) {
    std::vector<SymbolType> tokens;
    std::size_t pos = 0;

    while (pos < input.length()) {
        std::size_t max_length = 0;
        SymbolType max_token;
        // Try to match the longest prefix with a valid symbol in the alphabet
        for (const auto& symbol : alphabet) {
            if (input.compare(pos, symbol.length(), symbol) == 0 && symbol.length() > max_length) {
                max_length = symbol.length();
                max_token = symbol;
            }
        }
        if (max_length == 0) {
            // No valid symbol found, terminate or handle the error as needed
            break;
        }
        // Add the matched symbol to the list of tokens
        tokens.push_back(max_token);
        // Move the position to the next character after the matched symbol
        pos += max_length;
    }

    return tokens;
}

// Partial specialization for char
template <>
inline std::vector<char> tokenize_input(const std::string& input, const std::unordered_set<char>& alphabet) {
    std::vector<char> tokens;
    std::size_t pos = 0;

    while (pos < input.length()) {
        char symbol = input[pos];
        // Check if the symbol is in the alphabet
        if (alphabet.count(symbol) > 0) {
            // Add the matched symbol to the list of tokens
            tokens.push_back(symbol);
            // Move the position to the next character
            pos++;
        } else {
            // No valid symbol found, terminate or handle the error as needed
            break;
        }
    }

    return tokens;
}

#endif //FINITE_STATE_AUTOMATA_TOKENS_H
