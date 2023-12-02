#ifndef FINITE_STATE_AUTOMATA_STATE_H
#define FINITE_STATE_AUTOMATA_STATE_H

// State class representing a state in the Finite State Automaton
template <typename StateType>
class State {
public:
    State(const StateType& name, bool is_start = false, bool is_accept = false)
            : name_(name), is_start_(is_start), is_accept_(is_accept) {}

    const StateType& get_name() const {
        return name_;
    }

    bool is_start_state() const {
        return is_start_;
    }

    bool is_accept_state() const {
        return is_accept_;
    }

private:
    StateType name_;  // State name
    bool is_start_;   // Flag indicating if the state is a start state
    bool is_accept_;  // Flag indicating if the state is an accept state
};

#endif //FINITE_STATE_AUTOMATA_STATE_H
