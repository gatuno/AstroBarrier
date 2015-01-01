/*
 * astro-types.h
 * This file is part of Astro Barrier
 *
 * Copyright (C) 2014 - Félix Arreola Rodríguez
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

#ifndef __ASTRO_TYPES_H__
#define __ASTRO_TYPES_H__

#include <SDL.h>

#include "util-bresenham.h"

/* Enumerar las imágenes */
enum {
	IMG_ARCADE,
	
	IMG_ASTRO,
	IMG_ASTRO_DESTROYED,
	
	IMG_ASTRO_BLUE,
	
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
	
	IMG_TARGET_EXPAND_1,
	IMG_TARGET_EXPAND_2,
	IMG_TARGET_EXPAND_3,
	IMG_TARGET_EXPAND_4,
	IMG_TARGET_EXPAND_5,
	
	IMG_TARGET_EXPAND_RED,
	
	IMG_BLOCK_NORMAL,
	IMG_BLOCK_MINI,
	IMG_BLOCK_BIG,
	IMG_BLOCK_TALL,
	
	IMG_BLOCK_ORANGE,
	IMG_BLOCK_ORANGE_MINI,
	
	IMG_LINE1,
	IMG_LINE2_A,
	IMG_LINE2_B,
	IMG_LINE3,
	IMG_LINE4_A,
	IMG_LINE4_B,
	IMG_LINE5,
	IMG_LINE6_A,
	IMG_LINE6_B,
	IMG_LINE6_C,
	IMG_LINE7,
	IMG_LINE8,
	IMG_LINE9,
	IMG_LINE10,
	IMG_LINE11_A,
	IMG_LINE11_B,
	IMG_LINE12_A,
	IMG_LINE12_B,
	IMG_LINE13,
	IMG_LINE14,
	IMG_LINE15,
	IMG_LINE16_A,
	IMG_LINE16_B,
	IMG_LINE17,
	IMG_LINE18_A,
	IMG_LINE18_B,
	IMG_LINE18_C,
	IMG_LINE18_D,
	IMG_LINE19_A,
	IMG_LINE19_B,
	IMG_LINE19_C,
	IMG_LINE19_D,
	IMG_LINE20,
	IMG_LINE21,
	IMG_LINE22_A,
	IMG_LINE22_B,
	IMG_LINE23_A,
	IMG_LINE23_B,
	IMG_LINE24,
	IMG_LINE25,
	IMG_LINE26_A,
	IMG_LINE26_B,
	IMG_LINE27,
	IMG_LINE28,
	IMG_LINE29,
	IMG_LINE30,
	
	IMG_TURRET_1,
	IMG_TURRET_2,
	IMG_TURRET_3,
	IMG_TURRET_4,
	
	IMG_TURRET_DESTROYED,
	
	IMG_TURRET_SHOOT_1,
	IMG_TURRET_SHOOT_2,
	IMG_TURRET_SHOOT_3,
	IMG_TURRET_SHOOT_4,
	
	IMG_SWITCH_ORANGE,
	IMG_SWITCH_ORANGE_HIT,
	
	IMG_NEXT_LEVEL,
	IMG_RESTART,
	
	NUM_IMAGES
};

typedef struct {
	struct SDL_Rect rect;
	int image;
} Linea;

typedef struct {
	struct SDL_Rect rect;
	int image;
	int total_vel;
	Punto puntos[350];
	int pos;
	int animar;
	int golpeado;
	
	/* Eventos lua */
	const char *on_hit;
} Target;

typedef struct {
	struct SDL_Rect rect;
	int image;
} Bloque;

typedef struct {
	SDL_Rect rect;
	
	int pack;
	int timer;
} SecretAstro;

typedef struct {
	SDL_Rect astro_rect;
	
	int next_level, explicacion, secret_pack;
	
	int n_lineas, n_targets, n_bloques;
	int tiros, hits_requeridos, has_turret;
	
	Linea lineas[10];
	Target targets[10];
	Bloque bloques[10];
	
	SecretAstro blue;
	
	/* Eventos lua */
	const char *on_ship_move;
} AstroStatus;

#endif /* __ASTRO_TYPES_H__ */

