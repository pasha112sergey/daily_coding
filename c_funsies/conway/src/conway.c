#include <stdio.h>
#include "../include/conway_header.h"
#include <unistd.h>
#include <string.h>
#include <ncurses.h>
#include <stdlib.h>
#include <signal.h>
#include <SDL3/SDL.h>

static int grid[_ROWS][_COLS];

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

void init_grid(int *startsx, int *startsy, int n)
{
    for (int i = 0; i < n; i++)
    {
        grid[startsy[i]][startsx[i]] = 1;
    }
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
    for (int i = 0; i < _ROWS; i++) {
        SDL_Rect r = {0, i * CELL_SIZE, WIDTH, 1};
        SDL_FillSurfaceRect(surface, &r, SDL_MapSurfaceRGB(surface, 0, 90, 0));
    }

    for (int j = 0; j < _COLS; j++) {
        SDL_Rect r = {j * CELL_SIZE, 0, 1, HEIGHT};
        SDL_FillSurfaceRect(surface, &r,  SDL_MapSurfaceRGB(surface, 0, 90, 0));
    }

    return 1;
}

void update_anim(SDL_Surface *surface)
{
    for (int i = 0; i < _ROWS; i++)
    {
        for (int j = 0; j < _COLS; j++)
        {
            SDL_Rect r = {j * CELL_SIZE, i * CELL_SIZE, CELL_SIZE , CELL_SIZE };
            if (grid[i][j] == 1)
            {
                SDL_FillSurfaceRect(surface, &r, SDL_MapSurfaceRGB(surface, 0, 255, 0));
            }
        }
    }
    update_grid();
    SDL_Delay(100);
}

static int startxs[_ROWS] = {-1};
static int startys[_COLS] = {-1};
static int count = 0;


void handle_click(int x, int y, SDL_Surface *surface, SDL_Window *window)
{

    int row = y / CELL_SIZE;
    int col = x / CELL_SIZE;
    if (!grid[row][col])
        grid[row][col] = 1;
    else 
        grid[row][col] = 0;
}

void draw_initial(SDL_Surface *surface)
{
    for (int i = 0; i < _ROWS; i++)
    {
        for (int j = 0; j < _COLS; j++)
        {
            if (grid[i][j] == 1)
            {
                 SDL_Rect rect = {j * CELL_SIZE, i * CELL_SIZE, CELL_SIZE, CELL_SIZE};
                SDL_FillSurfaceRect(surface, &rect, SDL_MapSurfaceRGB(surface, 0, 255, 0));
            }
        }
    }
}

void reset_grid(int **start_grid)
{
    for (int i = 0; i < _ROWS; i++)
    {
        for (int j = 0; j < _COLS; j++)
        {
            grid[i][j] = start_grid[i][j];
        }
    }
}

int **save_grid()
{
    int **grid_to_save = (int **) malloc(sizeof(int *) * _ROWS);
    for (int i = 0; i < _ROWS; i++)
    {
        grid_to_save[i] = (int *)malloc(sizeof(int) * _COLS);
        memcpy(grid_to_save[i], grid[i], sizeof(int) * _COLS);
    }
    return grid_to_save;
}

int main()
{ 
    SDL_Window *window;
    SDL_Renderer *renderer;
    SDL_Surface *surface;


    if (!SDL_Init(SDL_INIT_VIDEO))
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't initialize SDL: %s", SDL_GetError());
        return 3;
    }

    SDL_Event event;

    window = SDL_CreateWindow("Conway’s Game of Life", WIDTH, HEIGHT, 0);
    signal(SIGINT, handle_sigint);
    int run = 0;
    while (1)
    {
        surface = SDL_GetWindowSurface(window);
        SDL_Rect rect = {10, 10, 50, 50};

        // fill in black
        SDL_FillSurfaceRect(surface, NULL, SDL_MapSurfaceRGB(surface, 0, 0, 0));

        // draw the rectangle
        draw_grid(surface);
        draw_initial(surface);
        
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
                        init_grid = save_grid();
                        run = 1;
                    }
                    else if (event.key.scancode == SDL_SCANCODE_R)
                    {
                        run = 0;
                        reset_grid(init_grid);
                    }
                    break;
                }
        }
    }
} 