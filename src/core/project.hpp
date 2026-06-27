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

/**
 * @class Project
 * @brief Orchestrates configuration ingestion and state saves, abstraction layer
 * bridging core computation with external interfaces (CLI/GUI).
 */
class Project
{
  public:
    Project() : name_("Untitled TM") {}

    /**
     * @brief Parses a TM configuration file using toml++ and instantiates the TuringMachine.
     * @param filepath Path to the target validation .toml file.
     * @return true If serialization and validation passed smoothly, false otherwise.
     */
    bool load_project(const std::filesystem::path &filepath);

    /**
     * @brief Exports the current machine configuration snapshot back to a TOML file.
     * @param filepath Output path footprint.
     * @return true On successful file flush.
     */
    [[nodiscard]] bool save_project(const std::filesystem::path &filepath) const;

    // Execution Bridge accessors
    [[nodiscard]] bool has_active_machine() const { return this->machine_ != nullptr; }
    [[nodiscard]] std::shared_ptr<TuringMachine> get_machine() { return this->machine_; }
    [[nodiscard]] std::string_view get_name() const { return this->name_; }

  private:
    std::shared_ptr<TuringMachine> machine_;
    std::string name_;
    std::string description_;
};

#endif // PROJECT_HPP
