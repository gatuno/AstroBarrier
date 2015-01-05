/*
 * astro.c
 * This file is part of Astro Barrier
 *
 * Copyright (C) 2013, 2014 - Félix Arreola Rodríguez
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
#include <SDL_mixer.h>
#include <SDL_ttf.h>

#include <stdint.h>
#include <string.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#ifdef HAVE_CONFIG_H
#	include "config.h"
#endif

#include "util-bresenham.h"

#include "astro-types.h"
#include "astro-lua.h"

#include "cp-button.h"
#include "cpstamp.h"

#define FPS (1000/24)
#define MAX_RECTS 16

#define RANDOM(x) ((int) (x ## .0 * rand () / (RAND_MAX + 1.0)))

#define GAME_AREA_X 250
#define GAME_AREA_Y 79

#ifdef __MINGW32__
#	define GAMEDATA_DIR "./"
#endif

const char *images_names[NUM_IMAGES] = {
	GAMEDATA_DIR "images/arcade.png",
	
	GAMEDATA_DIR "images/boton-close-up.png",
	GAMEDATA_DIR "images/boton-close-over.png",
	GAMEDATA_DIR "images/boton-close-down.png",
	
	GAMEDATA_DIR "images/button-start.png",
	
	GAMEDATA_DIR "images/astro.png",
	GAMEDATA_DIR "images/astro_logo.png",
	GAMEDATA_DIR "images/astro_destroyed.png",
	GAMEDATA_DIR "images/astro_blue.png",
	
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
	
	GAMEDATA_DIR "images/target_mini_lightgreen.png",
	GAMEDATA_DIR "images/target_mini_orange.png",
	
	GAMEDATA_DIR "images/target_expand1.png",
	GAMEDATA_DIR "images/target_expand2.png",
	GAMEDATA_DIR "images/target_expand3.png",
	GAMEDATA_DIR "images/target_expand4.png",
	GAMEDATA_DIR "images/target_expand5.png",
	GAMEDATA_DIR "images/target_expand_red.png",
	
	GAMEDATA_DIR "images/target_500_green.png",
	GAMEDATA_DIR "images/target_500_red.png",
	
	GAMEDATA_DIR "images/block_normal.png",
	GAMEDATA_DIR "images/block_mini.png",
	GAMEDATA_DIR "images/block_big.png",
	GAMEDATA_DIR "images/block_tall.png",
	
	GAMEDATA_DIR "images/block_orange.png",
	GAMEDATA_DIR "images/block_orange_mini.png",
	
	GAMEDATA_DIR "images/line1.png",
	GAMEDATA_DIR "images/line2a.png",
	GAMEDATA_DIR "images/line2b.png",
	GAMEDATA_DIR "images/line3.png",
	GAMEDATA_DIR "images/line4a.png",
	GAMEDATA_DIR "images/line4b.png",
	GAMEDATA_DIR "images/line5.png",
	GAMEDATA_DIR "images/line6a.png",
	GAMEDATA_DIR "images/line6b.png",
	GAMEDATA_DIR "images/line6c.png",
	GAMEDATA_DIR "images/line7.png",
	GAMEDATA_DIR "images/line8.png",
	GAMEDATA_DIR "images/line9.png",
	GAMEDATA_DIR "images/line10.png",
	GAMEDATA_DIR "images/line11a.png",
	GAMEDATA_DIR "images/line11b.png",
	GAMEDATA_DIR "images/line12a.png",
	GAMEDATA_DIR "images/line12b.png",
	GAMEDATA_DIR "images/line13.png",
	GAMEDATA_DIR "images/line14.png",
	GAMEDATA_DIR "images/line15.png",
	GAMEDATA_DIR "images/line16a.png",
	GAMEDATA_DIR "images/line16b.png",
	GAMEDATA_DIR "images/line17.png",
	GAMEDATA_DIR "images/line18a.png",
	GAMEDATA_DIR "images/line18b.png",
	GAMEDATA_DIR "images/line18c.png",
	GAMEDATA_DIR "images/line18d.png",
	GAMEDATA_DIR "images/line19a.png",
	GAMEDATA_DIR "images/line19b.png",
	GAMEDATA_DIR "images/line19c.png",
	GAMEDATA_DIR "images/line19d.png",
	GAMEDATA_DIR "images/line20.png",
	GAMEDATA_DIR "images/line21.png",
	GAMEDATA_DIR "images/line22a.png",
	GAMEDATA_DIR "images/line22b.png",
	GAMEDATA_DIR "images/line23a.png",
	GAMEDATA_DIR "images/line23b.png",
	GAMEDATA_DIR "images/line24.png",
	GAMEDATA_DIR "images/line25a.png",
	GAMEDATA_DIR "images/line25b.png",
	GAMEDATA_DIR "images/line26a.png",
	GAMEDATA_DIR "images/line26b.png",
	GAMEDATA_DIR "images/line27.png",
	GAMEDATA_DIR "images/line28.png",
	GAMEDATA_DIR "images/line29.png",
	GAMEDATA_DIR "images/line30.png",
	GAMEDATA_DIR "images/line31.png",
	
	GAMEDATA_DIR "images/turret1.png",
	GAMEDATA_DIR "images/turret2.png",
	GAMEDATA_DIR "images/turret3.png",
	GAMEDATA_DIR "images/turret4.png",
	GAMEDATA_DIR "images/turret_destroyed.png",
	
	GAMEDATA_DIR "images/turret_shoot1.png",
	GAMEDATA_DIR "images/turret_shoot2.png",
	GAMEDATA_DIR "images/turret_shoot3.png",
	GAMEDATA_DIR "images/turret_shoot4.png",
	
	GAMEDATA_DIR "images/switch_orange.png",
	GAMEDATA_DIR "images/switch_orange_hit.png",
	
	GAMEDATA_DIR "images/next_level.png",
	GAMEDATA_DIR "images/restart.png"
};

enum {
	SND_START_GAME,
	SND_GAME_OVER,
	SND_SHOOT,
	SND_HIT,
	SND_ONE_UP,
	SND_YELLOW_HIT,
	SND_ORANGE_SWITCH,
	SND_SLIDE,
	SND_TURRET_SPLODE,
	SND_LEVEL_DONE,
	SND_GUN_SPLODE,
	SND_WIN,
	
	NUM_SOUNDS
};

const char *sound_names[NUM_SOUNDS] = {
	GAMEDATA_DIR "sounds/start_game.wav",
	GAMEDATA_DIR "sounds/game_over.wav",
	GAMEDATA_DIR "sounds/shoot.wav",
	GAMEDATA_DIR "sounds/barrier_hit.wav",
	GAMEDATA_DIR "sounds/oneup.wav",
	GAMEDATA_DIR "sounds/yellow_hit.wav",
	GAMEDATA_DIR "sounds/orange_switch.wav",
	GAMEDATA_DIR "sounds/slide.wav",
	GAMEDATA_DIR "sounds/turret_splode.wav",
	GAMEDATA_DIR "sounds/level_done.wav",
	GAMEDATA_DIR "sounds/gun_splode.wav",
	GAMEDATA_DIR "sounds/win.wav"
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

enum {
	BUTTON_NONE = 0,
	BUTTON_START,
	BUTTON_CLOSE,
	
	NUM_BUTTONS
};

#define OFFICIAL_LEVEL_PACK GAMEDATA_DIR "levels/official"

/* Prototipos de función */
int game_intro (void);
int game_explain (void);
int game_loop (void);
void redibujar_nivel (int);
void setup (void);
SDL_Surface * set_video_mode (unsigned);
int map_button_in_opening (int, int);
int map_button_in_game (int, int);

/* Variables globales */
SDL_Surface * screen, *game_buffer, *stretch_buffer;
SDL_Surface * images [NUM_IMAGES];

SDL_Color amarillo = {0xff, 0xff, 0x00};
SDL_Color blanco = {0xff, 0xff, 0xff};
SDL_Color azul = {0x99, 0xff, 0xff};

int use_sound;
Mix_Chunk * sounds[NUM_SOUNDS];

SDL_Rect rects[MAX_RECTS];
int num_rects = 0;

TTF_Font *ttf24_burbank_small;
TTF_Font *ttf20_burbank_small;

int nivel_actual;

Categoria *c;

int main (int argc, char *argv[]) {
	setup ();
	iniciarCPStamp ();
	
	nivel_actual = 0;
	
	/* Registrar botones */
	cp_registrar_botones (NUM_BUTTONS);
	cp_registrar_boton (BUTTON_CLOSE, IMG_BUTTON_CLOSE_UP);
	cp_button_start ();
	
	/* Registrar las estampas */
	c = abrir_cat (STAMP_TYPE_GAME, "Astro Barrier", "astro-barrier");
	
	if (c == NULL) {
		printf ("Falló al inicializar las estampas\n");
	}
	
	if (!esta_registrada (c, 51)) {
		registrar_estampa (c, 51, "Astro 5", "Finish 5 levels", GAMEDATA_DIR "images/stamps/51.png", STAMP_TYPE_GAME, STAMP_EASY);
	}
	
	if (!esta_registrada (c, 52)) {
		registrar_estampa (c, 52, "Astro 5 Max", "Finish 5 levels without missing a shoot", GAMEDATA_DIR "images/stamps/52.png", STAMP_TYPE_GAME, STAMP_EASY);
	}
	if (!esta_registrada (c, 53)) {
		registrar_estampa (c, 53, "Astro 40", "Complete levels 1-40", GAMEDATA_DIR "images/stamps/53.png", STAMP_TYPE_GAME, STAMP_NORMAL);
	}
	if (!esta_registrada (c, 54)) {
		registrar_estampa (c, 54, "Ship Blast", "Use a Blaster to shoot a ship", GAMEDATA_DIR "images/stamps/54.png", STAMP_TYPE_GAME, STAMP_NORMAL);
	}
	if (!esta_registrada (c, 55)) {
		registrar_estampa (c, 55, "1-up Blast", "Use a Blaster to shoot a 1-up", GAMEDATA_DIR "images/stamps/55.png", STAMP_TYPE_GAME, STAMP_NORMAL);
	}
	if (!esta_registrada (c, 56)) {
		registrar_estampa (c, 56, "Astro Secret", "Complete all Secret Levels", GAMEDATA_DIR "images/stamps/56.png", STAMP_TYPE_GAME, STAMP_NORMAL);
	}
	if (!esta_registrada (c, 57)) {
		registrar_estampa (c, 57, "Astro 10 Max", "Complete levels 1-10 without missing a shoot", GAMEDATA_DIR "images/stamps/57.png", STAMP_TYPE_GAME, STAMP_NORMAL);
	}
	if (!esta_registrada (c, 58)) {
		registrar_estampa (c, 58, "Astro 20 Max", "Complete levels 1-20 without missing a shoot", GAMEDATA_DIR "images/stamps/58.png", STAMP_TYPE_GAME, STAMP_NORMAL);
	}
	if (!esta_registrada (c, 59)) {
		registrar_estampa (c, 59, "Astro Expert", "Complete the Expert Levels", GAMEDATA_DIR "images/stamps/59.png", STAMP_TYPE_GAME, STAMP_HARD);
	}
	if (!esta_registrada (c, 60)) {
		registrar_estampa (c, 60, "Astro 30 Max", "Complete levels 1-30 without missing a shoot", GAMEDATA_DIR "images/stamps/60.png", STAMP_TYPE_GAME, STAMP_HARD);
	}
	if (!esta_registrada (c, 61)) {
		registrar_estampa (c, 61, "Astro 1-up", "Collect 8 1-ups", GAMEDATA_DIR "images/stamps/61.png", STAMP_TYPE_GAME, STAMP_HARD);
	}
	if (!esta_registrada (c, 62)) {
		registrar_estampa (c, 62, "Astro Master", "Complete 25 levels + Secret and Expert", GAMEDATA_DIR "images/stamps/62.png", STAMP_TYPE_GAME, STAMP_EXTREME);
	}
	SDL_Event event;
	do {
		if (game_intro () == GAME_QUIT) break;
		if (nivel_actual == 0 && game_explain () == GAME_QUIT) break;
		if (game_loop () == GAME_QUIT) break;
		
		do {
			SDL_WaitEvent(&event);
		} while (event.type != SDL_QUIT);
	} while (1 == 0);
	
	cerrar_registro (c);
	
	SDL_Quit ();
	return EXIT_SUCCESS;
}

int game_intro (void) {
	int done = 0;
	SDL_Event event;
	Uint32 last_time, now_time;
	SDL_Rect rect;
	int map;
	SDL_Surface *texto;
	SDLKey key;
	
	SDL_BlitSurface (images[IMG_ARCADE], NULL, screen, NULL);
	
	texto = TTF_RenderUTF8_Blended (ttf24_burbank_small, "Astro Barrier", blanco);
	
	rect.x = GAME_AREA_X + ((stretch_buffer->w - texto->w) / 2) + 1;
	rect.w = texto->w;
	rect.h = texto->h;
	rect.y = GAME_AREA_Y - 3 - rect.h;
	
	SDL_BlitSurface (texto, NULL, screen, &rect);
	SDL_FreeSurface (texto);
	
	SDL_FillRect (game_buffer, NULL, 0); /* Transparencia total */
	
	SDL_BlitSurface (images[IMG_GAMEINTRO], NULL, game_buffer, NULL);
	
	rect.w = images[IMG_BUTTON_START]->w;
	rect.x = (game_buffer->w - rect.w) / 2;
	rect.y = 360;
	rect.h = images[IMG_BUTTON_START]->h;
	
	SDL_BlitSurface (images[IMG_BUTTON_START], NULL, game_buffer, &rect);
	
	texto = TTF_RenderUTF8_Blended (ttf24_burbank_small, "START", amarillo);
	
	rect.x = rect.x + (images[IMG_BUTTON_START]->w - texto->w) / 2;
	rect.y = 360 + (images[IMG_BUTTON_START]->h - texto->h) / 2 + 2;
	rect.w = texto->w;
	rect.h = texto->h;
	
	SDL_BlitSurface (texto, NULL, game_buffer, &rect);
	SDL_FreeSurface (texto);
	
	rect.x = 37;
	rect.y = 16;
	rect.w = images[IMG_ASTRO_LOGO]->w;
	rect.h = images[IMG_ASTRO_LOGO]->h;
	
	SDL_BlitSurface (images[IMG_ASTRO_LOGO], NULL, game_buffer, &rect);
	
	/* Escalar el area de juego antes de copiarla a la pantalla */
	SDL_StretchSurfaceBlit (game_buffer, NULL, stretch_buffer, NULL);
	
	rect.x = GAME_AREA_X;
	rect.y = GAME_AREA_Y;
	rect.w = stretch_buffer->w;
	rect.h = stretch_buffer->h;
	
	SDL_BlitSurface (stretch_buffer, NULL, screen, &rect);
	
	SDL_Flip (screen);
	
	do {
		last_time = SDL_GetTicks ();
		
		while (SDL_PollEvent(&event) > 0) {
			switch (event.type) {
				case SDL_QUIT:
					/* Vamos a cerrar la aplicación */
					done = GAME_QUIT;
					break;
				case SDL_MOUSEMOTION:
					map = map_button_in_opening (event.motion.x, event.motion.y);
					cp_button_motion (map);
					break;
				case SDL_MOUSEBUTTONDOWN:
					map = map_button_in_opening (event.button.x, event.button.y);
					cp_button_down (map);
					break;
				case SDL_MOUSEBUTTONUP:
					map = map_button_in_opening (event.button.x, event.button.y);
					map = cp_button_up (map);
					
					switch (map) {
						case BUTTON_START:
							done = GAME_CONTINUE;
							break;
						case BUTTON_CLOSE:
							done = GAME_QUIT;
							break;
					}
					break;
				case SDL_KEYDOWN:
					key = event.key.keysym.sym;
					
					if (key == SDLK_RETURN) {
						nivel_actual = 1;
						done = GAME_CONTINUE;
					} else if (key == SDLK_1) {
						nivel_actual = 10;
						done = GAME_CONTINUE;
					} else if (key == SDLK_2) {
						nivel_actual = 20;
						done = GAME_CONTINUE;
					} else if (key == SDLK_3) {
						nivel_actual = 30;
						done = GAME_CONTINUE;
					}
					break;
			}
		}
		
		//if (cp_button_refresh[BUTTON_CLOSE]) {
			rect.x = 707; rect.y = 16;
			rect.w = images[IMG_BUTTON_CLOSE_UP]->w; rect.h = images[IMG_BUTTON_CLOSE_UP]->h;
			
			SDL_BlitSurface (images[IMG_ARCADE], &rect, screen, &rect);
			
			SDL_BlitSurface (images[cp_button_frames[BUTTON_CLOSE]], NULL, screen, &rect);
			//rects[num_rects++] = rect;
			SDL_UpdateRects (screen, 1, &rect);
			//cp_button_refresh[BUTTON_CLOSE] = 0;
		//}
		
		now_time = SDL_GetTicks ();
		if (now_time < last_time + FPS) SDL_Delay(last_time + FPS - now_time);
	} while (!done);
	
	return done;
}

int game_explain (void) {
	int done = 0;
	SDL_Event event;
	Uint32 last_time, now_time;
	SDL_Rect rect;
	int map;
	SDL_Surface *texto;
	SDLKey key;
	Linea l;
	Target t;
	
	nivel_actual = 1;
	
	/* Preparar el target y linea de prueba */
	l.image = IMG_LINE1;
	l.rect.x = 4;
	l.rect.y = 108;
	l.rect.w = images[IMG_LINE1]->w;
	l.rect.h = images[IMG_LINE1]->h;
	
	t.image = IMG_TARGET_NORMAL_GREEN;
	t.rect.w = images[IMG_TARGET_NORMAL_GREEN]->w;
	t.rect.h = images[IMG_TARGET_NORMAL_GREEN]->h;
	
	t.puntos[0].x = t.rect.x = 16;
	t.puntos[0].y = t.rect.y = 94;
	line (16, 92, 312, 92, &t.puntos[0], 40);
	line (312, 92, 16, 92, &t.puntos[40], 40);
	t.total_vel = 80;
	t.pos = 0;
	
	SDL_BlitSurface (images[IMG_ARCADE], NULL, screen, NULL);
	
	texto = TTF_RenderUTF8_Blended (ttf24_burbank_small, "Astro Barrier", blanco);
	
	rect.x = GAME_AREA_X + ((stretch_buffer->w - texto->w) / 2) + 1;
	rect.w = texto->w;
	rect.h = texto->h;
	rect.y = GAME_AREA_Y - 3 - rect.h;
	
	SDL_BlitSurface (texto, NULL, screen, &rect);
	SDL_FreeSurface (texto);
	
	SDL_FillRect (game_buffer, NULL, 0); /* Transparencia total */
	
	SDL_BlitSurface (images[IMG_GAMEINTRO], NULL, game_buffer, NULL);
	
	SDL_Flip (screen);
	
	do {
		last_time = SDL_GetTicks ();
		
		num_rects = 0;
		
		while (SDL_PollEvent(&event) > 0) {
			switch (event.type) {
				case SDL_QUIT:
					/* Vamos a cerrar la aplicación */
					done = GAME_QUIT;
					break;
				case SDL_MOUSEMOTION:
					map = map_button_in_opening (event.motion.x, event.motion.y);
					cp_button_motion (map);
					break;
				case SDL_MOUSEBUTTONDOWN:
					map = map_button_in_opening (event.button.x, event.button.y);
					cp_button_down (map);
					break;
				case SDL_MOUSEBUTTONUP:
					map = map_button_in_opening (event.button.x, event.button.y);
					map = cp_button_up (map);
					
					if (map == BUTTON_CLOSE) done = GAME_QUIT;
					break;
				case SDL_KEYDOWN:
					key = event.key.keysym.sym;
					
					if (key == SDLK_RETURN || key == SDLK_SPACE) {
						done = GAME_CONTINUE;
					}
					break;
			}
		}
		
		SDL_BlitSurface (images[IMG_GAMEINTRO], NULL, game_buffer, NULL);
		
		/* Dibujar la linea de fondo */
		SDL_BlitSurface (images[l.image], NULL, game_buffer, (SDL_Rect *)&l);
		
		/* Dibujar el target */
		SDL_BlitSurface (images[t.image], NULL, game_buffer, (SDL_Rect *)&t);
		t.pos++;
		if (t.pos >= t.total_vel) t.pos = 0;
		t.rect.x = t.puntos[t.pos].x;
		t.rect.y = t.puntos[t.pos].y;
		
		/* Dibujar el boton de cierre */
		if (cp_button_refresh[BUTTON_CLOSE]) {
			rect.x = 707; rect.y = 16;
			rect.w = images[IMG_BUTTON_CLOSE_UP]->w; rect.h = images[IMG_BUTTON_CLOSE_UP]->h;
			
			SDL_BlitSurface (images[IMG_ARCADE], &rect, screen, &rect);
			
			SDL_BlitSurface (images[cp_button_frames[BUTTON_CLOSE]], NULL, screen, &rect);
			rects[num_rects++] = rect;
			
			cp_button_refresh[BUTTON_CLOSE] = 0;
		}
		
		/* Redibujar el marco blanco por los objetos que se salen */
		SDL_BlitSurface (images[IMG_FRAME], NULL, game_buffer, NULL);
		
		/* Escalar el area de juego antes de copiarla a la pantalla */
		SDL_StretchSurfaceBlit (game_buffer, NULL, stretch_buffer, NULL);
		
		rect.x = GAME_AREA_X;
		rect.y = GAME_AREA_Y;
		rect.w = stretch_buffer->w;
		rect.h = stretch_buffer->h;
		
		SDL_BlitSurface (stretch_buffer, NULL, screen, &rect);
		rects[num_rects++] = rect;
		
		SDL_UpdateRects (screen, num_rects, rects);
		now_time = SDL_GetTicks ();
		if (now_time < last_time + FPS) SDL_Delay(last_time + FPS - now_time);
	} while (!done);
	
	return done;
}

int game_loop (void) {
	int done = 0, map;
	SDL_Event event;
	SDLKey key;
	Uint32 last_time, now_time;
	SDL_Rect rect;
	char buffer[20];
	int g, *h;
	AstroStatus astro;
	
	int astro_dir = 0;
	SDL_Surface *texto;
	int shooting, space_toggle, enter_toggle, switch_toggle, astro_destroyed;
	int turret_shooting, turret_dir;
	int switch_timer;
	SDL_Rect shoot_rect, turret_shoot_rect;
	
	int contador_hits, refresh_tiros, refresh_score;
	int score, save_score;
	int pantalla_abierta = 0, timer_pantalla = 0;
	int lives_collected, vidas;
	
	int *has_turret = &(astro.has_turret);
	Target *targets = astro.targets;
	Linea *lineas = astro.lineas;
	Bloque *bloques = astro.bloques;
	
	astro_destroyed = shooting = space_toggle = enter_toggle = switch_toggle = turret_shooting = FALSE;
	
	vidas = 2;
	lives_collected = score = save_score = 0;
	
	astro.astro_rect.w = images[IMG_ASTRO]->w;
	astro.astro_rect.h = images[IMG_ASTRO]->h;
	astro.astro_rect.x = 160;
	astro.astro_rect.y = 396;
	
	if (!leer_nivel (OFFICIAL_LEVEL_PACK, nivel_actual, &astro)) {
		return GAME_QUIT;
	}
	
	if (*has_turret != -1) {
		turret_dir = targets[*has_turret].image - IMG_TURRET_1;
		turret_shoot_rect.w = images[IMG_TURRET_SHOOT_1 + turret_dir]->w;
		turret_shoot_rect.h = images[IMG_TURRET_SHOOT_1 + turret_dir]->h;
	}
	
	contador_hits = 0;
	refresh_score = refresh_tiros = TRUE;
	shoot_rect.w = images[IMG_SHOOT]->w;
	shoot_rect.h = images[IMG_SHOOT]->h;
	
	SDL_BlitSurface (images[IMG_ARCADE], NULL, screen, NULL);
	
	redibujar_nivel (nivel_actual);
	
	SDL_UpdateRects (screen, num_rects, rects);
	num_rects = 0;
	
	SDL_FillRect (game_buffer, NULL, 0); /* Transparencia total */
	
	do {
		last_time = SDL_GetTicks ();
		
		restaurar_dibujado (screen);
		
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
					} else if (key == SDLK_ESCAPE) {
						done = GAME_QUIT;
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
				case SDL_MOUSEMOTION:
					map = map_button_in_game (event.motion.x, event.motion.y);
					cp_button_motion (map);
					break;
				case SDL_MOUSEBUTTONDOWN:
					map = map_button_in_game (event.button.x, event.button.y);
					cp_button_down (map);
					break;
				case SDL_MOUSEBUTTONUP:
					map = map_button_in_game (event.button.x, event.button.y);
					map = cp_button_up (map);
					
					if (map == BUTTON_CLOSE) {
						done = GAME_QUIT;
					}
					break;
			}
		}
		
		if (space_toggle && !shooting && !pantalla_abierta) {
			shooting = TRUE;
			shoot_rect.x = astro.astro_rect.x + 22;
			shoot_rect.y = astro.astro_rect.y + 8;
			astro.tiros--;
			refresh_tiros = TRUE;
			if (use_sound) Mix_PlayChannel (-1, sounds[SND_SHOOT], 0);
		}
		
		/* Que el turret también tire */
		if (*has_turret != -1 && space_toggle && !turret_shooting && !pantalla_abierta) {
			if (targets[*has_turret].image != IMG_TURRET_DESTROYED) {
				turret_shooting = TRUE;
				
				if (turret_dir == 0 || turret_dir == 2) {
					turret_shoot_rect.x = targets[*has_turret].rect.x + 20;
					turret_shoot_rect.y = targets[*has_turret].rect.y + 26;
				} else {
					turret_shoot_rect.x = targets[*has_turret].rect.x + 26;
					turret_shoot_rect.y = targets[*has_turret].rect.y + 20;
				}
			}
		}
		
		/* Mover los objetivos */
		for (g = 0; g < astro.n_targets; g++) {
			h = &targets[g].pos;
			targets[g].rect.x = targets[g].puntos[*h].x;
			targets[g].rect.y = targets[g].puntos[*h].y;
		}
		
		if (shooting) {
			/* Verificar colisiones contra los bloques */
			for (g = 0; g < astro.n_bloques; g++) {
				if (SDL_HasIntersection (&shoot_rect, (SDL_Rect *)&bloques[g])) {
					shooting = FALSE;
				}
			}
			
			/* Verificar colisiones contra los targets */
			for (g = 0; g < astro.n_targets; g++) {
				if (g == *has_turret) {
					/* La colisión contra el turret es un poco recortada */
					rect = targets[g].rect;
					switch (turret_dir) {
						case 0:
							rect.x += 16;
						case 2:
							rect.w -= 16;
							rect.h -= 28;
							rect.y += 14;
							break;
						case 1:
							rect.y += 16;
						case 3:
							rect.x += 14;
							rect.w -= 28;
							rect.h -= 16;
							break;
					}
					
					if (SDL_HasIntersection (&shoot_rect, &rect)) {
						if (!targets[g].golpeado) {
							targets[g].golpeado = TRUE;
							targets[g].animar = FALSE;
							targets[g].image = IMG_TURRET_DESTROYED;
							
							shooting = FALSE;
							score += 5;
							refresh_score = TRUE;
							if (use_sound) Mix_PlayChannel (-1, sounds[SND_TURRET_SPLODE], 0);
						}
					}
				} else if (SDL_HasIntersection (&shoot_rect, (SDL_Rect *)&targets[g])) {
					if (!targets[g].golpeado) {
						switch (targets[g].image) {
							case IMG_TARGET_NORMAL_GREEN:
							case IMG_TARGET_MINI_GREEN:
							case IMG_TARGET_BIG_GREEN:
								targets[g].golpeado = TRUE;
								targets[g].animar = FALSE;
								targets[g].image += 2; /* Cambiar a rojo */
								contador_hits++;
								score += 10;
								refresh_score = TRUE;
								if (use_sound) Mix_PlayChannel (-1, sounds[SND_HIT], 0);
								if (targets[g].on_hit != NULL) {
									lua_astro_call_target_hit (&targets[g]);
								}
								break;
							case IMG_TARGET_NORMAL_BLUE:
							case IMG_TARGET_MINI_BLUE:
							case IMG_TARGET_BIG_BLUE:
								if (contador_hits == 0) {
									/* Golpearon la vida primero */
									vidas++;
									lives_collected++;
									if (lives_collected == 8) {
										earn_stamp (c, 61);
									}
									score += 100;
									if (use_sound) Mix_PlayChannel (-1, sounds[SND_ONE_UP], 0);
									/* TODO: Mostrar aviso de 1-up que se desvanece */
								} else {
									/* Lástima, ya no es tan importante la vida */
									score += 25;
									if (use_sound) Mix_PlayChannel (-1, sounds[SND_HIT], 0);
								}
								refresh_score = TRUE;
								contador_hits++;
								targets[g].golpeado = TRUE;
								targets[g].animar = FALSE;
								targets[g].image += 3; /* Cambiar a rojo */
								if (targets[g].on_hit != NULL) {
									lua_astro_call_target_hit (&targets[g]);
								}
								break;
							case IMG_SWITCH_ORANGE:
								/* Golpearon el switch naranja, desaparecer los bloques lentamente */
								switch_toggle = TRUE;
								switch_timer = 0;
								targets[g].image++; /* Switch golpeado */
								targets[g].golpeado = TRUE;
								targets[g].animar = FALSE;
								if (use_sound) Mix_PlayChannel (-1, sounds[SND_ORANGE_SWITCH], 0);
								break;
							case IMG_TARGET_NORMAL_YELLOW:
							case IMG_TARGET_MINI_YELLOW:
							case IMG_TARGET_BIG_YELLOW:
								targets[g].image--; /* Cambiar a verde */
								contador_hits++;
								score += 25;
								refresh_score = TRUE;
								if (use_sound) Mix_PlayChannel (-1, sounds[SND_YELLOW_HIT], 0);
								break;
							case IMG_TARGET_EXPAND_1:
								targets[g].golpeado = TRUE;
								targets[g].animar = FALSE;
								contador_hits++;
								/* El cambio de color ocurre abajo */
								score += 25;
								refresh_score = TRUE;
								if (use_sound) Mix_PlayChannel (-1, sounds[SND_SLIDE], 0);
								if (targets[g].on_hit != NULL) {
									lua_astro_call_target_hit (&targets[g]);
								}
								break;
							case IMG_TARGET_MINI_LIGHTGREEN:
								targets[g].golpeado = TRUE;
								targets[g].animar = FALSE;
								targets[g].image++; /* Cambiar a naranja */
								contador_hits++;
								score += 10;
								refresh_score = TRUE;
								if (use_sound) Mix_PlayChannel (-1, sounds[SND_HIT], 0);
								if (targets[g].on_hit != NULL) {
									lua_astro_call_target_hit (&targets[g]);
								}
								break;
							case IMG_TARGET_500_GREEN:
								targets[g].golpeado = TRUE;
								targets[g].animar = FALSE;
								targets[g].image++; /* Cambiar a rojo */
								contador_hits++;
								
								score += 500;
								refresh_score = TRUE;
								if (use_sound) Mix_PlayChannel (-1, sounds[SND_START_GAME], 0);
								break;
						}
					} /* Cierro if de detenido */
					shooting = FALSE;
				} /* Cierro if de intersección */
			} /* Cierro for contra los targets */
			
			/* Colisión contra la nave azul, si existe */
			if (astro.blue.pack != 0 && astro.blue.timer > 607 && astro.blue.timer < 700) {
				if (SDL_HasIntersection (&shoot_rect, (SDL_Rect *)&astro.blue.rect)) {
					astro.blue.timer = 700;
					shooting = FALSE;
					if (use_sound) Mix_PlayChannel (-1, sounds[SND_ORANGE_SWITCH], 0);
				}
			}
		} /* Si está tirando */
		
		/* Si el turret está tirando */
		if (*has_turret != -1 && turret_shooting) {
			/* Verificar colisiones contra los bloques */
			for (g = 0; g < astro.n_bloques; g++) {
				if (SDL_HasIntersection (&turret_shoot_rect, (SDL_Rect *)&bloques[g])) {
					turret_shooting = FALSE;
				}
			}
			
			/* Verificar colisiones contra los targets */
			for (g = 0; g < astro.n_targets; g++) {
				if (g == *has_turret) continue;
				if (SDL_HasIntersection (&turret_shoot_rect, (SDL_Rect *)&targets[g])) {
					if (!targets[g].golpeado) {
						switch (targets[g].image) {
							case IMG_TARGET_NORMAL_GREEN:
							case IMG_TARGET_MINI_GREEN:
							case IMG_TARGET_BIG_GREEN:
								targets[g].golpeado = TRUE;
								targets[g].animar = FALSE;
								targets[g].image += 2; /* Cambiar a rojo */
								contador_hits++;
								score += 10;
								refresh_score = TRUE;
								if (use_sound) Mix_PlayChannel (-1, sounds[SND_HIT], 0);
								if (targets[g].on_hit != NULL) {
									lua_astro_call_target_hit (&targets[g]);
								}
								break;
							case IMG_TARGET_NORMAL_BLUE:
							case IMG_TARGET_MINI_BLUE:
							case IMG_TARGET_BIG_BLUE:
								if (contador_hits == 0) {
									/* Golpearon la vida primero */
									vidas++;
									lives_collected++;
									earn_stamp (c, 55);
									if (lives_collected == 8) {
										earn_stamp (c, 61);
									}
									score += 100;
									if (use_sound) Mix_PlayChannel (-1, sounds[SND_ONE_UP], 0);
									/* TODO: Mostrar aviso de 1-up que se desvanece */
								} else {
									/* Lástima, ya no es tan importante la vida */
									score += 25;
									if (use_sound) Mix_PlayChannel (-1, sounds[SND_HIT], 0);
								}
								refresh_score = TRUE;
								contador_hits++;
								targets[g].golpeado = TRUE;
								targets[g].animar = FALSE;
								targets[g].image += 3; /* Cambiar a rojo */
								if (targets[g].on_hit != NULL) {
									lua_astro_call_target_hit (&targets[g]);
								}
								break;
							case IMG_SWITCH_ORANGE:
								/* Golpearon el switch naranja, desaparecer los bloques lentamente */
								switch_toggle = TRUE;
								switch_timer = 0;
								targets[g].image++; /* Switch golpeado */
								targets[g].golpeado = TRUE;
								targets[g].animar = FALSE;
								if (use_sound) Mix_PlayChannel (-1, sounds[SND_ORANGE_SWITCH], 0);
								break;
							case IMG_TARGET_NORMAL_YELLOW:
							case IMG_TARGET_MINI_YELLOW:
							case IMG_TARGET_BIG_YELLOW:
								targets[g].image -= 1; /* Cambiar a verde */
								contador_hits++;
								score += 25;
								refresh_score = TRUE;
								if (use_sound) Mix_PlayChannel (-1, sounds[SND_YELLOW_HIT], 0);
								break;
							case IMG_TARGET_EXPAND_1:
								targets[g].golpeado = TRUE;
								targets[g].animar = FALSE;
								contador_hits++;
								/* El cambio de color ocurre abajo */
								score += 25;
								refresh_score = TRUE;
								if (use_sound) Mix_PlayChannel (-1, sounds[SND_SLIDE], 0);
								if (targets[g].on_hit != NULL) {
									lua_astro_call_target_hit (&targets[g]);
								}
								break;
						}
					} /* Cierro if de detenido */
					turret_shooting = FALSE;
				} /* Cierro if de intersección */
			} /* Cierro for contra los targets */
			
			/* Colisión contra la nave azul, si existe */
			if (astro.blue.pack != 0 && astro.blue.timer > 607 && astro.blue.timer < 700) {
				if (SDL_HasIntersection (&turret_shoot_rect, (SDL_Rect *)&astro.blue.rect)) {
					astro.blue.timer = 700;
					turret_shooting = FALSE;
					earn_stamp (c, 54);
					if (use_sound) Mix_PlayChannel (-1, sounds[SND_ORANGE_SWITCH], 0);
				}
			}
			
			/* Colisión contra la nave */
			if (SDL_HasIntersection (&astro.astro_rect, &turret_shoot_rect)) {
				/* Nave destruida */
				pantalla_abierta = SCREEN_RESTART;
				shooting = FALSE;
				turret_shooting = FALSE;
				astro_destroyed = TRUE;
				if (vidas == 0) {
					/* Game Over */
					done = GAME_CONTINUE;
					pantalla_abierta = SCREEN_NONE;
					if (use_sound) Mix_PlayChannel (-1, sounds[SND_GAME_OVER], 0);
				} else {
					if (use_sound) Mix_PlayChannel (-1, sounds[SND_GUN_SPLODE], 0);
				}
			}
		} /* Si está tirando el turret*/
		
		if (!pantalla_abierta && contador_hits >= astro.hits_requeridos) {
			/* Nivel terminado, hay que abrir la pantalla de siguiente nivel */
			pantalla_abierta = SCREEN_NEXT_LEVEL;
			score += (astro.tiros * 10);
			astro.tiros = 0;
			refresh_score = TRUE;
			refresh_tiros = TRUE;
			if (astro.next_level == -1) {
				if (((nivel_actual >> 17) & 0x07) == 2) {
					earn_stamp (c, 59);
				}
				done = GAME_CONTINUE;
				pantalla_abierta = SCREEN_NONE;
			} else {
				if (use_sound) Mix_PlayChannel (-1, sounds[SND_LEVEL_DONE], 0);
			}
		} else if (!pantalla_abierta && astro.tiros == 0 && !shooting) {
			/* Se acabaron los tiros, reiniciar el nivel */
			pantalla_abierta = SCREEN_RESTART;
			if (vidas == 0) {
				done = GAME_CONTINUE;
				pantalla_abierta = SCREEN_NONE;
				if (use_sound) Mix_PlayChannel (-1, sounds[SND_GAME_OVER], 0);
			}
		}
		
		/* Mover la nave a su nueva posición */
		if (!astro_destroyed && astro_dir < 0 && astro.astro_rect.x > 5) {
			astro.astro_rect.x -= 5;
			if (astro.on_ship_move != NULL) lua_astro_call_ship_move ();
		} else if (!astro_destroyed && astro_dir > 0 && astro.astro_rect.x < 315) {
			astro.astro_rect.x += 5;
			if (astro.on_ship_move != NULL) lua_astro_call_ship_move ();
		}
		
		/* Redibujar todo */
		SDL_BlitSurface (images[IMG_GAMEAREA], NULL, game_buffer, NULL);
		
		/* Dibujar las lineas de fondo */
		for (g = 0; g < astro.n_lineas; g++) {
			SDL_BlitSurface (images[lineas[g].image], NULL, game_buffer, (SDL_Rect *)&lineas[g]);
		}
		
		if (switch_toggle && switch_timer < 17) {
			switch_timer++;
		}
		
		if (astro.blue.pack != 0) {
			if (astro.blue.timer <= 607 || astro.blue.timer >= 700) {
				astro.blue.timer++;
			}
		}
		
		if (*has_turret != -1 && turret_shooting) {
			g = IMG_TURRET_SHOOT_1 + turret_dir;
			turret_shoot_rect.w = images[g]->w;
			turret_shoot_rect.h = images[g]->h;
			
			SDL_BlitSurface (images[g], NULL, game_buffer, &turret_shoot_rect);
			
			switch (turret_dir) {
				case 0:
					turret_shoot_rect.x += 30;
					break;
				case 1:
					turret_shoot_rect.y += 30;
					break;
				case 2:
					turret_shoot_rect.x -= 30;
					break;
				case 3:
					turret_shoot_rect.y -= 30;
					break;
			}
			
			if (turret_shoot_rect.x < 0 || turret_shoot_rect.x > 400 || turret_shoot_rect.y < 0 || turret_shoot_rect.y > 480) {
				turret_shooting = FALSE;
			}
		}
		
		/* Dibujar los targets */
		for (g = 0; g < astro.n_targets; g++) {
			if (targets[g].golpeado && targets[g].image < IMG_TARGET_EXPAND_RED && targets[g].image >= IMG_TARGET_EXPAND_1) {
				targets[g].image++;
				if (targets[g].image < IMG_TARGET_EXPAND_5) {
					targets[g].puntos[targets[g].pos].x -= 8;
					targets[g].rect.x -= 8;
					targets[g].rect.w += 16;
				}
			}
			SDL_BlitSurface (images[targets[g].image], NULL, game_buffer, (SDL_Rect *)&targets[g]);
			if (targets[g].animar) {
				targets[g].pos++;
				if (targets[g].pos >= targets[g].total_vel) targets[g].pos = 0;
			}
		}
		
		/* Dibujar los bloques */
		for (g = 0; g < astro.n_bloques; g++) {
			if (bloques[g].image == IMG_BLOCK_ORANGE && switch_toggle) {
				if (switch_timer < 17) {
					rect.y = switch_timer * 4;
					bloques[g].rect.y += 4;
					rect.x = 0;
					rect.h = bloques[g].rect.h -= 4;
					rect.w = bloques[g].rect.w;
					SDL_BlitSurface (images[bloques[g].image], &rect, game_buffer, (SDL_Rect *)&bloques[g]);
				}
			} else if (bloques[g].image == IMG_BLOCK_ORANGE_MINI && switch_toggle) {
				if (switch_timer < 17) {
					rect.y = switch_timer * 2;
					bloques[g].rect.y += 2;
					rect.x = 0;
					rect.h = bloques[g].rect.h -= 2;
					rect.w = bloques[g].rect.w;
					SDL_BlitSurface (images[bloques[g].image], &rect, game_buffer, (SDL_Rect *)&bloques[g]);
				}
			} else {
				SDL_BlitSurface (images[bloques[g].image], NULL, game_buffer, (SDL_Rect *)&bloques[g]);
			}
		}
		
		if (astro.blue.pack != 0) {
			if (astro.blue.timer >= 600 && astro.blue.timer <= 607) {
				/* Dibujar con alfa */
				g = SDL_ALPHA_OPAQUE * (astro.blue.timer - 600) / 8;
				SDL_SetAlpha (images[IMG_ASTRO_BLUE], SDL_SRCALPHA, g);
				SDL_BlitSurface (images[IMG_ASTRO_BLUE], NULL, game_buffer, (SDL_Rect *)&astro.blue.rect);
			} else if (astro.blue.timer > 607 && astro.blue.timer < 700) {
				SDL_SetAlpha (images[IMG_ASTRO_BLUE], SDL_SRCALPHA, SDL_ALPHA_OPAQUE);
				SDL_BlitSurface (images[IMG_ASTRO_BLUE], NULL, game_buffer, (SDL_Rect *)&astro.blue.rect);
			} else if (astro.blue.timer >= 700) {
				g = astro.blue.timer - 700;
				
				if (g % 4 == 3 || g % 4 == 0) {
					SDL_BlitSurface (images[IMG_ASTRO_BLUE], NULL, game_buffer, (SDL_Rect *)&astro.blue.rect);
				} else {
					SDL_BlitSurface (images[IMG_ASTRO], NULL, game_buffer, (SDL_Rect *)&astro.blue.rect);
				}
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
			rect.w = 115;
			rect.h = 36;
			SDL_BlitSurface (images[IMG_ARCADE], &rect, screen, &rect);
			rects[num_rects++] = rect;
			
			/* Borrar la parte de abajo de los tiros */
			rect.x = 0;
			rect.y = 436;
			rect.w = 376;
			rect.h = 52;
			SDL_FillRect (game_buffer, &rect, 0); /* Transparencia total */
			
			SDL_SetAlpha (images[IMG_SHOOT], 0, 0);
			/* Y de los tiros disponibles: 456 */
			/* X 3, espaciado de: 4 pixeles */
			if (astro.tiros == 0) {
				/* Dibujar la palabra "0 shoots" */
			} else if (astro.tiros > 0) {
				rect.x = 3;
				rect.y = 456;
				rect.w = images[IMG_SHOOT]->w;
				rect.h = images[IMG_SHOOT]->h;
				for (g = 0; g < astro.tiros; g++) {
					SDL_BlitSurface (images[IMG_SHOOT], NULL, game_buffer, &rect);
					rect.x += (4 + rect.w);
				}
			}
			SDL_SetAlpha (images[IMG_SHOOT], SDL_SRCALPHA, 0);
			
			refresh_tiros = FALSE;
		}
		
		if (refresh_score) {
			/* Borrar el area debajo de la zona de juego */
			rect.x = GAME_AREA_X + 130;
			rect.y = GAME_AREA_Y + 305;
			rect.w = 148;
			rect.h = 36;
			SDL_BlitSurface (images[IMG_ARCADE], &rect, screen, &rect);
			rects[num_rects++] = rect;
			
			/* Redibujar las vidas y el score */
			sprintf (buffer, "Lives: %d", vidas);
			texto = TTF_RenderUTF8_Blended (ttf20_burbank_small, buffer, amarillo);
			rect.x = GAME_AREA_X + 115;
			rect.y = GAME_AREA_Y + 320;
			rect.w = texto->w;
			rect.h = texto->h;
			
			SDL_BlitSurface (texto, NULL, screen, &rect);
			SDL_FreeSurface (texto);
			
			sprintf (buffer, "%d", score);
			texto = TTF_RenderUTF8_Blended (ttf20_burbank_small, buffer, amarillo);
			rect.x = GAME_AREA_X + stretch_buffer->w - 5 - texto->w;
			rect.y = GAME_AREA_Y + 320;
			rect.w = texto->w;
			rect.h = texto->h;
			
			SDL_BlitSurface (texto, NULL, screen, &rect);
			SDL_FreeSurface (texto);
			
			refresh_score = FALSE;
		}
		
		/* Si es que hay, redibujar la pantalla de reiniciar o de siguiente nivel */
		if (pantalla_abierta == SCREEN_NEXT_LEVEL) {
			rect.x = 65;
			rect.y = 100;
			rect.h = images[IMG_NEXT_LEVEL]->h;
			rect.w = images[IMG_NEXT_LEVEL]->w;
			
			SDL_BlitSurface (images[IMG_NEXT_LEVEL], NULL, game_buffer, &rect);
			
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
		
		if (!astro_destroyed) {
			SDL_BlitSurface (images[IMG_ASTRO], NULL, game_buffer, &astro.astro_rect);
		} else {
			SDL_BlitSurface (images[IMG_ASTRO_DESTROYED], NULL, game_buffer, &astro.astro_rect);
		}
		
		/* Redibujar el marco blanco por los objetos que se salen */
		SDL_BlitSurface (images[IMG_FRAME], NULL, game_buffer, NULL);
		
		/* Escalar el area de juego antes de copiarla a la pantalla */
		SDL_StretchSurfaceBlit (game_buffer, NULL, stretch_buffer, NULL);
		
		rect.x = GAME_AREA_X;
		rect.y = GAME_AREA_Y;
		rect.w = stretch_buffer->w;
		rect.h = stretch_buffer->h;
		
		SDL_BlitSurface (stretch_buffer, NULL, screen, &rect);
		rects[num_rects++] = rect;
		
		if (cp_button_refresh[BUTTON_CLOSE]) {
			rect.x = 707; rect.y = 16;
			rect.w = images[IMG_BUTTON_CLOSE_UP]->w; rect.h = images[IMG_BUTTON_CLOSE_UP]->h;
			
			SDL_BlitSurface (images[IMG_ARCADE], &rect, screen, &rect);
			
			SDL_BlitSurface (images[cp_button_frames[BUTTON_CLOSE]], NULL, screen, &rect);
			rects[num_rects++] = rect;
			cp_button_refresh[BUTTON_CLOSE] = 0;
		}
		
		if (activar_estampa) {
			dibujar_estampa (screen, c, TRUE);
			rects[num_rects++] = stamp_rect;
		}
		
		SDL_UpdateRects (screen, num_rects, rects);
		num_rects = 0;
		
		g = FALSE; /* Bandera de cargar nivel */
		if (astro.blue.pack != 0 && astro.blue.timer >= 710) {
			g = TRUE;
			nivel_actual = 1 | (astro.blue.pack << 17);
		}
		
		if (pantalla_abierta && (timer_pantalla > 56 || enter_toggle)) {
			g = TRUE;
			if (pantalla_abierta == SCREEN_RESTART) {
				/* Ocultar y reiniciar el nivel */
				vidas--;
				refresh_score = TRUE;
				/* TODO: nivel_reiniciado = TRUE */
				score = save_score;
			} else if (pantalla_abierta == SCREEN_NEXT_LEVEL) {
				/* Pasar al siguiente nivel */
				nivel_actual = astro.next_level;
			}
		}
		
		if (nivel_actual & (1 << 16) && timer_pantalla > 20 && enter_toggle && (astro.blue.pack == 0 || astro.blue.timer < 700)) {
			g = TRUE;
			nivel_actual = astro.next_level;
		}
		
		if (g == TRUE) {
			if (nivel_actual == -1) {
				/* Si llegó al final, puntos extras por las vidas */
				if (use_sound) Mix_PlayChannel (-1, sounds[SND_WIN], 0);
				score = score + vidas * 50;
				done = GAME_CONTINUE;
			} else {
				if (!leer_nivel (OFFICIAL_LEVEL_PACK, nivel_actual, &astro)) {
					/* Falló al cargar el nivel */
					done = GAME_QUIT;
				} else {
					redibujar_nivel (nivel_actual);
					if (*has_turret != -1) {
						turret_dir = targets[*has_turret].image - IMG_TURRET_1;
						turret_shoot_rect.w = images[IMG_TURRET_SHOOT_1 + turret_dir]->w;
						turret_shoot_rect.h = images[IMG_TURRET_SHOOT_1 + turret_dir]->h;
					}
				}
			}
			save_score = score;
			shooting = turret_shooting = FALSE;
			astro_destroyed = FALSE;
			switch_toggle = FALSE;
			contador_hits = 0;
			refresh_tiros = TRUE;
			pantalla_abierta = SCREEN_NONE;
			timer_pantalla = 0;
		}
		
		now_time = SDL_GetTicks ();
		if (now_time < last_time + FPS) SDL_Delay(last_time + FPS - now_time);
		
	} while (!done);
	
	return done;
}

void redibujar_nivel (int level) {
	SDL_Surface *texto;
	SDL_Rect rect;
	char buffer[10];
	
	rect.x = GAME_AREA_X;
	rect.y = GAME_AREA_Y - 35;
	rect.w = stretch_buffer->w + 40;
	rect.h = 35;
	SDL_BlitSurface (images[IMG_ARCADE], &rect, screen, &rect);
	rects[num_rects++] = rect;
	
	switch ((level >> 17) & 0x07) {
		case 0:
			texto = TTF_RenderUTF8_Blended (ttf24_burbank_small, "Astro Barrier", blanco);
			break;
		case 1:
			texto = TTF_RenderUTF8_Blended (ttf24_burbank_small, "Secret Levels", amarillo);
			break;
		case 2:
			texto = TTF_RenderUTF8_Blended (ttf24_burbank_small, "Secret Levels", azul);
			break;
		default:
			texto = TTF_RenderUTF8_Blended (ttf24_burbank_small, "Secret Levels", blanco);
			break;
	}
	rect.x = GAME_AREA_X + 12;
	rect.w = texto->w;
	rect.h = texto->h;
	rect.y = GAME_AREA_Y - 3 - rect.h;
	
	SDL_BlitSurface (texto, NULL, screen, &rect);
	SDL_FreeSurface (texto);
	
	texto = TTF_RenderUTF8_Blended (ttf20_burbank_small, "L", blanco);
	
	rect.x = GAME_AREA_X + 220;
	rect.w = texto->w;
	rect.h = texto->h;
	rect.y = GAME_AREA_Y - 4 - rect.h;
	
	SDL_BlitSurface (texto, NULL, screen, &rect);
	
	rect.x = GAME_AREA_X + 223 + texto->w;
	SDL_FreeSurface (texto);
	
	sprintf (buffer, "%d", (level & 0xFF));
	switch ((level >> 17) & 0x07) {
		case 1:
			texto = TTF_RenderUTF8_Blended (ttf20_burbank_small, buffer, amarillo);
			break;
		case 2:
			texto = TTF_RenderUTF8_Blended (ttf20_burbank_small, buffer, azul);
			break;
		default:
			texto = TTF_RenderUTF8_Blended (ttf20_burbank_small, buffer, blanco);
	}
	
	rect.w = texto->w;
	rect.h = texto->h;
	rect.y = GAME_AREA_Y - 4 - rect.h;
	
	SDL_BlitSurface (texto, NULL, screen, &rect);
	SDL_FreeSurface (texto);
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
	
	image = IMG_Load (GAMEDATA_DIR "images/icon.png");
	if (image) {
		SDL_WM_SetIcon (image, NULL);
		SDL_FreeSurface (image);
	}
	SDL_WM_SetCaption ("Astro Barrier", "Astro Barrier");
	
	/* Crear la pantalla de dibujado */
	screen = set_video_mode (0);
	
	if (screen == NULL) {
		fprintf (stderr,
			"Error: Can't setup 760x480 video mode.\n"
			"The error returned by SDL is:\n"
			"%s\n", SDL_GetError());
		exit (1);
	}
	
	use_sound = 1;
	if (SDL_InitSubSystem (SDL_INIT_AUDIO) < 0) {
		fprintf (stdout,
			"Warning: Can't initialize the audio subsystem\n"
			"Continuing...\n");
		use_sound = 0;
	}
	
	if (use_sound) {
		/* Inicializar el sonido */
		if (Mix_OpenAudio (22050, AUDIO_S16, 2, 4096) < 0) {
			fprintf (stdout,
				"Warning: Can't initialize the SDL Mixer library\n");
			use_sound = 0;
		} else {
			Mix_AllocateChannels (3);
		}
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
	
	if (use_sound) {
		for (g = 0; g < NUM_SOUNDS; g++) {
			sounds[g] = Mix_LoadWAV (sound_names [g]);
			
			if (sounds[g] == NULL) {
				fprintf (stderr,
					"Failed to load data file:\n"
					"%s\n"
					"The error returned by SDL is:\n"
					"%s\n", sound_names [g], SDL_GetError ());
				SDL_Quit ();
				exit (1);
			}
			Mix_VolumeChunk (sounds[g], MIX_MAX_VOLUME / 2);
		}
	}
	if (TTF_Init () < 0) {
		fprintf (stderr,
			"Error: Can't initialize the SDL TTF library\n"
			"%s\n", TTF_GetError ());
		SDL_Quit ();
		exit (1);
	}
	
	ttf24_burbank_small = TTF_OpenFont (GAMEDATA_DIR "burbanksb.ttf", 24);
	
	if (!ttf24_burbank_small) {
		fprintf (stderr,
			"Failed to load font file 'Burbank Small Bold'\n"
			"The error returned by SDL is:\n"
			"%s\n", TTF_GetError ());
		SDL_Quit ();
		exit (1);
	}
	//TTF_SetFontStyle (ttf24_burbank_small, TTF_STYLE_BOLD);
	
	ttf20_burbank_small = TTF_OpenFont (GAMEDATA_DIR "burbanksb.ttf", 20);
	
	if (!ttf20_burbank_small) {
		fprintf (stderr,
			"Failed to load font file 'Burbank Small Bold'\n"
			"The error returned by SDL is:\n"
			"%s\n", TTF_GetError ());
		SDL_Quit ();
		exit (1);
	}
	TTF_SetFontHinting (ttf20_burbank_small, TTF_HINTING_LIGHT);
	
	/* La nave color azul tiene transparencia especial */
	SDL_SetColorKey (images[IMG_ASTRO_BLUE], SDL_SRCCOLORKEY, SDL_MapRGB (images[IMG_ASTRO_BLUE]->format, 0xFF, 0, 0xFF));
	
	/* En esta situación tan especial, necesito varios buffers intermedios */
	game_buffer = SDL_AllocSurface (SDL_SWSURFACE, images[IMG_GAMEAREA]->w, images[IMG_GAMEAREA]->h + 52, 32, 0x00FF0000, 0x0000FF00, 0x000000FF, 0xFF000000);
	stretch_buffer = SDL_AllocSurface (SDL_SWSURFACE, 263, 305 + 36, 32, 0x00FF0000, 0x0000FF00, 0x000000FF, 0xFF000000);
	
	/* Quitarle el alpha, para que cuando se copie a la superficie no tenga problemas */
	SDL_SetAlpha (images[IMG_GAMEAREA], 0, 0);
	SDL_SetAlpha (images[IMG_GAMEINTRO], 0, 0);
	
	srand (SDL_GetTicks ());
}

int map_button_in_game (int x, int y) {
	if (x >= 707 && x < 736 && y >= 16 && y < 45) return BUTTON_CLOSE;
	return BUTTON_NONE;
}

int map_button_in_opening (int x, int y) {
	if (x >= 321 && x < 461 && y >= 331 && y < 367) return BUTTON_START;
	if (x >= 707 && x < 736 && y >= 16 && y < 45) return BUTTON_CLOSE;
	return BUTTON_NONE;
}
