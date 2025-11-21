#include <SDL3/SDL.h>
#include <iostream>
#include "../include/project.hpp"
#include <vector>

const double PLANE_POINT_Z = 10.0;

class Point2D {
    private:
        double x;
        double y;
    public:
        Point2D(double x, double y) : x(x), y(y) {};

        void drawPoint(SDL_Renderer *renderer)
        {
            float w = 5;
            float h = 5;
            SDL_FRect rect = {(float)this->x, (float)this->y, w, h};
            // std::cout << "pointer to rect is: " << &rect;
            SDL_SetRenderDrawColor(renderer, 253, 0, 0, 253);
            SDL_RenderFillRect(renderer, &rect);
        }

        friend std::ostream& operator<<(std::ostream &strm, const Point2D &a) {
            strm << "x = " << a.x << " y = " << a.y;
            return strm;
        }
};

class Point3D {
    private:
        double x;
        double y;
        double z;

    public:
        Point3D(double x, double y, double z) : x(x), y(y), z(z) {
        };

        friend std::ostream& operator<<(std::ostream &strm, const Point3D &a) {
            strm << "x = " << a.x << " y = " << a.y << " z = " << a.z;
            return strm;
        }

        // getter
        double getX() {return this -> x;}
        double getY() {return this -> y;}
        double getZ() {return this -> z;}

        // setter`
        void setZ(double z) {this -> z = z;}
        void setY(double y) {this -> y = y;}
        void setX(double x) {this -> x = x;}

        Point2D projectPoint()
        {
            double ratio = PLANE_POINT_Z / this->z;

            double x_ = this->x * ratio;
            double y_ = this->y * ratio;

            Point2D p_(x_, y_);
            return p_;
        }
};


class Cube {
    private:
        std::vector<Point3D> points;
        double start_pos;

    public:
        Cube(float w, double start_pos = WIDTH/2)
        {
            // define start pos
            this->start_pos = start_pos;

            // bottom sq (in clockwise order)
            points.push_back(Point3D(start_pos, start_pos, start_pos));
            points.push_back(Point3D(start_pos, start_pos + w, start_pos));
            points.push_back(Point3D(start_pos + w, start_pos + w, start_pos));
            points.push_back(Point3D(start_pos + w, start_pos, start_pos));

            // top sq (in clockwise order)
            points.push_back(Point3D(start_pos, start_pos, start_pos + w));
            points.push_back(Point3D(start_pos, start_pos + w, start_pos + w));
            points.push_back(Point3D(start_pos + w, start_pos + w, start_pos + w));
            points.push_back(Point3D(start_pos + w, start_pos, start_pos + w));
        }

        // overload << for printing
        friend std::ostream &operator<<(std::ostream &strm, const Cube &cube)
        {
            const int vertices = 8;
            for (int i = 0; i < vertices; i++)
            {
                strm << "Point(" << i << ") : " << cube.points[i] << "\n";
            }
            return strm;
        }

        // getter
        std::vector<Point3D> &getVertices() { return this->points; }
};


class Camera
{
    private:
        Point3D camera_pos;
        double plane_pos_z;

    public:
        Camera() : camera_pos(0, 0, 0), plane_pos_z(2) {};
};

int main()
{
    std::cout << std::unitbuf;

    if (!SDL_Init(SDL_INIT_VIDEO))
    {
        std::cout << "ERROR INITIALIZING\n";
    }

    SDL_Window *window = SDL_CreateWindow("Rotating Cube", WIDTH, HEIGHT, 0);
    SDL_Surface *surface = SDL_GetWindowSurface(window);
    SDL_Renderer *renderer = SDL_GetRenderer(window);

    SDL_Event event;
    bool running = true;

    Cube cube(50, WIDTH/2);
    Camera cam;


    // std::cout << a;
    // SDL_Rect rect = {50, 50, 50, 50};


    while (running)
    {
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
        SDL_RenderClear(renderer);
        std::cout << "------------\n";
        for (Point3D p3 : cube.getVertices())
        {
            Point2D p2 = p3.projectPoint();
            std::cout << "3d: ";
            std::cout << p3;
            std::cout << " 2d: ";
            std::cout << p2 << std::endl;
            // drawPoint(renderer);
        }
        std::cout << "\n------------\n";

        
        // Point2D test(WIDTH/2, HEIGHT/2);
        // test.drawPoint(renderer);
        
        // std::cout << "Here\n";
        
        while (SDL_PollEvent(&event))
        {
            // SDL_FillSurfaceRect(surface, &rect, 255);
            if (event.type == SDL_EVENT_QUIT)
            {                
                running = false;
                break;
            }
        }
        SDL_RenderPresent(renderer);
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


