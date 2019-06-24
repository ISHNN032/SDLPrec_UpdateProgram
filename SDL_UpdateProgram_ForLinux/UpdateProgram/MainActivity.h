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

	struct Button* buttons[2];
	struct Layout* layout;
	struct ProgressBar* progessbar;

	int currentButtonEvent[2];
};

struct Layout {
	int width, height;

	Layout(int screen_w, int screen_h) {
		width = screen_w;
		height = screen_h;
	}
	void Draw() {
		SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);

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
	int pos_x, pos_y, angle;
	ProgressBar(int x, int y) {
		if (!splite.loadFromFile("./Resources/Images/roll.bmp")) {
			printf("Unable to load image %s! SDL Error: %s\n", "Resources/Images/roll.bmp", SDL_GetError());
		}
		pos_x = x;
		pos_y = y;
		angle = 0;
	}
	void render() {
		splite.render(pos_x, pos_y, NULL, angle, NULL);
	}
	void setProgress(int per) {
		angle = per;
	}
};

struct Button {
	SDL_Rect rect;
	SDL_Color drawcolor;
	Button(int x, int y, int w, int h, SDL_Color color) {
		drawcolor = color;
		rect = {x, y, w, h};
	}
	void render() {
		SDL_SetRenderDrawColor(gRenderer, drawcolor.r, drawcolor.g, drawcolor.b, drawcolor.a);
		SDL_RenderDrawRect(gRenderer, &rect);
	}
	void setColor(SDL_Color color) {
		drawcolor = color;
	}
};

#endif //_MAINACTIVITY_