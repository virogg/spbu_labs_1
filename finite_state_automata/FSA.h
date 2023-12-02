#ifndef FINITE_STATE_AUTOMATA_FSA_H
#define FINITE_STATE_AUTOMATA_FSA_H

#include <stdexcept>
#include <fstream>
#include <sstream>
#include <unordered_set>
#include <unordered_map>

// Finite State Automaton (FSA) class template
template <typename StateType, typename SymbolType>
class FiniteStateAutomaton {
public:
    FiniteStateAutomaton(
            const std::unordered_set<SymbolType>& alphabet,
            const std::string& statesFile,
            const std::string& transitionsFile)
            : alphabet_(alphabet), current_state_(states_.end())
    {
        // Load states from file
        loadStates(statesFile);
        // Load transitions from file
        loadTransitions(transitionsFile);
    }

    void add_state(const StateType& state_name, bool is_start = false, bool is_accept = false) {
        auto state_it = states_.find(state_name);
        if (state_it != states_.end()) {
            if (is_start && start_state_ == states_.end()) {
                start_state_ = state_it;
            }
            if (is_accept) {
                accept_states_.insert(state_name);
            }
        } else {
            auto new_state = std::make_shared<State<StateType>>(state_name, is_start, is_accept);
            states_.insert({state_name, new_state});
            auto new_state_it = states_.find(state_name);
            if (is_start && start_state_ == states_.end()) {
                start_state_ = new_state_it;
            }
            if (is_accept) {
                accept_states_.insert(state_name);
            }
        }
    }

    void add_transition(const StateType& from_state, const SymbolType& symbol, const StateType& to_state) {
        auto state_it = states_.find(from_state);
        if (state_it == states_.end()) {
            throw std::invalid_argument("States in transition must be defined");
        }

        if (alphabet_.count(symbol) == 0) {
            throw std::invalid_argument("Symbol not in the alphabet");
        }

        auto key = std::make_pair(from_state, symbol);
        if (transitions_.count(key) > 0) {
            throw std::invalid_argument("Duplicate transition");
        }
        transitions_[key] = to_state;
    }

    void reset() {
        current_state_ = start_state_;
    }

    template <typename InputType>
    bool process_input(const InputType& input) {
        reset();
        tokenize_input(input, alphabet_);
        for (const auto& symbol : input) {
            auto transition_key = std::make_pair(current_state_->first, symbol);
            auto transition_it = transitions_.find(transition_key);
            if (transition_it == transitions_.end()) {
                return false;
            }
            current_state_ = states_.find(transition_it->second);
            if (current_state_ == states_.end()) {
                // Handle the case where the state is not found (it should not happen)
                throw std::logic_error("Internal error: State not found in states set.");
            }
        }

        return accept_states_.count(current_state_->second->get_name()) > 0;
    }


private:
    std::unordered_map<StateType, std::shared_ptr<State<StateType>>> states_;
    std::unordered_map<std::pair<StateType, SymbolType>, StateType, PairHash<StateType, SymbolType>> transitions_;
    typename std::unordered_map<StateType, std::shared_ptr<State<StateType>>>::const_iterator start_state_;
    std::unordered_set<StateType> accept_states_;
    typename std::unordered_map<StateType, std::shared_ptr<State<StateType>>>::const_iterator current_state_;
    std::unordered_set<SymbolType> alphabet_;

    // Function to load states from a file
    void loadStates(const std::string& statesFile) {
        std::ifstream file(statesFile);
        if (!file.is_open()) {
            throw std::runtime_error("Failed to open states file: " + statesFile);
        }

        std::string line;
        while (std::getline(file, line)) {
            std::istringstream iss(line);
            StateType state_name;
            bool is_start = false;
            bool is_accept = false;
            iss >> state_name >> std::boolalpha >> is_start >> is_accept;
            add_state(state_name, is_start, is_accept);
        }
        file.close();
    }

    // Function to load transitions from a file
    void loadTransitions(const std::string& transitionsFile) {
        std::ifstream file(transitionsFile);
        if (!file.is_open()) {
            throw std::runtime_error("Failed to open transitions file: " + transitionsFile);
        }

        std::string line;
        while (std::getline(file, line)) {
            std::istringstream iss(line);
            StateType from_state;
            StateType to_state;
            SymbolType symbol;
            iss >> from_state >> symbol >> to_state;
            add_transition(from_state, symbol, to_state);
        }
        file.close();
    }
};

#endif //FINITE_STATE_AUTOMATA_FSA_H
