#ifndef FINITE_STATE_AUTOMATA_HASH_H
#define FINITE_STATE_AUTOMATA_HASH_H

#include "string"

// Hash function for std::pair (used in std::unordered_map)
template <typename StateType, typename SymbolType>
struct PairHash;

// Partial specialization for SymbolType = std::string
template <typename StateType>
struct PairHash<StateType, std::string> {
    std::size_t operator () (const std::pair<StateType, std::string>& p) const {
        auto h1 = std::hash<StateType>{}(p.first);
        auto h2 = std::hash<std::string>{}(p.second);
        return h1 ^ h2;
    }
};

// General case for SymbolType other than std::string
template <typename StateType, typename SymbolType>
struct PairHash {
    std::size_t operator () (const std::pair<StateType, SymbolType>& p) const {
        auto h1 = std::hash<StateType>{}(p.first);
        auto h2 = std::hash<SymbolType>{}(p.second);
        return h1 ^ h2;
    }
};

#endif //FINITE_STATE_AUTOMATA_HASH_H
