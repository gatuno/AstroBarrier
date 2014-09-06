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

#include "SDL_stretchcode.h"
#include "SDL_stretchasm.h"

/* Robado sínicamente desde la SDL_stretch */
int SDL_StretchSurfaceBlit(SDL_Surface *src, SDL_Rect *srcrect,
			   SDL_Surface *dst, SDL_Rect *dstrect) {
	int i = 0;
	int src_row, dst_row;
	SDL_Rect rect;
#     if defined SDL_STRETCH_USE_ASM
	unsigned char* code = 0;
#     endif
	const int bpp = dst->format->BytesPerPixel;
	auto int dest_w;
	auto int dest_x;

	if ( src->format->BitsPerPixel != dst->format->BitsPerPixel ) {
		SDL_SetError("Only works with same format surfaces");
		return(-1);
	}

	/* Verify the blit rectangles */
	if ( srcrect ) {
		if ( (srcrect->x < 0) || (srcrect->y < 0) ||
		     ((srcrect->x+srcrect->w) > src->w) ||
		     ((srcrect->y+srcrect->h) > src->h) ) {
			SDL_SetError("Invalid source blit rectangle");
			return(-1);
		}
	} else {
		rect.x = 0;
		rect.y = 0;
		rect.w = src->w;
		rect.h = src->h;
		srcrect = &rect;
	}

        dest_w = srcrect->w * dst->w / src->w;
        dest_x = srcrect->x * dst->w / src->w;

#      ifdef SDL_STRETCH_USE_ASM
	/* Write the opcodes for this stretch */
	/* if ( (bpp != 3) */
	code = SDL_SetRowStretchCode(srcrect->w, dest_w, bpp);
#      endif


	/*if (PRERUN)   // let the compiler do the dead-code removal 
	{   // Pre-Run the stretch blit (the invisible lines) 
	    src_row = 0;
	    dst_row = 0;
	    while (1)
	    {
		if (src_row >= srcrect->y) break;
		dst_row++; i += src->h;
		if (i < dst->h) continue;
		do { i -= dst->h; src_row++; } while (i >= dst->h);
	    }
	}else */
	{   /* or compute the resulting dst_row and i-fraction directly: */
	    src_row = srcrect->y;
	    dst_row = ((src_row * dst->h)+(src->h-1)) / src->h;
	    i = (dst_row * src->h - src_row * dst->h ) % dst->h;
	    if (i < 0) i += dst->h;
	}

	if (dstrect) { /*returnvalue*/
	    dstrect->y = dst_row;
	    dstrect->x = dest_x;
	    dstrect->w = dest_w;
	}


	while (src_row < srcrect->y + srcrect->h)
	{
	    Uint8 *srcp, *dstp;
	    srcp = (Uint8 *)src->pixels
	        + (src_row*src->pitch)
		+ (srcrect->x*bpp);
	draw:
	    dstp = (Uint8 *)dst->pixels
	        + (dst_row*dst->pitch)
		+ (dest_x*bpp);
#          ifdef SDL_STRETCH_USE_ASM
	    if (code)
	    {
#               ifdef SDL_STRETCH_CALL
                SDL_STRETCH_CALL(code, srcp, dstp);
#               else
                SDL_RunRowStretchCode(code, srcp, dstp);
#               endif
	    }else
#          endif /*USE_ASM*/
	    {
		switch (bpp) {
		case 1:
		    SDL_StretchRow1(srcp, srcrect->w, dstp, dest_w);
		    break;
		case 2:
		    SDL_StretchRow2((Uint16 *)srcp, srcrect->w,
				    (Uint16 *)dstp, dest_w);
		    break;
		case 3:
		    SDL_StretchRow3(srcp, srcrect->w, dstp, dest_w);
		    break;
		case 4:
		    SDL_StretchRow4((Uint32 *)srcp, srcrect->w,
				    (Uint32 *)dstp, dest_w);
		    break;
		}
	    }

	    dst_row++; i += src->h;
	    if (dst_row == dst->h) break; /*oops*/
	    if (i < dst->h) goto draw; /* draw the line again */
	    do { i -= dst->h; src_row++; } while (i >= dst->h);
	}

	if (dstrect) dstrect->h = dst_row - dstrect->y; /*returnvalue*/

	return(0);
}

void circle (int xc, int yc, int r, Punto *puntos, int vel, int dir) {
	int p;
	int x, y, e;
	Punto temp[250];
	
	p = 1 - r;
	x = 0;
	y = r;
	e = 1;
	
	temp[0].x = x;
	temp[0].y = y;
	
	while (x < y) {
		if (p < 0) {
			x++;
			
			p = p + (2 * x) + 1;
		} else {
			x++;
			y--;
			p = p + (2 * x) + 1 - (2 * y);
		}
		
		temp[e].x = x;
		temp[e++].y = y;
	}
	
	x = vel / 8;
	r = 0;
	//printf ("Total de puntos del circulo %i, yo voy a tomar: %i\n", e, x);
	//printf ("circulo, recogiendo puntos\n");
	/* Recoger los puntos */
	for (y = 0; y < e; y++) {
		p = roundf (((float) (e * r)) / ((float) x));
		//printf ("Punto: %i\n", y);
		if (p == y) {
			//printf ("Tomo este punto y lo pongo en R: %i\n", r);
			//printf ("Y en %i, %i, %i, %i, %i, %i, %i\n", (2 * x) - r, (2 * x) + r, (4 * x) - r, (4 * x) + r, (6 * x) - r, (6 * x) + r, (8 * x) - r);
			/* Tomar este punto */
			puntos[r].x = puntos[(4 * x) - r].x = xc + temp[y].x;
			puntos[r].y = puntos[(8 * x) - r].y = yc + temp[y].y;
		
			puntos[(2 * x) - r].x = puntos[(2 * x) + r].x = xc + temp[y].y;
			puntos[(2 * x) - r].y = puntos[(6 * x) + r].y = yc + temp[y].x;
		
			puntos[(2 * x) + r].y = puntos[(6 * x) - r].y = yc - temp[y].x;
			puntos[(4 * x) - r].y = puntos[(4 * x) + r].y = yc - temp[y].y;
		
			puntos[(4 * x) + r].x = puntos[(8 * x) - r].x = xc - temp[y].x;
			puntos[(6 * x) - r].x = puntos[(6 * x) + r].x = xc - temp[y].y;
		
			r++;
		}
	}

	puntos[x].x = puntos[x * 3].x = xc + temp[e - 1].x;
	puntos[x].y = puntos[x * 7].y = yc + temp[e - 1].y;
	puntos[x * 3].y = puntos[x * 5].y = yc - temp[e - 1].y;
	puntos[x * 5].x = puntos[x * 7].x = xc - temp[e - 1].x;
	
	if (dir == 0) {
		for (y = 0; y < vel / 2; y++) {
			temp[e] = puntos[y];
			puntos[y] = puntos[vel - y - 1];
			puntos[vel - y - 1] = temp[e];
		}
	}
}
