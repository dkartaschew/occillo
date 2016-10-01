/*
 *    Copyright (C) 2014 Darran Kartaschew
 *
 *    This file is part of Occillo.
 *
 *    Occillo is free software: you can redistribute it and/or modify
 *    it under the terms of the GNU General Public License as published by
 *    the Free Software Foundation, either version 3 of the License, or
 *    (at your option) any later version.
 *
 *    Occillo is distributed in the hope that it will be useful,
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *    GNU General Public License for more details.
 *
 *    You should have received a copy of the GNU General Public License
 *    along with Occillo.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "Configuration.h"

#ifdef _WIN32
#include <windows.h>
std::string getInstallPath();
// The following MUST match what the MSI installer uses for the installation path.
#define OCCILLO_REGISTRY "SOFTWARE\\Occillo Development Team\\Occillo"
#define OCCILLO_REGISTRY_PATH "Path"
#endif

Configuration::Configuration() {
	fullscreen = OCCILLO_DEFAULT_FULLSCREEN;
	displayHeight = OCCILLO_DEFAULT_RES_HEIGHT;
	displayWidth = OCCILLO_DEFAULT_RES_WIDTH;
	sndVolume = OCCILLO_DEFAULT_VOLUME;
	musicVolume = OCCILLO_DEFAULT_VOLUME;
	dataPath = PACKAGE_DATADIR;
	gameConfiguration = OCCILLO_DEFAULT_GAME;
	playerName = getDefaultUserName();
	soundBufferSize = OCCILLO_DEFAULT_SOUND_BUFFER_SIZE;
	controllerDeadZone = OCCILLO_DEFAULT_CONTROLLERDEADZONE;
	menuFocusAlpha = OCCILLO_DEFAULT_MENUFOCUSALPHA;
	menuNonFocusAlpha = OCCILLO_DEFAULT_MENUNONFOCUSALPHA;
#ifdef _WIN32
	registryDataPath = getInstallPath();
#else
	registryDataPath = "";
#endif
}

Configuration::Configuration(const Configuration& orig) {
	fullscreen = orig.fullscreen;
	displayHeight = orig.displayHeight;
	displayWidth = orig.displayWidth;
	sndVolume = orig.sndVolume;
	musicVolume = orig.musicVolume;
	dataPath = orig.dataPath;
	gameConfiguration = orig.gameConfiguration;
	soundBufferSize = orig.soundBufferSize;
	controllerDeadZone = orig.controllerDeadZone;
	menuFocusAlpha = orig.menuFocusAlpha;
	menuNonFocusAlpha = orig.menuNonFocusAlpha;
	registryDataPath = orig.registryDataPath;
}

Configuration::~Configuration() {
}

bool Configuration::load(const std::string& file) {
	GKeyFile *keyfile;
	GKeyFileFlags flags = G_KEY_FILE_NONE;
	GError *error = nullptr;

	keyfile = g_key_file_new();

	std::string* basepath = locateConfigFile();
	if (basepath == nullptr) {
		basepath = new std::string;
	}
	*basepath += file;
	g_info("%s[%d] : Looking for config file %s", __FILE__, __LINE__, basepath->c_str());

	if (!g_key_file_load_from_file(keyfile, basepath->c_str(), flags, &error)) {
		g_info("%s[%d] : config file error: %s", __FILE__, __LINE__, error->message);
		delete basepath;
		g_key_file_free(keyfile);
		g_error_free(error);
		return false;
	} else {
		g_info("%s[%d] : config file loaded.", __FILE__, __LINE__);
		try {
			setFullscreen(g_key_file_get_boolean(keyfile, PACKAGE_NAME, OCCILLO_CONFIGURATION_SETTING_FULLSCREEN, nullptr));
			setDisplayWidth(g_key_file_get_integer(keyfile, PACKAGE_NAME, OCCILLO_CONFIGURATION_SETTING_DISPLAY_WIDTH, nullptr));
			setDisplayHeight(g_key_file_get_integer(keyfile, PACKAGE_NAME, OCCILLO_CONFIGURATION_SETTING_DISPLAY_HEIGHT, nullptr));
			setSoundEffectVolume(g_key_file_get_integer(keyfile, PACKAGE_NAME, OCCILLO_CONFIGURATION_SETTING_SOUND_VOLUME, nullptr));
			setMusicVolume(g_key_file_get_integer(keyfile, PACKAGE_NAME, OCCILLO_CONFIGURATION_SETTING_MUSIC_VOLUME, nullptr));
			setAudioBufferSize(g_key_file_get_integer(keyfile, PACKAGE_NAME, OCCILLO_CONFIGURATION_SETTING_SOUND_BUFFER_SIZE, nullptr));
			setDataPath(getStringField(keyfile, OCCILLO_CONFIGURATION_SETTING_DATA_PATH));
			setGameConfiguration(getStringField(keyfile, OCCILLO_CONFIGURATION_SETTING_GAME_CONFIGURATION));
			setPlayerName(getStringField(keyfile, OCCILLO_CONFIGURATION_SETTING_PLAYERNAME));
			setControllerDeadZone(g_key_file_get_integer(keyfile, PACKAGE_NAME, OCCILLO_CONFIGURATION_SETTING_CONTROLLERDEADZONE, nullptr));
			setMenuFocusAlpha(g_key_file_get_integer(keyfile, PACKAGE_NAME, OCCILLO_CONFIGURATION_SETTING_MENUFOCUSALPHA, nullptr));
			setMenuNonFocusAlpha(g_key_file_get_integer(keyfile, PACKAGE_NAME, OCCILLO_CONFIGURATION_SETTING_MENUNONFOCUSALPHA, nullptr));
		} catch (...) {
			g_info("%s[%d] : config file load failed.", __FILE__, __LINE__);
			g_key_file_free(keyfile);
			delete basepath;
			return false;
		}
	}
	g_key_file_free(keyfile);
	delete basepath;
	return true;
}

bool Configuration::store(const std::string& file) {
	GKeyFile *keyfile;
	GError *error = nullptr;

	keyfile = g_key_file_new();
	g_key_file_set_boolean(keyfile, PACKAGE_NAME, OCCILLO_CONFIGURATION_SETTING_FULLSCREEN, fullscreen);
	g_key_file_set_integer(keyfile, PACKAGE_NAME, OCCILLO_CONFIGURATION_SETTING_DISPLAY_HEIGHT, displayHeight);
	g_key_file_set_integer(keyfile, PACKAGE_NAME, OCCILLO_CONFIGURATION_SETTING_DISPLAY_WIDTH, displayWidth);
	g_key_file_set_integer(keyfile, PACKAGE_NAME, OCCILLO_CONFIGURATION_SETTING_SOUND_VOLUME, sndVolume);
	g_key_file_set_integer(keyfile, PACKAGE_NAME, OCCILLO_CONFIGURATION_SETTING_MUSIC_VOLUME, musicVolume);
	g_key_file_set_integer(keyfile, PACKAGE_NAME, OCCILLO_CONFIGURATION_SETTING_SOUND_BUFFER_SIZE, soundBufferSize);
	g_key_file_set_string(keyfile, PACKAGE_NAME, OCCILLO_CONFIGURATION_SETTING_DATA_PATH, dataPath.c_str());
	g_key_file_set_string(keyfile, PACKAGE_NAME, OCCILLO_CONFIGURATION_SETTING_GAME_CONFIGURATION, gameConfiguration.c_str());
	g_key_file_set_string(keyfile, PACKAGE_NAME, OCCILLO_CONFIGURATION_SETTING_PLAYERNAME, playerName.c_str());
	g_key_file_set_integer(keyfile, PACKAGE_NAME, OCCILLO_CONFIGURATION_SETTING_CONTROLLERDEADZONE, controllerDeadZone);
	g_key_file_set_integer(keyfile, PACKAGE_NAME, OCCILLO_CONFIGURATION_SETTING_MENUFOCUSALPHA, menuFocusAlpha);
	g_key_file_set_integer(keyfile, PACKAGE_NAME, OCCILLO_CONFIGURATION_SETTING_MENUNONFOCUSALPHA, menuNonFocusAlpha);
	std::string* basepath = locateConfigFile();
	if (basepath == nullptr) {
		basepath = new std::string;
	}
	// Ensure base path exists.
	mkdirs(*basepath);
	*basepath += file;
	g_info("%s[%d] : Storing config file %s", __FILE__, __LINE__, basepath->c_str());

	if (!g_key_file_save_to_file(keyfile, basepath->c_str(), &error)) {
		g_info("%s[%d] : %s", __FILE__, __LINE__, error->message);
		g_key_file_free(keyfile);
		delete basepath;
		return false;
	} else {
		g_info("%s[%d] : %s", __FILE__, __LINE__, "Stored");
	}
	g_key_file_free(keyfile);
	delete basepath;
	return true;
}

bool Configuration::isFullscreen() {
	return fullscreen;
}

void Configuration::setFullscreen(bool fullscreen) {
	this->fullscreen = fullscreen;
	g_info("%s[%d] : Configuration: Fullscreen %d", __FILE__, __LINE__, fullscreen);
}

int Configuration::getDisplayWidth() {
	return displayWidth;
}

void Configuration::setDisplayWidth(int width) {
	if (width <= 0 || width >= 8192) {
		return;
	}
	displayWidth = width;
	g_info("%s[%d] : Configuration: Display Width %d", __FILE__, __LINE__, width);

}

int Configuration::getDisplayHeight() {
	return displayHeight;
}

void Configuration::setDisplayHeight(int height) {
	if (height <= 0 || height >= 4096) {
		return;
	}
	displayHeight = height;
	g_info("%s[%d] : Configuration: Display Height %d", __FILE__, __LINE__, height);

}

int Configuration::getSoundEffectVolume() {
	return sndVolume;
}

void Configuration::setSoundEffectVolume(int volume) {
	if (volume < 0 || volume > MIX_MAX_VOLUME) {
		return;
	}
	sndVolume = volume;
	g_info("%s[%d] : Configuration: Sound effects volume %d", __FILE__, __LINE__, volume);

}

int Configuration::getMusicVolume() {
	return musicVolume;
}

void Configuration::setMusicVolume(int volume) {
	if (volume < 0 || volume > MIX_MAX_VOLUME) {
		return;
	}
	musicVolume = volume;
	g_info("%s[%d] : Configuration: Music volume %d", __FILE__, __LINE__, volume);
}

int Configuration::getAudioBufferSize() {
	return soundBufferSize;
}

void Configuration::setAudioBufferSize(int bufferSize) {
	if (bufferSize <= 0) {
		return;
	}
	this->soundBufferSize = bufferSize;
}

/*
int Configuration::getBrickHeight() {
    return displayHeight / OCCILLO_BRICK_UNIT_HEIGHT;
}

int Configuration::getBrickWidth() {
    return displayWidth / OCCILLO_BRICK_UNIT_WIDE;
}
*/
std::string* Configuration::getDataPath() {
	return &dataPath;
}

void Configuration::setDataPath(const std::string& dataPath) {
	this->dataPath = dataPath;
	g_info("%s[%d] : Configuration: Data Path %s", __FILE__, __LINE__, dataPath.c_str());

}

std::string* Configuration::getGameConfiguration() {
	return &gameConfiguration;
}

void Configuration::setGameConfiguration(const std::string& gameConfiguration) {
	this->gameConfiguration = gameConfiguration;
	g_info("%s[%d] : Configuration: Game Configuration %s", __FILE__, __LINE__, gameConfiguration.c_str());

}

std::string* Configuration::getPlayerName() {
	return &playerName;
}

void Configuration::setPlayerName(const std::string& playerName) {
	this->playerName = playerName;
	g_info("%s[%d] : Configuration: Player name %s", __FILE__, __LINE__, playerName.c_str());

}

std::string Configuration::locateResource(const std::string& resource) {

	std::string file = resource;
	g_info("%s[%d] : Locating resource %s", __FILE__, __LINE__, file.c_str());
	/*
	 * Determine if file exists... if not, then try to prepend config->getDataPath() to it.
	 */
	if (g_file_test(file.c_str(), G_FILE_TEST_EXISTS)) {
		g_info("%s[%d] : Found resource %s", __FILE__, __LINE__, file.c_str());
		return file;
	}
	// File doesn't exist. Add datapath to it...
	std::string file2 = dataPath + G_DIR_SEPARATOR_S + file;
	if (g_file_test(file2.c_str(), G_FILE_TEST_EXISTS)) {
		g_info("%s[%d] : Found resource %s", __FILE__, __LINE__, file2.c_str());
		return file2;
	}

	// Use value as stored in registry, the MSI installer will set this.
	if (!registryDataPath.empty()) {
		file2 = registryDataPath + G_DIR_SEPARATOR_S + file;
		if (g_file_test(file2.c_str(), G_FILE_TEST_EXISTS)) {
			g_info("%s[%d] : Found resource %s", __FILE__, __LINE__, file2.c_str());
			return file2;
		}
	}

	// datapath + file doesn't exit, try again with package datadir (compile time installation path)
	file2 = PACKAGE_DATADIR G_DIR_SEPARATOR_S + file;
	if (g_file_test(file2.c_str(), G_FILE_TEST_EXISTS)) {
		g_info("%s[%d] : Found resource %s", __FILE__, __LINE__, file2.c_str());
		return file2;
	}
	g_info("%s[%d] : Resource not found? %s", __FILE__, __LINE__, file.c_str());
	return "";
}

std::string* Configuration::locateConfigFile() {
	/*
	 * Since we have cairo, we get glib, so use it's functions to get the
	 * config dir. Note: the returned string is owned by glib.
	 */
	const gchar* baseconfig = g_get_user_config_dir();
	if (baseconfig == nullptr) {
		return nullptr;
	}
	std::string* b = new std::string(baseconfig);
	b->append("/" PACKAGE_NAME "/");
	return b;
}

int Configuration::mkdirs(std::string path) {
#ifdef _WIN32
	// Win32
	size_t pre = 0, pos;
	std::string dir;
	int mdret = 0;

	if (path[path.size() - 1] != '\\') {
		// force trailing / so we can handle everything in loop
		path += '\\';
	}

	while ((pos = path.find_first_of('\\', pre)) != std::string::npos) {
		dir = path.substr(0, pos++);
		pre = pos;
		if (dir.size() == 0) {
			continue; // if leading \ first time is 0 length
		}
		bool res = CreateDirectoryA(dir.c_str(), NULL);
		if ((res == FALSE)) {
			mdret = GetLastError();
			if (mdret != ERROR_ALREADY_EXISTS) {
				return mdret;
			}
		}
	}
	return mdret;

#else
	// POSIX
	size_t pre = 0, pos;
	std::string dir;
	int mdret = 0;
	mode_t mode = 0755;

	if (path[path.size() - 1] != '/') {
		// force trailing / so we can handle everything in loop
		path += '/';
	}

	while ((pos = path.find_first_of('/', pre)) != std::string::npos) {
		dir = path.substr(0, pos++);
		pre = pos;
		if (dir.size() == 0) {
			continue; // if leading / first time is 0 length
		}
		if ((mdret = mkdir(dir.c_str(), mode)) && errno != EEXIST) {
			return mdret;
		}
	}
	return mdret;
#endif
}

std::string Configuration::getDefaultUserName() {
#ifdef _WIN32
	// Win32
	char name[32768];
	DWORD sz = 32768;
	bool ret = GetUserNameA(name, &sz);
	if (ret == true) {
		return std::string(name);
	} else {
		return std::string(OCCILLO_DEFAULT_PLAYERNAME);
	}
#else
	// POSIX
	/*    struct passwd *pw = nullptr;
	    // get the effective user id.
	    uid_t uid = geteuid();
	    // get the passwd DB entry for given UID.
	    pw = getpwuid(uid);
	    if (pw != nullptr) {
	        // and get the name.
	        if (pw->pw_gecos != nullptr) {
	            return std::string(pw->pw_gecos); // real name
	        }
	        return std::string(pw->pw_name); // user name
	    }

	        */
	// Use glib instead of getpwuid();
	std::string name = std::string(g_get_real_name());
	if (name.compare("Unknown") == 0) {
		name = std::string(g_get_user_name());
	}
	return name;
#endif
}

std::string Configuration::getStringField(GKeyFile *keyfile, const char* key) {
	gchar* str = g_key_file_get_string(keyfile, PACKAGE_NAME, key, nullptr);
	if (str == nullptr) {
		return "";
	}
	std::string value = std::string(str);
	g_free(str);
	return value;
}

int Configuration::getMenuFocusAlpha() {
	return menuFocusAlpha;
}

void Configuration::setMenuFocusAlpha(int alpha) {
	this->menuFocusAlpha = alpha;
}

int Configuration::getMenuNonFocusAlpha() {
	return menuNonFocusAlpha;
}

void Configuration::setMenuNonFocusAlpha(int alpha) {
	this->menuNonFocusAlpha = alpha;
}

int Configuration::getControllerDeadZone() {
	return controllerDeadZone;
}

void Configuration::setControllerDeadZone(int deadzone) {
	this->controllerDeadZone = deadzone;
}

#ifdef _WIN32

std::string getInstallPath() {
	// Check HKCU, and HKLM for path value.
	HKEY hKey;
	LONG lRes = RegOpenKeyExA(HKEY_CURRENT_USER, OCCILLO_REGISTRY, 0, KEY_READ, &hKey);
	if (hKey != ERROR_SUCCESS) {
		RegCloseKey(&hKey);
		lRes = RegOpenKeyExA(HKEY_LOCAL_MACHINE, OCCILLO_REGISTRY, 0, KEY_READ, &hKey);
		if (lRes != ERROR_SUCCES) {
			RegCloseKey(&hKey);
			return "";
		}
	}
	// We should have a key...
	std::string strValue = "";
	CHAR szBuffer[512];
	DWORD dwBufferSize = sizeof(szBuffer);
	ULONG nError = RegQueryValueExA(hKey, OCCILLO_REGISTRY_PATH, 0, NULL, (LPBYTE)szBuffer, &dwBufferSize);
	if (ERROR_SUCCESS == nError) {
		strValue = szBuffer;
	}
	RegCloseKey(&hKey);
	return strValue;
}
#endif
