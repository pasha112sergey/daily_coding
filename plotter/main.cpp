#include "gbm.hpp"
#include <SDL2/SDL.h>
#include <iostream>
#include "function.hpp"
#include "point.cpp"
#include "config.hpp"
using namespace std;

void drawAxes(SDL_Renderer *renderer) {
	Uint8 r, g, b, a;
	if (SDL_GetRenderDrawColor(renderer, &r, &g, &b, &a) != 0) {
		cout << "Error! getrenderdrawcolor failed" << endl;
		return;
	}

	if (SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0xff) != 0) {
		cout << "Error setting renderer draw color to black!" << endl;
		return;
	}

	// x axis
	SDL_RenderDrawLine(renderer, 0, HALF_HEIGHT, WINDOW_WIDTH, HALF_HEIGHT);
	// y axis
	SDL_RenderDrawLine(renderer, HALF_WIDTH, 0, HALF_WIDTH, WINDOW_HEIGHT);

	
	if (SDL_SetRenderDrawColor(renderer, r, g, b, a) != 0) {
		cout << "Error setting renderer draw color to black!" << endl;
		return;
	}
}

int main() {
	string funcStr;
	cout << "Please input your function as a function of x: ";
	cin >> funcStr;
	cout << funcStr;
	Function func(funcStr);
	double sig = 0.2;
	double dt = 0.001;

	GBM_Function stoc(func, sig, dt);

	SDL_Window *w;
	SDL_Renderer *r;
	SDL_Surface *surf;
	
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		cerr << "init error" << SDL_GetError() << endl;
		return 1;
	}	
	
	if (SDL_CreateWindowAndRenderer(WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_RESIZABLE, &w, &r)) {
		cerr << "error: " << SDL_GetError() << endl;
		return 1;
	}	
	
	bool quit = false;
	SDL_RenderClear(r);
	SDL_Event e;
	while (!quit) {
		SDL_PollEvent(&e);
		if (e.type == SDL_QUIT) {
			break;
		}

		SDL_RenderClear(r);
		SDL_SetRenderDrawColor(r, 0xff, 0xff, 0xff, 0xff);

		drawAxes(r);
		
		double prevY = func.evalAt(DOMAIN_MIN);
		for (double x = DOMAIN_MIN; x < DOMAIN_MAX; x+=STEP) {
			double dy = stoc.dy(x);
			double y = prevY + dy;
			Point p{x, y};
			p.plot(w, r);	
			prevY = y;
		}
		SDL_RenderPresent(r);
	}

	SDL_DestroyRenderer(r);
	SDL_DestroyWindow(w);
	SDL_Quit();
	return 0;
}		
