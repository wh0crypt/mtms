/**
 * @file alphabet.hpp
 * @author wh0crypt (wh0crypt@proton.me)
 * @brief Alphabet class to manage the collection of valid symbols for the Turing Machine.
 * @version 0.1
 * @date 2026-06-27
 *
 * @copyright Copyright (c) 2026 wh0crypt. Licensed under the MIT License.
 */

#ifndef ALPHABET_HPP
#define ALPHABET_HPP

#include "symbol.hpp"

#include <iostream>
#include <set>
#include <string_view>

/**
 * @class Alphabet
 * @brief Represents the formal alphabet (\f$\Sigma\f$ or \f$\Gamma\f$) of a Turing Machine.
 *
 * It stores a unique, ordered set of Symbol instances.
 */
class Alphabet
{
  public:
    /**
     * @brief Default constructor. Initializes an empty alphabet.
     */
    Alphabet() = default;

    /**
     * @brief Explicit constructor to build an Alphabet from a string view.
     * Iterates through the provided string and inserts each unique character as a Symbol.
     *
     * @param str The sequence of characters representing the alphabet symbols.
     */
    explicit Alphabet(std::string_view str) { this->insert(str); }

    /**
     * @brief Explicit constructor to build an Alphabet from an existing set of Symbols.
     * @param symbols A standard set containing the initial Symbols.
     */
    explicit Alphabet(const std::set<Symbol> &symbols) { this->insert(symbols); }

    /**
     * @brief Getter for the underlying standard set of symbols.
     * @return const std::set<Symbol>& A read-only reference to the symbol set.
     */
    [[nodiscard]] const std::set<Symbol> &get_set() const { return this->set_; }

    /**
     * @brief Retrieves the total number of unique symbols currently in the alphabet.
     * @return std::size_t The cardinality of the alphabet set.
     */
    [[nodiscard]] std::size_t size() const { return this->set_.size(); }

    /**
     * @brief Checks if a specific Symbol belongs to this alphabet.
     * @param symbol The Symbol instance to look up.
     * @return true If the symbol is present in the alphabet, false otherwise.
     */
    [[nodiscard]] bool contains(const Symbol &symbol) const { return this->set_.contains(symbol); }

    /**
     * @brief Returns an iterator to the beginning of the alphabet set.
     */
    [[nodiscard]] auto begin() const { return this->set_.begin(); }

    /**
     * @brief Returns an iterator to the end of the alphabet set.
     */
    [[nodiscard]] auto end() const { return this->set_.end(); }

    /**
     * @brief Prints the alphabet in mathematical set notation format (e.g., "{0, 1}").
     * @param os The target output stream (defaults to std::cout).
     */
    void print(std::ostream &os = std::cout) const;

    /**
     * @brief Overloaded stream insertion operator to print the Alphabet object.
     * @param os The target output stream.
     * @param alphabet The Alphabet instance to be streamed.
     * @return std::ostream& Reference to the output stream to allow cascading.
     */
    friend std::ostream &operator<<(std::ostream &os, const Alphabet &alphabet);

  private:
    std::set<Symbol> set_; ///< The unique, sorted collection of symbols composing the alphabet.

    /**
     * @brief Private helper template to genericly process and insert elements into the alphabet.
     * Automatically filters out any `kBlank` symbols and wraps primitive types uniformly.
     *
     * @tparam T A collection type that supports range-based loops (e.g., string_view, std::set).
     * @param data The iterable dataset containing potential symbols.
     */
    template <class T> void insert(const T &data);
};

template <class T> void Alphabet::insert(const T &data)
{
    for (const auto &s : data)
    {
        this->set_.insert(Symbol(s));
    }
}

#endif // ALPHABET_HPP
