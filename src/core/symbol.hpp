/**
 * @file symbol.hpp
 * @author wh0crypt (wh0crypt@proton.me)
 * @brief Symbol class to represent the symbols on the Turing Machine's tape.
 * @version 0.1
 * @date 2026-06-27
 *
 * @copyright Copyright (c) 2026 wh0crypt. Licensed under the MIT License.
 */

#ifndef SYMBOL_HPP
#define SYMBOL_HPP

#include <iostream>
#include <ostream>

/**
 * @brief Representation of the default blank symbol used to fill the Turing Machine's tape.
 */
constexpr char kBlank = ' ';

/**
 * @class Symbol
 * @brief Represents a single symbol on the Turing Machine's tape.
 * This class acts as a lightweight wrapper around a primitive `char`. It provides
 * safe comparison mechanics through the C++20 spaceship operator and integrates
 * natively with C++ standard output streams.
 */
class Symbol
{
  public:
    /**
     * @brief Default constructor. Initializes the symbol as a blank/white space.
     */
    Symbol() : char_(kBlank) {}

    /**
     * @brief Explicit constructor to create a Symbol from a raw character.
     * @param symbol The raw character representation to be stored.
     */
    explicit Symbol(char symbol) : char_(symbol) {}

    /**
     * @brief Getter for the underlying raw character.
     * @return char The primitive character stored inside the Symbol object.
     */
    [[nodiscard]] char get_char() const { return this->char_; }

    /**
     * @brief Defaulted three-way comparison operator (C++20 spaceship operator).
     * Instructs the compiler to automatically generate standard relational
     * operators (<, <=, >, >=, ==, !=) based on the underlying char_ member.
     *
     * @param other The other Symbol instance to compare against.
     * @return auto A strong ordering comparison category (std::strong_ordering).
     */
    [[nodiscard]] auto operator<=>(const Symbol &other) const = default;

    /**
     * @brief Defaulted copy assignment operator.
     * Performs a fast, direct bitwise copy of the underlying primitive data.
     *
     * @param other The source Symbol to copy from.
     * @return Symbol& Reference to this instance (*this) to allow chain assignment.
     */
    Symbol &operator=(const Symbol &other) = default;

    /**
     * @brief Prints the symbol's character representation to the specified output stream.
     * @param os The output stream target (defaults to std::cout).
     */
    void print(std::ostream &os = std::cout) const { os << this->char_; }

    /**
     * @brief Overloaded stream insertion operator for native stream integration.
     * Allows syntax like `std::cout << my_symbol;` by forwarding to the print method.
     *
     * @param os The target output stream.
     * @param symbol The Symbol instance to be printed.
     * @return std::ostream& Reference to the output stream to allow cascading.
     */
    friend std::ostream &operator<<(std::ostream &os, const Symbol &symbol);

  private:
    char char_; ///< The primitive character backing this Symbol.
};

/**
 * @brief Global implementation of the stream insertion operator for Symbol.
 * @note Marked as inline to prevent ODR violations across multiple translation units.
 */
inline std::ostream &operator<<(std::ostream &os, const Symbol &symbol)
{
    symbol.print(os);
    return os;
}

#endif // SYMBOL_HPP
