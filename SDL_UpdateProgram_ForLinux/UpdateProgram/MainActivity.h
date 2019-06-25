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
	void drawLayout(int);
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
		if (!splite.loadFromFile("./Resources/Images/dot/DOT_0.bmp")) {
			printf("Unable to load image %s! SDL Error: %s\n", "Resources/Images/dot/DOT_0.bmp", SDL_GetError());
		}
		pos_x = x;
		pos_y = y;
		angle = 0;
	}
	void render() {
		splite.render(pos_x, pos_y, NULL, 0, NULL);
	}
	void setProgress(int per) {
		splite.loadFromFile("./Resources/Images/dot/DOT_" + std::to_string(per%5) + ".bmp");
	}
};

struct Button {
	LTexture splite;
	SDL_Rect rect;
	SDL_Color drawcolor;
	bool flag;

	Button(int x, int y, int w, int h, SDL_Color color, bool b) {
		rect = {x + 1, y + 1, w, h};
		drawcolor = color;
		flag = b;
		onButtonUp();
	}
	void render() {
		splite.render(rect.x - 1, rect.y - 1, NULL, 0, NULL);
		SDL_SetRenderDrawColor(gRenderer, drawcolor.r, drawcolor.g, drawcolor.b, drawcolor.a);
		SDL_RenderDrawRect(gRenderer, &rect);
	}
	void setColor(SDL_Color color) {
		drawcolor = color;
	}
	void onButtonDown(){
		if(flag){
			if (!splite.loadFromFile("./Resources/Images/button/nuleunye.bmp"))
				printf("Unable to load image %s! SDL Error: %s\n", "Resources/Images/button/nuleunye.bmp", SDL_GetError());
		}
		else{
			if (!splite.loadFromFile("./Resources/Images/button/nuleunaniyo.bmp"))
				printf("Unable to load image %s! SDL Error: %s\n", "Resources/Images/button/nuleunaniyo.bmp", SDL_GetError());
		}
	}
	void onButtonUp(){
		if(flag){
			if (!splite.loadFromFile("./Resources/Images/button/ye.bmp"))
				printf("Unable to load image %s! SDL Error: %s\n", "Resources/Images/button/ye.bmp", SDL_GetError());
		}
		else{
			if (!splite.loadFromFile("./Resources/Images/button/aniyo.bmp"))
				printf("Unable to load image %s! SDL Error: %s\n", "Resources/Images/button/aniyo.bmp", SDL_GetError());
		}
	}
};

#endif //_MAINACTIVITY_