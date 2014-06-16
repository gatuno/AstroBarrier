/*
 * util-bresenham.h
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
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef __UTIL_BRESENHAM_H__
#define __UTIL_BRESENHAM_H__

#include <SDL.h>

#ifndef FALSE
#define FALSE 0
#endif

#ifndef TRUE
#define TRUE !FALSE
#endif

#define SWAP(a, b, t) ((t) = (a), (a) = (b), (b) = (t))

/* Definición de un Punto */
typedef struct {
	int x, y;
} Punto;

void line (int x1, int y1, int x2, int y2, Punto *puntos, int vel);

__attribute__((always_inline)) static inline int SDL_RectEmpty (const SDL_Rect *r) {
	return ((!r) || (r->w <= 0) || (r->h <= 0)) ? TRUE : FALSE;
}

int SDL_HasIntersection (const SDL_Rect * A, const SDL_Rect * B);

#endif /* __UTIL_BRESENHAM_H__ */

