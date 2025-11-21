#include <SDL3/SDL.h>
#include <iostream>
#include "../include/project.hpp"
#include <vector>

class Point {
    private:
        double x;
        double y;
        double z;
    public:
        Point(double x, double y, double z) : x(x), y(y), z(z) {
        };

        friend std::ostream& operator<<(std::ostream &strm, const Point &a) {
            strm << "x = " << a.x << " y = " << a.y << " z = " << a.z;
            return strm;
        }
};

// vector list might've been a better idea
class Cube {
    private: 
        std::vector<Point> points;
    public:
        // Point a;
        // Point b;
        // Point c;
        // Point d;
        // Point e;
        // Point f;
        // Point g;
        // Point h;

        // Cube(float width) : a(0,0,0), b(width, 0, 0), c(0, width, 0), d(width, width, 0), e(0,0,width), f(width, 0, width), g(0,width, width), h(width, width, width)// we could have extra parameters that will have a start coordinate
        Cube (float w)
        {
            // bottom sq (in clockwise order)
            points.push_back(Point(0, 0, 0));
            points.push_back(Point(0, w, 0));
            points.push_back(Point(w, w, 0));
            points.push_back(Point(w, 0, 0));

            // top sq (in clockwise order)
            points.push_back(Point(0, 0, w));
            points.push_back(Point(0, w, w));
            points.push_back(Point(w, w, w));
            points.push_back(Point(w, 0, w));

        }

        friend std::ostream& operator<<(std::ostream &strm, const Cube &cube)
        {
            const int vertices = 8;
            for (int i = 0; i < vertices; i++)
            {
                strm << "Point(" << i << ") : " << cube.points[i] << "\n";
            }
            return strm;
        }
};

int main()
{
    std::cout << std::unitbuf;

    if (!SDL_Init(SDL_INIT_VIDEO))
    {
        std::cout << "ERROR INITIALIZING\n";
    }

    SDL_Window *window = SDL_CreateWindow("Rotating Cube", WIDTH, HEIGHT, 0);
    // SDL_Surface *surface = SDL_GetWindowSurface(window);


    SDL_Event event;
    bool running = true;

    Cube a(2);
    std::cout << a;
    while (running)
    {
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_EVENT_QUIT)
            {                
                running = false;
                break;
            }
        }
    }

    
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}

/*
 I would like to create a cube, which is defined by a sidelength -> then, I'd like to create points (8 points with xyz coordinates)
 OOP: Point class -> constructor would have double x,y,z as class fields
 Cube class -> constructor that takes in a sidelength, and would have class fields: sidelength, 8 Point objects
*/


