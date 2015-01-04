# Automake file for Astro Barrier

gamedatadir = $(pkgdatadir)/data

bin_PROGRAMS = astro-barrier
astro_barrier_SOURCES = astro.c util-bresenham.c sdlstretchcode.c astro-lua.c cp-button.c
EXTRA_DIST = util-bresenham.h SDL_stretchasm.h SDL_stretchcode.h astro-lua.h astro-types.h cp-button.h

astro_barrier_CPPFLAGS = -DGAMEDATA_DIR=\"$(gamedatadir)/\" -DLOCALEDIR=\"$(localedir)\" $(AM_CPPFLAGS)
astro_barrier_CFLAGS = $(SDL_CFLAGS) $(SDL_image_CFLAGS) $(SDL_mixer_CFLAGS) $(SDL_ttf_CFLAGS) $(LUA_CFLAGS) $(CPSTAMP_CFLAGS) $(AM_CFLAGS)
astro_barrier_LDADD = $(SDL_LIBS) $(SDL_image_LIBS) $(SDL_mixer_LIBS) $(SDL_ttf_LIBS) $(LUA_LIBS) $(CPSTAMP_LIBS)

