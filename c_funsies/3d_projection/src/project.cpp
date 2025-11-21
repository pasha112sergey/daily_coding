#include <SDL3/SDL.h>
#include <iostream>
#include "../include/project.hpp"
#include <vector>
#include <cmath>
#include <numbers>

class Point3D;
class Cube;
class Camera;

class Point2D {
    public:
        double x;
        double y;
        Point2D(double x, double y) : x(x), y(y) {};

        void drawPoint(SDL_Renderer *renderer)
        {
            float w = 1;
            float h = 1;
            SDL_FRect rect = {(float)this->x, (float)this->y, w, h};
            SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
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

        Point2D projectPoint(double z0)
        {
            double ratio = 1/(this->z * z0);

            double x_ = this->x * ratio + WIDTH/2 - 25;
            double y_ = this->y * ratio + WIDTH/2 - 25;

            Point2D p_(x_, y_);
            return p_;
        }
};

class Camera
{
    private:
        Point3D camera_pos;
        int fov;

    public:
        Camera(int fov) : camera_pos(0, 0, 0), fov(M_PI/180 * fov) {};

        double calculate_z0()
        {
            return 1 / (std::tan(this->fov / 2));
        }

        int getFov() { return this->fov; }
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



        void drawCube(SDL_Renderer *renderer, double z0)
        {
            // render all lines
            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
            std::vector<Point2D> mp;

            for (int i = 0; i < 8; i++)
            {
                mp.push_back(points[i].projectPoint(z0));
                points[i].projectPoint(z0).drawPoint(renderer);
            }
            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

            // top square
            SDL_RenderLine(renderer, mp[0].x, mp[0].y, mp[1].x, mp[1].y);
            SDL_RenderLine(renderer, mp[1].x, mp[1].y, mp[2].x, mp[2].y);
            SDL_RenderLine(renderer, mp[2].x, mp[2].y, mp[3].x, mp[3].y);
            SDL_RenderLine(renderer, mp[3].x, mp[3].y, mp[0].x, mp[0].y);

            // bottom square 
            SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
            SDL_RenderLine(renderer, mp[4].x, mp[4].y, mp[5].x, mp[5].y);
            SDL_RenderLine(renderer, mp[5].x, mp[5].y, mp[6].x, mp[6].y);
            SDL_RenderLine(renderer, mp[6].x, mp[6].y, mp[7].x, mp[7].y);
            SDL_RenderLine(renderer, mp[7].x, mp[7].y, mp[4].x, mp[4].y);

            // their connection
            SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
            SDL_RenderLine(renderer, mp[0].x, mp[0].y, mp[4].x, mp[4].y);
            SDL_RenderLine(renderer, mp[1].x, mp[1].y, mp[5].x, mp[5].y);
            SDL_RenderLine(renderer, mp[2].x, mp[2].y, mp[6].x, mp[6].y);
            SDL_RenderLine(renderer, mp[3].x, mp[3].y, mp[7].x, mp[7].y);

            // SDL_RenderLine(renderer, points[0]);
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
    SDL_Surface *surface = SDL_GetWindowSurface(window);
    SDL_Renderer *renderer = SDL_GetRenderer(window);

    SDL_Event event;
    bool running = true;

    Cube cube(100, 1);
    const int FOV = 10;

    Camera cam(120);

    // std::cout << a;
    // SDL_Rect rect = {50, 50, 50, 50};


    while (running)
    {
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
        SDL_RenderClear(renderer);
        std::cout << "------------\n";
        // for (Point3D p3 : cube.getVertices())
        // {
        //     Point2D p2 = p3.projectPoint(cam.calculate_z0());
        //     std::cout << "3d: ";
        //     std::cout << p3;
        //     std::cout << " 2d: ";
        //     std::cout << p2 << std::endl;
        //     std::cout << "z0: " << cam.calculate_z0();
        //     std::cout << "\nfov: " << cam.getFov();
        //     p2.drawPoint(renderer);
        // }

        cube.drawCube(renderer, cam.calculate_z0());
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


