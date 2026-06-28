/**
 * @file turing_machine.cpp
 * @author wh0crypt (wh0crypt@proton.me)
 * @brief Implementation of the TuringMachine engine logic.
 * @version 0.1
 * @date 2026-06-27
 *
 * @copyright Copyright (c) 2026 wh0crypt. Licensed under the MIT License.
 */

#include "tm.hpp"

#include "string.hpp"

#include <print>

constexpr int kMaxSteps = 10000;

/**
 * @brief Explicit constructor to fully instantiate a Multi-Tape Turing Machine configuration.
 */
TuringMachine::TuringMachine(
    Alphabet input_alpha,
    Alphabet tape_alpha,
    std::set<State> states,
    State initial_state,
    std::size_t tape_count
) noexcept(false)
    : input_alphabet_(std::move(input_alpha)), tape_alphabet_(std::move(tape_alpha)),
      states_(std::move(states)), start_state_(initial_state),
      current_state_(std::move(initial_state)), tape_count_(tape_count)
{
    // Initialize the vector with 'k' empty tapes
    this->tapes_.resize(this->tape_count_);
}

/**
 * @brief Validates input string and loads it exclusively onto the FIRST tape (Tape 0).
 * All secondary work tapes (1 to k-1) remain empty (filled with virtual kBlank).
 */
bool TuringMachine::load_input(std::string_view input) noexcept(false)
{
    String str(input);
    if (!str.is_valid_for(this->input_alphabet_))
    {
        return false;
    }

    this->tapes_.assign(this->tape_count_, Tape());
    this->tapes_[0] = Tape(str);
    this->current_state_ = this->start_state_;
    return true;
}

/**
 * @brief Executes a single computational transition step across ALL parallel tapes.
 *
 * Scans symbols from every tape head, matches the collective tuple against the
 * multi-tape transition matrix, and updates all tracks simultaneously.
 */
bool TuringMachine::step() noexcept(false)
{
    if (this->current_state_.is_accept())
    {
        return false;
    }

    // Gather the currently read symbols from all parallel tape heads
    std::vector<Symbol> current_read;
    current_read.reserve(this->tape_count_);
    for (const auto &tape : this->tapes_)
    {
        current_read.push_back(tape.read());
    }

    // Search for a matching transition rule based on the current state and read symbols
    const Transition *next_step_rule = nullptr;
    for (const auto &[id, transition] : this->transitions_)
    {
        if (transition.get_current_state() == this->current_state_ &&
            transition.get_read_symbols() == current_read)
        {
            next_step_rule = &transition;
            break; // Found (DMT assumes a unique valid coincidence)
        }
    }

    // If no multi-tape rule matches the current machine vector layout, halt (Crash/Reject)
    if (next_step_rule == nullptr)
    {
        return false;
    }

    // Apply side-effects simultaneously to all affected tracks using the found rule
    for (std::size_t i = 0; i < this->tape_count_; ++i)
    {
        this->tapes_[i].write(next_step_rule->get_write_symbols()[i]);
        this->tapes_[i].move(next_step_rule->get_directions()[i]);
    }

    this->current_state_ = next_step_rule->get_next_state();
    return true;
}

/**
 * @brief Operational runtime loop triggering sequential computation steps.
 *
 * Integrates a safety threshold loop counter (kMaxSteps) to force-halt execution
 * and prevent infinite runtime hanging situations inside the sandbox environment.
 */
void TuringMachine::run() noexcept(false)
{
    int step_counter = 0;
    while (this->step())
    {
        if (++step_counter >= kMaxSteps)
        {
            std::println(
                stderr,
                "[-] Execution forced-halted. Exceeded max security step limit ({} cycles).",
                kMaxSteps
            );
            break;
        }
    }
}
