#ifndef _MAINACTIVITY_
#define _MAINACTIVITY_

#include <SDL.h>
#include <SDL_ttf.h>
#include <string>
#include "LTexture.h"

//The window renderer
extern SDL_Renderer* gRenderer;

#pragma once
class MainActivity
{
public:
	MainActivity(std::string, int, int);
	bool init();
	void loop();
	~MainActivity();
private:
	bool loadMedia();
	void setLayout();
	void drawLayout();
	void checkButtonEvent(SDL_Event*);

	std::string PROGRAM_NAME;
	int SCREEN_WIDTH;
	int SCREEN_HEIGHT;

	SDL_Rect buttons[2];
};

struct Layout {
	int width, height;

	Layout(int screen_w, int screen_h) {
		width = screen_w;
		height = screen_h;
	}
	void Draw() {
		SDL_RenderDrawRect(gRenderer, new SDL_Rect{ 0, 0, width , height });
		SDL_RenderDrawRect(gRenderer, new SDL_Rect{ width / 4, height / 4,
							width / 2, height / 2 });
		SDL_RenderDrawLine(
			gRenderer, 0, height / 2, width, height / 2);
		SDL_RenderDrawLine(
			gRenderer, width / 2, 0, width / 2, height);
	}
};

/*
struct ProgressBar {
	SDL_Rect edge, filling;
	ProgressBar(int x, int y, int w, int h) {
		edge = { x, y, w, h };
		filling = { x, y, 0, h };
	}
	void render() {
		SDL_RenderDrawRect(gRenderer, &edge);
	}
	void setProgress(int per) {
		filling.w = (int)round(edge.w / 100.0 * per);
		SDL_RenderFillRect(gRenderer, &filling);
	}
};
*/
struct ProgressBar {
	LTexture splite;
	SDL_Rect edge, filling;
	ProgressBar(int x, int y, int w, int h) {
		if (!splite.loadFromFile("./Resources/Images/roll.bmp")) {
		printf("Unable to load image %s! SDL Error: %s\n", "Resources/Images/roll.bmp", SDL_GetError());
		}
		edge = { x, y, w, h };
		filling = { x, y, 0, h };
	}
	void render() {
		splite.render(edge.x, edge.y);
	}
	void setProgress(int per) {
		splite.render(edge.x, edge.y, NULL, per, NULL);

		//filling.w = (int)round(edge.w / 180.0 * per);
		//SDL_RenderFillRect(gRenderer, &filling);
	}
};

#endif //_MAINACTIVITY_