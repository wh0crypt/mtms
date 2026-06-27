/**
 * @file main.cpp
 * @brief Dedicated entry point for the mtms-cli executable binary.
 * @version 0.2
 * @date 2026-06-27
 *
 * @copyright Copyright (c) 2026 wh0crypt. Licensed under the MIT License.
 */

#include "../core/project.hpp"
#include "cli.hpp"
#include "program.hpp"

#include <cstdlib>
#include <print>

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        Program::print_usage();
        return EXIT_FAILURE;
    }

    Program::Options options;
    Program::parse_args(argc, argv, options);

    if (options.show_help)
    {
        Program::print_help();
        return EXIT_SUCCESS;
    }

    if (options.err_code != EXIT_SUCCESS)
    {
        std::println(stderr, "\033[1;31m[!] {}\033[0m", options.err_msg);
        return options.err_code;
    }

    Project project;
    if (!project.load_project(options.config_file))
    {
        std::println(
            stderr,
            "\033[1;31m[!] Blueprint compilation or validation failed for: {}\033[0m",
            options.config_file
        );
        return EXIT_FAILURE;
    }

    auto machine_ptr = project.get_machine();
    int exit_code = run_cli_session(*machine_ptr, options.input, !options.batch_exec);

    if (exit_code == EXIT_SUCCESS && !options.output_file.empty())
    {
        std::println("[-] Exporting current blueprint layout to {}...", options.output_file);
        if (project.save_project(options.output_file))
        {
            std::println("\033[1;32m[*] Project successfully exported to disk.\033[0m");
        }
        else
        {
            std::println(
                stderr,
                "\033[1;31m[!] Serialization pipeline failed to write destination file.\033[0m"
            );
        }
    }

    return exit_code;
}
