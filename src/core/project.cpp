/**
 * @file project_manager.cpp
 * @author wh0crypt (wh0crypt@proton.me)
 * @brief Implementation of TOML parsing logic via toml++.
 * @version 0.1
 * @date 2026-06-27
 *
 * @copyright Copyright (c) 2026 wh0crypt. Licensed under the MIT License.
 */

#include "project.hpp"

#include <fstream>
#include <print>
#include <set>
#include <toml++/toml.hpp>
#include <unordered_map>
#include <vector>

#define TOML_HEADER_ONLY 1

bool Project::load_project(const std::filesystem::path &filepath)
{
    try
    {
        auto config = toml::parse_file(filepath.string());

        // Extract Metadata
        this->name_ = config["metadata"]["name"].value_or("Untitled TM");
        this->description_ = config["metadata"]["description"].value_or("");

        // Get Machine Specs
        std::size_t tape_count = config["machine"]["tape_count"].value_or(1);
        std::string init_state_name = config["machine"]["initial_state"].value_or("");

        // Parse States
        std::set<State> parsed_states;
        State initial_state;
        std::unordered_map<std::string, State> state_map;
        if (auto states_array = config["states"].as_array())
        {
            for (auto &&node : *states_array)
            {
                auto &&table = *node.as_table();
                std::string name = table["name"].value_or("");
                bool is_accept = table["is_accept"].value_or(false);

                State s(name, is_accept);
                if (state_map.contains(name))
                {
                    std::println(stderr, "[!] Duplicate state '{}'.", name);
                    return false;
                }
                parsed_states.insert(s);
                state_map.emplace(name, s);

                if (name == init_state_name) initial_state = s;
            }
        }

        // Parse Input Alphabet (Sigma)
        std::set<Symbol> input_symbols;
        if (auto input_arr = config["machine"]["input_alphabet"].as_array())
        {
            for (auto &&node : *input_arr)
            {
                std::string sym_str = node.value_or("");
                if (!sym_str.empty())
                {
                    Symbol sym(sym_str[0]);

                    // Formal Rule 1: Sigma cannot contain the blank character
                    if (sym.get_char() == kBlank)
                    {
                        std::println(
                            stderr,
                            "[!] Input alphabet (Sigma) cannot contain the blank symbol."
                        );
                        return false;
                    }
                    input_symbols.insert(sym);
                }
            }
        }

        // Parse Tape Alphabet (Gamma)
        std::set<Symbol> tape_symbols;
        if (auto tape_arr = config["machine"]["tape_alphabet"].as_array())
        {
            for (auto &&node : *tape_arr)
            {
                std::string sym_str = node.value_or(" ");
                Symbol sym(sym_str.empty() ? kBlank : sym_str[0]);
                tape_symbols.insert(sym);
            }
        }

        // Make sure Blank symbol is part of Tape Alphabet
        tape_symbols.insert(Symbol(kBlank));

        // Transitions Parsing Matrix
        std::set<Transition> transition_table;
        if (auto trans_array = config["transitions"].as_array())
        {
            for (auto &&node : *trans_array)
            {
                auto &&table = *node.as_table();

                std::string curr_name = table["current_state"].value_or("");
                std::string next_name = table["next_state"].value_or("");

                auto curr_it = state_map.find(curr_name);
                auto next_it = state_map.find(next_name);

                if (curr_it == state_map.end() || next_it == state_map.end())
                {
                    std::println(stderr, "[!] Transition references an undefined state.");
                    return false;
                }

                std::vector<Symbol> read_vec;
                std::vector<Symbol> write_vec;
                std::vector<Direction> dir_vec;

                // Parse read array
                if (auto read_arr = table["read"].as_array())
                {
                    for (auto &&s_node : *read_arr)
                    {
                        std::string sym_str = s_node.value_or(" ");
                        Symbol sym(sym_str.empty() ? kBlank : sym_str[0]);
                        read_vec.push_back(sym);
                        tape_symbols.insert(sym);
                    }
                }

                // Parse write array
                if (auto write_arr = table["write"].as_array())
                {
                    for (auto &&s_node : *write_arr)
                    {
                        std::string sym_str = s_node.value_or(" ");
                        Symbol sym(sym_str.empty() ? kBlank : sym_str[0]);
                        write_vec.push_back(sym);
                        tape_symbols.insert(sym);
                    }
                }

                // Parse direction array
                if (auto dir_arr = table["direction"].as_array())
                {
                    for (auto &&d_node : *dir_arr)
                    {
                        std::string d_str = d_node.value_or("STAY");
                        Direction d = (d_str == "LEFT")    ? Direction::LEFT
                                      : (d_str == "RIGHT") ? Direction::RIGHT
                                                           : Direction::STAY;
                        dir_vec.push_back(d);
                    }
                }

                transition_table.emplace(
                    curr_it->second,
                    next_it->second,
                    std::move(read_vec),
                    std::move(write_vec),
                    std::move(dir_vec)
                );
            }
        }

        // Mathematical Inclusion Validation (Σ ⊂ Γ)
        Alphabet input_alpha(input_symbols);
        Alphabet tape_alpha(tape_symbols);

        for (const auto &sym : input_symbols)
        {
            if (!tape_alpha.contains(sym))
            {
                std::println(
                    stderr,
                    "[!] Input alphabet symbol '{}' is missing from the tape alphabet (Gamma).",
                    sym.get_char()
                );
                return false;
            }
        }

        // Engine Instantiation handover
        this->machine_ = std::make_shared<TuringMachine>(
            input_alpha,
            tape_alpha,
            parsed_states,
            initial_state,
            transition_table,
            tape_count
        );

        return true;
    }
    catch (const toml::parse_error &err)
    {
        std::println(stderr, "[!] TOML parsing failed: {}", err.description());
        return false;
    }
}

bool Project::save_project(const std::filesystem::path &filepath) const
{
    if (!this->has_active_machine()) return false;

    try
    {
        // Build Metadata block
        toml::table root;
        root.insert_or_assign(
            "metadata",
            toml::table{{"name", this->name_}, {"description", this->description_}}
        );

        // Build Machine block mapping configuration markers
        auto machine = this->machine_;
        toml::array input_alpha_arr;
        for (const auto &sym : machine->get_input_alphabet().get_set())
        {
            input_alpha_arr.push_back(std::string(1, sym.get_char()));
        }

        toml::array tape_alpha_arr;
        for (const auto &sym : machine->get_tape_alphabet().get_set())
        {
            tape_alpha_arr.push_back(std::string(1, sym.get_char()));
        }

        root.insert_or_assign(
            "machine",
            toml::table{
                {"tape_count", static_cast<int64_t>(machine->get_tape_count())},
                {"initial_state", machine->get_start_state().get_label()},
                {"input_alphabet", input_alpha_arr},
                {"tape_alphabet", tape_alpha_arr}
            }
        );

        // Serialize States array
        toml::array states_arr;
        for (const auto &state : machine->get_states())
        {
            states_arr.push_back(
                toml::table{{"name", state.get_label()}, {"is_accept", state.is_accept()}}
            );
        }
        root.insert_or_assign("states", states_arr);

        // Serialize Transitions array matrix
        toml::array trans_arr;
        for (const auto &trans : machine->get_transition_table())
        {
            toml::array read_sub_arr;
            for (const auto &sym : trans.get_read_symbols())
            {
                read_sub_arr.push_back(std::string(1, sym.get_char()));
            }

            toml::array write_sub_arr;
            for (const auto &sym : trans.get_write_symbols())
            {
                write_sub_arr.push_back(std::string(1, sym.get_char()));
            }

            toml::array dir_sub_arr;
            for (const auto &dir : trans.get_directions())
            {
                std::string d_str = (dir == Direction::LEFT)    ? "LEFT"
                                    : (dir == Direction::RIGHT) ? "RIGHT"
                                                                : "STAY";
                dir_sub_arr.push_back(d_str);
            }

            trans_arr.push_back(
                toml::table{
                    {"current_state", trans.get_current_state().get_label()},
                    {"next_state", trans.get_next_state().get_label()},
                    {"read", read_sub_arr},
                    {"write", write_sub_arr},
                    {"direction", dir_sub_arr}
                }
            );
        }
        root.insert_or_assign("transitions", trans_arr);

        // Open stream pipeline and dump the structured tables
        std::ofstream file(filepath, std::ios::out | std::ios::trunc);
        if (!file.is_open()) return false;

        file << root << "\n";
        return true;
    }
    catch (const std::exception &e)
    {
        std::println(stderr, "[!] TOML saving failed: {}", e.what());
        return false;
    }
}
