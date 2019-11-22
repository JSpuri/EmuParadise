#include "headers/screen.hpp"

#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */
#include <signal.h>
#include <stdio.h>

// tabela de cores da paleta do nes
const Uint32 colors[16 * 4] = { 0x545454, 0x001E74, 0x081090, 0x300088, 0x440064, 0x5C0030, 0x540400, 0x3C1800, 0x202A00, 0x083A00, 0x004000, 0x003C00, 0x00323C, 0x000000, 0x000000, 0x000000,
								0x989698, 0x084BC4, 0x3032EC, 0x5C1EE4, 0x8814B0, 0xA01464, 0x982220, 0x783C00, 0x545A00, 0x287200, 0x087C00, 0x007628, 0x006678, 0x000000, 0x000000, 0x000000,
								0xECEEEC, 0x4C9AEC, 0x787CEC, 0xB062EC, 0xE454EC, 0xEC58B4, 0xEC6A64, 0xD48820, 0xA0AA00, 0x74C400, 0x4CD020, 0x38CC6C, 0x38B4CC, 0x3C3C3C, 0x000000, 0x000000,
								0xECEEEC, 0xA8CCEC, 0xBCBCEC, 0xD4B2EC, 0xECAEEC, 0xECAED4, 0xECB4B0, 0xE4C490, 0xCCD278, 0xB4DE78, 0xA8E290, 0x98E2B4, 0xA0D6E4, 0xA0A2A0, 0x000000, 0x000000
};

Controller *joypads;

SDL_Window* window;

SDL_Renderer* renderer;
SDL_Texture* texture;

SDL_Event event;

Uint32* pixels = new Uint32[NES_WIDTH * NES_HEIGHT];

const Uint8* keystates;

void tela(uint32_t *p_matrix, Controller *controllers) {

    joypads = controllers;

	bool leftMouseButtonDown = false;
	bool quit = false;
	SDL_Event event;

	// SDL_Init(SDL_INIT_VIDEO);

  struct sigaction action;
  sigaction(SIGINT, NULL, &action);
  SDL_Init(SDL_INIT_EVERYTHING);
  sigaction(SIGINT, &action, NULL);

	window = SDL_CreateWindow("EmuParadise", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, 0);

	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGB888, SDL_TEXTUREACCESS_STREAMING, NES_WIDTH, NES_HEIGHT);

	// inicializa todos os pixels da textura como branco
	memset(pixels, 255, NES_WIDTH * NES_HEIGHT * sizeof(Uint32));

    //for (int i = 0; i < NES_WIDTH; i++) {
        //for (int j = 0; j < NES_HEIGHT; j++) {
            //pixels[j * NES_WIDTH + i] = colors[p_matrix)[i][j]];
        //}
    //}

	SDL_UpdateTexture(texture, NULL, pixels, NES_WIDTH * sizeof(Uint32));

	SDL_RenderClear(renderer);
	SDL_RenderCopy(renderer, texture, NULL, NULL);
	SDL_RenderPresent(renderer);

}

void updateControllers() {
    int i = 0;

	keystates = SDL_GetKeyboardState(NULL);

    for (; SDL_PollEvent(&event) && i < 2; i++) {   //[> Loop until there are no events left on the queue <]
        switch (event.type) {         //[> Process the appropiate event type <]
            case SDL_KEYUP:
                    // printf("Tecla soltada!\n");
                /*switch(event.key.keysym.sym){
                    case SDLK_v: joypads->ReleaseButtonJP1('A'); break;
                    case SDLK_b: joypads->ReleaseButtonJP1('B'); break;
                    case SDLK_g: joypads->ReleaseButtonJP1('s'); break;
                    case SDLK_f: joypads->ReleaseButtonJP1('S'); break;
                    case SDLK_w: joypads->ReleaseButtonJP1('U'); break;
                    case SDLK_s: joypads->ReleaseButtonJP1('D'); break;
                    case SDLK_a: joypads->ReleaseButtonJP1('L'); break;
                    case SDLK_d: joypads->ReleaseButtonJP1('R'); break;

                    case SDLK_o: joypads->ReleaseButtonJP2('A'); break;
                    case SDLK_p: joypads->ReleaseButtonJP2('B'); break;
                    case SDLK_0: joypads->ReleaseButtonJP2('s'); break;
                    case SDLK_9: joypads->ReleaseButtonJP2('S'); break;
                    case SDLK_UP: joypads->ReleaseButtonJP2('U'); break;
                    case SDLK_DOWN: joypads->ReleaseButtonJP2('D'); break;
                    case SDLK_LEFT: joypads->ReleaseButtonJP2('L'); break;
                    case SDLK_RIGHT: joypads->ReleaseButtonJP2('R'); break;
                }
                break;

            case SDL_KEYDOWN:        //[> Handle a KEYDOWN event <]
                    // printf("Tecla pressionada!\n");
                switch(event.key.keysym.sym){
                    case SDLK_v: joypads->PressButtonJP1('A'); break;
                    case SDLK_b: joypads->PressButtonJP1('B'); break;
                    case SDLK_g: joypads->PressButtonJP1('s'); break;
                    case SDLK_f: joypads->PressButtonJP1('S'); break;
                    case SDLK_w: joypads->PressButtonJP1('U'); break;
                    case SDLK_s: joypads->PressButtonJP1('D'); break;
                    case SDLK_a: joypads->PressButtonJP1('L'); break;
                    case SDLK_d: joypads->PressButtonJP1('R'); break;

                    case SDLK_o: joypads->PressButtonJP2('A'); break;
                    case SDLK_p: joypads->PressButtonJP2('B'); break;
                    case SDLK_0: joypads->PressButtonJP2('s'); break;
                    case SDLK_9: joypads->PressButtonJP2('S'); break;
                    case SDLK_UP: joypads->PressButtonJP2('U'); break;
                    case SDLK_DOWN: joypads->PressButtonJP2('D'); break;
                    case SDLK_LEFT: joypads->PressButtonJP2('L'); break;
                    case SDLK_RIGHT: joypads->PressButtonJP2('R'); break;
                }
                break;*/

            case SDL_MOUSEMOTION:
                break;

            case SDL_QUIT:
                exit(1);
                break;

            default:                 //[> Report an unhandled event <]
                break;
            //printf("I don't know what this event is!\n");
        }
    }

	if(keystates[SDL_SCANCODE_UP])		joypads->PressButtonJP1('U');
	if(keystates[SDL_SCANCODE_DOWN])	joypads->PressButtonJP1('D');
	if(keystates[SDL_SCANCODE_RIGHT])	joypads->PressButtonJP1('R');
	if(keystates[SDL_SCANCODE_LEFT])	joypads->PressButtonJP1('L');
	if(keystates[SDL_SCANCODE_Z])		joypads->PressButtonJP1('A');
	if(keystates[SDL_SCANCODE_X])		joypads->PressButtonJP1('B');
	if(keystates[SDL_SCANCODE_V])		joypads->PressButtonJP1('s');
	if(keystates[SDL_SCANCODE_C])		joypads->PressButtonJP1('S');
}

void updateTela(uint32_t *p_matrix) {

	// srand(time(NULL));

	//for (int i = 0; i < NES_WIDTH; i++) {
		//for (int j = 0; j < NES_HEIGHT; j++) {
            //pixels[j * NES_WIDTH + i] = colors[(*p_matrix)[i][j]];
        //}
    //}

    updateControllers();

    SDL_UpdateTexture(texture, NULL, p_matrix, NES_WIDTH * sizeof(uint32_t));

    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, texture, NULL, NULL);
    SDL_RenderPresent(renderer);
}
