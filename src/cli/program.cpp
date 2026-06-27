/**
 * @file program.cpp
 * @brief Implementation of the main CLI functions.
 * @version 0.1
 * @date 2026-06-27
 *
 * @copyright Copyright (c) 2026 wh0crypt. Licensed under the MIT License.
 */

#include "program.hpp"

#include <cstdlib>
#include <filesystem>
#include <print>
#include <vector>

void Program::print_usage()
{
    std::print(
        "Usage: mtms-cli [-h|--help] -c <CONFIG> -i <INPUT> [-b|--batch] [-o <OUTPUT>]\n"
        "Try 'mtms-cli --help' for more information.\n"
    );
}

void Program::print_help()
{
    std::print(
        "mtms-cli -- a Modern Turing Machine Simulator for CLI.\n\n"
        "Usage: mtms-cli [options]\n\n"
        "OPTIONS:\n"
        "  -c, --config <path>   Path to the machine configuration TOML file (Required)\n"
        "  -i, --input <string>  The input string to load onto Tape 0 (Required)\n"
        "  -b, --batch           Run in non-interactive batch mode (Outputs raw summary result)\n"
        "  -o, --output <path>   Export the loaded/modified configuration back to a TOML file\n"
        "  -h, --help            Display this help instructions menu\n\n"
        "EXAMPLES:\n"
        "  mtms-cli --config examples/bin_inc.toml --input \"1011\" --batch\n"
        "  mtms-cli -c examples/bin_inc.toml -i \"0111\" -b\n"
    );
}

void Program::parse_args(int argc, char *argv[], Program::Options &options)
{
    std::vector<std::string_view> args(argv + 1, argv + argc);

    for (auto it = args.begin(), end = args.end(); it != end && !options.show_help; ++it)
    {
        if (*it == "-h" || *it == "--help")
        {
            options.show_help = true;
            return;
        }

        if (*it == "-b" || *it == "--batch")
        {
            options.batch_exec = true;
            continue;
        }

        if (*it == "-c" || *it == "--config")
        {
            if (++it == end)
            {
                options.err_msg = "Missing config file parameter.";
                options.err_code = EXIT_FAILURE;
                return;
            }

            std::filesystem::path path = *it;
            if (path.extension() != ".toml")
            {
                options.err_code = EXIT_FAILURE;
                options.err_msg = "Config file must be a TOML file.\n";
                return;
            }
            options.config_file = *it;
            continue;
        }

        if (*it == "-o" || *it == "--output")
        {
            if (++it == end)
            {
                options.err_msg = "Missing output file parameter.";
                options.err_code = EXIT_FAILURE;
                return;
            }

            std::filesystem::path path = *it;
            if (path.extension() != ".toml") path.replace_extension(".toml");
            options.output_file = path.string();
            continue;
        }

        if (*it == "-i" || *it == "--input")
        {
            if (++it == end)
            {
                options.err_msg = "Missing input string parameter.";
                options.err_code = EXIT_FAILURE;
                return;
            }
            options.input = *it;
            continue;
        }

        options.err_msg = "Unknown or invalid parameter option rule constraint '" +
                          std::string(*it) + "' passed.";
        options.err_code = EXIT_FAILURE;
        return;
    }

    if (!options.show_help && (options.config_file.empty() || options.input.empty()))
    {
        options.err_code = EXIT_FAILURE;
        options.err_msg = "Missing mandatory options (--config and --input are "
                          "required).";
        return;
    }
}
