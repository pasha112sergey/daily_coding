#include <stdio.h>
#include "../include/conway_header.h"
#include <unistd.h>
#include <string.h>
#include <ncurses.h>
#include <stdlib.h>
#include <signal.h>
#include <SDL3/SDL.h>

void clear_screen()
{
    printf("\e[2J");
    printf("\e[H");
    fflush(stdout);

}

static int cell_sz = CELL_SIZE;
static int rows = _ROWS;
static int cols = _COLS;
static int scaling_factor = 1;
static int speed = 100;
int **grid = NULL;

int **update_grid_to_scaled_size(int **grid, int temp_rows, int temp_cols)
{
    // we have rows, we have cols
    int **new_grid = calloc(temp_rows, sizeof(int *));
    for (int i = 0; i < temp_rows; i++)
    {
        new_grid[i] = calloc(temp_cols, sizeof(int));
    }

    // now i have to shift every thing a quarter of the way down a
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
        {
            if (grid[i][j] == 1)
            {
                int mapped_i = i + (int) (temp_rows / 4);
                int mapped_j = j + (int) (temp_cols / 4);
                new_grid[mapped_i][mapped_j] = 1; // transformation must happen here
            }
        }
    }
    free(grid);
    return new_grid;
}

void update_grid(){
    int temp_rows = rows;
    int temp_cols = cols;
    int **temp_grid = init_grid(rows, cols);
    int doubled = 0;

    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
            temp_grid[i][j] = grid[i][j];
    }
    // copied the grid

    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
        {
            int count = count_neigh(i, j);
            if (grid[i][j])
            {
                if (count == 2 || count == 3)
                    temp_grid[i][j] = 1;
                else
                    temp_grid[i][j] = 0; // here is supposed to be 0
            }
            else if (grid[i][j] == 0 && count == 3)
                temp_grid[i][j] = 1;
        }
    }

    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
        {
            grid[i][j] = temp_grid[i][j];

            if(grid[i][j] == 1)
                if ((doubled == 0) && (i <= 0 || i >= rows-1 || j <= 0 || j >= cols-1)) { 
                        // bounds detection
                        doubled = 1;
                }
        }
    }

    if (doubled)
    {
        scaling_factor *= 2;
        temp_cols =  _COLS * scaling_factor;
        temp_rows = _ROWS * scaling_factor;
        cell_sz = CELL_SIZE / scaling_factor;
        grid = update_grid_to_scaled_size(grid, temp_rows, temp_cols);
        rows = temp_rows;
        cols = temp_cols;
        speed -= 15;
    }
}

int count_neigh(int x, int y)
{
    int ncount = 0;

    /* 
    (x-1, y-1)---(x, y-1)---(x+1, y-1)
        |     \     |       /    |
        |       \   |      /     |  
    (x-1,  y )---(x,  y )---(x+1,  y )
        |      /     |     \     |
        |     /      |      \    |
    (x-1, y+1)---(x, y+1)---(x+1, y+1)
    */
    if (x-1 >= 0 && y-1 >= 0 && grid[x-1][y-1]) // (x-1, y-1)
        ncount++;
    if (y-1 >= 0 && grid[x][y-1]) // (x, y-1)
        ncount++;
    if (x+1 < cols && y-1 >= 0 && grid[x+1][y-1]) //(x+1, y-1)
        ncount++;
    if (x-1 >= 0 && grid[x-1][y]) //(x-1, y)
        ncount++;
    if (x+1 < cols && grid[x+1][y]) // (x+1, y)
        ncount++;
    if (x-1 >= 0 && y+1 < rows && grid[x-1][y+1]) // (x-1, y+1)
        ncount++;
    if (y + 1 < cols && grid[x][y+1]) // (x, y+1)
        ncount++;
    if (x+1 < rows && y+1 < cols && grid[x+1][y+1]) // (x+1, y+1)
        ncount++;

    return ncount;
}

int **init_grid(int rows, int cols)
{
    int **grid = calloc(rows, sizeof(int *));
    for (int i = 0; i < rows; i++)
    {
        grid[i] = calloc(cols, sizeof(int));
    }
    return grid;
}

void handle_sigint()
{
    exit(0);
}

void quit_and_clean(SDL_Window *window, int** init_grid)
{
    SDL_DestroyWindow(window);
    SDL_Quit();
    if (init_grid != NULL)
        free(init_grid);
    handle_sigint();
}

int draw_grid(SDL_Surface *surface)
{
    for (int i = 0; i < rows; i++)
    {
        SDL_Rect r = {0, i * cell_sz, WIDTH, 1};
        SDL_FillSurfaceRect(surface, &r, SDL_MapSurfaceRGB(surface, 0, 15, 0));
    }

    for (int j = 0; j < cols; j++) {
        SDL_Rect r = {j * cell_sz, 0, 1, HEIGHT};
        SDL_FillSurfaceRect(surface, &r,  SDL_MapSurfaceRGB(surface, 0, 15, 0));
    }
    return 1;
}

void update_anim(SDL_Surface *surface)
{
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
        {
            SDL_Rect r = {j * cell_sz, i * cell_sz, cell_sz , cell_sz };
            if (grid[i][j] == 1)
            {
                SDL_FillSurfaceRect(surface, &r, SDL_MapSurfaceRGB(surface, 0, 255, 0));
            }
        }
    }
    update_grid();
    SDL_Delay(speed);
}


void handle_click(int x, int y, SDL_Surface *surface, SDL_Window *window)
{
    int row = y / cell_sz;
    int col = x / cell_sz;
    if (!grid[row][col])
        grid[row][col] = 1;
    else 
        grid[row][col] = 0;
}

void draw_initial(SDL_Surface *surface)
{

    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
        {
            if (grid[i][j] == 1)
            {
                SDL_Rect rect = {j * cell_sz, i * cell_sz, cell_sz, cell_sz};
                SDL_FillSurfaceRect(surface, &rect, SDL_MapSurfaceRGB(surface, 0, 255, 0));
            }
        }
    }


}

void reset_grid(int **start_grid, int old_rows, int old_cols, int old_sz, int old_scaling_factor, int old_speed)
{
    for (int i = 0; i < cols; i++)
    {
        free(grid[i]);
    }
    free(grid);

    grid = init_grid(old_rows, old_cols);

    for (int i = 0; i < old_rows; i++)
    {
        for (int j = 0; j < old_cols; j++)
        {
            grid[i][j] = start_grid[i][j];
        }
    }
    rows = old_rows;
    cols = old_cols;
    cell_sz = old_sz;
    speed = old_speed;
    scaling_factor = old_scaling_factor;
}

int **save_grid(int *old_cols, int *old_rows, int *old_cell_sz, int *old_scaling_factor, int *old_speed)
{
    int **grid_to_save = (int **) malloc(sizeof(int *) * rows);
    for (int i = 0; i < rows; i++)
    {
        grid_to_save[i] = (int *)malloc(sizeof(int) * cols);
        memcpy(grid_to_save[i], grid[i], sizeof(int) * cols);
    }
    *old_cell_sz = cell_sz;
    *old_speed = speed;
    *old_scaling_factor = scaling_factor;
    *old_cols = cols;
    *old_rows = rows;

    return grid_to_save;
}

int main()
{
    SDL_Window *window;
    SDL_Renderer *renderer;
    SDL_Surface *surface;
    grid = init_grid(rows, cols); // good

    if (!SDL_Init(SDL_INIT_VIDEO))
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't initialize SDL: %s", SDL_GetError());
        return 3;
    }

    SDL_Event event;

    window = SDL_CreateWindow("Conway’s Game of Life", WIDTH, HEIGHT, 0);
    signal(SIGINT, handle_sigint);
    int run = 0;
    int old_rows = rows;
    int old_cols = cols;
    int old_cell_sz = cell_sz;
    int old_scaling_factor = scaling_factor;
    int old_speed = speed;
    while (1)
    {
        surface = SDL_GetWindowSurface(window);

        // fill in black
        SDL_FillSurfaceRect(surface, NULL, SDL_MapSurfaceRGB(surface, 0, 0, 0));

        // draw the rectangle
        draw_grid(surface); // good 
        draw_initial(surface); // good
        
        if (run)
            update_anim(surface);
        
        SDL_UpdateWindowSurface(window);
        int **init_grid;

        while (SDL_PollEvent(&event))
        {
            switch(event.type)
            {
                case SDL_EVENT_QUIT:
                    quit_and_clean(window, init_grid);
                    break;
                case SDL_EVENT_MOUSE_BUTTON_DOWN:
                    handle_click(event.motion.x, event.motion.y, surface, window);
                    break;
                case SDL_EVENT_KEY_DOWN:
                    if (event.key.scancode == SDL_SCANCODE_SPACE)
                    {
                        init_grid = save_grid(&old_rows, &old_cols, &old_cell_sz, &old_scaling_factor, &old_speed);
                        run = 1;
                    }
                    else if (event.key.scancode == SDL_SCANCODE_R)
                    {
                        run = 0;
                        reset_grid(init_grid, old_rows, old_cols, old_cell_sz, old_scaling_factor, old_speed);
                    }
                    break;
                }
        }
    }
} 