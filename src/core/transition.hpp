/**
 * @file transition.hpp
 * @author wh0crypt (wh0crypt@proton.me)
 * @brief Transition class representing a single rule in the Turing Machine's transition function.
 * @version 0.1
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

/**
 * @class Transition
 * @brief Represents a quintuple transition rule (δ: Q × Γ → Q × Γ × {L, R}).
 *
 * Defines the action to take when the Turing Machine is in a specific state
 * and reads a specific symbol from the tape.
 */
class Transition
{
  public:
    /**
     * @brief Default constructor. Initializes an empty transition shifting LEFT.
     */
    Transition() : dir_(Direction::LEFT) {}

    /**
     * @brief Explicit constructor to define a complete transition rule.
     * Passes heavy objects (States) by value and moves them to eliminate redundant allocations.
     *
     * @param current The required state to trigger this transition.
     * @param next The target state the machine will transition into.
     * @param read The symbol that must be read under the tape head.
     * @param write The symbol to be written onto the tape.
     * @param dir The direction to move the tape head (LEFT or RIGHT).
     */
    explicit Transition(State current, State next, Symbol read, Symbol write, Direction dir)
        : current_state_(std::move(current)), next_state_(std::move(next)), read_symbol_(read),
          write_symbol_(write), dir_(dir)
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
     * @brief Retrieves the symbol that must be present under the tape head.
     * Returning by value is optimal since Symbol is a lightweight, trivially-copyable type.
     *
     * @return Symbol The expected symbol on the tape.
     */
    [[nodiscard]] Symbol get_read_symbol() const { return this->read_symbol_; }

    /**
     * @brief Retrieves the new symbol that will overwrite the current tape cell.
     * @return Symbol The symbol to be written onto the tape.
     */
    [[nodiscard]] Symbol get_write_symbol() const { return this->write_symbol_; }

    /**
     * @brief Retrieves the direction (LEFT or RIGHT) in which the tape head must shift.
     * @return Direction The head movement direction enum value.
     */
    [[nodiscard]] Direction get_direction() const { return this->dir_; }

    /**
     * @brief Defaulted three-way comparison operator (C++20 spaceship operator).
     * Instructs the compiler to automatically generate standard relational
     * operators (<, <=, >, >=, ==, !=) based on the underlying members.
     *
     * @param other The other Transition instance to compare against.
     * @return auto A strong ordering comparison category (std::strong_ordering).
     */
    auto operator<=>(const Transition &other) const = default;

  private:
    State current_state_; ///< Source state for the transition rule.
    State next_state_;    ///< Destination state after execution.
    Symbol read_symbol_;  ///< Expected symbol on the tape.
    Symbol write_symbol_; ///< New symbol to overwrite onto the tape.
    Direction dir_;       ///< Head shift direction.
};

#endif // TRANSITION_HPP
