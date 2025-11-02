#include "../include/fields.hpp"
#include <iostream>
#include <vector>
#include <SDL3/SDL.h>
#include <cstdlib> // for random
using namespace std;

class Vector {
    public:
        float mag;
        float x;
        float y;
        void setValues(float m, float x, float y);
};

void Vector :: setValues(float m, float _x, float _y)
{
    mag = m;
    x = _x;
    y = _y;
}

vector<vector<Vector>> vectors (ROWS, vector<Vector>(COLS));


void drawVectors(vector<vector<Vector>> lines = vectors, SDL_Renderer *renderer = NULL)
{
    for (int i = 0; i < ROWS; i++)
    {
        for (int j = 0; j < COLS; j++)
        {
            SDL_RenderLine(renderer, j * CELL_SZ, i * CELL_SZ, lines[i][j].x, lines[i][j].y);
        }
    }
}

void init_vectors()
{
    for (int i = 0; i < ROWS; i++)
    {
        for (int j = 0; j < COLS; j++)
        {
            Vector v;
            v.setValues(1, 1, 1);
            vectors[i].push_back(v);
        }
    }
}

void handle_sigint(int signal)
{
    (void)signal;
    cout << endl << "sigint received" << endl;
    exit(0);
}

void draw_grid(SDL_Surface *surface)
{
    for (int i = 0; i < ROWS; i++)
    {
        SDL_Rect rect = {0, i * CELL_SZ, WIDTH, 1};
        SDL_FillSurfaceRect(surface, &rect, SDL_MapSurfaceRGB(surface, 0, 100, 0));
    }

    for (int i = 0; i < COLS; i++)
    {
        SDL_Rect rect = {i * CELL_SZ, 0, 1, HEIGHT};
        SDL_FillSurfaceRect(surface, &rect, SDL_MapSurfaceRGB(surface, 0, 100, 0));
    }
}


int main() {
    if(!SDL_Init(SDL_INIT_VIDEO))
        cout << "Error initializing" << endl;

    SDL_Window *window;
    SDL_Surface *surface;
    SDL_Event event;
    SDL_Renderer *renderer;

    window = SDL_CreateWindow("Electric Fields Simulation", WIDTH, HEIGHT, 0);
    renderer = SDL_GetRenderer(window);
    signal(SIGINT, handle_sigint);
    while (1)
    {

        surface = SDL_GetWindowSurface(window);
        SDL_FillSurfaceRect(surface, NULL, SDL_MapSurfaceRGB(surface, 0, 0, 0));
        // draw_grid(surface);
        drawVectors(vectors, renderer);
        while (SDL_PollEvent(&event))
        {   
            switch(event.type)
            {
                case SDL_EVENT_QUIT:
                    cout << "quit clicked" << endl;
                    SDL_DestroyWindow(window);
                    exit(0);
                    break;
            }
        }
        SDL_UpdateWindowSurface(window); 
    }
}