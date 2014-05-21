/*
 * astro.c
 * This file is part of Astro Barrier
 *
 * Copyright (C) 2013 - Félix Arreola Rodríguez
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

/* ----------------
 * LEGAL NOTICE
 * ----------------
 *
 * This game is NOT related to Club Penguin in any way. Also,
 * this game is not intended to infringe copyrights, the graphics and
 * sounds used are Copyright of Disney.
 *
 * The new SDL code is written by Gatuno, and is released under
 * the term of the GNU General Public License.
 */

#include <stdlib.h>
#include <stdio.h>

#include <SDL.h>
#include <SDL_image.h>

#include "config.h"

#define FPS (1000/24)

#define SWAP(a, b, t) ((t) = (a), (a) = (b), (b) = (t))
#define RANDOM(x) ((int) (x ## .0 * rand () / (RAND_MAX + 1.0)))

#ifndef FALSE
#define FALSE 0
#endif

#ifndef TRUE
#define TRUE !FALSE
#endif

#define GAME_AREA_X 250
#define GAME_AREA_Y 79

/* Enumerar las imágenes */
enum {
	IMG_ARCADE,
	
	IMG_ASTRO,
	
	IMG_FRAME,
	IMG_GAMEAREA,
	IMG_GAMEINTRO,
	
	IMG_SHOOT,
	
	NUM_IMAGES
};

const char *images_names[NUM_IMAGES] = {
	GAMEDATA_DIR "images/arcade.png",
	
	GAMEDATA_DIR "images/astro.png",
	
	GAMEDATA_DIR "images/frame.png",
	GAMEDATA_DIR "images/gamearea.png",
	GAMEDATA_DIR "images/gameintro.png",
	
	GAMEDATA_DIR "images/shoot.png"
};

/* Codigos de salida */
enum {
	GAME_NONE = 0, /* No usado */
	GAME_CONTINUE,
	GAME_QUIT
};

/* Estructuras */

/* Prototipos de función */
int game_loop (void);
void setup (void);
SDL_Surface * set_video_mode(unsigned);

/* Variables globales */
SDL_Surface * screen;
SDL_Surface * images [NUM_IMAGES];

int main (int argc, char *argv[]) {
	
	setup ();
	
	do {
		if (game_loop () == GAME_QUIT) break;
	} while (1 == 0);
	
	SDL_Quit ();
	return EXIT_SUCCESS;
}

int game_loop (void) {
	int done = 0;
	SDL_Event event;
	SDLKey key;
	Uint32 last_time, now_time;
	SDL_Rect rect;
	
	int astro_dir = 0;
	SDL_Rect astro_rect;
	SDL_Surface *game_buffer;
	int shooting = FALSE;
	SDL_Rect shoot_rect;
	
	shoot_rect.w = images[IMG_SHOOT]->w;
	shoot_rect.h = images[IMG_SHOOT]->h;
	
	astro_rect.w = images[IMG_ASTRO]->w;
	astro_rect.h = images[IMG_ASTRO]->h;
	astro_rect.x = 160;
	astro_rect.y = 396;
	
	SDL_BlitSurface (images[IMG_ARCADE], NULL, screen, NULL);
	
	/* En esta situación tan especial, necesito varios buffers intermedios */
	game_buffer = SDL_AllocSurface (SDL_SWSURFACE, images[IMG_GAMEAREA]->w, images[IMG_GAMEAREA]->h, 32, 0x00FF0000, 0x0000FF00, 0x000000FF, 0xFF000000);
	
	/* Quitarle el alpha, para que cuando se copie a la superficie no tenga problemas */
	SDL_SetAlpha (images[IMG_GAMEAREA], 0, 0);
	
	SDL_EventState (SDL_MOUSEMOTION, SDL_IGNORE);
	
	do {
		last_time = SDL_GetTicks ();
		
		while (SDL_PollEvent(&event) > 0) {
			switch (event.type) {
				case SDL_QUIT:
					/* Vamos a cerrar la aplicación */
					done = GAME_QUIT;
					break;
				case SDL_KEYDOWN:
					key = event.key.keysym.sym;
					
					if (key == SDLK_LEFT) {
						astro_dir--;
					}
					
					if (key == SDLK_RIGHT) {
						astro_dir++;
					}
					
					if (key == SDLK_SPACE && !shooting) {
						shooting = TRUE;
						shoot_rect.x = astro_rect.x + 22;
						shoot_rect.y = astro_rect.y + 4;
					}
					break;
				case SDL_KEYUP:
					key = event.key.keysym.sym;
					
					if (key == SDLK_LEFT) {
						astro_dir++;
					}
					
					if (key == SDLK_RIGHT) {
						astro_dir--;
					}
					break;
			}
		}
		
		if (shooting) {
			shoot_rect.y -= 24;
			if (shoot_rect.y < 0) {
				shooting = FALSE;
			}
		}
		
		SDL_BlitSurface (images[IMG_GAMEAREA], NULL, game_buffer, NULL);
		
		/* Mover la nave a su nueva posición */
		if (astro_dir < 0 && astro_rect.x > 5) {
			astro_rect.x -= 5;
		} else if (astro_dir > 0 && astro_rect.x < 315) {
			astro_rect.x += 5;
		}
		
		if (shooting) {
			SDL_BlitSurface (images[IMG_SHOOT], NULL, game_buffer, &shoot_rect);
		}
		SDL_BlitSurface (images[IMG_ASTRO], NULL, game_buffer, &astro_rect);
		
		/* TODO: Escalar el area de juego antes de copiarla a la pantalla */
		rect.x = GAME_AREA_X;
		//rect.y = GAME_AREA_Y; original
		rect.y = 20;
		rect.w = game_buffer->w;
		rect.h = game_buffer->h;
		
		SDL_BlitSurface (game_buffer, NULL, screen, &rect);
		
		SDL_Flip (screen);
		
		now_time = SDL_GetTicks ();
		if (now_time < last_time + FPS) SDL_Delay(last_time + FPS - now_time);
		
	} while (!done);
	
	return done;
}

/* Set video mode: */
/* Mattias Engdegard <f91-men@nada.kth.se> */
SDL_Surface * set_video_mode (unsigned flags) {
	/* Prefer 16bpp, but also prefer native modes to emulated 16bpp. */

	int depth;

	depth = SDL_VideoModeOK (760, 480, 16, flags);
	return depth ? SDL_SetVideoMode (760, 480, depth, flags) : NULL;
}

void setup (void) {
	SDL_Surface * image;
	int g;
	
	/* Inicializar el Video SDL */
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		fprintf (stderr,
			"Error: Can't initialize the video subsystem\n"
			"The error returned by SDL is:\n"
			"%s\n", SDL_GetError());
		exit (1);
	}
	
	/* Crear la pantalla de dibujado */
	screen = set_video_mode (0);
	
	if (screen == NULL) {
		fprintf (stderr,
			"Error: Can't setup 760x480 video mode.\n"
			"The error returned by SDL is:\n"
			"%s\n", SDL_GetError());
		exit (1);
	}
	
	for (g = 0; g < NUM_IMAGES; g++) {
		image = IMG_Load (images_names[g]);
		
		if (image == NULL) {
			fprintf (stderr,
				"Failed to load data file:\n"
				"%s\n"
				"The error returned by SDL is:\n"
				"%s\n", images_names[g], SDL_GetError());
			SDL_Quit ();
			exit (1);
		}
		
		images[g] = image;
		/* TODO: Mostrar la carga de porcentaje */
	}
	
	srand (SDL_GetTicks ());
}

