/**
 * @file alphabet.cpp
 * @author wh0crypt (wh0crypt@proton.me)
 * @brief Implementation of the structural Alphabet class methods.
 * @version 0.1
 * @date 2026-06-27
 *
 * @copyright Copyright (c) 2026 wh0crypt. Licensed under the MIT License.
 *
 */

#include "alphabet.hpp"

#include <ranges>

/**
 * @brief Serializes the sorted collection layout using mathematical format markers.
 * Leverages C++20/C++23 structural views pipelines to enforce precise delimiter spacing.
 */
void Alphabet::print(std::ostream &os) const
{
    os << "{";
    for (auto [idx, s] : std::views::enumerate(this->set_))
    {
        s.print(os);
        if (idx + 1 < this->size())
        {
            os << ", ";
        }
    }
    os << "}";
}

/**
 * @brief Direct pipeline bridge forwarding stream requests toward the internal print interface.
 */
std::ostream &operator<<(std::ostream &os, const Alphabet &alphabet)
{
    alphabet.print(os);
    return os;
}
