#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.63])
AC_INIT(wombattraining, 0.1.5, hubert.mis@gmail.com)
AC_CONFIG_SRCDIR([src/bonus.h])
AM_INIT_AUTOMAKE
AC_CONFIG_HEADER([config.h])

# Checks for programs.
AC_PROG_CC
AC_PROG_INSTALL

# Checks for libraries.

# Checks for header files.
AC_CHECK_HEADERS([stdlib.h])
AC_CHECK_HEADERS([stdio.h])
AC_CHECK_HEADERS([ctype.h])
AC_CHECK_HEADERS([time.h])

# Checks for typedefs, structures, and compiler characteristics.

# Checks for library functions.
AC_FUNC_MALLOC
AC_FUNC_REALLOC
AC_CHECK_FUNCS([mkdir])
AC_CHECK_FUNCS([strstr])
AC_CHECK_FUNCS([strcspn])
AC_CHECK_FUNCS([strchr])
AC_CHECK_FUNCS([strlen])
AC_CHECK_FUNCS([atexit])
AC_CHECK_FUNCS([srand])
AC_CHECK_FUNCS([rand])

AC_CHECK_LIB(  m, main, LIBS="$LIBS -lm" )

SDL_VERSION=1.2.0
AM_PATH_SDL($SDL_VERSION, :, AC_MSG_ERROR([*** SDL version $SDL_VERSION not found!]))
CFLAGS="$CFLAGS $SDL_CFLAGS"
LIBS="$LIBS $SDL_LIBS"

AC_CHECK_HEADER( SDL_opengl.h, LIBS="$LIBS -lGL -lGLU", AC_MSG_ERROR([*** SDL openGL support not found!]))

FT_VERSION=2.1.0
AC_CHECK_FT2($FT_VERSION, :, AC_MSG_ERROR([*** Freetype2 version $FT_VERSION not found!]))
CFLAGS="$CFLAGS $FT2_CFLAGS"
LIBS="$LIBS $FT2_LIBS"

AC_CONFIG_FILES([Makefile src/Makefile conf/Makefile pixmaps/Makefile])
AC_OUTPUT
