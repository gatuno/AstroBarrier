/*
 * astro-lua.c
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

#include <stdlib.h>
#include <stdio.h>

#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>

#include <SDL.h>

#include "astro-lua.h"
#include "astro-types.h"

#include "util-bresenham.h"

/* Variables externas */
extern SDL_Surface * images[];

/* Variables privadas */
AstroStatus *lua_astro_game;
lua_State *L = NULL;

const char *lua_consts[NUM_IMAGES] = {
	/* Se omiten los primeros 7 por ser irrelevantes para los scripts */
	"", "", "", "", "", "", "", "",
	"IMG_ASTRO_BLUE",
	"", "", "", "",
	"IMG_TARGET_NORMAL_BLUE",
	"IMG_TARGET_NORMAL_GREEN",
	"IMG_TARGET_NORMAL_YELLOW",
	"",
	"IMG_TARGET_MINI_BLUE",
	"IMG_TARGET_MINI_GREEN",
	"IMG_TARGET_MINI_YELLOW",
	"",
	"IMG_TARGET_BIG_BLUE",
	"IMG_TARGET_BIG_GREEN",
	"IMG_TARGET_BIG_YELLOW",
	"",
	"IMG_TARGET_MINI_LIGHTGREEN",
	"",
	"IMG_TARGET_EXPAND",
	"", "", "", "", "",
	"IMG_TARGET_500", "",
	"IMG_BLOCK_NORMAL",
	"IMG_BLOCK_MINI",
	"IMG_BLOCK_BIG",
	"IMG_BLOCK_TALL",
	
	"IMG_BLOCK_ORANGE",
	"IMG_BLOCK_ORANGE_MINI",
	
	"IMG_LINE1",
	"IMG_LINE2_A",
	"IMG_LINE2_B",
	"IMG_LINE3",
	"IMG_LINE4_A",
	"IMG_LINE4_B",
	"IMG_LINE5",
	"IMG_LINE6_A",
	"IMG_LINE6_B",
	"IMG_LINE6_C",
	"IMG_LINE7",
	"IMG_LINE8",
	"IMG_LINE9",
	"IMG_LINE10",
	"IMG_LINE11_A",
	"IMG_LINE11_B",
	"IMG_LINE12_A",
	"IMG_LINE12_B",
	"IMG_LINE13",
	"IMG_LINE14",
	"IMG_LINE15",
	"IMG_LINE16_A",
	"IMG_LINE16_B",
	"IMG_LINE17",
	"IMG_LINE18_A",
	"IMG_LINE18_B",
	"IMG_LINE18_C",
	"IMG_LINE18_D",
	"IMG_LINE19_A",
	"IMG_LINE19_B",
	"IMG_LINE19_C",
	"IMG_LINE19_D",
	"IMG_LINE20",
	"IMG_LINE21",
	"IMG_LINE22_A",
	"IMG_LINE22_B",
	"IMG_LINE23_A",
	"IMG_LINE23_B",
	"IMG_LINE24",
	"IMG_LINE25_A",
	"IMG_LINE25_B",
	"IMG_LINE26_A",
	"IMG_LINE26_B",
	"IMG_LINE27",
	"IMG_LINE28",
	"IMG_LINE29",
	"IMG_LINE30",
	"IMG_LINE31",
	
	"IMG_TURRET_1",
	"IMG_TURRET_2",
	"IMG_TURRET_3",
	"IMG_TURRET_4",
	
	"", "", "", "", "",
	"IMG_SWITCH_ORANGE",
};

/* Callbacks */
void lua_astro_call_ship_move (void) {
	if (lua_astro_game->on_ship_move == NULL) return;
	/* Empujar los globales */
#if LUA_VERSION_NUM == 501
	lua_pushvalue (L, LUA_GLOBALSINDEX);
#elif LUA_VERSION_NUM == 502
	lua_rawgeti(L, LUA_REGISTRYINDEX, LUA_RIDX_GLOBALS);
#endif
	lua_pushstring (L, lua_astro_game->on_ship_move);
	lua_rawget (L, -2);
	lua_pushinteger (L, lua_astro_game->astro_rect.x);
	lua_pushinteger (L, lua_astro_game->astro_rect.y);
	lua_pcall (L, 2, 0, 0);
	lua_pop (L, 1);
}

void lua_astro_call_target_hit (Target *t) {
	if (t->on_hit == NULL) return;
	
	/* Empujar las globales */
#if LUA_VERSION_NUM == 501
	lua_pushvalue (L, LUA_GLOBALSINDEX);
#elif LUA_VERSION_NUM == 502
	lua_rawgeti(L, LUA_REGISTRYINDEX, LUA_RIDX_GLOBALS);
#endif
	lua_pushstring (L, t->on_hit);
	lua_rawget (L, -2);
	lua_pushlightuserdata (L, (void *)t);
	lua_pcall (L, 1, 0, 0);
	lua_pop (L, 1);
}

static void stackDump (lua_State *L) {
  int i;
  int top = lua_gettop(L);
  for (i = 1; i <= top; i++) {  /* repeat for each level */
    int t = lua_type(L, i);
    switch (t) {

      case LUA_TSTRING:  /* strings */
        printf("'%s'", lua_tostring(L, i));
        break;

      case LUA_TBOOLEAN:  /* booleans */
        printf(lua_toboolean(L, i) ? "true" : "false");
        break;

      case LUA_TNUMBER:  /* numbers */
        printf("%g", lua_tonumber(L, i));
        break;

      default:  /* other values */
        printf("%s", lua_typename(L, t));
        break;

    }
    printf("  ");  /* put a separator */
  }
  printf("\n");  /* end the listing */
}

int lua_astro_ship_get (lua_State *L) {
	const char *prop;
	
	if (lua_isstring (L, 2)) {
		prop = lua_tostring (L, 2);
		
		if (strcmp (prop, "x") == 0) {
			lua_pushinteger (L, lua_astro_game->astro_rect.x);
		} else if (strcmp (prop, "y") == 0) {
			lua_pushinteger (L, lua_astro_game->astro_rect.y);
		}
		return 1;
	}
	
	lua_pushnil (L);
	return 1;
}

int lua_astro_ship_set (lua_State *L) {
	const char *prop;
	
	if (lua_isstring (L, 2)) {
		prop = lua_tostring (L, 2);
		
		if (strcmp (prop, "on_move") == 0) {
			if (luaL_checkstring (L, -1)) {
				lua_astro_game->on_ship_move = lua_tostring (L, -1);
			}
		}
	}
	
	return 0;
}

int lua_astro_level_set (lua_State *L) {
	const char *prop;
	
	if (lua_isstring (L, 2)) {
		prop = lua_tostring (L, 2);
		
		if (strcmp (prop, "next_level") == 0) {
			lua_astro_game->next_level = luaL_checkint (L, 3);
		} else if (strcmp (prop, "shoots") == 0) {
			lua_astro_game->tiros = luaL_checkint (L, 3);
		} else if (strcmp (prop, "hits_required") == 0) {
			lua_astro_game->hits_requeridos = luaL_checkint (L, 3);
		} else if (strcmp (prop, "explanation") == 0) {
			lua_astro_game->explicacion = luaL_checkint (L, 3);
		} else if (strcmp (prop, "secret_pack") == 0) {
			lua_astro_game->secret_pack = luaL_checkint (L, 3);
		}
	}
	
	return 0;
}

int lua_astro_new_secret (lua_State *L) {
	int pack = luaL_checkint (L, -3);
	int x = luaL_checkint (L, -2);
	int y = luaL_checkint (L, -1);
	
	lua_astro_game->blue.pack = pack;
	lua_astro_game->blue.timer = 0;
	lua_astro_game->blue.rect.x = x;
	lua_astro_game->blue.rect.y = y;
	lua_astro_game->blue.rect.w = images[IMG_ASTRO_BLUE]->w;
	lua_astro_game->blue.rect.h = images[IMG_ASTRO_BLUE]->h;
	
	return 0;
}

int lua_astro_targets_new_target (lua_State *L) {
	int image = luaL_checkint (L, 1);
	int p = lua_astro_game->n_targets;
	
	lua_astro_game->targets[p].image = image;
	lua_astro_game->targets[p].rect.w = images[image]->w;
	lua_astro_game->targets[p].rect.h = images[image]->h;
	lua_astro_game->targets[p].pos = 0;
	lua_astro_game->targets[p].golpeado = FALSE;
	lua_astro_game->targets[p].animar = TRUE;
	lua_astro_game->targets[p].total_vel = 0;
	lua_astro_game->targets[p].on_hit = NULL;
	lua_pushlightuserdata (L, (void *)&lua_astro_game->targets[p]);
	
	lua_astro_game->n_targets++;
	
	if (image >= IMG_TURRET_1 && image <= IMG_TURRET_4) {
		lua_astro_game->has_turret = p;
	}
	
	luaL_getmetatable (L, "astro.target");
	lua_setmetatable (L, -2);
	
	return 1;
}

int lua_astro_targets_get_target (lua_State *L) {
	int index = luaL_checkint (L, -1);
	luaL_argcheck (L, 0 <= index && index < lua_astro_game->n_targets, 2, "index out of range");
	lua_pushlightuserdata (L, (void *)&lua_astro_game->targets[index]);
	
	luaL_getmetatable (L, "astro.target");
	lua_setmetatable (L, -2);
	return 1;
}

int lua_astro_target_add_point (lua_State *L) {
	int x, y;
	
	Target *t = luaL_checkudata (L, 1, "astro.target");
	x = luaL_checkint (L, 2);
	y = luaL_checkint (L, 3);
	
	t->puntos[t->total_vel].x = x;
	t->puntos[t->total_vel].y = y;
	t->total_vel++;
	
	return 0;
}

int lua_astro_target_trace_line (lua_State *L) {
	int x, y, vel;
	int last_p;
	
	Target *t = luaL_checkudata (L, 1, "astro.target");
	
	x = luaL_checkint (L, 2);
	y = luaL_checkint (L, 3);
	vel = luaL_checkint (L, 4);
	last_p = t->total_vel - 1;
	line (t->puntos[last_p].x, t->puntos[last_p].y, x, y, &t->puntos[last_p], vel);
	t->total_vel += vel;
	
	t->puntos[t->total_vel - 1].x = x;
	t->puntos[t->total_vel - 1].y = y;
	
	return 0;
}

int lua_astro_target_close_line (lua_State *L) {
	int vel;
	int last_p;
	
	Target *t = luaL_checkudata (L, 1, "astro.target");
	
	vel = luaL_checkint (L, 2);
	last_p = t->total_vel - 1;
	line (t->puntos[last_p].x, t->puntos[last_p].y, t->puntos[0].x, t->puntos[0].y, &t->puntos[last_p], vel);
	
	t->total_vel += vel - 1;
	
	return 0;
}

int lua_astro_target_trace_circle (lua_State *L) {
	int x, y, radio, vel, dir;
	
	Target *t = luaL_checkudata (L, -6, "astro.target");
	
	x = luaL_checkint (L, -5);
	y = luaL_checkint (L, -4);
	radio = luaL_checkint (L, -3);
	vel = luaL_checkint (L, -2);
	luaL_checktype (L, -1, LUA_TBOOLEAN);
	dir = lua_toboolean (L, -1);
	
	circle (x, y, radio, &t->puntos[t->total_vel], vel, dir);
	
	t->total_vel += vel;
	
	return 0;
}

int lua_astro_target_on_hit (lua_State *L) {
	Target *t = luaL_checkudata (L, 1, "astro.target");
	t->on_hit = luaL_checkstring (L, 2);
	
	return 0;
}

int lua_astro_target_set_animar (lua_State *L) {
	Target *t = luaL_checkudata (L, 1, "astro.target");
	
	luaL_checktype (L, -1, LUA_TBOOLEAN);
	
	t->animar = lua_toboolean (L, -1);
	return 0;
}

int lua_astro_target_get_animar (lua_State *L) {
	Target *t = luaL_checkudata (L, 1, "astro.target");
	
	lua_pushboolean (L, t->animar);
	
	return 1;
}

int lua_astro_target_set_pos (lua_State *L) {
	Target *t = luaL_checkudata (L, 1, "astro.target");
	
	t->pos = luaL_checkint (L, -1);
	
	return 0;
}

int lua_astro_target_get_pos (lua_State *L) {
	Target *t = luaL_checkudata (L, 1, "astro.target");
	
	lua_pushinteger (L, t->pos);
	
	return 1;
}

int lua_astro_target_get_stop (lua_State *L) {
	Target *t = luaL_checkudata (L, 1, "astro.target");
	
	lua_pushboolean (L, t->golpeado);
	
	return 1;
}

int lua_astro_target_clear_points (lua_State *L) {
	Target *t = luaL_checkudata (L, 1, "astro.target");
	
	t->total_vel = 0;
	
	return 0;
}

int lua_astro_lines_new_line (lua_State *L) {
	int image = luaL_checkint (L, 1);
	int x = luaL_checkint (L, 2);
	int y = luaL_checkint (L, 3);
	
	int p = lua_astro_game->n_lineas;
	lua_astro_game->lineas[p].image = image;
	lua_astro_game->lineas[p].rect.x = x;
	lua_astro_game->lineas[p].rect.y = y;
	lua_astro_game->lineas[p].rect.w = images[image]->w;
	lua_astro_game->lineas[p].rect.h = images[image]->h;
	lua_pushlightuserdata (L, (void *)&lua_astro_game->lineas[p]);
	
	lua_astro_game->n_lineas++;
	
	luaL_getmetatable (L, "astro.line");
	lua_setmetatable (L, -2);
	
	return 1;
}

int lua_astro_lines_get_line (lua_State *L) {
	int index = luaL_checkint (L, 2);
	luaL_argcheck (L, 0 <= index && index < lua_astro_game->n_lineas, 2, "index out of range");
	lua_pushlightuserdata (L, (void *)&lua_astro_game->lineas[index]);
	
	luaL_getmetatable (L, "astro.line");
	lua_setmetatable (L, -2);
	
	return 1;
}

int lua_astro_line_move (lua_State *L) {
	Linea *l = luaL_checkudata (L, 1, "astro.line");
	int x = luaL_checkint (L, 2);
	int y = luaL_checkint (L, 3);
	
	l->rect.x = x;
	l->rect.y = y;
	
	return 0;
}

int lua_astro_blocks_new_block (lua_State *L) {
	int image = luaL_checkint (L, 1);
	int x = luaL_checkint (L, 2);
	int y = luaL_checkint (L, 3);
	
	int p = lua_astro_game->n_bloques;
	lua_astro_game->bloques[p].image = image;
	lua_astro_game->bloques[p].rect.x = x;
	lua_astro_game->bloques[p].rect.y = y;
	lua_astro_game->bloques[p].rect.w = images[image]->w;
	lua_astro_game->bloques[p].rect.h = images[image]->h;
	lua_pushlightuserdata (L, (void *)&lua_astro_game->bloques[p]);
	
	lua_astro_game->n_bloques++;
	
	luaL_getmetatable (L, "astro.block");
	lua_setmetatable (L, -2);
	
	return 1;
}

int lua_astro_blocks_get_block (lua_State *L) {
	int index = luaL_checkint (L, 2);
	luaL_argcheck (L, 0 <= index && index < lua_astro_game->n_bloques, 2, "index out of range");
	lua_pushlightuserdata (L, (void *)&lua_astro_game->bloques[index]);
	
	luaL_getmetatable (L, "astro.block");
	lua_setmetatable (L, -2);
	
	return 1;
}

int lua_astro_block_move (lua_State *L) {
	Linea *l = luaL_checkudata (L, 1, "astro.block");
	int x = luaL_checkint (L, 2);
	int y = luaL_checkint (L, 3);
	
	l->rect.x = x;
	l->rect.y = y;
	
	return 0;
}

void lua_astro_set_constantes (lua_State *L) {
	char bufferconst [256];
	int g;
	
	for (g = 0; g <= IMG_SWITCH_ORANGE; g++) {
		if (lua_consts[g][0] == '\0') continue;
		sprintf (bufferconst, "%s=%i", lua_consts[g], g);
		luaL_dostring (L, bufferconst);
	}
}

int leer_nivel (const char *pack, int level, AstroStatus *astro) {
	char archivo[2048]; /* Hacer máx de esto */
	int g;
	
	int explicacion, secreto, numero;
	
	numero = level & 0xFF;
	explicacion = (level >> 16) & 0x01;
	secreto = (level >> 17) & 0x07;
	
	astro->has_turret = -1;
	astro->blue.pack = 0;
	astro->blue.timer = 0;
	astro->on_ship_move = NULL;
	
	if (explicacion ){
		if (secreto != 0) {
			sprintf (archivo, "%s/astro_nivel_s-%i_e-%i.astro", pack, secreto, numero);
		} else {
			sprintf (archivo, "%s/astro_nivel_e-%i.astro", pack, numero);
		}
	} else {
		if (secreto != 0) {
			sprintf (archivo, "%s/astro_nivel_s-%i_%i.astro", pack, secreto, numero);
		} else {
			sprintf (archivo, "%s/astro_nivel_%i.astro", pack, numero);
		}
	}
	
	/* Quiero un ambiente limpio */
	if (L != NULL) {
		lua_close (L);
	}
	
	/* Para acceso global */
	lua_astro_game = astro;
	
	L = luaL_newstate ();
	luaL_openlibs (L);
	
	luaL_dostring (L, "math.round = function (num, idp)\nlocal mult = 10^(idp or 0)\nreturn math.floor(num * mult + 0.5) / mult\nend");
	
	/* Empujar los globales */
#if LUA_VERSION_NUM == 501
	lua_pushvalue (L, LUA_GLOBALSINDEX);
#elif LUA_VERSION_NUM == 502
	lua_rawgeti(L, LUA_REGISTRYINDEX, LUA_RIDX_GLOBALS);
#endif
	/* Una nueva tabla para "Astro" */
	lua_pushstring (L, "astro");
	lua_newtable (L);
	
	/* Stack: Table (Astro) */
	lua_pushstring (L, "ship");
	lua_newtable (L);
	
	luaL_newmetatable (L, "astro.meta.ship");
	lua_pushstring (L, "__index");
	lua_pushcfunction (L, lua_astro_ship_get);
	lua_rawset (L, -3);
	
	lua_pushstring (L, "__newindex");
	lua_pushcfunction (L, lua_astro_ship_set);
	lua_rawset (L, -3);
	
	lua_setmetatable (L, -2);
	
	/* Empujar el espacio de nombres "ship" */
	lua_rawset (L, -3);
	
	/* Stack: Table (Astro) */
	lua_pushstring (L, "secret");
	lua_pushcfunction (L, lua_astro_new_secret);
	lua_rawset (L, -3);
	
	/* Stack: Table (Astro) */
	lua_pushstring (L, "level");
	lua_newtable (L);
	
	luaL_newmetatable (L, "astro.meta.level");
	lua_pushstring (L, "__newindex");
	lua_pushcfunction (L, lua_astro_level_set);
	lua_rawset (L, -3);
	
	/*lua_pushstring (L, "__index");
	lua_pushcfunction (L, lua_astro_level_get);
	lua_rawset (L, -3);*/
	
	lua_setmetatable (L, -2);
	
	/* Empujar el espacio de nombres "level" */
	lua_rawset (L, -3);
	
	/* Limpiar las lineas */
	astro->n_lineas = 0;
	
	/* Metatabla para una linea */
	luaL_newmetatable (L, "astro.line");
	
	lua_pushstring (L, "__index");
	lua_pushvalue (L, -2);
	lua_settable (L, -3);
	
	lua_pushstring (L, "move");
	lua_pushcfunction (L, lua_astro_line_move);
	lua_rawset (L, -3);
	
	lua_pop (L, 1);
	
	/* Crear y recuperar targets */
	lua_pushstring (L, "lines");
	lua_newtable (L);
	
	lua_pushstring (L, "new");
	lua_pushcfunction (L, lua_astro_lines_new_line);
	lua_rawset (L, -3);
	
	lua_pushstring (L, "get");
	lua_pushcfunction (L, lua_astro_lines_get_line);
	lua_rawset (L, -3);
	
	/* Metatabla */
	luaL_newmetatable (L, "astro.meta.lines");
	lua_pushstring (L, "__index");
	lua_pushcfunction (L, lua_astro_lines_get_line);
	lua_rawset (L, -3);
	lua_setmetatable (L, -2);
	
	/* Empujar el espacio de nombres "lines" */
	lua_rawset (L, -3);
	
	/* Limpiar los bloques */
	astro->n_bloques = 0;
	
	/* Metatabla para un bloque */
	luaL_newmetatable (L, "astro.block");
	
	lua_pushstring (L, "__index");
	lua_pushvalue (L, -2);
	lua_settable (L, -3);
	
	lua_pushstring (L, "move");
	lua_pushcfunction (L, lua_astro_block_move);
	lua_rawset (L, -3);
	
	lua_pop (L, 1);
	
	/* Crear y recuperar targets */
	lua_pushstring (L, "blocks");
	lua_newtable (L);
	
	lua_pushstring (L, "new");
	lua_pushcfunction (L, lua_astro_blocks_new_block);
	lua_rawset (L, -3);
	
	lua_pushstring (L, "get");
	lua_pushcfunction (L, lua_astro_blocks_get_block);
	lua_rawset (L, -3);
	
	/* Metatabla */
	luaL_newmetatable (L, "astro.meta.blocks");
	lua_pushstring (L, "__index");
	lua_pushcfunction (L, lua_astro_blocks_get_block);
	lua_rawset (L, -3);
	lua_setmetatable (L, -2);
	
	/* Empujar el espacio de nombres "blocks" */
	lua_rawset (L, -3);
	
	/* Limpiar todos los targets */
	astro->n_targets = 0;
	
	/* Metatabla para un target */
	luaL_newmetatable (L, "astro.target");
	
	lua_pushstring (L, "__index");
	lua_pushvalue (L, -2);
	lua_settable (L, -3);
	
	lua_pushstring (L, "addPoint");
	lua_pushcfunction (L, lua_astro_target_add_point);
	lua_rawset (L, -3);
	
	lua_pushstring (L, "traceLine");
	lua_pushcfunction (L, lua_astro_target_trace_line);
	lua_rawset (L, -3);
	
	lua_pushstring (L, "closeLine");
	lua_pushcfunction (L, lua_astro_target_close_line);
	lua_rawset (L, -3);
	
	lua_pushstring (L, "traceCircle");
	lua_pushcfunction (L, lua_astro_target_trace_circle);
	lua_rawset (L, -3);
	
	lua_pushstring (L, "setAnimate");
	lua_pushcfunction (L, lua_astro_target_set_animar);
	lua_rawset (L, -3);
	
	lua_pushstring (L, "getAnimate");
	lua_pushcfunction (L, lua_astro_target_get_animar);
	lua_rawset (L, -3);
	
	lua_pushstring (L, "setPos");
	lua_pushcfunction (L, lua_astro_target_set_pos);
	lua_rawset (L, -3);
	
	lua_pushstring (L, "getPos");
	lua_pushcfunction (L, lua_astro_target_get_pos);
	lua_rawset (L, -3);
	
	lua_pushstring (L, "onHit");
	lua_pushcfunction (L, lua_astro_target_on_hit);
	lua_rawset (L, -3);
	
	lua_pushstring (L, "getStopped");
	lua_pushcfunction (L, lua_astro_target_get_stop);
	lua_rawset (L, -3);
	
	lua_pushstring (L, "clearPoints");
	lua_pushcfunction (L, lua_astro_target_clear_points);
	lua_rawset (L, -3);
	
	lua_pop (L, 1);
	
	/* Crear y recuperar targets */
	lua_pushstring (L, "targets");
	lua_newtable (L);
	
	lua_pushstring (L, "new");
	lua_pushcfunction (L, lua_astro_targets_new_target);
	lua_rawset (L, -3);
	
	lua_pushstring (L, "get");
	lua_pushcfunction (L, lua_astro_targets_get_target);
	lua_rawset (L, -3);
	
	/* Metatabla */
	luaL_newmetatable (L, "astro.meta.targets");
	lua_pushstring (L, "__index");
	lua_pushcfunction (L, lua_astro_targets_get_target);
	lua_rawset (L, -3);
	lua_setmetatable (L, -2);
	
	/* Empujar el espacio de nombres "targets" */
	lua_rawset (L, -3);
	
	/* Empujar el espacio de nombres "astro" */
	lua_rawset (L, -3);
	
	lua_astro_set_constantes (L);
	
	/* Una vez listo el ambiente, intentar cargar el archivo */
	if (luaL_loadfile (L, archivo)) {
		/* Falló */
		fprintf(stderr, "\nFATAL ERROR:\n  %s: %s\n\n", "luaL_loadfile() failed", lua_tostring(L, -1));
		return FALSE;
	}
	if (lua_pcall (L, 0, 0, 0)) {
		fprintf(stderr, "\nFATAL ERROR:\n  %s: %s\n\n", "lua_pcall() failed", lua_tostring(L, -1));
		return FALSE;
	}
	
	lua_pop (L, 1);
	
	return TRUE;
}

