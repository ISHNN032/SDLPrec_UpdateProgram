#include "Global.h"
#include "LTexture.h"

//The window we'll be rendering to
SDL_Window* gWindow = NULL;

//The window renderer
SDL_Renderer* gRenderer = NULL;

//Globally used font
TTF_Font* gFont = NULL;

bool update_ongoing = false;