#include <iostream>
#include <unordered_set>
#include <unordered_map>
#include <memory>
#include <vector>
#include "Tokens.h"
#include "Hash.h"
#include "State.h"
#include "FSA.h"


int main() {
    std::unordered_set<char> char_alphabet = {'0', '1'};

    // Assuming 'states.txt' contains state definitions and 'transitions.txt' contains transition definitions
    FiniteStateAutomaton<std::string, char> fsa_char(char_alphabet, "states.txt", "transitions.txt");

    //std::vector<char> input_str = {'1', '0', '1', '1', '0'};
    std::string input_str = "010110";
    bool result_vec = fsa_char.process_input(input_str);

    if (result_vec) {
        std::cout << "The input is accepted by the automaton.\n";
    } else {
        std::cout << "The input is not accepted by the automaton.\n";
    }

    return 0;
}
