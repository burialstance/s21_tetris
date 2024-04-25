#ifndef BRICKGAME_TETRIS_UTILS_UTILS_H
#define BRICKGAME_TETRIS_UTILS_UTILS_H

#include <stdio.h>
#include <stdlib.h>

/**
 * @brief Creates a dynamically allocated 2D array (matrix) with specified rows
 * and columns.
 *
 * This function dynamically allocates memory for a 2D array (matrix) with the
 * specified number of rows and columns. It returns a pointer to the first
 * element of the array.
 *
 * @param rows The number of rows in the matrix.
 * @param cols The number of columns in the matrix.
 * @return A pointer to the first element of the dynamically allocated 2D array.
 */
int **create_matrix(size_t rows, size_t cols);

/**
 * @brief Frees the memory allocated for a dynamically allocated 2D matrix.
 *
 * This function is designed to deallocate the memory for a 2D matrix that was
 * dynamically allocated using `malloc` or `calloc`. It takes two parameters: a
 * pointer to the matrix and the number of rows in the matrix. The function
 * iterates over each row of the matrix, freeing the memory allocated for each
 * row using `free`.
 *
 * @param matrix Pointer to the 2D matrix to be destroyed.
 * @param rows The number of rows in the matrix.
 */
void destroy_matrix(int **matrix, size_t rows);

/**
 * @brief Calculates the reward count based on the number of erased lines.
 *
 * This function calculates the reward count for a game based on the number of
 * lines erased. The reward count is typically used to determine the score or
 * points awarded for clearing lines. The function takes an integer representing
 * the number of erased lines and returns a size_t value, which is an unsigned
 * integer type.
 *
 * @param erased_lines The number of lines erased.
 * @return The calculated reward count as a size_t value.
 */
size_t get_reward_count(int ereased_lines);

/**
 * @brief Determines the level based on a given score.
 *
 * This function calculates and returns the level associated with a given score.
 * The level is determined based on the score, with each level representing a
 * range of scores. The function uses the `size_t` data type for the return
 * value
 *
 * @param score The score for which the level is to be determined.
 * @return The level associated with the given score, represented as a `size_t`
 * value.
 */
size_t get_level_by_score(int score);

/**
 * @brief Reads the high score from a file.
 *
 * This function reads the high score from a specified file. The high score is
 * expected to be stored in a format that can be directly read as an integer.
 * The function returns the high score as an integer.
 *
 * @param filename The name of the file from which to read the high score.
 * @return The high score read from the file, or an error value if the file
 * could not be read.
 */
int read_highscore_from_file(const char *filename);

/**
 * @brief Writes a highscore to a file.
 *
 * This function writes a highscore value to a specified file. The highscore is
 * written as an integer in a human-readable format, rather than as binary data,
 * which would not be directly readable by humans.
 *
 * @param filename The name of the file to which the highscore should be
 * written.
 * @param highscore The highscore value to be written to the file.
 */
void write_highscore_to_file(const char *filename, int highscore);

#endif
