#ifndef PPU_H
#define SDL_MAIN_HANDLED
#include<vector>
//LINUX
#include "SDL2/SDL.h"
//Windows
//#include <SDL.h>
#include <stdio.h>
#include<iostream>

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
#endif PPU_H
