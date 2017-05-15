AC_INIT([Astro Barrier], [0.9.0], [], [astro-barrier])

AC_REVISION([Revision 1])

AC_CONFIG_AUX_DIR([build-aux])

AC_CONFIG_SRCDIR([src/astro.c])

# For debian systems, /usr as default
AC_PREFIX_DEFAULT([/usr])

# We need Gcc
AC_PROG_CC

# and automake
AM_INIT_AUTOMAKE([-Wall -Werror])

# Translate this program
AM_GNU_GETTEXT_VERSION([0.19.3])
AM_GNU_GETTEXT([external])

AM_PROG_CC_C_O

# Check for pkg-config
PKG_PROG_PKG_CONFIG

SDL_VERSION=1.2.14
SDL_IMAGE_VERSION=1.2.10
SDL_TTF_VERSION=2.0.11
SDL_MIXER_VERSION=1.2.12
LUA_VERSION=5.1.5
CPSTAMP_VERSION=0.9.1

AC_MSG_CHECKING([if you have SDL installed on your system])
PKG_CHECK_EXISTS([sdl >= $SDL_VERSION], [AC_MSG_RESULT([yes])], [AC_MSG_FAILURE([SDL not found in your system])])
PKG_CHECK_MODULES(SDL, [sdl >= $SDL_VERSION], [], [])

AC_MSG_CHECKING([if you have SDL_image installed on your system])
PKG_CHECK_EXISTS([SDL_image >= $SDL_IMAGE_VERSION], [AC_MSG_RESULT([yes])], [AC_MSG_FAILURE([SDL_image not found in your system])])
PKG_CHECK_MODULES(SDL_image, [SDL_image >= $SDL_IMAGE_VERSION], [], [])

AC_MSG_CHECKING([if you have SDL_ttf installed on your system])
PKG_CHECK_EXISTS([SDL_ttf >= $SDL_TTF_VERSION], [AC_MSG_RESULT([yes])], [AC_MSG_FAILURE([SDL_ttf not found in your system])])
PKG_CHECK_MODULES(SDL_ttf, [SDL_ttf >= $SDL_TTF_VERSION], [], [])

AC_MSG_CHECKING([if you have SDL_mixer installed on your system])
PKG_CHECK_EXISTS([SDL_mixer >= $SDL_MIXER_VERSION], [AC_MSG_RESULT([yes])], [AC_MSG_FAILURE([SDL_mixer not found in your system])])
PKG_CHECK_MODULES(SDL_mixer, [SDL_mixer >= $SDL_MIXER_VERSION], [], [])

AC_MSG_CHECKING([if you have Club Penguin library Stamp installed on your system])
PKG_CHECK_EXISTS([cpstamp >= $CPSTAMP_VERSION], [AC_MSG_RESULT([yes])], [AC_MSG_FAILURE([Club Penguin library Stamp not found in your system])])
PKG_CHECK_MODULES(CPSTAMP, [cpstamp >= $CPSTAMP_VERSION], [], [])

AC_CONFIG_HEADERS([config.h])

AC_CHECK_TOOL(WINDRES, windres)

# Revisar el host
AC_CANONICAL_HOST

case $host_os in
  *mingw32* ) MINGW32=yes;;
          * ) MINGW32=no;;
esac
case $host_os in
  *cygwin* ) CYGWIN=yes;;
         * ) CYGWIN=no;;
esac
case $host_os in
  *linux* ) LINUX=yes;;
        * ) LINUX=no;;
esac
case $host_os in
 *darwin* ) MACOSX=yes;;
       * ) MACOSX=no;;
esac

AM_CONDITIONAL(MINGW32, test x$MINGW32 = xyes)
AM_CONDITIONAL(LINUX, test x$LINUX = xyes)

if test "$MINGW32" = "yes"; then
	# En win32, sólo hay lua, sin version.
	AC_MSG_CHECKING([if you have Lua installed on your system])
	PKG_CHECK_EXISTS([lua >= $LUA_VERSION], [AC_MSG_RESULT([yes])], [AC_MSG_FAILURE([Lua not found in your system])])
	PKG_CHECK_MODULES(LUA, [lua >= $LUA_VERSION], [], [])
else
	# Para Linux, buscar la lua5.1
	AC_MSG_CHECKING([if you have Lua 5.1 installed on your system])
	PKG_CHECK_EXISTS([lua5.1 >= $LUA_VERSION], [AC_MSG_RESULT([yes])], [AC_MSG_FAILURE([Lua 5.1 not found in your system])])
	PKG_CHECK_MODULES(LUA, [lua5.1 >= $LUA_VERSION], [], [])
fi

AC_CONFIG_FILES([
                 Makefile
                 src/Makefile
                 data/Makefile
                 po/Makefile.in
])

AC_OUTPUT
