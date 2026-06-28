/**
 * @file tape.cpp
 * @author wh0crypt (wh0crypt@proton.me)
 * @brief Implementation of the Tape class methods for the Turing Machine.
 * @version 0.1
 * @date 2026-06-27
 *
 * @copyright Copyright (c) 2026 wh0crypt. Licensed under the MIT License.
 */

#include "tape.hpp"

#include <algorithm>
#include <ranges>

/**
 * @brief Custom constructor to load a formal Word into the machine's memory.
 * Uses C++20 views::enumerate to extract both the structural index and the
 * Symbol object, populating the map from coordinate 0 onward.
 */
Tape::Tape(String str) : head_pos_(0)
{
    for (auto [idx, symbol] : std::views::enumerate(str))
    {
        // Cast formal syntax index to signed integer to prevent underflow on shifts
        this->cells_[static_cast<int>(idx)] = symbol;
    }
}

/**
 * @brief Scans the active cell under the simulation head.
 *
 * Safeguards the infinite tape constraint by looking up coordinates via std::map::find.
 * If the element is missing, it dynamically rolls a virtual kBlank context.
 */
Symbol Tape::read() const
{
    auto it = this->cells_.find(this->head_pos_);
    if (it == this->cells_.end())
    {
        return Symbol(kBlank);
    }
    return it->second;
}

/**
 * @brief Renders a structural blueprint of the active tape cells to any std::ostream.
 * Automatically wraps current track margins based on the dynamic map bounds and the
 * current hardware head displacement coordinates.
 */
void Tape::print(std::ostream &os) const
{
    os << "Tape: ";

    int min_idx = std::min(0, this->head_pos_);
    int max_idx = std::max(0, this->head_pos_);

    // If there is data, expand limits to show the whole string
    if (!this->cells_.empty())
    {
        min_idx = std::min(min_idx, this->cells_.begin()->first);
        max_idx = std::max(max_idx, this->cells_.rbegin()->first);
    }

    // Row 1: Render structural tape cell elements sequentially
    for (int i = min_idx; i <= max_idx; ++i)
    {
        auto it = this->cells_.find(i);
        Symbol s = (it != this->cells_.end()) ? it->second : Symbol(kBlank);
        os << "| " << s << " ";
    }
    os << "|\n";

    // Row 2: Render alignment tracks and drop the head tracking marker (^)
    os << "      "; // Absolute padding to sync with the "Tape: " prefix layout
    for (int i = min_idx; i <= max_idx; ++i)
    {
        if (i == this->head_pos_)
        {
            os << "  ^ ";
            break; // Break execution early: markers past the head track are non-constructive
        }
        os << "    "; // Pad alignment spacing proportional to cell size metrics ("| x ")
    }
    os << "\n";
}

/**
 * @brief Global stream insertion operator overload.
 *
 * Seamlessly pipes the custom Tape print layout directly into standard I/O pipelines.
 */
std::ostream &operator<<(std::ostream &os, const Tape &tape)
{
    tape.print(os);
    return os;
}
