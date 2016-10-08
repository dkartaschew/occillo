AUTOMAKE_OPTIONS=subdir-objects
ACLOCAL_AMFLAGS=${ACLOCAL_FLAGS}

check_PROGRAMS = collision colours

collision_SOURCES = \
    collision.h \
    collision.cc \
    TestRunner.cc \
    ../src/game/Paddle.cc \
    ../src/game/Ball.cc \
    ../src/util/Texture.cc \
		../src/util/StringUtil.cc \
		../src/game/Box.cc \
		../src/game/Brick.cc \
		../src/util/Point.cc

collision_INCLUDES = -I../src -I../src/util -I../src/game -I../src/ui -I../src/menu ${DBG}

collision_CXXFLAGS = $(CPPUNIT_CFLAGS) ${collision_INCLUDES} ${SDL2_CFLAGS} ${SDL2_image_CFLAGS} ${SDL2_ttf_CFLAGS} ${SDL2_mixer_CFLAGS}  ${librsvg_CFLAGS} ${cairo_CFLAGS} ${cairo_svg_CFLAGS}
collision_LDADD = ${SDL2_LIBS} ${SDL2_image_LIBS} ${SDL2_ttf_LIBS} ${SDL2_mixer_LIBS} ${librsvg_LIBS} ${cairo_LIBS} ${cairo_svg_LIBS} @LIBINTL@
collision_LDFLAGS = $(CPPUNIT_LIBS) -ldl

colours_SOURCES = \
    colours.h \
    colours.cc \
    TestRunner.cc \
		../src/util/StringUtil.cc \
    ../src/util/Texture.cc 

colours_INCLUDES = -I../src -I../src/util -I../src/game -I../src/ui -I../src/menu ${DBG}

colours_CXXFLAGS = $(CPPUNIT_CFLAGS) ${collision_INCLUDES} ${SDL2_CFLAGS} ${SDL2_image_CFLAGS} ${SDL2_ttf_CFLAGS} ${SDL2_mixer_CFLAGS}  ${librsvg_CFLAGS} ${cairo_CFLAGS} ${cairo_svg_CFLAGS}
colours_LDADD = ${SDL2_LIBS} ${SDL2_image_LIBS} ${SDL2_ttf_LIBS} ${SDL2_mixer_LIBS} ${librsvg_LIBS} ${cairo_LIBS} ${cairo_svg_LIBS} @LIBINTL@
colours_LDFLAGS = $(CPPUNIT_LIBS) -ldl
