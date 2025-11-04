#include <SDL3/SDL.h>

#define KNRM  "\x1B[0m" // Reset to normal
#define KRED  "\x1B[31m" // Red foreground
#define KGRN  "\x1B[32m" // Green foreground
#define KYEL  "\x1B[33m" // Yellow foreground
#define KBLU  "\x1B[34m" // Blue foreground
#define KMAG  "\x1B[35m" // Magenta foreground
#define KCYN  "\x1B[36m" // Cyan foreground
#define KWHT  "\x1B[37m" // White foreground
#define N 20
#define KRST  "\x1b[2J"
#define KMVBCK "\x1b[1;A"

#define CELL_SIZE 16
#define HEIGHT 512
#define WIDTH 512
#define _COLS WIDTH/CELL_SIZE
#define _ROWS HEIGHT/CELL_SIZE
#define RUNS 100
#define SDL_MAIN_USE_CALLBACKS 1  /* use the callbacks instead of main() */
#define MAX_SCALING_FACTOR 4

void print_grid(int g[_ROWS][_COLS]);
int count_neigh(int x, int y);
void update_grid();
void clear_screen();
int **init_grid(int rows, int cols);

// cleanup
void quit_and_clean(SDL_Window *window, int** init_grid);
// handle interrupt signal
void handle_sigint();
