#include "utils.h"

/**
 * @brief Creates a dynamically allocated 2D matrix with specified rows and
 * columns.
 *
 * This function dynamically allocates memory for a 2D matrix with a specified
 * number of rows and columns. It first allocates memory for an array of
 * pointers, each of which will point to a row in the matrix. Then, for each
 * row, it allocates memory for an array of integers, representing the columns
 * of that row. If memory allocation fails at any point, the function prints an
 * error message and exits the program.
 *
 * @param rows The number of rows in the matrix.
 * @param cols The number of columns in the matrix.
 * @return A pointer to the dynamically allocated 2D matrix.
 */
int **create_matrix(size_t rows, size_t cols) {
  int **matrix = calloc(rows, sizeof(int *));
  if (!matrix) {
    char error[] = "Cannot allocate mem for matrix(cols=%d, rows=%d)\n";
    fprintf(stderr, error, cols, rows);
    exit(-1);
  }
  for (size_t i = 0; i < rows; i++) {
    matrix[i] = calloc(cols, sizeof(int));
  }
  return matrix;
}

/**
 * @brief Frees a dynamically allocated 2D array.
 *
 * This function is designed to free a 2D array that was dynamically allocated
 * using `malloc` or `calloc`. It takes a pointer to the array and the height of
 * the array as arguments. The function iterates over each row of the array,
 * freeing each row using `free`. After all rows have been freed, it frees the
 * memory allocated for the array of pointers using `free` again. This ensures
 * that all memory allocated for the 2D array is properly released, preventing
 * memory leaks.
 *
 * @param matrix Pointer to the 2D array to be freed.
 * @param height The number of rows in the 2D array.
 */
void destroy_matrix(int **matrix, size_t height) {
  if (!matrix) return;
  for (size_t i = 0; i < height; i++) {
    free(matrix[i]);
  }
  free(matrix);
}

/**
 * @brief Calculates the reward based on the number of erased lines in a game.
 *
 * This function determines the reward for erasing lines in a game, based on the
 * number of lines erased. It uses a static array of rewards, where each index
 * corresponds to a different number of lines erased, and the value at that
 * index is the reward for erasing that many lines. The function checks if the
 * number of erased lines is greater than 0 and then calculates the reward based
 * on the number of lines. If the number of erased lines is 4 or more, the
 * maximum reward is returned. Otherwise, the reward is determined by the number
 * of lines erased.
 *
 * @param erased_lines The number of lines erased in the game.
 * @return The reward for the number of erased lines.
 */
size_t get_reward_count(int ereased_lines) {
  static int rewards[4] = {100, 300, 700, 1500};
  int reward = 0;
  if (ereased_lines > 0) {
    reward = (ereased_lines >= 4) ? rewards[3] : rewards[ereased_lines - 1];
  }
  return reward;
}

/**
 * @brief Calculates the level based on the player's score.
 *
 * This function determines the player's level based on their score. It uses a
 * fixed score per level threshold (600 points) to calculate the level. If the
 * calculated level is 0, it is set to 1 to ensure that the player has at least
 * one level. If the calculated level exceeds 10, it is capped at 10 to prevent
 * the player from exceeding the maximum level.
 *
 * @param score The player's current score.
 * @return The player's level based on their score.
 */
size_t get_level_by_score(int score) {
  int score_per_lvl = 600;
  int lvl = score / score_per_lvl;
  if (lvl == 0) {
    lvl = 1;
  } else if (lvl > 10) {
    lvl = 10;
  }
  return lvl;
}

/**
 * @brief Reads the highscore from a file.
 *
 * This function attempts to read the highscore from a specified file. It opens
 * the file in read mode, reads the highscore value using `fscanf`, and then
 * closes the file. If the file cannot be opened, or if the highscore cannot be
 * read, the function returns 0 as the default highscore.
 *
 * @param filename The name of the file to read the highscore from.
 * @return The highscore read from the file, or 0 if the file cannot be opened
 * or the highscore cannot be read.
 */
int read_highscore_from_file(const char *filename) {
  int highscore = 0;
  FILE *file = fopen(filename, "r");
  if (file) {
    fscanf(file, "highscore: %d", &highscore);
    fclose(file);
  }

  return highscore;
}

/**
 * @brief Writes a highscore to a file.
 *
 * This function opens a file specified by the filename parameter in write mode,
 * writes the highscore to the file in a human-readable format, and then closes
 * the file. The highscore is written as a string with the label "highscore: "
 * followed by the numerical value of the highscore.
 *
 * @param filename The name of the file to write the highscore to.
 * @param highscore The highscore value to be written to the file.
 */
void write_highscore_to_file(const char *filename, int highscore) {
  FILE *file = fopen(filename, "w");
  if (file) {
    fprintf(file, "highscore: %d", highscore);
    fclose(file);
  }
}
