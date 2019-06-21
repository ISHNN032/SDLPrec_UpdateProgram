#ifndef _GLOBAL_
#define _GLOBAL_

#include <SDL.h>
#include <SDL_ttf.h>

#pragma once

//The window we'll be rendering to
extern SDL_Window* gWindow;

//The window renderer
extern SDL_Renderer* gRenderer;

//Globally used font
extern TTF_Font* gFont;

#endif // DEBUG