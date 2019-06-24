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
	//init render
	SDL_RenderPresent(gRenderer);
	
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
	gFont = TTF_OpenFont("./Fonts/NanumBarunGothic.ttf", 28);
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
		han2unicode("Update File Detected", unicode);

		if (!gTextTexture.loadFromRenderedText(unicode, textColor))
		{
			printf("Failed to render text texture!\n");
			success = false;
		}
	}

	if (!gBackground.loadFromFile("./Resources/Images/cubes_structure_black_blue_1024x600.bmp")) {
		printf("Unable to load image %s! SDL Error: %s\n", "Resources/Images/cubes_structure_black_blue_1024x600.bmp", SDL_GetError());
		success = false;
	}

	return success;
}

void MainActivity::setLayout()
{
	layout = new Layout(SCREEN_WIDTH, SCREEN_HEIGHT);

	buttons[0] = new Button( SCREEN_WIDTH / 2 - 180, SCREEN_HEIGHT / 2 -15, 120, 30, {0xFF,0xFF,0xFF,0xFF} );
	buttons[1] = new Button( SCREEN_WIDTH / 2 + 60, SCREEN_HEIGHT / 2 - 15, 120, 30, {0xFF,0xFF,0xFF,0xFF} );

	progessbar = new ProgressBar(SCREEN_WIDTH / 2 - 120, SCREEN_HEIGHT / 2 + 80);
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
		buttons[i]->render();
	}

	progessbar->render();

	SDL_RenderPresent(gRenderer);
}

void MainActivity::checkButtonEvent(SDL_Event* e) {
	for (int i =0; i< sizeof(buttons) / sizeof(buttons[0]); ++i)
	{
		if (e->button.x > buttons[i]->rect.x && e->button.x < buttons[i]->rect.x + buttons[i]->rect.w
			&& e->button.y > buttons[i]->rect.y && e->button.y < buttons[i]->rect.y + buttons[i]->rect.h) {
			switch (e->type) {
			case SDL_MOUSEMOTION:
				if (currentButtonEvent[i] != 1) {
					currentButtonEvent[i] = 1;
					std::cout << "Button [" << i << "] on" << std::endl;
					buttons[i]->setColor({0xFF, 0, 0, 0});
					drawLayout();
				}
				break;

			case SDL_MOUSEBUTTONDOWN:
				if (currentButtonEvent[i] != 2) {
					currentButtonEvent[i] = 2;
					std::cout << "Button [" << i << "] down" << std::endl;
					buttons[i]->setColor({0, 0, 0xFF, 0});
					drawLayout();

					if(i == 1){
						for (int i = 1; i <= 180; i++) {
							progessbar->setProgress(i);
							drawLayout();
							sleep(0.1);
						}
					}
				}
				break;

			case SDL_MOUSEBUTTONUP:
				if (currentButtonEvent[i] != 3) {
					currentButtonEvent[i] = 3;
					std::cout << "Button [" << i << "] up" << std::endl;
					buttons[i]->setColor({0, 0xFF, 0, 0});
					drawLayout();
				}
				break;
			}
		}
		else {
			if (currentButtonEvent[i] != 0) {
				currentButtonEvent[i] = 0;
				buttons[i]->setColor({0xFF, 0xFF, 0xFF, 0xFF});
				drawLayout();
			}
		}
		SDL_RenderPresent(gRenderer);
	}
}

#pragma endregion
