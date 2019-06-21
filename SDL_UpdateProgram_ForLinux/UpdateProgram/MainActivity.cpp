#define _CRT_SECURE_NO_WARNINGS

#include "Global.h"
#include "MainActivity.h"
#include "LTexture.h"
#include "han2unicode.h"
#include <thread>
#include <iostream>

//Top left position
SDL_Point mPosition;

//Rendered texture
LTexture gTextTexture;

//Background Image
LTexture gBackground;


int currentButtonEvent[2];

Layout* layout;

ProgressBar* progess;
int pro = 0;

MainActivity::MainActivity(std::string name, int width, int height)
{
	PROGRAM_NAME = name;
	SCREEN_WIDTH = width;
	SCREEN_HEIGHT = height;

	if (!init())
	{
		std::cout << "Failed to initialize!" << std::endl;
	}
	if (!loadMedia()) {
		std::cout << "Failed to load media!" << std::endl;
	}
	else {
		setLayout();
		drawLayout();
		loop();
	}

	currentButtonEvent[0] = 0;
	currentButtonEvent[1] = 0;
}

bool MainActivity::init()
{
	//Initialize SDL
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		std::cout << "SDL could not initialize! SDL Error: " << SDL_GetError() << std::endl;
		return false;
	}

	//Set texture filtering to linear
	if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1"))
	{
		std::cout << "Warning: Linear texture filtering not enabled!" << std::endl;
	}

	//Initialize SDL_ttf
	if (TTF_Init() == -1)
	{
		printf("SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError());
		return false;
	}

	//Create window
	gWindow = SDL_CreateWindow("KLAK Update ", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
	if (gWindow == NULL)
	{
		std::cout << "Window could not be created! SDL Error: " << SDL_GetError() << std::endl;
		return false;
	}

	//Create renderer for window
	gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED);
	if (gRenderer == NULL)
	{
		std::cout << "Renderer could not be created! SDL Error: " << SDL_GetError() << std::endl;
		return false;
	}

	return true;
}

void MainActivity::loop()
{
	//Main loop flag
	bool quit = false;

	//Event handler
	SDL_Event e;

	//While application is running
	while (!quit)
	{
		//Handle events on queue
		while (SDL_PollEvent(&e) != 0)
		{
			//User requests quit
			if (e.type == SDL_QUIT)
			{
				quit = true;
			}
			else {
				checkButtonEvent(&e);
				if (e.type == SDL_MOUSEBUTTONDOWN) {
					int x, y;
					SDL_GetMouseState(&x, &y);

					switch (e.button.button)
					{
					case SDL_BUTTON_LEFT: {
						printf("M_LEFT | clicks: %d | pos: %d , %d\n", e.button.clicks, e.button.x, e.button.y); break;
					}
					case SDL_BUTTON_RIGHT: {
						printf("M_RIGHT | clicks: %d | pos: %d , %d\n", e.button.clicks, e.button.x, e.button.y); break;
					}
					default:
						std::cout << "M_DE" << std::endl;		break;
					}
				}
			}
		}
		//loop here
	}
}


MainActivity::~MainActivity()
{
	gBackground.free();

	//Free loaded images
	gTextTexture.free();

	//Free global font
	TTF_CloseFont(gFont);
	gFont = NULL;

	//Destroy window
	SDL_DestroyRenderer(gRenderer);
	SDL_DestroyWindow(gWindow);
	gWindow = NULL;
	gRenderer = NULL;

	//Quit SDL subsystems
	TTF_Quit();
	SDL_Quit();
}

#pragma region Private Method

bool MainActivity::loadMedia()
{
	//Loading success flag
	bool success = true;

	//Open the font
	gFont = TTF_OpenFont("Fonts/NanumBarunGothic.ttf", 28);
	if (gFont == NULL)
	{
		printf("Failed to load lazy font! SDL_ttf Error: %s\n", TTF_GetError());
		success = false;
	}
	else
	{
		//Render text
		SDL_Color textColor = { 0xFF, 0xFF, 0xFF };

		unsigned short unicode[128];
		han2unicode("Update파일을 발견했습니다.", unicode);

		if (!gTextTexture.loadFromRenderedText(unicode, textColor))
		{
			printf("Failed to render text texture!\n");
			success = false;
		}
	}

	if (!gBackground.loadFromFile("Resources/Images/cubes_structure_black_blue_1024x600.bmp")) {
		printf("Unable to load image %s! SDL Error: %s\n", "Resources/Images/cubes_structure_black_blue_1024x600.bmp", SDL_GetError());
		success = false;
	}

	return success;
}

void MainActivity::setLayout()
{
	layout = new Layout(SCREEN_WIDTH, SCREEN_HEIGHT);

	buttons[0] = { SCREEN_WIDTH / 2 - 180, SCREEN_HEIGHT / 2 -15, 120, 30 };
	buttons[1] = { SCREEN_WIDTH / 2 + 60, SCREEN_HEIGHT / 2 - 15, 120, 30 };

	progess = new ProgressBar(SCREEN_WIDTH / 2 - 120, SCREEN_HEIGHT / 2 + 80, 240, 30);
}

void MainActivity::drawLayout()
{
	SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
	SDL_RenderClear(gRenderer);

	gBackground.render(0, 0);

	layout->Draw();

	gTextTexture.render((SCREEN_WIDTH - gTextTexture.getWidth()) / 2,
		(SCREEN_HEIGHT - gTextTexture.getHeight()) / 2 - 80);
	//Don't forget too free your surface and texture

	for (int i =0; i< sizeof(buttons) / sizeof(buttons[0]); ++i)
	{
		//if (buttons[i] != NULL) {
			SDL_RenderDrawRect(gRenderer, &buttons[i]);
		//}
	}

	progess->render();

	//SDL_RenderPresent(gRenderer);
}

void MainActivity::checkButtonEvent(SDL_Event* e) {
	int index = 0;
	for (int i =0; i< sizeof(buttons) / sizeof(buttons[0]); ++i)
	{
		//if (buttons[i] != NULL) {
			if (e->button.x > buttons[i].x && e->button.x < buttons[i].x + buttons[i].w
				&& e->button.y > buttons[i].y && e->button.y < buttons[i].y + buttons[i].h) {
				switch (e->type) {
				case SDL_MOUSEMOTION:
					if (currentButtonEvent[index] != 1) {
						currentButtonEvent[index] = 1;
						std::cout << "Button [" << index << "] on" << std::endl;
						SDL_SetRenderDrawColor(gRenderer, 0xFF, 0, 0, 0);

						gBackground.render(0, 0);
						SDL_RenderDrawRect(gRenderer, &buttons[i]);
					}
					break;

				case SDL_MOUSEBUTTONDOWN:
					if (currentButtonEvent[index] != 2) {
						currentButtonEvent[index] = 2;
						std::cout << "Button [" << index << "] down" << std::endl;
						SDL_SetRenderDrawColor(gRenderer, 0, 0, 0xFF, 0);
						SDL_RenderDrawRect(gRenderer, &buttons[i]);

						/*
						for (int i = 1; i <= 100; i++) {
							progess->setProgress(i);
							SDL_RenderPresent(gRenderer);
							sleep(1);
						}
						*/
					}
					break;

				case SDL_MOUSEBUTTONUP:
					if (currentButtonEvent[index] != 3) {
						currentButtonEvent[index] = 3;
						std::cout << "Button [" << index << "] up" << std::endl;
						SDL_SetRenderDrawColor(gRenderer, 0, 0xFF, 0, 0);
						SDL_RenderDrawRect(gRenderer, &buttons[i]);
					}
					break;
				}
			}
			else {
				if (currentButtonEvent[index] != 0) {
					currentButtonEvent[index] = 0;
					SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
					SDL_RenderDrawRect(gRenderer, &buttons[i]);
				}
			}
		//}
		index++;
	}
}

#pragma endregion
