#include <stdio.h>
#include "../include/conway_header.h"
#include <unistd.h>
#include <string.h>
#include <ncurses.h>
#include <stdlib.h>
#include <signal.h>
#include <SDL3/SDL.h>

int grid[_ROWS][_COLS];

void clear_screen()
{
    printf("\e[2J");
    printf("\e[H");
    fflush(stdout);

} 

void update_grid(){
    int temp_grid[_ROWS][_COLS] = {0};
    for (int i = 0; i < _ROWS; i++)
    {
        for (int j = 0; j < _COLS; j++)
            temp_grid[i][j] = grid[i][j];
    }
    // copied the grid

    for (int i = 0; i < _ROWS; i++)
    {
        for (int j = 0; j < _COLS; j++)
        {
            int count = count_neigh(i, j);
            if (grid[i][j])
            {
              if (count == 2 || count == 3)
                temp_grid[i][j] = 1;
            else
                temp_grid[i][j] = 0;
            }
            else if (grid[i][j] == 0 && count == 3)
                temp_grid[i][j] = 1;

        }
    }
    for (int i = 0; i < _ROWS; i++)
    {
        for (int j = 0; j < _COLS; j++)
            grid[i][j] = temp_grid[i][j];
    }

}

void print_grid(int g[_ROWS][_COLS])
{
    clear_screen();
    for (int i = 0; i < _ROWS; i++)
    {
        for (int j = 0; j < _COLS; j++)
        {
            if (g[i][j] == 1)
                printf("%s# ", KBLU);
            else
                printf("%s# ", KNRM);
        }
        printf("\n");
    }
    printf("---------------\n");
}

int count_neigh(int x, int y)
{
    int ncount = 0;

    /* 
    (x-1, y-1)---(x, y-1)---(x+1, y-1)
    (x-1,  y )---(x,  y )---(x+1,  y )
    (x-1, y+1)---(x, y+1)---(x+1, y+1)
    */
    if (x-1 >= 0 && y-1 >= 0 && grid[x-1][y-1]) // (x-1, y-1)
        ncount++;
    if (y-1 >= 0 && grid[x][y-1]) // (x, y-1)
        ncount++;
    if (x+1 < _COLS && y-1 >= 0 && grid[x+1][y-1]) //(x+1, y-1)
        ncount++;
    if (x-1 >= 0 && grid[x-1][y]) //(x-1, y)
        ncount++;
    if (x+1 < _COLS && grid[x+1][y]) // (x+1, y)
        ncount++;
    if (x-1 >= 0 && y+1 < _ROWS && grid[x-1][y+1]) // (x-1, y+1)
        ncount++;
    if (y + 1 < _COLS && grid[x][y+1]) // (x, y+1)
        ncount++;
    if (x+1 < _ROWS && y+1 < _COLS && grid[x+1][y+1]) // (x+1, y+1)
        ncount++;

    return ncount;
}

void init_grid()
{
    int init_x = _COLS / 2;
    int init_y = _ROWS / 2;

    grid[init_y][init_x] = 1;
    grid[init_y][init_x + 2] = 1;
    grid[init_y - 1][init_x + 1] = 1;
    grid[init_y + 1][init_x + 1] = 1;
    grid[init_y + 1][init_x + 3] = 1;
    grid[init_y + 2][init_x] = 1;
    grid[init_y][init_x - 2] = 1;
}

void handle_sigint()
{
    exit(0);
}

void quit_and_clean(SDL_Window *window)
{
    SDL_DestroyWindow(window);
    SDL_Quit();
    handle_sigint();
}

int draw_grid(SDL_Surface *surface)
{
    for (int i = 0; i < _ROWS; i++) {
        SDL_Rect r = {0, i * CELL_SIZE, WIDTH, 1};
        SDL_FillSurfaceRect(surface, &r, SDL_MapSurfaceRGB(surface, 0, 0, 0));
    }

    for (int j = 0; j < _COLS; j++) {
        SDL_Rect r = {j * CELL_SIZE, 0, 1, HEIGHT};
        SDL_FillSurfaceRect(surface, &r,  SDL_MapSurfaceRGB(surface, 0, 0, 0));
    }

    return 1;
}

int main()
{
    SDL_Window *window;
    SDL_Renderer *renderer;
    SDL_Surface *surface;

    if (!SDL_Init(SDL_INIT_VIDEO)) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't initialize SDL: %s", SDL_GetError());
        return 3;
    }

    SDL_Event event;

    window = SDL_CreateWindow("Conway’s Game of Life", WIDTH, HEIGHT, 0);
    signal(SIGINT, handle_sigint);
    while (1)
    {
        surface = SDL_GetWindowSurface(window);
        SDL_Rect rect = {10, 10, 50, 50};

        // fill in black
        SDL_FillSurfaceRect(surface, NULL, SDL_MapSurfaceRGB(surface, 169, 169, 169));

        // draw the rectangle
        draw_grid(surface);
        SDL_UpdateWindowSurface(window);

        while(SDL_PollEvent(&event)) {
            if (event.type == SDL_EVENT_QUIT)
            {
                quit_and_clean(window);
            }
        }
    }
}