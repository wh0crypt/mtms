/**
 * @file cli.cpp
 * @author wh0crypt (wh0crypt@proton.me)
 * @brief Implementation of the interactive CLI session loops.
 * @version 0.1
 * @date 2026-06-27
 *
 * @copyright Copyright (c) 2026 wh0crypt. Licensed under the MIT License.
 *
 */

#include "cli.hpp"

#include <cstdlib>
#include <iostream>
#include <print>

/**
 * @brief Flushes the active terminal frame using standard ANSI escape codes.
 *
 * Highly performant viewport reset preventing rendering flickering sequences.
 */
inline void clear_viewport() { std::print("\033[2J\033[1;1H"); }

/**
 * @brief Renders the consolidated simulation status metrics and stacked tapes layout.
 */
void render_dashboard(const TuringMachine &machine, int steps)
{
    clear_viewport();
    std::print(
        "==================================================\n"
        "      MODERN TURING MACHINE SIMULATOR (mtms)      \n"
        "==================================================\n"
        " Current State : [{}]\n"
        " Steps Executed: {}\n"
        "--------------------------------------------------\n\n",
        machine.get_current_state().get_label(),
        steps
    );

    // Cascade rendering streams across all structurally active tapes
    const auto &tapes = machine.get_tapes();
    for (std::size_t i = 0; i < tapes.size(); ++i)
    {
        std::println(" [Tape {}] ", i);
        tapes[i].print(std::cout);
        std::println("");
    }
    std::println("==================================================");
}

int run_cli_session(TuringMachine &machine, std::string_view input_word, bool interactive)
{
    // Bootstrap validation pipeline over input alphabet parameters
    if (!machine.load_input(input_word))
    {
        std::println(
            stderr,
            "\n\033[1;31m[!] Input string contains symbols outside the validation alphabet."
        );
        return EXIT_FAILURE;
    }

    int step_counter = 0;

    // Interactive Mode
    if (interactive)
    {
        render_dashboard(machine, step_counter);
        std::print("\n Press [ENTER] to initialize computation step sequences...");
        std::cin.ignore();

        while (machine.step())
        {
            render_dashboard(machine, ++step_counter);
            std::print("\n Press [ENTER] to advance execution ticks...");
            std::cin.get();
        }

        render_dashboard(machine, step_counter);
        std::print("\n Execution Halted. ");
        std::println(
            "{}",
            machine.get_current_state().is_accept()
                ? "\033[1;32m[STRING ACCEPTED]\033[0m"
                : "\033[1;31m[STRING REJECTED / HALT CRASH]\033[0m"
        );
        std::println("==================================================");
    }
    // Batch Mode
    else
    {
        while (machine.step()) ++step_counter;

        if (machine.get_current_state().is_accept())
        {
            std::println(
                "STATUS=ACCEPTED;STEPS={};FINAL_STATE={}",
                step_counter,
                machine.get_current_state().get_label()
            );
        }
        else
        {
            std::println(
                "STATUS=REJECTED;STEPS={};FINAL_STATE={}",
                step_counter,
                machine.get_current_state().get_label()
            );
            return EXIT_FAILURE;
        }
    }

    return EXIT_SUCCESS;
}
