#pragma once

#define JOY_DEAD_ZONE 2000
#define JOY_AXE_MAX  32767
#define MARGIN 500

struct EventConfig
{
	EventConfig() :keyID(0), joyID(0)
	{
	}
	EventConfig(unsigned int key, unsigned int joy) :keyID(key), joyID(joy)
	{
	}
	unsigned int keyID;
	unsigned int joyID;
};

// Allow to pick what user wants to play with.
enum class PlayWith
{
	KEYBOARD, GAMEPAD
};

enum Joystick
{
	A = 10 + MARGIN, B = 11 + MARGIN, X = 12 + MARGIN, Y = 13 + MARGIN, HOME = 14 + MARGIN, LB = 8 + MARGIN, RB = 9 + MARGIN,
	L3 = 6 + MARGIN, R3 = 7 + MARGIN, SELECT = 5 + MARGIN, START = 4 + MARGIN, DPAD_UP = 0 + MARGIN, DPAD_DOWN = 1 + MARGIN,
	DPAD_LEFT = 2 + MARGIN, DPAD_RIGHT = 3 + MARGIN, LT = 15 + MARGIN, RT = 16 + MARGIN, AXE1_UP = 17 + MARGIN,
	AXE1_DOWN = 18 + MARGIN, AXE1_LEFT = 19 + MARGIN, AXE1_RIGHT = 20 + MARGIN, AXE2_UP = 21 + MARGIN, AXE2_DOWN = 22 + MARGIN,
	AXE2_LEFT = 23 + MARGIN, AXE2_RIGHT = 24 + MARGIN, NOTHING = 25 + MARGIN
};

class GameWindow;

class EventHandler
{
public:
	void Init(int screenWidth, int screenHeight);
	void destroy();

	void update(float deltaTime);

	void updateJoystickAxis(int axe, int value, SDL_Window* window);

	void updateJoystickHats(int value);

	void pressKey(unsigned int keyID);

	void releaseKey(unsigned int keyID);

	bool isKeyDown(unsigned int keyID);

	bool isKeyPressed(unsigned int keyID);

	bool isEventDown(unsigned int eventID);

	bool isEventPressed(unsigned int eventID);

	void releaseEvent(unsigned int eventID);

	bool isJoyConnect()
	{
		return m_joystick != nullptr;
	}

	void addEvent(unsigned int eventID, unsigned int keyID, unsigned int joyID)
	{
		m_eventConfig[eventID] = EventConfig(keyID, joyID);
	}

	void updateMapping(unsigned int eventID, PlayWith util);

	std::string getMapping(unsigned int eventID, PlayWith util);

	std::string getMapping(unsigned int eventID);

	void updateConfig();
	void saveConfig();
	void clearConfig();
	bool loadConfigFile();
	bool saveConfigFile();

	void setMouseCoords(float x, float y);
	void setPlayWith(PlayWith util)
	{
		m_playWith = util;
	}

	glm::vec2 getMouseCoords() const
	{
		return m_mouseCoords;
	}
	PlayWith getPlayWith()
	{
		return m_playWith;
	}
	EventConfig getEventKey(unsigned int eventID)
	{
		return m_eventConfig[eventID];
	}

private:
	bool wasKeyDown(unsigned int keyID);
	std::string getMapping(unsigned int eventID, PlayWith util, std::unordered_map<unsigned int, EventConfig>& map);

	std::unordered_map<unsigned int, EventConfig> m_eventConfig;
	std::unordered_map<unsigned int, EventConfig> m_eventConfigTemp;
	std::unordered_map<unsigned int, bool> m_keyMap;
	std::unordered_map<unsigned int, bool> m_previousKeyMap;
	glm::vec2 m_mouseCoords = glm::vec2(0);	
	glm::vec2 m_joyAxeValue = glm::vec2(0);	
	PlayWith m_playWith = PlayWith::KEYBOARD;
	SDL_Joystick * m_joystick = nullptr;
	int m_screenWidth, m_screenHeight;
	float m_axisTimeLapse = 0.0f;
};