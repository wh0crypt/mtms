/**
 * @file direction.hpp
 * @author wh0crypt (wh0crypt@proton.me)
 * @brief Direction enum class representing the possible directions to move on a Turing Machine.
 * @version 0.1
 * @date 2026-06-27
 *
 * @copyright Copyright (c) 2026 wh0crypt. Licensed under the MIT License.
 */

#ifndef DIRECTION_HPP
#define DIRECTION_HPP

/**
 * @enum Direction
 * @brief Specifies the movement direction of the tape head.
 */
enum class Direction
{
    LEFT,  ///< Move the tape head one cell to the left.
    RIGHT, ///< Move the tape head one cell to the right.
    STAY   ///< Keep the tape head on the current cell.
};

#endif // DIRECTION_HPP
