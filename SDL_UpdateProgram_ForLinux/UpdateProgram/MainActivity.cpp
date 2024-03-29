#define _CRT_SECURE_NO_WARNINGS

#include "MainActivity.h"
#include "Global.h"
#include "UpdateManager.h"
#include "han2unicode.h"
#include <iostream>
#include <unistd.h>
#include <pthread.h>

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
		drawLayout(0);
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

	buttons[0] = new Button( SCREEN_WIDTH / 2 - 180, SCREEN_HEIGHT / 2 + 60, 118, 34, {0xFF,0xFF,0xFF,0xFF}, true );
	buttons[1] = new Button( SCREEN_WIDTH / 2 + 60, SCREEN_HEIGHT / 2 + 60, 118, 34, {0xFF,0xFF,0xFF,0xFF}, false );

	progessbar = new ProgressBar(SCREEN_WIDTH / 2 - 120, SCREEN_HEIGHT / 2 + 60);
}

void MainActivity::drawLayout(int scene)
{
	SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
	SDL_RenderClear(gRenderer);

	gBackground.render(0, 0);

	layout->Draw();

	gTextTexture.render((SCREEN_WIDTH - gTextTexture.getWidth()) / 2,
		(SCREEN_HEIGHT - gTextTexture.getHeight()) / 2 - 80);

	switch (scene)
	{
	case 0:
		for (int i =0; i< sizeof(buttons) / sizeof(buttons[0]); ++i)
		{
			buttons[i]->render();
		}
		break;
	case 1:
		progessbar->render();
		break;
	default:
		break;
	}

	SDL_RenderPresent(gRenderer);
}

void *t_function(void *data)
{
	UpdateManager manager = UpdateManager();
	if(! manager.updateFW()){
		std::cout<<"FW Update Failed."<<std::endl;
	}
	update_ongoing = false;
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
					drawLayout(0);
				}
				break;

			case SDL_MOUSEBUTTONDOWN:
				if (currentButtonEvent[i] != 2) {
					currentButtonEvent[i] = 2;
					std::cout << "Button [" << i << "] down" << std::endl;
					buttons[i]->setColor({0, 0, 0xFF, 0});
					buttons[i]->onButtonDown();
					drawLayout(0);

					if(i == 0){
						unsigned short unicode[128];
						han2unicode("Updating", unicode);
						gTextTexture.loadFromRenderedText(unicode, {0xFF, 0xFF, 0xFF});
						
						pthread_t p_thread;
						int status;

						update_ongoing = true;

						//thread 생성에 실해하였다면 : thread id 값이 0보다 작으면
						if (pthread_create(&p_thread, NULL, t_function, NULL) < 0)
						{
							perror("thread create error : ");
							exit(0);
						}

						int j = 0;
						while(update_ongoing){
							progessbar->setProgress(j++);
							drawLayout(1);
							usleep(250000);
						}
					}
				}
				break;

			case SDL_MOUSEBUTTONUP:
				if (currentButtonEvent[i] != 3) {
					currentButtonEvent[i] = 3;
					std::cout << "Button [" << i << "] up" << std::endl;
					unsigned short unicode[128];
					han2unicode("Update File Detected", unicode);
					gTextTexture.loadFromRenderedText(unicode, {0xFF, 0xFF, 0xFF});

					buttons[i]->setColor({0, 0xFF, 0, 0});
					buttons[i]->onButtonUp();
					drawLayout(0);
				}
				break;
			}
		}
		else {
			switch (e->type) {
				case SDL_MOUSEBUTTONDOWN:
					if (currentButtonEvent[i] != 0) {
						currentButtonEvent[i] = 0;
						unsigned short unicode[128];
						han2unicode("Update File Detected", unicode);
						gTextTexture.loadFromRenderedText(unicode, {0xFF, 0xFF, 0xFF});
						buttons[i]->setColor({0xFF, 0xFF, 0xFF, 0xFF});
						buttons[i]->onButtonUp();
						drawLayout(0);
					}
					break;
			}
		}
	}
}

#pragma endregion
