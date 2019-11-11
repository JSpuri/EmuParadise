#ifndef SDL_MAIN_HANDLED
#define SDL_MAIN_HANDLED
<<<<<<< HEAD

#include <SDL2/SDL.h>
#include <stdio.h>
#include<iostream>
#include<vector>
=======
#include<vector>
//LINUX
#include "SDL2/SDL.h"
//Windows
//#include <SDL.h>
#include <stdio.h>
#include<iostream>
#include "../common/constants.hpp"
>>>>>>> 4f5ed3c443c1a59ca60077f6e1bd1c41678d018d

//Screen dimension constants
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;
const int NES_WIDTH = 256;
const int NES_HEIGHT = 240;

void tela(std::vector<std::vector<uint8_t>> p_matrix);
void updateTela(std::vector<std::vector<uint8_t>> p_matrix);
//void put_pixel8(SDL_Surface* surface, int x, int y, Uint8 pixel);
//bool initSDL();
//bool loadMedia();
//void closeSDL();
#endif
