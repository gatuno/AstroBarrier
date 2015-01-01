/*
 * astro-lua.h
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

#include "astro-types.h"

#ifndef __ASTRO_LUA_H__
#define __ASTRO_LUA_H__

void lua_astro_call_ship_move (void);
void lua_astro_call_target_hit (Target *);

int leer_nivel (int, AstroStatus *);
void leer_pack (const char *, int *, int *);

#endif /* __ASTRO_LUA_H__ */

