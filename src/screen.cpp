#include "headers/screen.hpp"
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */

// tabela de cores da paleta do nes
const Uint32 colors[16 * 4] = { 0x545454, 0x001E74, 0x081090, 0x300088, 0x440064, 0x5C0030, 0x540400, 0x3C1800, 0x202A00, 0x083A00, 0x004000, 0x003C00, 0x00323C, 0x000000, 0x000000, 0x000000,
								0x989698, 0x084BC4, 0x3032EC, 0x5C1EE4, 0x8814B0, 0xA01464, 0x982220, 0x783C00, 0x545A00, 0x287200, 0x087C00, 0x007628, 0x006678, 0x000000, 0x000000, 0x000000,
								0xECEEEC, 0x4C9AEC, 0x787CEC, 0xB062EC, 0xE454EC, 0xEC58B4, 0xEC6A64, 0xD48820, 0xA0AA00, 0x74C400, 0x4CD020, 0x38CC6C, 0x38B4CC, 0x3C3C3C, 0x000000, 0x000000,
								0xECEEEC, 0xA8CCEC, 0xBCBCEC, 0xD4B2EC, 0xECAEEC, 0xECAED4, 0xECB4B0, 0xE4C490, 0xCCD278, 0xB4DE78, 0xA8E290, 0x98E2B4, 0xA0D6E4, 0xA0A2A0, 0x000000, 0x000000
};


SDL_Window* window;

SDL_Renderer* renderer;
SDL_Texture* texture;

void tela(std::vector<std::vector<uint8_t>> p_matrix) {

	bool leftMouseButtonDown = false;
	bool quit = false;
	SDL_Event event;

	SDL_Init(SDL_INIT_VIDEO);

	window = SDL_CreateWindow("EmuParadise", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, 0);

	renderer = SDL_CreateRenderer(window, -1, 0);
	texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGB888, SDL_TEXTUREACCESS_STATIC, NES_WIDTH, NES_HEIGHT);
	Uint32* pixels = new Uint32[NES_WIDTH * NES_HEIGHT];

	// inicializa todos os pixels da textura como branco
	memset(pixels, 255, NES_WIDTH * NES_HEIGHT * sizeof(Uint32));

	for (int i = 0; i < NES_WIDTH; i++) {
		for (int j = 0; j < NES_HEIGHT; j++) {
			pixels[j * NES_WIDTH + i] = colors[p_matrix[i][j]];
		}
	}


	SDL_UpdateTexture(texture, NULL, pixels, NES_WIDTH * sizeof(Uint32));

	SDL_RenderClear(renderer);
	SDL_RenderCopy(renderer, texture, NULL, NULL);
	SDL_RenderPresent(renderer);

}
void updateTela(std::vector<std::vector<uint8_t>> p_matrix) {

	srand(time(NULL));

	Uint32* pixels = new Uint32[NES_WIDTH * NES_HEIGHT];
	for (int i = 0; i < NES_WIDTH; i++) {
		for (int j = 0; j < NES_HEIGHT; j++) {
			pixels[j * NES_WIDTH + i] = p_matrix[i][j];
		}
	}


	SDL_UpdateTexture(texture, NULL, pixels, NES_WIDTH * sizeof(Uint32));

	SDL_RenderClear(renderer);
	SDL_RenderCopy(renderer, texture, NULL, NULL);
	SDL_RenderPresent(renderer);


}


