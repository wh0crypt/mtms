/**
 * @file project.hpp
 * @author wh0crypt (wh0crypt@proton.me)
 * @brief Session manager controlling serialization pipelines via toml++.
 * @version 0.1
 * @date 2026-06-27
 *
 * @copyright Copyright (c) 2026 wh0crypt. Licensed under the MIT License.
 */

#ifndef PROJECT_HPP
#define PROJECT_HPP

#include "tm.hpp"

#include <filesystem>
#include <memory>
#include <string>
#include <toml++/toml.hpp>
#include <unordered_map>
#include <unordered_set>

#define TOML_HEADER_ONLY 1

/**
 * @class Project
 * @brief Orchestrates configuration ingestion and state saves, abstraction layer
 * bridging core computation with external interfaces (CLI/GUI).
 */
class Project
{
  public:
    Project() noexcept : name_("Untitled TM") {}

    /**
     * @brief Parses a TM configuration file using toml++ and instantiates the TuringMachine.
     * @param filepath Path to the target validation .toml file.
     * @return true If serialization and validation passed smoothly, false otherwise.
     */
    bool load_project(const std::filesystem::path &filepath) noexcept(false);

    /**
     * @brief Exports the current machine configuration snapshot back to a TOML file.
     * @param filepath Output path footprint.
     * @return true On successful file flush.
     */
    [[nodiscard]] bool save_project(const std::filesystem::path &filepath) const noexcept(false);

    /**
     * @brief Checks if there is a valid Turing Machine instance active.
     * @return true If machine_ is not null, false otherwise.
     */
    [[nodiscard]] bool has_active_machine() const noexcept { return this->machine_ != nullptr; }

    /**
     * @brief Retrieves the pointer to the active Turing Machine instance.
     * @return std::shared_ptr<TuringMachine> The shared reference to the machine.
     */
    [[nodiscard]] std::shared_ptr<TuringMachine> get_machine() noexcept { return this->machine_; }

    /**
     * @brief Retrieves the project's friendly configuration name.
     * @return std::string_view View of the project name.
     */
    [[nodiscard]] std::string_view get_name() const noexcept { return this->name_; }

  private:
    std::shared_ptr<TuringMachine> machine_; ///< Active Turing Machine computation engine backend.
    std::string name_;        ///< Friendly identifier extracted from config metadata.
    std::string description_; ///< Project description block or summary.

    /**
     * @brief Extracts project identification block from TOML file.
     * @param config Reference to the loaded TOML table root structure.
     * @return true If extraction succeeded without strict type breaks.
     */
    bool parse_metadata(const toml::table &config) noexcept;

    /**
     * @brief Parses and instantiates structural computational states from TOML configuration array.
     * @param config Reference to the parsed TOML table structure.
     * @param parsed_states Target unique set to populate with active valid states.
     * @param initial_state Output wrapper reference pointing to the set's verified initial state
     * node.
     * @param state_map Reference dictionary mapping text state keys to object values for
     * sub-routine lookup.
     * @return true If states are successfully declared without critical key duplicates.
     */
    static bool parse_states(
        const toml::table &config,
        std::set<State> &parsed_states,
        State &initial_state,
        std::unordered_map<std::string, State> &state_map
    ) noexcept(false);

    /**
     * @brief Validates and builds formal inclusion alphabets (Sigma and Gamma matrices).
     * @param config Reference to the parsed TOML table structure.
     * @param input_alpha Output destination reference holding the parsed Sigma symbols subset.
     * @param tape_alpha Output destination reference holding the parsed Gamma symbols subset.
     * @return true If sets do not breach standard formal rules (e.g., Sigma containing Blank
     * character).
     */
    static bool parse_alphabets(
        const toml::table &config,
        Alphabet &input_alpha,
        Alphabet &tape_alpha
    ) noexcept(false);

    /**
     * @brief Decodes the matrix table containing legal execution step pathways.
     * @param config Reference to the parsed TOML table structure.
     * @param tape_count Dimensional bounds required per sub-array alignment.
     * @param state_map Internal registry to cross-reference mapped text nodes against objects.
     * @param machine Shared pointer to the active Turing Machine instance to populate with
     * transitions.
     * @param graph Adjacency map tracker built to accelerate structural connectivity queries.
     * @return true If arrays successfully map out a valid deterministic runtime framework.
     */
    static bool parse_transitions(
        const toml::table &config,
        std::size_t tape_count,
        const std::unordered_map<std::string, State> &state_map,
        const std::shared_ptr<TuringMachine> &machine,
        std::unordered_map<std::string, std::vector<std::string>> &graph
    ) noexcept(false);

    /**
     * @brief Computes reaching pathways from the start state over the adjacency graph via DFS.
     * @param start_state The initial root label node token to start traversal.
     * @param graph The static structural matrix tracker.
     * @return std::unordered_set<std::string> Set containing labels of all states reached from
     * start.
     */
    static std::unordered_set<std::string> compute_reachability(
        const std::string &start_state,
        const std::unordered_map<std::string, std::vector<std::string>> &graph
    ) noexcept(false);

    /**
     * @brief Verifies structural integrity rules against isolated or broken nodes.
     * @param parsed_states Unique set containing every configured state object.
     * @param reachable Evaluated cluster tracking every accessible node token.
     * @param graph Structural adjacency map tracker.
     * @return true If there are no non-accepting states missing critical processing pathways.
     */
    static bool validate_machine_structure(
        const std::set<State> &parsed_states,
        const std::unordered_set<std::string> &reachable,
        const std::unordered_map<std::string, std::vector<std::string>> &graph
    ) noexcept(false);
};

#endif // PROJECT_HPP
