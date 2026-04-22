#include <iostream>
#include <SDL2/SDL.h>
#include "config.hpp"
using namespace std;

class Point {
	private: 	
		double _x;
		double _y;

		double getScreenX() const {
			return HALF_WIDTH + _x * (WINDOW_WIDTH / 20);
		}


		double getScreenY() const {
			return HALF_HEIGHT - _y * (WINDOW_HEIGHT / 20); 
		}

	public: 
		Point(double x, double y) : _x{x}, _y{y} {}
		
		void plot(SDL_Window *w, SDL_Renderer *r) const {
			double x = getScreenX();
			double y = getScreenY();
			if (x > WINDOW_WIDTH || x < 0) {
				return;
			}
			if (y > WINDOW_HEIGHT || y < 0) {
				return;
			}
			Uint8 _r,_g,_b,_a;
			if (SDL_GetRenderDrawColor(r, &_r, &_g, &_b, &_a) != 0) {	
				cout << "Error!" << endl;
				return;
			}

			SDL_SetRenderDrawColor(r, 0xff, 0x00, 0x00, 0xff);

			if (SDL_RenderDrawPoint(r, x, y) != 0) {
				cout << "error drawing point!" << endl;
				return;
			}
			SDL_SetRenderDrawColor(r, _r, _g, _b, _a);
		}
};

