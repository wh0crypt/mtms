/**
 * @file turing_machine.hpp
 * @author wh0crypt (wh0crypt@proton.me)
 * @brief Multi-Tape TuringMachine class orchestrating the parallel automata simulation loops.
 * @version 0.2
 * @date 2026-06-27
 *
 * @copyright Copyright (c) 2026 wh0crypt. Licensed under the MIT License.
 */

#ifndef TURING_MACHINE_HPP
#define TURING_MACHINE_HPP

#include "alphabet.hpp"
#include "state.hpp"
#include "tape.hpp"
#include "transition.hpp"

#include <set>
#include <string_view>
#include <vector>

/**
 * @class TuringMachine
 * @brief The central execution engine representing a deterministic Multi-Tape Turing Machine.
 *
 * Manages an arbitrary number of parallel structural Tapes, dispatching updates
 * simultaneously using synchronized multi-dimensional transition rules.
 */
class TuringMachine
{
  public:
    /**
     * @brief Default constructor. Creates an uninitialized machine.
     */
    TuringMachine() : tape_count_(1) {}

    /**
     * @brief Explicit constructor to fully instantiate a Multi-Tape Turing Machine configuration.
     * @param input_alpha The initial string input validation alphabet (\f$\Sigma\f$).
     * @param tape_alpha The wider tape work alphabet (\f$\Gamma\f$).
     * @param states The complete set of legal operational conditions (Q).
     * @param initial_state The point of origin state where processing starts (\f$q_0\f$).
     * @param transition_table The complete operational rule matrix supporting multi-tape rules
     * (\f$\delta\f$).
     * @param tape_count The total number of parallel tape tracks available (k).
     */
    explicit TuringMachine(
        Alphabet input_alpha,
        Alphabet tape_alpha,
        std::set<State> states,
        State initial_state,
        std::set<Transition> transition_table,
        std::size_t tape_count
    );

    /**
     * @brief Validates an input word against the input alphabet and loads it onto Tape 0.
     *
     * Clears all secondary tracks, filling them with virtual blank context variables.
     *
     * @param input The raw character view representation to parse.
     * @return true If the input string belongs entirely to the input alphabet, false otherwise.
     */
    [[nodiscard]] bool load_input(std::string_view input);

    /**
     * @brief Executes a single computational transition step across all parallel tapes.
     *
     * Queries multi-tape rules matching the active conditions vector layout.
     *
     * @return true If a valid transition was found and executed successfully,
     * false if the machine halted or broke down.
     */
    bool step();

    /**
     * @brief Loops execution triggers calling step() continuously until a halting condition
     * occurs or the safety step counter threshold breaks execution.
     */
    void run();

    /**
     * @brief Retrieves the formal input validation alphabet.
     * @return const Alphabet& A read-only reference to the Sigma (\f$\Sigma\f$) set.
     */
    [[nodiscard]] const Alphabet &get_input_alphabet() const { return this->input_alphabet_; }

    /**
     * @brief Retrieves the wider operational tape workspace alphabet.
     * @return const Alphabet& A read-only reference to the Gamma (\f$\Gamma\f$) set.
     */
    [[nodiscard]] const Alphabet &get_tape_alphabet() const { return this->tape_alphabet_; }

    /**
     * @brief Retrieves the complete formal set of legal operational state conditions.
     * @return const std::set<State>& A read-only reference to the state collection space (Q).
     */
    [[nodiscard]] const std::set<State> &get_states() const { return this->states_; }

    /**
     * @brief Retrieves the factory default initial state configuration.
     * @return const State& A read-only reference to the origin state (\f$q_0\f$).
     */
    [[nodiscard]] const State &get_start_state() const { return this->start_state_; }

    /**
     * @brief Retrieves the active operational state context marker.
     * @return const State& A read-only reference to the current execution state.
     */
    [[nodiscard]] const State &get_current_state() const { return this->current_state_; }

    /**
     * @brief Retrieves the entire collection layout of parallel tape tracks.
     * @return const std::vector<Tape>& A read-only reference to the k-tape data array.
     */
    [[nodiscard]] const std::vector<Tape> &get_tapes() const { return this->tapes_; }

    /**
     * @brief Retrieves the complete multi-tape rule matrix table lookup registry.
     * @return const std::set<Transition>& A read-only reference to the delta (\f$\delta\f$)
     * function set.
     */
    [[nodiscard]] const std::set<Transition> &get_transition_table() const
    {
        return this->transition_table_;
    }

    /**
     * @brief Retrieves the total cardinality of parallel tapes operating inside the machine.
     * @return std::size_t The structural tape multiplier metrics (k).
     */
    [[nodiscard]] std::size_t get_tape_count() const { return this->tape_count_; }

    /**
     * @brief Adds a transition to the machine.
     * @return true If the transition was successfully inserted for the first time,
     * false if the transition was not inserted or already existed.
     */
    bool add_transition(const Transition &transition)
    {
        return this->transition_table_.insert(transition).second;
    }

    /**
     * @brief Removes a transition from the machine.
     * @return true If the transition was successfully erased,
     * false if the transition was not removed.
     */
    bool remove_transition(const Transition &transition)
    {
        return this->transition_table_.erase(transition) != 0;
    }

    /**
     * @brief Checks whether the machine contains a specific transition.
     * @return true If the transition is contained in the machine,
     * false if it is not.
     */
    [[nodiscard]] bool contains_transition(const Transition &transition) const
    {
        return this->transition_table_.contains(transition);
    }

  private:
    Alphabet input_alphabet_; ///< Input validation vocabulary (\f$\Sigma\f$).
    Alphabet tape_alphabet_;  ///< Structural execution workspace vocabulary (\f$\Gamma\f$).
    std::set<State> states_;  ///< Valid configurations collection.
    State start_state_;       ///< Factory default backup tracking state.
    State current_state_;     ///< Active contextual processing marker.
    std::vector<Tape> tapes_; ///< Collection of k dynamic memory layouts.
    std::set<Transition> transition_table_; ///< Lookup index holding multi-tape rules matrices.
    std::size_t tape_count_;                ///< Cardinality multiplier tracking active tapes (k).
};

#endif // TURING_MACHINE_HPP
