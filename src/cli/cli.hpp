/**
 * @file cli.hpp
 * @author wh0crypt (wh0crypt@proton.me)
 * @brief Header definition for the interactive CLI dashboard interface.
 * @version 0.1
 * @date 2026-06-27
 *
 * @copyright Copyright (c) 2026 wh0crypt. Licensed under the MIT License.
 */

#ifndef CLI_HPP
#define CLI_HPP

#include "../core/tm.hpp"

#include <string_view>

/**
 * @brief Executes the simulation via the terminal view layer.
 * @param machine The initialized Multi-Tape Turing Machine engine instance.
 * @param input_word The raw string sequence to process.
 * @param interactive If true, steps interactively. If false, executes instantly (batch mode).
 * @return int Exit status code (EXIT_SUCCESS or EXIT_FAILURE).
 */
int run_cli_session(TuringMachine &machine, std::string_view input_word, bool interactive);

#endif // CLI_HPP
