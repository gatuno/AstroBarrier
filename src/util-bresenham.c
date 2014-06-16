/*
 * util-bresenham.c
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

#include <math.h>
#include "util-bresenham.h"

void line (int x1, int y1, int x2, int y2, Punto *puntos, int vel) {
	int delta_x;
	int delta_y;
	int double_delta;
	int dif_delta;
	int p;
	int t, inicio;
	int increment;
	
	delta_x = x2 - x1;
	delta_y = y2 - y1;
	
	if ((delta_x > 0 && delta_y > 0) || (delta_x < 0 && delta_y < 0)) {
		increment = 1;
	} else {
		increment = -1;
	}
	
	delta_x = abs (delta_x);
	delta_y = abs (delta_y);
	
	if (delta_x > delta_y) {
		/* Switch points */
		if (x1 > x2) {
			increment *= -1;
		}
		
		double_delta = delta_y * 2;
		dif_delta = double_delta - (2 * delta_x);
		p = double_delta - delta_x;
		
		delta_y = 0;
		inicio = x1;
		if (x1 <= x2) {
			do {
				//printf ("Bresenham: X: %i, Y: %i\n", x1, y1);
				t = roundf (((float) (delta_x * delta_y)) / ((float) vel));
				
				if (t == x1 - inicio) {
					puntos[delta_y].x = x1;
					puntos[delta_y].y = y1;
					delta_y++;
				}
				
				x1++;
				
				if (p < 0) {
					p = p + double_delta;
				} else {
					p = p + dif_delta;
					y1 = y1 + increment;
				}
			} while (x1 < x2);
		} else {
			do {
				//printf ("Bresenham: X: %i, Y: %i\n", x1, y1);
				t = roundf (((float) (delta_x * delta_y)) / ((float) vel));
				
				if (t == inicio - x1) {
					puntos[delta_y].x = x1;
					puntos[delta_y].y = y1;
					delta_y++;
				}
				
				x1--;
				
				if (p < 0) {
					p = p + double_delta;
				} else {
					p = p + dif_delta;
					y1 = y1 + increment;
				}
			} while (x2 < x1);
		}
	} else {
		/* Switch points */
		if (y1 > y2) {
			increment *= -1;
		}
		double_delta = delta_x * 2;
		dif_delta = double_delta - (2 * delta_y);
		p = double_delta - delta_y;
		
		delta_x = 0;
		inicio = y1;
		
		if (y1 <= y2) {
			do {
				//printf ("Bresenham: X: %i, Y: %i\n", x1, y1);
				t = roundf (((float) (delta_x * delta_y)) / ((float) vel));
				
				if (t == y1 - inicio) {
					puntos[delta_x].x = x1;
					puntos[delta_x].y = y1;
					delta_x++;
				}
				
				y1++;
				
				if (p < 0) {
					p = p + double_delta;
				} else {
					p = p + dif_delta;
					x1 = x1 + increment;
				}
			} while (y1 < y2);
		} else {
			do {
				//printf ("Bresenham: X: %i, Y: %i\n", x1, y1);
				t = roundf (((float) (delta_x * delta_y)) / ((float) vel));
				
				if (t == inicio - y1) {
					puntos[delta_x].x = x1;
					puntos[delta_x].y = y1;
					delta_x++;
				}
				
				y1--;
				
				if (p < 0) {
					p = p + double_delta;
				} else {
					p = p + dif_delta;
					x1 = x1 + increment;
				}
			} while (y2 < y1);
		}
	}
}

int SDL_HasIntersection (const SDL_Rect * A, const SDL_Rect * B) {
	int Amin, Amax, Bmin, Bmax;

	if (!A) {
		return FALSE;
	}

	if (!B) {
		return FALSE;
	}

	/* Special cases for empty rects */
	if (SDL_RectEmpty(A) || SDL_RectEmpty(B)) {
		return FALSE;
	}

	/* Horizontal intersection */
	Amin = A->x;
	Amax = Amin + A->w;
	Bmin = B->x;
	Bmax = Bmin + B->w;
	if (Bmin > Amin) Amin = Bmin;
	if (Bmax < Amax) Amax = Bmax;
	if (Amax <= Amin) return FALSE;

	/* Vertical intersection */
	Amin = A->y;
	Amax = Amin + A->h;
	Bmin = B->y;
	Bmax = Bmin + B->h;
	if (Bmin > Amin) Amin = Bmin;
	if (Bmax < Amax) Amax = Bmax;
	if (Amax <= Amin) return FALSE;

	return TRUE;
}

