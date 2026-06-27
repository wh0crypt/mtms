/**
 * @file string.cpp
 * @author wh0crypt (wh0crypt@proton.me)
 * @brief Implementation of the formal String class methods.
 * @version 0.1
 * @date 2026-06-27
 *
 * @copyright Copyright (c) 2026 wh0crypt. Licensed under the MIT License.
 */

#include "string.hpp"

#include <algorithm>

/**
 * @brief Custom constructor to morph a raw character stream view into a formal sequence.
 *
 * Leverages vector internal resizing strategies via sequential emplace_back pipeline pushes.
 */
String::String(std::string_view str)
{
    this->symbols_.reserve(str.size());
    for (const auto &ch : str)
    {
        this->symbols_.emplace_back(ch);
    }
}

/**
 * @brief Serializes the formal layout array back into a standard standard library string.
 *
 * Optimizes performance overhead by pre-allocating heap capacity tokens via reserve().
 */
std::string String::get_str() const
{
    std::string result;
    result.reserve(this->symbols_.size());

    for (const auto &sym : this->symbols_)
    {
        result.push_back(sym.get_char());
    }

    return result;
}

/**
 * @brief Predicate logic pipeline evaluating structural correctness against an Alphabet.
 *
 * Evaluates short-circuit logical breaks using standard library ranges algorithms.
 */
bool String::is_valid_for(const Alphabet &alphabet) const
{
    return std::ranges::all_of(this->symbols_, [&alphabet](const Symbol &s) {
        return alphabet.contains(s);
    });
}
