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

#include "util-bresenham.h"

#define FPS (1000/24)

#define RANDOM(x) ((int) (x ## .0 * rand () / (RAND_MAX + 1.0)))

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
	
	IMG_BLOCK_NORMAL,
	IMG_BLOCK_MINI,
	IMG_BLOCK_BIG,
	
	IMG_BLOCK_ORANGE,
	
	IMG_LINE1,
	IMG_LINE2_A,
	IMG_LINE2_B,
	IMG_LINE3,
	IMG_LINE4,
	IMG_LINE5,
	IMG_LINE6_A,
	IMG_LINE6_B,
	IMG_LINE7,
	IMG_LINE8,
	IMG_LINE9,
	IMG_LINE10,
	IMG_LINE11_A,
	IMG_LINE11_B,
	IMG_LINE12,
	
	IMG_SWITCH_ORANGE,
	IMG_SWITCH_ORANGE_HIT,
	
	IMG_NEXT_LEVEL,
	IMG_RESTART,
	
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
	
	GAMEDATA_DIR "images/block_normal.png",
	GAMEDATA_DIR "images/block_mini.png",
	GAMEDATA_DIR "images/block_big.png",
	
	GAMEDATA_DIR "images/block_orange.png",
	
	GAMEDATA_DIR "images/line1.png",
	GAMEDATA_DIR "images/line2a.png",
	GAMEDATA_DIR "images/line2b.png",
	GAMEDATA_DIR "images/line3.png",
	GAMEDATA_DIR "images/line4.png",
	GAMEDATA_DIR "images/line5.png",
	GAMEDATA_DIR "images/line6a.png",
	GAMEDATA_DIR "images/line6b.png",
	GAMEDATA_DIR "images/line7.png",
	GAMEDATA_DIR "images/line8.png",
	GAMEDATA_DIR "images/line9.png",
	GAMEDATA_DIR "images/line10.png",
	GAMEDATA_DIR "images/line11a.png",
	GAMEDATA_DIR "images/line11b.png",
	GAMEDATA_DIR "images/line12.png",
	
	GAMEDATA_DIR "images/switch_orange.png",
	GAMEDATA_DIR "images/switch_orange_hit.png",
	
	GAMEDATA_DIR "images/next_level.png",
	GAMEDATA_DIR "images/restart.png"
};

/* Codigos de salida */
enum {
	GAME_NONE = 0, /* No usado */
	GAME_CONTINUE,
	GAME_QUIT
};

/* Las posibles dos pantallas abiertas */
enum {
	SCREEN_NONE = 0,
	SCREEN_NEXT_LEVEL,
	SCREEN_RESTART
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

typedef struct {
	struct SDL_Rect rect;
	int image;
	int total_vel;
	Punto puntos[200];
	int pos;
	int detenido;
} Target;

typedef struct {
	struct SDL_Rect rect;
	int image;
} Bloque;

/* Prototipos de función */
int game_loop (void);
void setup (void);
SDL_Surface * set_video_mode(unsigned);
void leer_archivo (void);
void leer_nivel (int level, int *next_level, int *shoots, int *hitsrequired, Linea *lineas, int *n_lineas, Target *targets, int *n_targets, Bloque *bloques, int *n_bloques);

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
	int g, *h;
	
	int astro_dir = 0;
	SDL_Rect astro_rect;
	SDL_Surface *game_buffer, *stretch_buffer;
	int shooting = FALSE, space_toggle = FALSE, enter_toggle = FALSE, switch_toggle = FALSE;
	int switch_timer;
	SDL_Rect shoot_rect;
	
	int n_lineas, n_targets, n_bloques;
	int tiros, hits_requeridos, contador_hits, refresh_tiros;
	int sig_nivel;
	int pantalla_abierta = 0, timer_pantalla = 0;
	
	Linea lineas[10];
	Target targets[10];
	Bloque bloques[10];
	nivel_actual = 1;
	
	leer_nivel (nivel_actual, &sig_nivel, &tiros, &hits_requeridos, lineas, &n_lineas, targets, &n_targets, bloques, &n_bloques);
	contador_hits = 0;
	refresh_tiros = TRUE;
	shoot_rect.w = images[IMG_SHOOT]->w;
	shoot_rect.h = images[IMG_SHOOT]->h;
	
	astro_rect.w = images[IMG_ASTRO]->w;
	astro_rect.h = images[IMG_ASTRO]->h;
	astro_rect.x = 160;
	astro_rect.y = 396;
	
	SDL_BlitSurface (images[IMG_ARCADE], NULL, screen, NULL);
	
	/* En esta situación tan especial, necesito varios buffers intermedios */
	game_buffer = SDL_AllocSurface (SDL_SWSURFACE, images[IMG_GAMEAREA]->w, images[IMG_GAMEAREA]->h + 52, 32, 0x00FF0000, 0x0000FF00, 0x000000FF, 0xFF000000);
	SDL_FillRect (game_buffer, NULL, 0); /* Transparencia total */
	
	/* Quitarle el alpha, para que cuando se copie a la superficie no tenga problemas */
	SDL_SetAlpha (images[IMG_GAMEAREA], 0, 0);
	
	stretch_buffer = SDL_AllocSurface (SDL_SWSURFACE, 263, 305 + 36, 32, 0x00FF0000, 0x0000FF00, 0x000000FF, 0xFF000000);
	
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
					} else if (key == SDLK_RIGHT) {
						astro_dir++;
					} else if (key == SDLK_SPACE) {
						space_toggle = TRUE;
					} else if (key == SDLK_RETURN) {
						enter_toggle = TRUE;
					}
					break;
				case SDL_KEYUP:
					key = event.key.keysym.sym;
					
					if (key == SDLK_LEFT) {
						astro_dir++;
					} else if (key == SDLK_RIGHT) {
						astro_dir--;
					} else if (key == SDLK_SPACE) {
						space_toggle = FALSE;
					} else if (key == SDLK_RETURN) {
						enter_toggle = FALSE;
					}
					
					break;
			}
		}
		
		if (space_toggle && !shooting && !pantalla_abierta) {
			shooting = TRUE;
			shoot_rect.x = astro_rect.x + 22;
			shoot_rect.y = astro_rect.y + 8;
			tiros--;
			refresh_tiros = TRUE;
		}
		
		/* Mover los objetivos */
		for (g = 0; g < n_targets; g++) {
			h = &targets[g].pos;
			targets[g].rect.x = targets[g].puntos[*h].x;
			targets[g].rect.y = targets[g].puntos[*h].y;
		}
		
		if (shooting) {
			/* Verificar colisiones contra los targets */
			for (g = 0; g < n_targets; g++) {
				if (SDL_HasIntersection (&shoot_rect, (SDL_Rect *)&targets[g])) {
					if (!targets[g].detenido && (targets[g].image == IMG_TARGET_NORMAL_GREEN || targets[g].image == IMG_TARGET_MINI_GREEN || targets[g].image == IMG_TARGET_BIG_GREEN)) {
						targets[g].detenido = TRUE;
						targets[g].image += 2; /* Cambiar a rojo */
						contador_hits++;
						/* Para los verdes, sumar 10 puntos de score */
						/* TODO: Reproducir sonido */
					}
					
					if (!targets[g].detenido && (targets[g].image == IMG_TARGET_NORMAL_BLUE || targets[g].image == IMG_TARGET_MINI_BLUE || targets[g].image == IMG_TARGET_BIG_BLUE)) {
						if (contador_hits == 0) {
							/* Golpearon la vida primero */
							/* Aumentar las vidas por 1 */
							/* Aumentar las vidas recogidas por 1 */
							/* if (lives_collected == 8) {
								Enviar estampa 61
							} */
							/* Para las vidas en el primer golpe, sumar 100 puntos al score */
							/* TODO: Reproducir sonido */
							/* TODO: Mostrar aviso de 1-up que se desvanece */
						} else {
							/* Lástima, ya no es tan importante la vida */
							/* Para las vidas, sumar 25 puntos al score */
							/* TODO: Reproducir sonido */
						}
						contador_hits++;
						targets[g].detenido = TRUE;
						targets[g].image += 3; /* Cambiar a rojo */
					}
					
					if (!targets[g].detenido && targets[g].image == IMG_SWITCH_ORANGE) {
						/* Golpearon el switch naranja, desaparecer los bloques lentamente */
						switch_toggle = TRUE;
						switch_timer = 0;
						targets[g].image++; /* Switch golpeado */
						targets[g].detenido = TRUE;
					}
					
					if (!targets[g].detenido && (targets[g].image == IMG_TARGET_NORMAL_YELLOW || targets[g].image == IMG_TARGET_MINI_YELLOW || targets[g].image == IMG_TARGET_BIG_YELLOW)) {
						targets[g].image -= 1; /* Cambiar a verde */
						contador_hits++;
						/* Para los amarillos, sumar 25 puntos de score */
						/* TODO: Reproducir sonido */
					}
					
					shooting = FALSE;
				}
			}
			
			/* Verificar colisiones contra los bloques */
			for (g = 0; g < n_bloques; g++) {
				if (SDL_HasIntersection (&shoot_rect, (SDL_Rect *)&bloques[g])) {
					shooting = FALSE;
				}
			}
			
		}
		
		if (!pantalla_abierta && contador_hits >= hits_requeridos) {
			/* Nivel terminado, hay que abrir la pantalla de siguiente nivel */
			pantalla_abierta = SCREEN_NEXT_LEVEL;
			tiros = 0;
			refresh_tiros = TRUE;
			printf ("Debug: Ganaste, siguiente nivel\n");
		} else if (!pantalla_abierta && tiros == 0 && !shooting) {
			/* Se acabaron los tiros, reiniciar el nivel */
			pantalla_abierta = SCREEN_RESTART;
			printf ("Debug: Tiros acabados, reiniciando nivel\n");
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
		
		if (switch_toggle && switch_timer < 17) {
			switch_timer++;
		}
		
		/* Dibujar los bloques */
		for (g = 0; g < n_bloques; g++) {
			if (bloques[g].image == IMG_BLOCK_ORANGE && switch_toggle) {
				if (switch_timer < 17) {
					rect.y = switch_timer * 4;
					bloques[g].rect.y += 4;
					rect.x = 0;
					rect.h = bloques[g].rect.h -= 4;
					rect.w = bloques[g].rect.w;
					SDL_BlitSurface (images[bloques[g].image], &rect, game_buffer, (SDL_Rect *)&bloques[g]);
				}
			} else {
				SDL_BlitSurface (images[bloques[g].image], NULL, game_buffer, (SDL_Rect *)&bloques[g]);
			}
		}
		
		/* Dibujar los targets */
		for (g = 0; g < n_targets; g++) {
			SDL_BlitSurface (images[targets[g].image], NULL, game_buffer, (SDL_Rect *)&targets[g]);
			if (!targets[g].detenido) {
				targets[g].pos++;
				if (targets[g].pos >= targets[g].total_vel) targets[g].pos = 0;
			}
		}
		
		if (shooting) {
			if (shoot_rect.y >= 0) {
				SDL_BlitSurface (images[IMG_SHOOT], NULL, game_buffer, &shoot_rect);
			}
			
			shoot_rect.y -= 24;
			
			if (shoot_rect.y < -56) {
				shooting = FALSE;
			}
		}
		
		/* Redibujar la cantidad de tiros */
		if (refresh_tiros) {
			/* Borrar el area debajo de la zona de juego */
			rect.x = GAME_AREA_X;
			rect.y = GAME_AREA_Y + 305;
			rect.w = 144;
			rect.h = 36;
			SDL_BlitSurface (images[IMG_ARCADE], &rect, screen, &rect);
			
			printf ("Debug: Refrescando tiros\n");
			/* Borrar la parte de abajo de los tiros */
			rect.x = 0;
			rect.y = 436;
			rect.w = 205;
			rect.h = 52;
			SDL_FillRect (game_buffer, &rect, 0); /* Transparencia total */
			
			SDL_SetAlpha (images[IMG_SHOOT], 0, 0);
			/* Y de los tiros disponibles: 456 */
			/* X 3, espaciado de: 4 pixeles */
			if (tiros == 0) {
				/* Dibujar la palabra "0 shoots" */
			} else if (tiros > 0) {
				printf ("Redibujando para %i tiros\n", tiros);
				rect.x = 3;
				rect.y = 456;
				rect.w = images[IMG_SHOOT]->w;
				rect.h = images[IMG_SHOOT]->h;
				for (g = 0; g < tiros; g++) {
					SDL_BlitSurface (images[IMG_SHOOT], NULL, game_buffer, &rect);
					rect.x += (4 + rect.w);
				}
			}
			SDL_SetAlpha (images[IMG_SHOOT], SDL_SRCALPHA, 0);
			refresh_tiros = FALSE;
		}
		
		/* Si es que hay, redibujar la pantalla de reiniciar o de siguiente nivel */
		if (pantalla_abierta == SCREEN_NEXT_LEVEL) {
			rect.x = 65;
			rect.y = 100;
			rect.h = images[IMG_NEXT_LEVEL]->h;
			rect.w = images[IMG_NEXT_LEVEL]->w;
			
			SDL_BlitSurface (images[IMG_NEXT_LEVEL], NULL, game_buffer, &rect);
			
			/* TODO: Dibujar la cantidad de puntos */
			timer_pantalla++;
		} else if (pantalla_abierta == SCREEN_RESTART) {
			rect.x = 65;
			rect.y = 100;
			rect.h = images[IMG_RESTART]->h;
			rect.w = images[IMG_RESTART]->w;
			
			SDL_BlitSurface (images[IMG_RESTART], NULL, game_buffer, &rect);
			timer_pantalla++;
		} else if (nivel_actual & (1 << 16)) {
			timer_pantalla++;
		}
		
		SDL_BlitSurface (images[IMG_ASTRO], NULL, game_buffer, &astro_rect);
		
		/* Redibujar el marco blanco por los objetos que se salen */
		SDL_BlitSurface (images[IMG_FRAME], NULL, game_buffer, NULL);
		
		/* Escalar el area de juego antes de copiarla a la pantalla */
		SDL_StretchSurfaceBlit (game_buffer, NULL, stretch_buffer, NULL);
		
		rect.x = GAME_AREA_X;
		rect.y = GAME_AREA_Y;
		rect.w = stretch_buffer->w;
		rect.h = stretch_buffer->h;
		
		SDL_BlitSurface (stretch_buffer, NULL, screen, &rect);
		
		SDL_Flip (screen);
		
		if (pantalla_abierta && timer_pantalla > 56) {
			if (pantalla_abierta == SCREEN_RESTART) {
				/* Ocultar y reiniciar el nivel */
				leer_nivel (nivel_actual, &sig_nivel, &tiros, &hits_requeridos, lineas, &n_lineas, targets, &n_targets, bloques, &n_bloques);
				switch_toggle = FALSE;
				contador_hits = 0;
				refresh_tiros = TRUE;
				/* TODO: nivel_reiniciado = TRUE */
				pantalla_abierta = SCREEN_NONE;
				/* TODO: reiniciar el score */
			} else if (pantalla_abierta == SCREEN_NEXT_LEVEL) {
				/* Pasar al siguiente nivel */
				if (sig_nivel != -1) {
					nivel_actual = sig_nivel;
					leer_nivel (nivel_actual, &sig_nivel, &tiros, &hits_requeridos, lineas, &n_lineas, targets, &n_targets, bloques, &n_bloques);
					switch_toggle = FALSE;
					contador_hits = 0;
					refresh_tiros = TRUE;
				} else {
					done = GAME_CONTINUE;
				}
				pantalla_abierta = SCREEN_NONE;
			}
			timer_pantalla = 0;
		}
		
		if (nivel_actual & (1 << 16) && timer_pantalla > 20 && enter_toggle) {
			/* Cargar el siguiente nivel, porque presionó enter */
			if (sig_nivel != -1) {
				nivel_actual = sig_nivel;
				leer_nivel (nivel_actual, &sig_nivel, &tiros, &hits_requeridos, lineas, &n_lineas, targets, &n_targets, bloques, &n_bloques);
				
				switch_toggle = FALSE;
				timer_pantalla = 0;
				contador_hits = 0;
				refresh_tiros = TRUE;
			} else {
				done = GAME_CONTINUE;
			}
		}
		
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
	
	if (temp != 1) {
		/* Archivo incompleto */
		fprintf (stderr, "Versión del archivo de niveles incorrecto.\n");
		close (fd_levels);
		exit (EXIT_FAILURE);
	}
	
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

void leer_nivel (int level, int *next_level, int *shoots, int *hitsrequired, Linea *lineas, int *n_lineas, Target *targets, int *n_targets, Bloque *bloques, int *n_bloques) {
	/* Brincar directo al nivel y leer la información */
	int g, h, i, k;
	off_t pos;
	uint32_t temp;
	Punto puntos[20];
	int tomar[20];
	
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
	*shoots = temp;
	
	read (fd_levels, &temp, sizeof (uint32_t));
	*hitsrequired = temp;
	
	read (fd_levels, &temp, sizeof (uint32_t));
	*n_lineas = (temp & 0xFF);
	*n_bloques = (temp & 0xFF00) >> 8;
	*n_targets = (temp & 0xFF0000) >> 16;
	
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
	
	for (g = 0; g < *n_bloques; g++) {
		read (fd_levels, &temp, sizeof (uint32_t));
		bloques[g].image = temp;
		
		read (fd_levels, &temp, sizeof (uint32_t));
		bloques[g].rect.x = temp;
		
		read (fd_levels, &temp, sizeof (uint32_t));
		bloques[g].rect.y = temp;
		
		/* Cargar su w y h */
		bloques[g].rect.w = images[bloques[g].image]->w;
		bloques[g].rect.h = images[bloques[g].image]->h;
	}
	
	for (g = 0; g < *n_targets; g++) {
		read (fd_levels, &temp, sizeof (uint32_t));
		targets[g].image = temp;
		targets[g].rect.w = images[temp]->w;
		targets[g].rect.h = images[temp]->h;
		
		read (fd_levels, &temp, sizeof (uint32_t));
		k = temp;
		
		for (h = 0; h < k; h++) {
			read (fd_levels, &temp, sizeof (uint32_t));
			puntos[h].x = temp;
			
			read (fd_levels, &temp, sizeof (uint32_t));
			puntos[h].y = temp;
			
			read (fd_levels, &temp, sizeof (uint32_t));
			tomar[h] = temp;
		}
		
		/* Ahora, utilizar la linea de bresenham para estimar los puntos intermedios */
		i = 0;
		for (h = 0; h < k; h++) {
			line (puntos[h].x, puntos[h].y, puntos[(h + 1) % k].x, puntos[(h + 1) % k].y, &targets[g].puntos[i], tomar[h]);
			i += tomar[h];
		}
		
		targets[g].total_vel = i;
		targets[g].pos = 0;
		targets[g].detenido = FALSE;
	}
}


