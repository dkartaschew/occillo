/*
     Copyright (C) 2014 Darran Kartaschew

     This file is part of Occillo.

        Occillo is free software: you can redistribute it and/or modify
        it under the terms of the GNU General Public License as published by
        the Free Software Foundation, either version 3 of the License, or
        (at your option) any later version.

        Occillo is distributed in the hope that it will be useful,
        but WITHOUT ANY WARRANTY; without even the implied warranty of
        MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
        GNU General Public License for more details.

        You should have received a copy of the GNU General Public License
        along with Occillo.  If not, see <http://www.gnu.org/licenses/>.  */

#ifndef CONFIGURATION_H
#define CONFIGURATION_H
#include <iostream>
#include <string>
#include <glib.h>
#include <cerrno>
#include <sys/stat.h>
#ifndef _WIN32
#include <pwd.h>
#include <unistd.h>
#endif
#include <sys/types.h>
#include <SDL_mixer.h>

#include "config.h"

#define OCCILLO_DEFAULT_GAME "game/game.ini"
#define OCCILLO_DEFAULT_FULLSCREEN false
#define OCCILLO_DEFAULT_RES_WIDTH 720
#define OCCILLO_DEFAULT_RES_HEIGHT 480
#define OCCILLO_DEFAULT_VOLUME 50
#define OCCILLO_DEFAULT_PLAYERNAME "Player"
#define OCCILLO_DEFAULT_SOUND_BUFFER_SIZE 1024
#define OCCILLO_DEFAULT_MENUFOCUSALPHA 255
#define OCCILLO_DEFAULT_MENUNONFOCUSALPHA 180
#define OCCILLO_DEFAULT_CONTROLLERDEADZONE 8000

#define OCCILLO_CONFIGURATION_SETTING_FULLSCREEN "fullscreen"
#define OCCILLO_CONFIGURATION_SETTING_DISPLAY_WIDTH "displayWidth"
#define OCCILLO_CONFIGURATION_SETTING_DISPLAY_HEIGHT "displayHeight"
#define OCCILLO_CONFIGURATION_SETTING_SOUND_VOLUME "soundVolume"
#define OCCILLO_CONFIGURATION_SETTING_MUSIC_VOLUME "musicVolume"
#define OCCILLO_CONFIGURATION_SETTING_SOUND_BUFFER_SIZE "soundBufferSize"
#define OCCILLO_CONFIGURATION_SETTING_DATA_PATH "dataPath"
#define OCCILLO_CONFIGURATION_SETTING_GAME_CONFIGURATION "game"
#define OCCILLO_CONFIGURATION_SETTING_PLAYERNAME "playerName"
#define OCCILLO_CONFIGURATION_SETTING_MENUFOCUSALPHA "menuFoucsAlpha"
#define OCCILLO_CONFIGURATION_SETTING_MENUNONFOCUSALPHA "menuNonFoucsAlpha"
#define OCCILLO_CONFIGURATION_SETTING_CONTROLLERDEADZONE "controllerDeadZone"

/**
 * @brief The engine configuration for display and audio.
 */
class Configuration {
public:
    /**
     * Create a configuration with default settings
     */
    Configuration();
    /**
     * Copy configuration to new object
     * @param orig item to copy.
     */
    Configuration(const Configuration& orig);
    /**
     *
     */
    virtual ~Configuration();
    /**
     * Load configuration from file
     * @param file The file to load from
     * @return TRUE if successful
     */
    bool load(const std::string& file);
    /**
     * Store the configuration into file.
     * @param file The file to store to.
     * @return  TRUE if successful
     */
    bool store(const std::string& file);
    /**
     * Get if full screen is set.
     * @return TRUE for full screen
     */
    bool isFullscreen();
    /**
     * Set full screen mode
     * @param fullscreen TRUE to set full screen.
     */
    void setFullscreen(bool fullscreen);
    /**
     * Get the display width to set
     * @return The display width
     */
    int getDisplayWidth();
    /**
     * Set the display width
     * @param width The requested width
     */
    void setDisplayWidth(int width);
    /**
     * Get the display height
     * @return The display height
     */
    int getDisplayHeight();
    /**
     * Set the display height
     * @param height The requested height
     */
    void setDisplayHeight(int height);
    /**
     * Get the sound effect volume
     * @return The volume (0-100) 0 = mute.
     */
    int getSoundEffectVolume();
    /**
     * Set the sound effect volume
     * @param volume The volume to set. Must be between 0 and 100. 0 = mute.
     */
    void setSoundEffectVolume(int volume);
    /**
     * Get the music volume
     * @return The volume (0-100) 0 = mute.
     */
    int getMusicVolume();
    /**
     * Set the music volume
     * @param volume The volume to set. Must be between 0 and 100. 0 = mute.
     */
    void setMusicVolume(int volume);
    /**
     * Get the audio buffer size
     * @return The size of the buffer to allocate.
     */
    int getAudioBufferSize();

    /**
     * Set the audio buffer size
     * @param bufferSize The size of the buffer to allocate.
     */
    void setAudioBufferSize(int bufferSize);
    /**
     * Get the focus menu alpha value.
     * @return The define alpha value.
     */
    int getMenuFocusAlpha();
    /**
     * Set the focus menu alpha value.
     * @param alpha the requested value.
     */
    void setMenuFocusAlpha(int alpha);
    /**
     * Get the non focus menu alpha value.
     * @return The define alpha value.
     */
    int getMenuNonFocusAlpha();
    /**
     * Set the non-focus menu alpha value.
     * @param alpha the requested value.
     */
    void setMenuNonFocusAlpha(int alpha);
    /**
     * Get the controller dead zone value.
     * @return The dead zone value.
     */
    int getControllerDeadZone();
    /**
     * Set the controller dead zone value.
     * @param deadzone The dead zone value.
     */
    void setControllerDeadZone(int deadzone);
    /**
     * Get the height in pixels of a single unit.
     * @return The heigh in pixels
     */
    //int getBrickHeight();
    /**
     * Get the width in pixels of a single brick unit
     * @return The width in pixels
     */
    //int getBrickWidth();
    /**
     * Get the data path
     * <br />
     * This is typically PACKAGE_DATADIR
     * @return The data path
     */
    std::string* getDataPath();
    /**
     * Set the data path
     * @param dataPath The data path.
     */
    void setDataPath(const std::string& dataPath);
    /**
     * Get the game definition file
     * @return The game file
     */
    std::string* getGameConfiguration();
    /**
     * Set the game configuration file.
     * @param gameConfiguration The location of the game configuration file.
     */
    void setGameConfiguration(const std::string& gameConfiguration);

    /**
     * Get the players name as last recorded.
     * @return The players name.
     */
    std::string* getPlayerName();
    /**
     * Set the players name
     * @param playerName The players name.
     */
    void setPlayerName(const std::string& playerName);

    /**
     * @brief Locate the given resource, and return a path with the resource/
     *
     * @param resource The resource to locate.
     *
     * @return A path to the resource, or an empty string if the resource can't be found.
     */
    std::string locateResource(const std::string& resource);

private:
    /**
     * @brief Flag for fullscreen mode (TRUE = fullscreen, FALSE = window).
     */
    bool fullscreen;
    /**
     * @brief The display width to use/set.
     */
    int displayWidth;
    /**
     * @brief The display height to use/set.
     */
    int displayHeight;
    /**
     * @brief The sound effects volume to use. (O = off, 100 = max).
     */
    int sndVolume;
    /**
     * @brief The music volume to use. (0 = off, 100 = max).
     */
    int musicVolume;
    /**
     * @brief The size of the audio buffer. (default = 1024).
     */
    int soundBufferSize;
    /**
     * @brief The menu alpha value for focused items. (default = 255).
     */
    int menuFocusAlpha;
    /**
     * @brief The menu alpha value for nonfocused items. (default = 180).
     */
    int menuNonFocusAlpha;
    /**
     * @brief The controller dead zone (default = 8000).
     */
    int controllerDeadZone;
    /**
     * @brief The data path for game assets.
     */
    std::string dataPath;
    /**
     * @brief The data path for game assets as stored in the win32 registry
     */
    std::string registryDataPath;
    /**
     * @brief The path to the game configuration file.
     */
    std::string gameConfiguration;
    /**
     * @brief The last used/stored player name. (defaults to username of the current user).
     */
    std::string playerName;
    /**
     * Get the correct name for the configuration file.
     * @return The configuration file.
     */
    std::string* locateConfigFile();
    /**
     * make all folders as specified in the path
     * @param path The path to create.
     */
    int mkdirs(std::string path);

    /**
     * Get the default/current username from the system.
     * @return The users name.
     */
    std::string getDefaultUserName();

    /**
     * @brief Get string field for key
     *
     * @param keyfile The keyfile
     * @param key The key to query
     *
     * @return The value for the given key.
     */
    std::string getStringField(GKeyFile *keyfile, const char* key);

};

#endif  /* CONFIGURATION_H */

