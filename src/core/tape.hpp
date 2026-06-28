/**
 * @file tape.hpp
 * @author wh0crypt (wh0crypt@proton.me)
 * @brief Tape class representing the infinite memory structure of the Turing Machine.
 * @version 0.1
 * @date 2026-06-27
 *
 * @copyright Copyright (c) 2026 wh0crypt. Licensed under the MIT License.
 */

#ifndef TAPE_HPP
#define TAPE_HPP

#include "direction.hpp"
#include "string.hpp"
#include "symbol.hpp"

#include <iostream>
#include <map>
#include <ostream>

/**
 * @class Tape
 * @brief Simulates the bilateral infinite tape layout of a Turing Machine.
 *
 * Uses a dynamic std::map indexed by signed integers to support infinite expansion
 * toward both negative and positive coordinates with logarithmic lookup overhead.
 */
class Tape
{
  public:
    /**
     * @brief Default constructor. Initializes an empty tape with the head at position 0.
     */
    Tape() : head_pos_(0) {}

    /**
     * @brief Constructs a Tape populated with an initial formal String.
     * Places the string's symbols sequentially starting at index 0 and sets the head to 0.
     *
     * @param string The formal String sequence to load onto the tape.
     */
    explicit Tape(String string);

    /**
     * @brief Reads the symbol currently located under the tape head.
     *
     * If the current coordinate has not been modified yet, it assumes the cell
     * belongs to the infinite blank space and safely returns a `kBlank` symbol.
     *
     * @return Symbol The symbol scanned at the current head position.
     */
    [[nodiscard]] Symbol read() const;

    /**
     * @brief Writes a symbol into the cell currently under the tape head.
     * Passed by value since Symbol is a lightweight, trivially-copyable type.
     *
     * @param symbol The Symbol instance to write.
     */
    void write(Symbol symbol) { this->cells_[this->head_pos_] = symbol; }

    /**
     * @brief Shifts the tape head one cell to the left, right or make it stay.
     *
     * Passed by value since Direction is an enum class.
     *
     * @param dir The movement direction (LEFT, RIGHT or STAY).
     */
    void move(Direction dir)
    {
        switch (dir)
        {
            case Direction::LEFT:
                --this->head_pos_;
                break;
            case Direction::RIGHT:
                ++this->head_pos_;
                break;
            case Direction::STAY:
            default:
                break;
        }
    }

    /**
     * @brief Renders the active layout of the tape and the head tracking to an output stream.
     * @param os The target output stream (defaults to std::cout).
     */
    void print(std::ostream &os = std::cout) const;

    /**
     * @brief Overloaded stream insertion operator to allow direct streaming of Tape instances.
     * @param os The target output stream.
     * @param tape The Tape instance to render.
     * @return std::ostream& A reference to the modified output stream.
     */
    friend std::ostream &operator<<(std::ostream &os, const Tape &tape);

  private:
    int head_pos_;                ///< Current mathematical index coordinate of the head.
    std::map<int, Symbol> cells_; ///< Memory cells mapping coordinates to written Symbols.
};

#endif // TAPE_HPP
