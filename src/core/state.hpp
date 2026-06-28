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
    State() noexcept : is_accept_(false) {}

    /**
     * @brief Explicit constructor to create a named State.
     * Takes the label by value and moves it internally to optimize memory allocations.
     *
     * @param label The unique string identifier for this state (e.g., "q0", "init").
     * @param is_accept Boolean flag indicating if this is an accepting/final state.
     */
    explicit State(std::string label, bool is_accept) noexcept(false)
        : label_(std::move(label)), is_accept_(is_accept)
    {}

    /**
     * @brief Getter for the state's unique label.
     * Returning a std::string_view prevents unnecessary string copies.
     *
     * @return std::string_view A view of the state's internal name string.
     */
    [[nodiscard]] std::string_view get_label() const noexcept { return this->label_; }

    /**
     * @brief Checks if this state is an accepting (final) state.
     * @return true If the machine should halt and accept when entering this state, false otherwise.
     */
    [[nodiscard]] bool is_accept() const noexcept { return this->is_accept_; }

    /**
     * @brief Compares two states for equality using only their unique identifier.
     *
     * Within the formal definition of a Turing Machine, each state is uniquely
     * identified by its label. Therefore, the acceptance flag is intentionally
     * ignored during equality comparisons.
     *
     * @param other The state to compare against.
     * @return true if both states share the same label, false otherwise.
     */
    [[nodiscard]] bool operator==(const State &other) const noexcept
    {
        return this->label_ == other.label_;
    }

    /**
     * @brief Provides lexicographical ordering based solely on the state's label.
     *
     * This ordering allows State objects to be stored in ordered associative
     * containers such as std::set while preserving the mathematical uniqueness
     * of states by their identifier.
     *
     * @param other The state to compare against.
     * @return std::strong_ordering The ordering relationship between both labels.
     */
    [[nodiscard]] auto operator<=>(const State &other) const noexcept
    {
        return this->label_ <=> other.label_;
    }

  private:
    std::string label_; ///< The unique identifier name of the state.
    bool is_accept_;    ///< Flag indicating if this is a final/halting state.
};

#endif // STATE_HPP
