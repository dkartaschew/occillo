
AUTOMAKE_OPTIONS=subdir-objects
ACLOCAL_AMFLAGS=${ACLOCAL_FLAGS}

bin_PROGRAMS = occillo
occillo_SOURCES = \
main.cc main.h \
IAppState.h \
GameConfiguration.cc GameConfiguration.h \
Configuration.cc Configuration.h \
util/Texture.cc util/Texture.h \
util/TextureRepository.cc util/TextureRepository.h \
util/Display.cc util/Display.h \
util/HighScore.cc util/HighScore.h \
util/HighScoreTable.cc util/HighScoreTable.h \
util/Resolution.cc util/Resolution.h \
util/Point.cc util/Point.h \
ui/IUIWidget.h \
ui/ItoString.h \
ui/UIWidget.cc ui/UIWidget.h \
ui/UIButton.cc ui/UIButton.h \
ui/UICursor.cc ui/UICursor.h \
ui/UILabel.cc ui/UILabel.h \
ui/UIImage.cc ui/UIImage.h \
ui/UICheckBox.cc ui/UICheckBox.h \
ui/UIText.cc ui/UIText.h \
ui/UISlider.cc ui/UISlider.h \
ui/UISelection.h \
menu/Menu.cc menu/Menu.h \
menu/MainMenu.cc menu/MainMenu.h \
menu/CreditsMenu.cc menu/CreditsMenu.h \
menu/HiScoreMenu.cc menu/HiScoreMenu.h \
menu/OptionsMenu.cc menu/OptionsMenu.h \
game/Box.cc game/Box.h \
game/Level.cc game/Level.h \
game/Brick.cc game/Brick.h \
game/Paddle.cc game/Paddle.h \
game/Ball.cc game/Ball.h \
game/EndGame.cc game/EndGame.h \
game/Game.cc game/Game.h \
game/GameState.cc game/GameState.h \
game/GameAnimation.h \
game/TextureFadeAnimation.cc game/TextureFadeAnimation.h \
game/BrickDestructionAnimation.cc game/BrickDestructionAnimation.h

if HAVE_WINDRES
occillo_SOURCES += occillo.rc
endif

.rc.o:
	windres $^ -o $@
%.o : %.rc
	windres $^ -o $@

if DEBUG
DBG = -g3 -O0
else
DBG = 
endif

occillo_INCLUDES = -I./util -I./game -I./ui -I./menu ${DBG}
occillo_CXXFLAGS = ${occillo_INCLUDES} ${SDL2_CFLAGS} ${SDL2_image_CFLAGS} ${SDL2_ttf_CFLAGS} ${SDL2_mixer_CFLAGS}  ${librsvg_CFLAGS} ${cairo_CFLAGS} ${cairo_svg_CFLAGS}
occillo_LDADD = ${SDL2_LIBS} ${SDL2_image_LIBS} ${SDL2_ttf_LIBS} ${SDL2_mixer_LIBS} ${librsvg_LIBS} ${cairo_LIBS} ${cairo_svg_LIBS} @LIBINTL@
occillo_LDFLAGS = $(LTLIBINTL)

datadir = @datadir@
datarootdir= @datarootdir@
localedir = @localedir@
DEFS = -DLOCALEDIR=\"$(localedir)\" -DPACKAGE_DATADIR=\"$(pkgdatadir)\" @DEFS@
