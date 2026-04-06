// Rope.cpp : Defines the entry point for the application.
//

#include "Rope.h"

#include "SDL.h"

#include <vector>

const int SCREEN_HEIGHT = 1000;
const int SCREEN_WIDTH = 1900;
const uint32_t WHITE = 0xFFFFFFFF;
const int RADIUS = 15;

struct Point {
	double x;
	double y;
	double old_x;
	double old_y;
	bool locked;
};

int main(int argc, char* argv[]) {

	SDL_Init(SDL_INIT_VIDEO);

	SDL_Window* window = SDL_CreateWindow("Hello World", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);

	SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

	SDL_RenderSetLogicalSize(renderer, SCREEN_WIDTH, SCREEN_HEIGHT);

	bool running = true;

	uint32_t* pixels = new uint32_t[SCREEN_WIDTH * SCREEN_HEIGHT];

	SDL_Texture* texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, SCREEN_WIDTH, SCREEN_HEIGHT);

	int mouseY = 0;
	int mouseX = 0;

	SDL_MaximizeWindow(window);

	SDL_Event e;

	uint64_t NOW = SDL_GetPerformanceCounter();
	uint64_t LAST = 0;
	double deltaTime = 0;

	std::vector<Point> ropeNodes;

	while (running) {

		SDL_GetMouseState(&mouseX, &mouseY);

		uint32_t bgColor = (255 << 24) | (39 << 16) | (41 << 8) | 40;

		LAST = NOW;
		NOW = SDL_GetPerformanceCounter();
		deltaTime = (double)((NOW - LAST) * 1000 / (double)SDL_GetPerformanceFrequency());

		//calculate new Point positions
		for (int i = 0; i < std::size(ropeNodes); i++) {
			Point& p = ropeNodes[i];

			double vx = p.x - p.old_x;
			double vy = p.y - p.old_y;

			p.old_x = p.x;
			p.old_y = p.y;
			
			p.x += vx;
			p.y += vy;
			p.y += 0.2; // Gravity
			// Bottom bound
			if (p.y > SCREEN_HEIGHT - RADIUS) {
				p.y = SCREEN_HEIGHT - RADIUS;

				p.old_y = p.y + (vy * 0.9);
			}
			// Top bound
			else if (p.y < RADIUS) {
				p.y = RADIUS;
				p.old_y = p.y + (vy * 0.9);
			}

			// Right bound
			if (p.x > SCREEN_WIDTH - RADIUS) {
				p.x = SCREEN_WIDTH - RADIUS;
				p.old_x = p.x + (vx * 0.9);
			}
			// Left bound
			else if (p.x < RADIUS) {
				p.x = RADIUS;
				p.old_x = p.x + (vx * 0.9);
			}
		}

		for (int i = 0; i < SCREEN_WIDTH * SCREEN_HEIGHT; i++) {
			pixels[i] = bgColor;
		}

		for (int i = 0; i < std::size(ropeNodes); i++) {
			Point& p = ropeNodes[i];

			int minX = std::max(0, (int)p.x - RADIUS);
			int minY = std::max(0, (int)p.y - RADIUS);
			int maxX = std::min(SCREEN_WIDTH, (int)p.x + RADIUS);
			int maxY = std::min(SCREEN_HEIGHT, (int)p.y + RADIUS);

			for (int py = minY; py < maxY; py++) {
				for (int px = minX; px < maxX; px++) {

					if ((px - p.x) * (px - p.x) + ((py - p.y) * (py - p.y)) <= (RADIUS * RADIUS)) {
						pixels[py * SCREEN_WIDTH + px] = 0xFFFFFFFF;
					}
				}
			}
		}

		while (SDL_PollEvent(&e)) {

			if (e.type == SDL_QUIT) {
				running = false;
			}
			if (e.type == SDL_KEYDOWN) {
				switch (e.key.keysym.sym) {
				case SDLK_ESCAPE:
					running = false;
					break;
				}
			}
			if (e.type == SDL_MOUSEBUTTONDOWN) {

				Point p{ (double)mouseX, (double)mouseY, (double)mouseX, (double)mouseY, true };
				ropeNodes.push_back(p);

			}
		}

		SDL_UpdateTexture(texture, nullptr, pixels, SCREEN_WIDTH * sizeof(uint32_t));
		SDL_RenderCopy(renderer, texture, nullptr, nullptr);
		SDL_RenderPresent(renderer);


	}

	SDL_DestroyTexture(texture);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);

	delete[] pixels;
	return 0;
}