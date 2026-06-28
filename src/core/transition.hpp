/**
 * @file transition.hpp
 * @author wh0crypt (wh0crypt@proton.me)
 * @brief Transition class representing a multi-tape rule in the TM's transition function.
 * @version 0.2
 * @date 2026-06-27
 *
 * @copyright Copyright (c) 2026 wh0crypt. Licensed under the MIT License.
 */

#ifndef TRANSITION_HPP
#define TRANSITION_HPP

#include "direction.hpp"
#include "state.hpp"
#include "symbol.hpp"

#include <utility>
#include <vector>

/**
 * @class Transition
 * @brief Represents a multi-tape transition rule (δ: Q × Γ^k → Q × Γ^k × {L, R}^k).
 *
 * Defines the synchronized actions to take across all k parallel tapes when the
 * Turing Machine is in a specific state.
 */
class Transition
{
  public:
    /**
     * @brief Default constructor. Initializes an empty transition shifting LEFT.
     */
    Transition() = default;

    /**
     * @brief Explicit constructor to define a complete multi-tape transition rule.
     * @param current The required state to trigger this transition.
     * @param next The target state the machine will transition into.
     * @param read The collection of symbols expected under each parallel tape head.
     * @param write The collection of new symbols to overwrite onto each tape track.
     * @param dir The collection of displacement directions for each tape head.
     */
    explicit Transition(
        State current,
        State next,
        std::vector<Symbol> read,
        std::vector<Symbol> write,
        std::vector<Direction> dir
    )
        : current_state_(std::move(current)), next_state_(std::move(next)),
          read_symbols_(std::move(read)), write_symbols_(std::move(write)),
          dir_vectors_(std::move(dir))
    {}

    /**
     * @brief Retrieves the source state required to trigger this transition.
     * @return const State& A read-only reference to the current execution state.
     */
    [[nodiscard]] const State &get_current_state() const { return this->current_state_; }

    /**
     * @brief Retrieves the target state the machine will enter after executing this rule.
     * @return const State& A read-only reference to the next execution state.
     */
    [[nodiscard]] const State &get_next_state() const { return this->next_state_; }

    /**
     * @brief Retrieves the collection of symbols that must be present under the tape heads.
     * @return const std::vector<Symbol>& A read-only reference to the expected symbols vector.
     */
    [[nodiscard]] const std::vector<Symbol> &get_read_symbols() const
    {
        return this->read_symbols_;
    }

    /**
     * @brief Retrieves the new symbols that will overwrite the current parallel tape cells.
     * @return const std::vector<Symbol>& A read-only reference to the symbols to be written.
     */
    [[nodiscard]] const std::vector<Symbol> &get_write_symbols() const
    {
        return this->write_symbols_;
    }

    /**
     * @brief Retrieves the directions in which each parallel tape head must shift.
     * @return const std::vector<Direction>& A read-only reference to the head movement vectors.
     */
    [[nodiscard]] const std::vector<Direction> &get_directions() const
    {
        return this->dir_vectors_;
    }

    /**
     * @brief Checks equality of transitions based on their semantic identity.
     *
     * This definition allows detecting non-determinism in the Turing Machine by
     * treating transitions with identical (state, read_symbols) as conflicts.
     *
     * @param other The transition to compare against.
     * @return true if both transitions represent the same input condition.
     */
    [[nodiscard]] bool operator==(const Transition &other) const
    {
        return current_state_ == other.current_state_ && read_symbols_ == other.read_symbols_;
    }

    /**
     * @brief Lexicographical ordering of transitions for container storage.
     *
     * @warning This ordering is purely structural and does NOT represent
     * mathematical equivalence of transitions in the Turing Machine definition.
     */
    [[nodiscard]] auto operator<=>(const Transition &other) const
    {
        if (auto cmp = current_state_ <=> other.current_state_; cmp != 0)
            return cmp;
        return read_symbols_ <=> other.read_symbols_;
    }

  private:
    State current_state_;                ///< Source state for the transition rule.
    State next_state_;                   ///< Destination state after execution.
    std::vector<Symbol> read_symbols_;   ///< Expected vector array of symbols on the tracks.
    std::vector<Symbol> write_symbols_;  ///< New vector array of symbols to write.
    std::vector<Direction> dir_vectors_; ///< Structural head shift directions vector.
};

#endif // TRANSITION_HPP
