#include "../include/fields.hpp"
#include <iostream>
#include <vector>
#include <SDL3/SDL.h>
#include <cmath>

using namespace std;

class FieldVector {
    public:
    float mag; // floats between 1 and 0
    float x; // floats between 1 and 0
    float y;   // floats between 1 and 0
    void setX(float x)
    {
        this->x = x;
    }
    void setY(float y)
    {
        this->y = y;
    }
    void setMag(float mag)
    {
        this->mag = mag;
    }
    FieldVector(float m, float x, float y) {
        this->mag = m;
        this->x = x;
        this->y = y;
    }
};

vector<vector<FieldVector>> vectors;

void drawVectors(vector<vector<FieldVector>> lines = vectors, SDL_Renderer *renderer = NULL)
{
    for (int i = 0; i < ROWS; i++)
    {
        for (int j = 0; j < COLS; j++)
        {
            int vec_x = j*CELL_SZ + CELL_SZ * lines[i][j].x;
            int vec_y = i*CELL_SZ + CELL_SZ * lines[i][j].y;
            cout << "vec_x: " << vec_x << endl;

            SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
            SDL_RenderLine(renderer, j * CELL_SZ, i * CELL_SZ, vec_x, vec_y);
        }
    }
}

void init_vectors()
{
    vectors.resize(ROWS);
    for (int i = 0; i < ROWS; i++)
    {
        vectors[i].reserve(COLS);
        for (int j = 0; j < COLS; j++)
        {
            vectors[i].push_back(FieldVector(0.2, 0.5, 1));
        }
    }
}

void handle_sigint(int signal)
{
    (void)signal;
    cout << endl << "sigint received" << endl;
    exit(0);
}

void draw_grid(SDL_Surface *surface, SDL_Renderer *renderer)
{
    SDL_SetRenderDrawColor(renderer, 0, 90, 0, 255);
    for (int i = 0; i < ROWS; i++)
    {
        SDL_RenderLine(renderer, 0, i * CELL_SZ, WIDTH, i*CELL_SZ);
    }
    for (int i = 0; i < COLS; i++)
    {
        SDL_RenderLine(renderer, i * CELL_SZ, 0, i*CELL_SZ, HEIGHT);
    }

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);

}

void updateVectors(vector<vector<FieldVector>> fvs=vectors, SDL_FRect *charge=NULL)
{
    float point_x;
    float point_y;
    if (charge)
    {
        point_x = charge->x;
        point_y = charge->y;
    }

    for (int i = 0; i < ROWS; i++)
    {
        for (int j = 0; j < COLS; j++)
        {
            // distance^2 = (i*CELL_SZ - point_y)^2 + (j*CELL_SZ - point_x)^2
            float distance = pow(pow(i * CELL_SZ - point_y, 2) + pow(j * CELL_SZ - point_x, 2), 0.5);
            // cout << "distance is: " << distance << endl;
            fvs[i][j].setMag((K_CONST * Q_CONST )/ pow(distance, 2));
            fvs[i][j].setX(fvs[i][j].mag * (j * CELL_SZ - point_x));
            fvs[i][j].setY(fvs[i][j].mag * (i * CELL_SZ - point_y));
        }
    }

}

int main() {
    if(!SDL_Init(SDL_INIT_VIDEO))
        cout << "Error initializing" << endl;

    init_vectors();
    SDL_Window *window;
    SDL_Surface *surface;
    SDL_Event event;
    SDL_Renderer *renderer;

    SDL_FRect point_charge = {CELL_SZ * COLS / 2, CELL_SZ * ROWS / 2, 5, 5};

    SDL_CreateWindowAndRenderer("Electricity!", WIDTH, HEIGHT, 0, &window, &renderer);
    signal(SIGINT, handle_sigint);

    while (1)
    {
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);
        // draw_grid(surface, renderer);

        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderFillRect(renderer, &point_charge);

        updateVectors(vectors, &point_charge);
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
        
        SDL_RenderPresent(renderer);
    }
}