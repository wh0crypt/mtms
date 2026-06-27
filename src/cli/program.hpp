/**
 * @file program.hpp
 * @brief Dedicated header for the main CLI functions.
 * @version 0.1
 * @date 2026-06-27
 *
 * @copyright Copyright (c) 2026 wh0crypt. Licensed under the MIT License.
 */

#include <cstdlib>
#include <string>

namespace Program
{
struct Options
{
    bool batch_exec = false;
    bool show_help = false;
    int err_code = EXIT_SUCCESS;
    std::string err_msg;
    std::string config_file;
    std::string output_file;
    std::string input;
};

void print_usage();
void print_help();
void parse_args(int argc, char* argv[], Program::Options &options);
}; // namespace Program
