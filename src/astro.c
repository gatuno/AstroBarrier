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

#define FPS (1000/24)

#define RANDOM(x) ((int) (x ## .0 * rand () / (RAND_MAX + 1.0)))

#define GAME_AREA_X 250
#define GAME_AREA_Y 79

const char *images_names[NUM_IMAGES] = {
	GAMEDATA_DIR "images/arcade.png",
	
	GAMEDATA_DIR "images/astro.png",
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

#define BASE_LEVEL_PACK GAMEDATA_DIR "levels/astro_nivel.astro"

/* Prototipos de función */
int game_loop (void);
void setup (void);
SDL_Surface * set_video_mode(unsigned);

/* Variables globales */
SDL_Surface * screen;
SDL_Surface * images [NUM_IMAGES];

int total_niveles;
int nivel_actual;

int main (int argc, char *argv[]) {
	
	setup ();
	
	/* TODO: Seleccionar un paquete de niveles */
	leer_pack ("levels/astro_pack.astro", &total_niveles, &nivel_actual);
	
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
	AstroStatus astro;
	
	int astro_dir = 0;
	SDL_Surface *game_buffer, *stretch_buffer;
	int shooting, space_toggle, enter_toggle, switch_toggle, astro_destroyed;
	int turret_shooting, turret_dir;
	int switch_timer;
	SDL_Rect shoot_rect, turret_shoot_rect;
	
	int contador_hits, refresh_tiros;
	int pantalla_abierta = 0, timer_pantalla = 0;
	
	int *has_turret = &(astro.has_turret);
	Target *targets = astro.targets;
	Linea *lineas = astro.lineas;
	Bloque *bloques = astro.bloques;
	
	astro_destroyed = shooting = space_toggle = enter_toggle = switch_toggle = turret_shooting = FALSE;
	
	nivel_actual = 1;
	
	astro.astro_rect.w = images[IMG_ASTRO]->w;
	astro.astro_rect.h = images[IMG_ASTRO]->h;
	astro.astro_rect.x = 160;
	astro.astro_rect.y = 396;
	
	if (!leer_nivel (nivel_actual, &astro)) {
		return GAME_QUIT;
	}
	
	if (*has_turret != -1) {
		turret_dir = targets[*has_turret].image - IMG_TURRET_1;
		turret_shoot_rect.w = images[IMG_TURRET_SHOOT_1 + turret_dir]->w;
		turret_shoot_rect.h = images[IMG_TURRET_SHOOT_1 + turret_dir]->h;
	}
	
	contador_hits = 0;
	refresh_tiros = TRUE;
	shoot_rect.w = images[IMG_SHOOT]->w;
	shoot_rect.h = images[IMG_SHOOT]->h;
	
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
			shoot_rect.x = astro.astro_rect.x + 22;
			shoot_rect.y = astro.astro_rect.y + 8;
			astro.tiros--;
			refresh_tiros = TRUE;
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
							/* Sumar 5  de score */
							/* TODO: Reproducir sonido */
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
								/* Para los verdes, sumar 10 puntos de score */
								/* TODO: Reproducir sonido */
								if (targets[g].on_hit != NULL) {
									lua_astro_call_target_hit (&targets[g]);
								}
								break;
							case IMG_TARGET_NORMAL_BLUE:
							case IMG_TARGET_MINI_BLUE:
							case IMG_TARGET_BIG_BLUE:
								if (contador_hits == 0) {
									/* Golpearon la vida primero */
									printf ("Ganaste 1 vida\n");
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
								break;
							case IMG_TARGET_NORMAL_YELLOW:
							case IMG_TARGET_MINI_YELLOW:
							case IMG_TARGET_BIG_YELLOW:
								targets[g].image--; /* Cambiar a verde */
								contador_hits++;
								/* Para los amarillos, sumar 25 puntos de score */
								/* TODO: Reproducir sonido */
								break;
							case IMG_TARGET_EXPAND_1:
								targets[g].golpeado = TRUE;
								targets[g].animar = FALSE;
								contador_hits++;
								/* El cambio de color ocurre abajo */
								/* Sumar 25 de score */
								/* TODO: Reproducir sonido */
								if (targets[g].on_hit != NULL) {
									lua_astro_call_target_hit (&targets[g]);
								}
								break;
							case IMG_TARGET_MINI_LIGHTGREEN:
								targets[g].golpeado = TRUE;
								targets[g].animar = FALSE;
								targets[g].image++; /* Cambiar a naranja */
								contador_hits++;
								/* Para los verdes, sumar 10 puntos de score */
								/* TODO: Reproducir sonido */
								if (targets[g].on_hit != NULL) {
									lua_astro_call_target_hit (&targets[g]);
								}
								break;
							case IMG_TARGET_500_GREEN:
								targets[g].golpeado = TRUE;
								targets[g].animar = FALSE;
								targets[g].image++; /* Cambiar a rojo */
								contador_hits++;
								
								/* Sumar 500 puntos */
								/* TODO: Reproducir sonido */
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
								/* Para los verdes, sumar 10 puntos de score */
								/* TODO: Reproducir sonido */
								if (targets[g].on_hit != NULL) {
									lua_astro_call_target_hit (&targets[g]);
								}
								break;
							case IMG_TARGET_NORMAL_BLUE:
							case IMG_TARGET_MINI_BLUE:
							case IMG_TARGET_BIG_BLUE:
								if (contador_hits == 0) {
									/* Golpearon la vida primero */
									printf ("Ganaste 1 vida por turret\n");
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
								break;
							case IMG_TARGET_NORMAL_YELLOW:
							case IMG_TARGET_MINI_YELLOW:
							case IMG_TARGET_BIG_YELLOW:
								targets[g].image -= 1; /* Cambiar a verde */
								contador_hits++;
								/* Para los amarillos, sumar 25 puntos de score */
								/* TODO: Reproducir sonido */
								break;
							case IMG_TARGET_EXPAND_1:
								targets[g].golpeado = TRUE;
								targets[g].animar = FALSE;
								contador_hits++;
								/* El cambio de color ocurre abajo */
								/* Sumar 25 de score */
								/* TODO: Reproducir sonido */
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
				}
			}
			
			/* Colisión contra la nave */
			if (SDL_HasIntersection (&astro.astro_rect, &turret_shoot_rect)) {
				/* Nave destruida */
				pantalla_abierta = SCREEN_RESTART;
				shooting = FALSE;
				turret_shooting = FALSE;
				astro_destroyed = TRUE;
			}
		} /* Si está tirando el turret*/
		
		if (!pantalla_abierta && contador_hits >= astro.hits_requeridos) {
			/* Nivel terminado, hay que abrir la pantalla de siguiente nivel */
			pantalla_abierta = SCREEN_NEXT_LEVEL;
			astro.tiros = 0;
			refresh_tiros = TRUE;
			printf ("Debug: Ganaste, siguiente nivel\n");
		} else if (!pantalla_abierta && astro.tiros == 0 && !shooting) {
			/* Se acabaron los tiros, reiniciar el nivel */
			pantalla_abierta = SCREEN_RESTART;
			printf ("Debug: Tiros acabados, reiniciando nivel\n");
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
			rect.w = 144;
			rect.h = 36;
			SDL_BlitSurface (images[IMG_ARCADE], &rect, screen, &rect);
			
			/* Borrar la parte de abajo de los tiros */
			rect.x = 0;
			rect.y = 436;
			rect.w = 205;
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
		
		SDL_Flip (screen);
		
		if (astro.blue.pack != 0 && astro.blue.timer >= 710) {
			nivel_actual = 1 | (astro.blue.pack << 17);
			
			if (!leer_nivel (nivel_actual, &astro)) {
				/* Falló al cargar el nivel */
				return GAME_QUIT;
			}
			switch_toggle = FALSE;
			contador_hits = 0;
			refresh_tiros = TRUE;
			timer_pantalla = 0;
		}
		
		if (pantalla_abierta && (timer_pantalla > 56 || enter_toggle)) {
			if (pantalla_abierta == SCREEN_RESTART) {
				/* Ocultar y reiniciar el nivel */
				astro_destroyed = FALSE;
				if (!leer_nivel (nivel_actual, &astro)) {
					/* Falló al cargar el nivel */
					return GAME_QUIT;
				}
				if (*has_turret != -1) {
					turret_dir = targets[*has_turret].image - IMG_TURRET_1;
					turret_shoot_rect.w = images[IMG_TURRET_SHOOT_1 + turret_dir]->w;
					turret_shoot_rect.h = images[IMG_TURRET_SHOOT_1 + turret_dir]->h;
				}
				switch_toggle = FALSE;
				contador_hits = 0;
				refresh_tiros = TRUE;
				/* TODO: nivel_reiniciado = TRUE */
				pantalla_abierta = SCREEN_NONE;
				/* TODO: reiniciar el score */
			} else if (pantalla_abierta == SCREEN_NEXT_LEVEL) {
				/* Pasar al siguiente nivel */
				if (astro.next_level != -1) {
					nivel_actual = astro.next_level;
					if (!leer_nivel (nivel_actual, &astro)) {
						return GAME_QUIT;
					}
					if (*has_turret != -1) {
						turret_dir = targets[*has_turret].image - IMG_TURRET_1;
						turret_shoot_rect.w = images[IMG_TURRET_SHOOT_1 + turret_dir]->w;
						turret_shoot_rect.h = images[IMG_TURRET_SHOOT_1 + turret_dir]->h;
					}
					switch_toggle = FALSE;
					if (nivel_actual & (1 << 16)) {
						printf ("Estoy cargando un nivel de explicación\n");
					}
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
			if (astro.next_level != -1) {
				nivel_actual = astro.next_level;
				if (!leer_nivel (nivel_actual, &astro)) {
					return GAME_QUIT;
				}
				if (*has_turret != -1) {
					turret_dir = targets[*has_turret].image - IMG_TURRET_1;
					turret_shoot_rect.w = images[IMG_TURRET_SHOOT_1 + turret_dir]->w;
					turret_shoot_rect.h = images[IMG_TURRET_SHOOT_1 + turret_dir]->h;
				}
				
				shooting = turret_shooting = FALSE;
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
	
	/* La nave color azul tiene transparencia especial */
	SDL_SetColorKey (images[IMG_ASTRO_BLUE], SDL_SRCCOLORKEY, SDL_MapRGB (images[IMG_ASTRO_BLUE]->format, 0xFF, 0, 0xFF));
	
	srand (SDL_GetTicks ());
}

