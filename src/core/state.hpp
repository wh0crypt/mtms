/**
 * @file state.hpp
 * @author wh0crypt (wh0crypt@proton.me)
 * @brief State class to represent a state within the Turing Machine's finite control.
 * @version 0.1
 * @date 2026-06-27
 *
 * @copyright Copyright (c) 2026 wh0crypt. Licensed under the MIT License.
 */

#ifndef STATE_HPP
#define STATE_HPP

#include <string>
#include <string_view>
#include <utility>

/**
 * @class State
 * @brief Represents a single state in the finite set of states (Q) of the Turing Machine.
 *
 * This class stores the unique identifier (label) of the state and determines
 * whether it acts as an accepting/halting condition for the machine's execution.
 */
class State
{
  public:
    /**
     * @brief Default constructor. Initializes an empty state that is not accepting.
     */
    State() : is_accept_(false) {}

    /**
     * @brief Explicit constructor to create a named State.
     * Takes the label by value and moves it internally to optimize memory allocations.
     *
     * @param label The unique string identifier for this state (e.g., "q0", "init").
     * @param is_accept Boolean flag indicating if this is an accepting/final state.
     */
    explicit State(std::string label, bool is_accept)
        : label_(std::move(label)), is_accept_(is_accept)
    {}

    /**
     * @brief Getter for the state's unique label.
     * Returning a std::string_view prevents unnecessary string copies.
     *
     * @return std::string_view A view of the state's internal name string.
     */
    [[nodiscard]] std::string_view get_label() const { return this->label_; }

    /**
     * @brief Checks if this state is an accepting (final) state.
     * @return true If the machine should halt and accept when entering this state, false otherwise.
     */
    [[nodiscard]] bool is_accept() const { return this->is_accept_; }

    /**
     * @brief Defaulted three-way comparison operator (C++20 spaceship operator).
     * Instructs the compiler to automatically generate standard relational
     * operators (<, <=, >, >=, ==, !=) based on the underlying label_ member.
     *
     * @param other The other State instance to compare against.
     * @return auto A strong ordering comparison category (std::strong_ordering).
     */
    auto operator<=>(const State &other) const = default;

  private:
    std::string label_; ///< The unique identifier name of the state.
    bool is_accept_;    ///< Flag indicating if this is a final/halting state.
};

#endif // STATE_HPP
