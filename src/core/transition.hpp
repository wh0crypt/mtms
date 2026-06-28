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
     * @param id Unique runtime identifier for UI and graph edge tracking.
     * @param current The required state to trigger this transition.
     * @param next The target state the machine will transition into.
     * @param read The collection of symbols expected under each parallel tape head.
     * @param write The collection of new symbols to overwrite onto each tape track.
     * @param dir The collection of displacement directions for each tape head.
     */
    explicit Transition(
        std::size_t id,
        State current,
        State next,
        std::vector<Symbol> read,
        std::vector<Symbol> write,
        std::vector<Direction> dir
    ) noexcept
        : id_(id), current_state_(std::move(current)), next_state_(std::move(next)),
          read_symbols_(std::move(read)), write_symbols_(std::move(write)),
          dir_vectors_(std::move(dir))
    {}

    /**
     * @brief Retrieves the unique identifier of this transition rule.
     * Useful for persistent mapping in GUI elements, undo/redo stacks, and graph edge binding.
     */
    [[nodiscard]] std::size_t get_id() const noexcept { return this->id_; }

    /**
     * @brief Retrieves the source state required to trigger this transition.
     * @return const State& A read-only reference to the current execution state.
     */
    [[nodiscard]] const State &get_current_state() const noexcept { return this->current_state_; }

    /**
     * @brief Retrieves the target state the machine will enter after executing this rule.
     * @return const State& A read-only reference to the next execution state.
     */
    [[nodiscard]] const State &get_next_state() const noexcept { return this->next_state_; }

    /**
     * @brief Retrieves the collection of symbols that must be present under the tape heads.
     * @return const std::vector<Symbol>& A read-only reference to the expected symbols vector.
     */
    [[nodiscard]] const std::vector<Symbol> &get_read_symbols() const noexcept
    {
        return this->read_symbols_;
    }

    /**
     * @brief Retrieves the new symbols that will overwrite the current parallel tape cells.
     * @return const std::vector<Symbol>& A read-only reference to the symbols to be written.
     */
    [[nodiscard]] const std::vector<Symbol> &get_write_symbols() const noexcept
    {
        return this->write_symbols_;
    }

    /**
     * @brief Retrieves the directions in which each parallel tape head must shift.
     * @return const std::vector<Direction>& A read-only reference to the head movement vectors.
     */
    [[nodiscard]] const std::vector<Direction> &get_directions() const noexcept
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
    [[nodiscard]] bool operator==(const Transition &other) const noexcept
    {
        return this->current_state_ == other.current_state_ &&
               this->read_symbols_ == other.read_symbols_;
    }

    /**
     * @brief Lexicographical ordering of transitions for container storage.
     *
     * @warning This ordering is purely structural and does NOT represent
     * mathematical equivalence of transitions in the Turing Machine definition.
     */
    [[nodiscard]] auto operator<=>(const Transition &other) const noexcept
    {
        if (auto cmp = this->current_state_ <=> other.current_state_; cmp != nullptr)
        {
            return cmp;
        }
        return this->read_symbols_ <=> other.read_symbols_;
    }

    /**
     * @brief Modifies the transition's destination state.
     */
    void set_next_state(State next) noexcept { this->next_state_ = std::move(next); }

    /**
     * @brief Modifies the whole reading symbols block (k-tapes).
     */
    void set_read_symbols(std::vector<Symbol> read) noexcept
    {
        this->read_symbols_ = std::move(read);
    }

    /**
     * @brief Modifies the whole writing symbols block (k-tapes).
     */
    void set_write_symbols(std::vector<Symbol> write) noexcept
    {
        this->write_symbols_ = std::move(write);
    }

    /**
     * @brief Modifies the heads' movement direction.
     */
    void set_directions(std::vector<Direction> dir) noexcept
    {
        this->dir_vectors_ = std::move(dir);
    }

    /**
     * @brief Allows granular modification of a specific tape cell.
     * @throw std::out_of_range If the tape index is invalid.
     */
    void set_read_symbol_at(std::size_t tape_idx, const Symbol &sym) noexcept(false)
    {
        if (tape_idx >= this->read_symbols_.size())
        {
            throw std::out_of_range("Tape index out of bounds for read_symbols_");
        }
        this->read_symbols_[tape_idx] = sym;
    }

    /**
     * @brief Allows granular modification of a specific tape cell.
     * @throw std::out_of_range If the tape index is invalid.
     */
    void set_write_symbol_at(std::size_t tape_idx, const Symbol &sym) noexcept(false)
    {
        if (tape_idx >= this->write_symbols_.size())
        {
            throw std::out_of_range("Tape index out of bounds for write_symbols_");
        }
        this->write_symbols_[tape_idx] = sym;
    }

    /**
     * @brief Allows granular modification of a specific tape cell.
     * @throw std::out_of_range If the tape index is invalid.
     */
    void set_direction_at(std::size_t tape_idx, const Direction &dir) noexcept(false)
    {
        if (tape_idx >= this->dir_vectors_.size())
        {
            throw std::out_of_range("Tape index out of bounds for dir_vectors_");
        }
        this->dir_vectors_[tape_idx] = dir;
    }

  private:
    std::size_t id_{0};   ///< Unique runtime identifier for UI and graph edge tracking.
    State current_state_; ///< Source state for the transition rule.
    State next_state_;    ///< Destination state after execution.
    std::vector<Symbol> read_symbols_;   ///< Expected vector array of symbols on the tracks.
    std::vector<Symbol> write_symbols_;  ///< New vector array of symbols to write.
    std::vector<Direction> dir_vectors_; ///< Structural head shift directions vector.
};

#endif // TRANSITION_HPP
