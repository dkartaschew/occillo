#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.59])
AC_INIT([occillo], [1.0.0], [chewy509@mailcity.com])
AC_CONFIG_HEADERS([config.h])
AC_CONFIG_SRCDIR([src/main.cc])
AC_CONFIG_AUX_DIR([config])
AC_CANONICAL_HOST

AC_SUBST(ARCH,[host_cpu])

AM_INIT_AUTOMAKE([1.10 no-define foreign])
# Checks for programs.
AC_PROG_CXX
AC_LANG([C++])
AC_PROG_INSTALL
AC_PROG_LN_S
AM_GNU_GETTEXT([external])
AM_GNU_GETTEXT_VERSION([0.18.3])
AX_CXX_COMPILE_STDCXX_11([noext],[mandatory])

AM_PATH_CPPUNIT(1.13.0)

AM_CONDITIONAL(HAVE_WINDRES, which windres > /dev/null)

AC_CHECK_PROGS([DOXYGEN], [doxygen])
if test -z "$DOXYGEN";
   then AC_MSG_WARN([Doxygen not found - continuing without Doxygen support])
fi

# Checks for header files.
AC_HEADER_STDC
AC_CHECK_HEADERS([libintl.h locale.h stdlib.h string.h strings.h unistd.h cstdlib string iostream stack set cerrno vector sys/stat.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_TYPE_SIZE_T
AC_CHECK_HEADER_STDBOOL
AC_TYPE_INT32_T
AC_TYPE_INT64_T
AC_TYPE_UINT16_T
AC_TYPE_UINT32_T
AC_TYPE_UINT64_T
AC_TYPE_UINT8_T

# Checks for library functions.
AC_CHECK_FUNCS([strchr])

# Common libraries
PKG_CHECK_MODULES([SDL2], [sdl2])
PKG_CHECK_MODULES([SDL2_image], [SDL2_image])
PKG_CHECK_MODULES([SDL2_ttf], [SDL2_ttf])
PKG_CHECK_MODULES([SDL2_mixer], [SDL2_mixer])
PKG_CHECK_MODULES([cairo], [cairo])
PKG_CHECK_MODULES([cairo_svg], [cairo-svg])
PKG_CHECK_MODULES([librsvg], [librsvg-2.0])

# Stick in "-Werror" if you want to be more aggressive.
# (No need to use AC_SUBST on this default substituted environment variable.)
# Only add these additional CFLAGS if we are using GCC. Other C compilers may
# not support them.
if test x"$GCC" == "xyes" ; then
    AX_CHECK_COMPILE_FLAG([ -Wall -Wextra -Wpedantic ], 
        [CXXFLAGS="$CXXFLAGS -Wall -Wextra -Wpedantic "])
fi

# Add debug support
AC_ARG_ENABLE(debug,
  AS_HELP_STRING(
    [--enable-debug],
    [enable debugging, default: no]),
    [case "${enableval}" in
      yes) debug=true ;;
      no)  debug=false ;;
      *)   AC_MSG_ERROR([bad value ${enableval} for --enable-debug]) ;;
    esac],
    [debug=false])
AM_CONDITIONAL(DEBUG, test x"$debug" = x"true")
AM_COND_IF(DEBUG,
    AC_DEFINE(DEBUG, 1, [Define to 0 if this is a release build]),
    AC_DEFINE(DEBUG, 0, [Define to 1 or higher if this is a debug build]))


AM_CONDITIONAL([HAVE_DOXYGEN],
[test -n "$DOXYGEN"])AM_COND_IF([HAVE_DOXYGEN], [AC_CONFIG_FILES([docs/Doxyfile])])

AC_CONFIG_FILES([
Makefile 
src/Makefile 
tests/Makefile
docs/Makefile
images/Makefile
music/Makefile
sounds/Makefile
game/Makefile
fonts/Makefile
po/Makefile.in
misc/occillo.desktop
misc/Makefile])
AC_OUTPUT
