#include "../include/fields.hpp"
#include <iostream>
#include <vector>
#include <algorithm>
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

class Charge {
    float x;
    float y;
    int charge;
    
    public:
        float v_x;
        float v_y;
        float a_x;
        float a_y;
        Charge(float x, float y, int charge, float v_x, float v_y, float a_x, float a_y)
        {
            this->x = x;
            this->y = y;
            this->charge = charge;
            this->v_x = v_x;
            this->v_y = v_y;
            this->a_x = a_x;
            this->a_y = a_y;
        }

        void setX(float x)
        {
            this->x = x;
        }

        void setY(float y)
        {
            this->y = y;
        }

        void setCharge(int charge)
        {
            this->charge = charge;
        }

        int getCharge()
        {
            return this->charge;
        }

        float getX()
        {
            return this->x;
        }

        float getY()
        {
            return this->y;
        }

        void updatePosition(FieldVector &field)
        {
            cout << "this x: " << this->x << " this y: " << this->y << endl;

            this->a_x = field.x / M_CONST;
            this->a_y = field.y / M_CONST;


            this->v_x = this->v_x + this->a_x * TIMESTEP;
            this->v_y = this->v_y + this->a_y * TIMESTEP;

            this->x = this->x + this->v_x * TIMESTEP;
            this->y = this->y + this->v_y * TIMESTEP;

            cout << "this x: " << this->x << " this y: " << this->y << endl;

            cout << endl;
            cout << endl;

        }

        void renderCharge(SDL_Renderer *renderer)
        {
            SDL_FRect rect = {this->x, this->y, 20, 20};
            SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
            SDL_RenderFillRect(renderer, &rect);
        }
};

vector<vector<FieldVector>> vectors;

vector<Charge> charges;

void setRendererColor(FieldVector v, SDL_Renderer *renderer)
{
    // SDL_SetRenderDrawColor(renderer, std::min((float)255, v.mag * 255), 0, 255, 255);
    if (v.mag < 0)
    {
        SDL_SetRenderDrawColor(renderer,  std::min((float)255, std::abs(v.mag) * 255), 0, 255, 255);
    }
    else
    {
        SDL_SetRenderDrawColor(renderer,  std::min((float)255, std::abs(v.mag) * 255), 0, 0, 255);
    }
}

void drawVectors(vector<vector<FieldVector>> &lines, SDL_Renderer *renderer)
{
    for (int i = 0; i < ROWS; i++)
    {
        for (int j = 0; j < COLS; j++)
        {
            int vec_x = (j + lines[i][j].x) * CELL_SZ;
            int vec_y = (i + lines[i][j].y) * CELL_SZ;

            setRendererColor(lines[i][j], renderer);
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
            vectors[i].push_back(FieldVector(1, 0.5, 1));
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

void updateVectors(vector<vector<FieldVector>> &fvs, vector<Charge> &charges)
{
    for (int i = 0; i < ROWS; i++)
    {
        for (int j = 0; j < COLS; j++)
        {
            float newX = 0;
            float newY = 0;
            float newMag = 0;
            float norm_factor = 0;
        
            // electric field at x,y = total sum of electric fields from all points
            for(Charge c : charges)
            {
                float point_x = c.getX();
                float point_y = c.getY();
                float distance = pow(pow(i * CELL_SZ - point_y, 2) + pow(j * CELL_SZ - point_x, 2), 0.5);

                if (distance > 0.5)
                {
                    newMag += ((K_CONST * Q_CONST) / pow(distance, 2)) * 1.5e13;
                    float mag = ((K_CONST * Q_CONST * c.getCharge()) / pow(distance, 2)) * 1.5e13;
                    newX += mag * (j * CELL_SZ - point_x);
                    newY += mag * (i * CELL_SZ - point_y);
                }
            }
            norm_factor = pow(pow(newX, 2) + pow(newY, 2), 0.5);

            fvs[i][j].setMag(newMag);
            fvs[i][j].setX(newX / norm_factor);
            fvs[i][j].setY(newY / norm_factor);
        }
    }
}

void renderCharges(SDL_Renderer * renderer, vector<Charge> &charges)
{
    for (Charge c : charges)
    {
        if (c.getCharge() > 0)
        {
            SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        }
        else
        {
            SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
        }
        SDL_FRect charge = {c.getX(), c.getY(), 5, 5};
        SDL_RenderFillRect(renderer, &charge);
    }
}

FieldVector updatePointCharge(SDL_Renderer *renderer, Charge charge)
{
    float newX = 0;
    float newY = 0;
    float newMag = 0;
    float norm_factor = 0;

    // electric field at x,y = total sum of electric fields from all points
    for(Charge c : charges)
    {
        float dx = charge.getX() - c.getX();
        float dy = charge.getY() - c.getY();
        float distance = sqrt(pow(dx,2) + pow(dy, 2));

        if (distance > 0.5)
        {
            float mag = ((K_CONST * Q_CONST * c.getCharge()) / (distance * distance)) * 1.5e13;
            newX += mag * dx / distance;
            newY += mag * dy / distance;
        }
    }
    
    FieldVector field(newMag, newX, newY);
    return field;
}

int main() {
    if(!SDL_Init(SDL_INIT_VIDEO))
        cout << "Error initializing" << endl;

    init_vectors();
    SDL_Window *window;
    SDL_Surface *surface;
    SDL_Event event;
    SDL_Renderer *renderer;

    Charge init_charge((float) (WIDTH / 2), (float) (HEIGHT / 2), 1, 0, 0, 0, 0);
    charges.push_back(init_charge);

    SDL_CreateWindowAndRenderer("Electricity!", WIDTH, HEIGHT, 0, &window, &renderer);
    signal(SIGINT, handle_sigint);

    bool mouseClicked = false;
    bool moving = false;
    Charge point_charge(0,0,0,0,0,0,0);
    while (1)
    {
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

        renderCharges(renderer, charges);
        updateVectors(vectors, charges);
        drawVectors(vectors, renderer);
        if (mouseClicked)
        {
            float x;
            float y;
            SDL_GetMouseState(&x, &y);
            charges[0].setX(x);
            charges[0].setY(y);
        }

        if (moving)
        {
            FieldVector field = updatePointCharge(renderer, point_charge);
            point_charge.updatePosition(field);
            point_charge.renderCharge(renderer);
        }
        while (SDL_PollEvent(&event))
        {
            switch (event.type)
            {
                case SDL_EVENT_QUIT:
                    cout << "quit clicked" << endl;
                    SDL_DestroyWindow(window);
                    exit(0);
                    break;
                case SDL_EVENT_MOUSE_BUTTON_DOWN:
                    mouseClicked = true;
                    break;
                case SDL_EVENT_MOUSE_BUTTON_UP:
                    mouseClicked = false;
                    break;
                case SDL_EVENT_KEY_DOWN:
                    if (event.key.scancode == SDL_SCANCODE_SPACE)
                    {
                        float x;
                        float y;
                        SDL_GetMouseState(&x, &y);

                        Charge c(x, y, -1, 0, 0, 0, 0);
                        charges.push_back(c);
                    }
                    else if(event.key.scancode == SDL_SCANCODE_A)
                    {
                        cout << "clicked!" << endl;
                        float x;
                        float y;
                        SDL_GetMouseState(&x, &y);

                        point_charge.setX(x);
                        point_charge.setY(y);
                        point_charge.setCharge(1);
                        point_charge.a_x = 0;
                        point_charge.a_y = 0;
                        point_charge.v_x = 0;
                        point_charge.v_y = 0;

                        moving = true;
                    }
                }
        }
        
        SDL_RenderPresent(renderer);
    }
}