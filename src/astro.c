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

#include <stdint.h>
#include <string.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#ifdef HAVE_CONFIG_H
#	include "config.h"
#endif

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
	
	IMG_TARGET_NORMAL_BLUE,
	IMG_TARGET_NORMAL_GREEN,
	IMG_TARGET_NORMAL_YELLOW,
	IMG_TARGET_NORMAL_RED,
	
	IMG_TARGET_MINI_BLUE,
	IMG_TARGET_MINI_GREEN,
	IMG_TARGET_MINI_YELLOW,
	IMG_TARGET_MINI_RED,
	
	IMG_TARGET_BIG_BLUE,
	IMG_TARGET_BIG_GREEN,
	IMG_TARGET_BIG_YELLOW,
	IMG_TARGET_BIG_RED,
	
	IMG_LINE1,
	
	NUM_IMAGES
};

const char *images_names[NUM_IMAGES] = {
	GAMEDATA_DIR "images/arcade.png",
	
	GAMEDATA_DIR "images/astro.png",
	
	GAMEDATA_DIR "images/frame.png",
	GAMEDATA_DIR "images/gamearea.png",
	GAMEDATA_DIR "images/gameintro.png",
	
	GAMEDATA_DIR "images/shoot.png",
	
	GAMEDATA_DIR "images/target_normal_blue.png",
	GAMEDATA_DIR "images/target_normal_green.png",
	GAMEDATA_DIR "images/target_normal_yellow.png",
	GAMEDATA_DIR "images/target_normal_red.png",
	
	GAMEDATA_DIR "images/target_mini_blue.png",
	GAMEDATA_DIR "images/target_mini_green.png",
	GAMEDATA_DIR "images/target_mini_yellow.png",
	GAMEDATA_DIR "images/target_mini_red.png",
	
	GAMEDATA_DIR "images/target_big_blue.png",
	GAMEDATA_DIR "images/target_big_green.png",
	GAMEDATA_DIR "images/target_big_yellow.png",
	GAMEDATA_DIR "images/target_big_red.png",
	
	GAMEDATA_DIR "images/line1.png"
};

/* Codigos de salida */
enum {
	GAME_NONE = 0, /* No usado */
	GAME_CONTINUE,
	GAME_QUIT
};

/* Estructuras */
typedef struct {
	int version;
	int total;
	
	int *levels;
	off_t *positions;
} InfoSet;

typedef struct {
	struct SDL_Rect rect;
	int image;
} Linea;

/* Prototipos de función */
int game_loop (void);
void setup (void);
SDL_Surface * set_video_mode(unsigned);
void leer_archivo (void);
void leer_nivel (int level, int *next_level, Linea *lineas, int *n_lineas);

/* Variables globales */
SDL_Surface * screen;
SDL_Surface * images [NUM_IMAGES];

int fd_levels;
InfoSet levels;

int nivel_actual;

int main (int argc, char *argv[]) {
	
	setup ();
	
	leer_archivo ();
	
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
	int g;
	
	int astro_dir = 0;
	SDL_Rect astro_rect;
	SDL_Surface *game_buffer;
	int shooting = FALSE;
	SDL_Rect shoot_rect;
	Linea lineas[10];
	int n_lineas;
	int sig_nivel;
	
	leer_nivel (nivel_actual, &sig_nivel, lineas, &n_lineas);
	
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
		}
		
		/* Mover la nave a su nueva posición */
		if (astro_dir < 0 && astro_rect.x > 5) {
			astro_rect.x -= 5;
		} else if (astro_dir > 0 && astro_rect.x < 315) {
			astro_rect.x += 5;
		}
		
		/* Redibujar todo */
		SDL_BlitSurface (images[IMG_GAMEAREA], NULL, game_buffer, NULL);
		
		/* Dibujar las lineas de fondo */
		for (g = 0; g < n_lineas; g++) {
			SDL_BlitSurface (images[lineas[g].image], NULL, game_buffer, (SDL_Rect *)&lineas[g]);
		}
		
		if (shooting) {
			if (shoot_rect.y < 0) {
				shooting = FALSE;
			}
			SDL_BlitSurface (images[IMG_SHOOT], NULL, game_buffer, &shoot_rect);
		}
		
		SDL_BlitSurface (images[IMG_ASTRO], NULL, game_buffer, &astro_rect);
		
		/* Redibujar el marco blanco por los objetos que se salen */
		SDL_BlitSurface (images[IMG_FRAME], NULL, game_buffer, NULL);
		
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

void leer_archivo (void) {
	/* Intentar abrir el archivo */
	uint32_t temp;
	int g;
	char buffer [6];
	
	fd_levels = open ("levels.bin", O_RDONLY);
	
	if (fd_levels < 0) {
		fprintf (stderr, "Error al abrir el archivo de niveles.\n");
		exit (EXIT_FAILURE);
	}
	
	/* Leer la cabecera, y las posiciones de los niveles */
	read (fd_levels, buffer, 5);
	buffer[5] = '\0';
	
	if (strcmp (buffer, "ASTRO") != 0) {
		fprintf (stderr, "Archivo inválido, cabecera inválida.\n");
		close (fd_levels);
		exit (EXIT_FAILURE);
	}
	
	read (fd_levels, &temp, sizeof (uint32_t));
	levels.version = temp;
	
	read (fd_levels, &temp, sizeof (uint32_t));
	levels.total = temp;
	
	read (fd_levels, &temp, sizeof (uint32_t));
	nivel_actual = temp;
	
	levels.levels = (int *) malloc (sizeof (int) * levels.total);
	levels.positions = (off_t *) malloc (sizeof (off_t) * levels.total);
	
	for (g = 0; g < levels.total; g++) {
		read (fd_levels, &temp, sizeof (uint32_t));
		levels.levels[g] = temp;
		
		read (fd_levels, &levels.positions[g], sizeof (off_t));
	}
}

void leer_nivel (int level, int *next_level, Linea *lineas, int *n_lineas) {
	/* Brincar directo al nivel y leer la información */
	int g;
	off_t pos;
	uint32_t temp;
	
	for (g = 0; g < levels.total; g++) {
		if (levels.levels[g] == level) {
			pos = levels.positions[g];
			break;
		}
	}
	
	lseek (fd_levels, pos, SEEK_SET);
	
	read (fd_levels, &temp, sizeof (uint32_t));
	
	if (level != temp) {
		/* No es el nivel correcto */
		exit (EXIT_FAILURE);
	}
	
	read (fd_levels, &temp, sizeof (uint32_t));
	*next_level = temp;
	
	read (fd_levels, &temp, sizeof (uint32_t));
	*n_lineas = (temp & 0xFF);
	
	for (g = 0; g < *n_lineas; g++) {
		read (fd_levels, &temp, sizeof (uint32_t));
		lineas[g].image = temp;
		
		read (fd_levels, &temp, sizeof (uint32_t));
		lineas[g].rect.x = temp;
		
		read (fd_levels, &temp, sizeof (uint32_t));
		lineas[g].rect.y = temp;
		
		/* Cargar su w y h */
		lineas[g].rect.w = images[lineas[g].image]->w;
		lineas[g].rect.h = images[lineas[g].image]->h;
	}
}

