#include "gbm.hpp"
#include "plotter.hpp"
#include <SDL2/SDL.h>
#include <iostream>
using namespace std;

int main() {
	cout << "Quadratic function plotter with GBM" << endl;
	double a, b, c;
	cout << "Please input the first coefficient: " << endl;	
	cin >> a;
	cout << "Second... " << endl;
	cin >> b;
	cout << "Last... " << endl;
	cin >> c;
	
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		cerr << "init error" << SDL_GetError() << endl;
		return 1;
	}	
	
	SDL_Window *window = SDL_CreateWindow(
		"plotter",
		SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
		600, 600,
		SDL_WINDOW_SHOWN
	);

	if (!window) {
		cerr << "Window could not be created" << SDL_GetError() << endl;
		SDL_Quit();
		return 1;
	}
	bool quit = false;
	while (!quit) {
	
	}
}	
