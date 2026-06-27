/**
 * @file string.hpp
 * @author wh0crypt (wh0crypt@proton.me)
 * @brief String class representing a formal string (sequence of symbols) over an alphabet.
 * @version 0.1
 * @date 2026-06-27
 *
 * @copyright Copyright (c) 2026 wh0crypt. Licensed under the MIT License.
 */

#ifndef STRING_HPP
#define STRING_HPP

#include "alphabet.hpp"
#include "symbol.hpp"

#include <string>
#include <string_view>
#include <vector>

/**
 * @class String
 * @brief Represents a formal string (w) consisting of a finite sequence of Symbols.
 *
 * This class encapsulates a collection of symbols, providing validation tools
 * against formal alphabets and standard iterator access for processing.
 */
class String
{
  public:
    /**
     * @brief Default constructor. Initializes an empty string with no symbols.
     */
    String() = default;

    /**
     * @brief Constructs a single-symbol string.
     * @param symbol The initial Symbol to wrap.
     */
    explicit String(const Symbol &symbol) { this->symbols_.emplace_back(symbol); }

    /**
     * @brief Constructs a formal String from a raw string view.
     * @param str The raw character sequence.
     */
    explicit String(std::string_view str);

    /**
     * @brief Constructs a String by taking ownership of an existing vector of symbols.
     *
     * Uses pass-by-value and std::move to prevent redundant overhead dynamic allocations.
     *
     * @param vec The source vector of symbols.
     */
    explicit String(std::vector<Symbol> vec) : symbols_(std::move(vec)) {}

    /**
     * @brief Returns the total number of symbols contained in the string.
     * @return std::size_t The length of the formal string.
     */
    [[nodiscard]] std::size_t length() const { return this->symbols_.size(); }

    /**
     * @brief Reconstructs a standard std::string representation from the underlying symbols.
     * @return std::string A standard string concatenation of all wrapped symbols.
     */
    [[nodiscard]] std::string get_str() const;

    /**
     * @brief Returns an iterator to the beginning of the symbol sequence.
     * Enables range-based loops directly on String instances.
     */
    [[nodiscard]] auto begin() const { return this->symbols_.begin(); }

    /**
     * @brief Returns an iterator to the end of the symbol sequence.
     */
    [[nodiscard]] auto end() const { return this->symbols_.end(); }

    /**
     * @brief Validates if all symbols within this string belong to a specific formal Alphabet.
     *
     * Uses C++20 ranges to perform a clean and optimized all-of predicate check.
     *
     * @param alphabet The formal Alphabet descriptor to validate against.
     * @return true If every single symbol is registered in the alphabet, false otherwise.
     */
    [[nodiscard]] bool is_valid_for(const Alphabet &alphabet) const;

    /**
     * @brief Defaulted three-way comparison operator (C++20 spaceship operator).
     *
     * Instructs the compiler to automatically generate standard relational
     * operators (<, <=, >, >=, ==, !=) based on the underlying symbols_ member.
     *
     * @param other The other String instance to compare against.
     * @return auto A strong ordering comparison category (std::strong_ordering).
     */
    auto operator<=>(const String &other) const = default;

  private:
    std::vector<Symbol> symbols_; ///< The finite sequence of symbols forming the string.
};

#endif // STRING_HPP
