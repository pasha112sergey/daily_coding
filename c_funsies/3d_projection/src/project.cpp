#include <SDL3/SDL.h>
#include <iostream>
#include "../include/project.hpp"
#include <vector>
#include <cmath>
#include <numbers>

class Point3D;
class Cube;
class Camera;

static float offset = WIDTH / 2;

inline double convertDegToRad(float deg)
{
    return M_PI / 180 * deg;
}

enum RotationType
{
    X = 1,
    Y = 2,
    Z = 4
};

std::vector<double> multiplyByMatrix(double x, double y, double z, double matrix[3][3])
{
    double _x = matrix[0][0] * x + matrix[0][1] * y + matrix[0][2] * z;
    double _y = matrix[1][0] * x + matrix[1][1] * y + matrix[1][2] * z;
    double _z = matrix[2][0] * x + matrix[2][1] * y + matrix[2][2] * z;

    std::vector<double> ans;

    ans.push_back(_x);
    ans.push_back(_y);
    ans.push_back(_z);

    return ans;
}
// enum class RotationType
// {
//     X,
//     Y,
//     Z
// };

class Point2D
{
public:
    double x;
    double y;
    Point2D(double x, double y) : x(x), y(y) {};

    void drawPoint(SDL_Renderer *renderer)
    {
        float w = 10;
        float h = 10;
        SDL_FRect rect = {(float)this->x + ::offset, (float)this->y + ::offset, w, h};
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        SDL_RenderFillRect(renderer, &rect);
    }

    friend std::ostream &operator<<(std::ostream &strm, const Point2D &a)
    {
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

        Point2D projectPoint(double foc_len)
        {
            double _x = foc_len * this->x / this->z;
            double _y = foc_len * this->y / this->z;

            Point2D p_(_x, _y);

            return p_;
        }
};

class Camera
{
    private:
        Point3D camera_pos;
        double fov;
        double foc_len;

    public:
        Camera(double fov) : camera_pos(0, 0, 0), fov(convertDegToRad(fov)), foc_len() {
            this->foc_len = ((HEIGHT / 2) / (std::tan(this->fov / 2)));
        };

        double getFocLen() { return this->foc_len; }
        double getFov() { return this->fov; }
};

class Cube {
    private:
        std::vector<Point3D> points;
        Point3D startCorner;
        double w;

    public:
        Cube(float w, Point3D startCorner) : startCorner(startCorner.getX(), startCorner.getY(), startCorner.getZ()), w(w)
        {
            // bottom sq (in clockwise order)
            points.push_back(Point3D(startCorner.getX(),         startCorner.getY(),          startCorner.getZ() + offset));
            points.push_back(Point3D(startCorner.getX(),         startCorner.getY() + w,      startCorner.getZ() + offset));
            points.push_back(Point3D(startCorner.getX() + w,     startCorner.getY() + w,      startCorner.getZ() + offset));
            points.push_back(Point3D(startCorner.getX() + w,     startCorner.getY(),          startCorner.getZ() + offset));

            // top Sq (in clockwise order)
            points.push_back(Point3D(startCorner.getX(),         startCorner.getY(),          startCorner.getZ() + w + offset));
            points.push_back(Point3D(startCorner.getX(),         startCorner.getY() + w,      startCorner.getZ() + w + offset));
            points.push_back(Point3D(startCorner.getX() + w,     startCorner.getY() + w,      startCorner.getZ() + w + offset));
            points.push_back(Point3D(startCorner.getX() + w,     startCorner.getY(),          startCorner.getZ() + w + offset));
        }

        friend std::ostream &operator<<(std::ostream &strm, const Cube &cube)
        {
            const int vertices = 8;
            for (int i = 0; i < vertices; i++)
            {
                strm << "Point(" << i << ") : " << cube.points[i] << "\n";
            }
            return strm;
        }

        std::vector<Point3D> &getVertices() { return this->points; }

        void drawCube(SDL_Renderer *renderer, double foc_len)
        {
            // render all lines
            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
            std::vector<Point2D> mp;

            for (int i = 0; i < 8; i++)
            {
                Point2D point = (points[i].projectPoint(foc_len));
                point.x += WIDTH / 2 ;
                point.y += HEIGHT / 2 ;
                mp.push_back(point);
                // points[i].projectPoint(foc_len).drawPoint(renderer);
            }
            SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);
            // top square
            SDL_RenderLine(renderer, mp[0].x, mp[0].y, mp[1].x, mp[1].y);
            SDL_RenderLine(renderer, mp[1].x, mp[1].y, mp[2].x, mp[2].y);
            SDL_RenderLine(renderer, mp[2].x, mp[2].y, mp[3].x, mp[3].y);
            SDL_RenderLine(renderer, mp[3].x, mp[3].y, mp[0].x, mp[0].y);

            SDL_SetRenderDrawColor(renderer, 0, 255, 255, 255);
            // bottom square 
            SDL_RenderLine(renderer, mp[4].x, mp[4].y, mp[5].x, mp[5].y);
            SDL_RenderLine(renderer, mp[5].x, mp[5].y, mp[6].x, mp[6].y);
            SDL_RenderLine(renderer, mp[6].x, mp[6].y, mp[7].x, mp[7].y);
            SDL_RenderLine(renderer, mp[7].x, mp[7].y, mp[4].x, mp[4].y);

            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
            // their connection
            SDL_RenderLine(renderer, mp[0].x, mp[0].y, mp[4].x, mp[4].y);
            SDL_RenderLine(renderer, mp[1].x, mp[1].y, mp[5].x, mp[5].y);
            SDL_RenderLine(renderer, mp[2].x, mp[2].y, mp[6].x, mp[6].y);
            SDL_RenderLine(renderer, mp[3].x, mp[3].y, mp[7].x, mp[7].y);

            // SDL_RenderLine(renderer, points[0]);
        }

        void drawVertices(SDL_Renderer *rend, double foc_len)
        {
            for (Point3D p : this->points)
            {
                p.projectPoint(foc_len).drawPoint(rend);
            }
        }

        void print_cube_projection(double foc_len)
        {
            int i = 0;
            for (Point3D p : this->points)
            {
                std::cout << "(" << i << ") " << p << " | " << p.projectPoint(foc_len) << std::endl;
            }
        }

        void rotateCube(double theta, int type)
        {
            theta = convertDegToRad(theta);

            double matrixX[3][3] = { 
                {std::cos(theta), -std::sin(theta), 0},
                {std::sin(theta), std::cos(theta), 0},
                {0, 0, 1}
            };

            double matrixY[3][3] = {
                {1, 0, 0},
                {0, std::cos(theta), -std::sin(theta)},
                {0, std::sin(theta), std::cos(theta)}};

            double matrixZ[3][3] = {
                {std::cos(theta), 0, std::sin(theta)},
                {0, 1, 0},
                {-std::sin(theta), 0, std::cos(theta)}};

            for (Point3D &p : points)
            {
                // first we translate it to the z axis
                // then, we rotate
                // then, we bring it back out
                double _x = p.getX() - startCorner.getX() - w/2;
                double _y = p.getY() - startCorner.getY() - w/2;
                double _z = p.getZ() - startCorner.getZ() - ::offset - w/2;

                std::vector<double> rotated;
                if (type & X)
                {
                    rotated = multiplyByMatrix(_x, _y, _z, matrixX);
                    _x = rotated[0];
                    _y = rotated[1];
                    _z = rotated[2];
                }
                if (type & Y)
                {
                    rotated = multiplyByMatrix(_x, _y, _z, matrixY);
                    _x = rotated[0];
                    _y = rotated[1];
                    _z = rotated[2];
                }
                if (type & Z)
                {
                    rotated = multiplyByMatrix(_x, _y, _z, matrixZ);
                    _x = rotated[0];
                    _y = rotated[1];
                    _z = rotated[2];
                }


                _x += startCorner.getX() + w / 2;
                _y += startCorner.getY() + w/2;
                _z += startCorner.getZ() + ::offset + w/2;

                p.setX(_x);
                p.setY(_y);
                p.setZ(_z);
            }
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

    const int FOV = 90;

    Camera cam(FOV);

    double foc_len = cam.getFocLen();

    // decide the coordinates relative to foc length
    const double width = 50;
    Point3D cornerPoint(-width/2, -width/2, 0);
    Cube cube(width, cornerPoint);

    std::cout << "focal length: " << foc_len;
    cube.print_cube_projection(foc_len);

    while (running)
    {
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
        SDL_RenderClear(renderer);

        cube.drawCube(renderer, foc_len);
        int rotationOption = Z;
        double theta = 0.3;
        cube.rotateCube(theta, rotationOption);

        while (SDL_PollEvent(&event))
        {
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


