#include <SDL.h>
#include <SDL_ttf.h>

#include "LTexture.h"

//The window we'll be rendering to
extern SDL_Window* gWindow = NULL;

//The window renderer
extern SDL_Renderer* gRenderer = NULL;

//Globally used font
extern TTF_Font* gFont = NULL;