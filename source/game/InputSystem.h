#pragma once

constexpr auto JOY_DEAD_ZONE = 2000;
constexpr auto JOY_AXE_MAX = 32767;
constexpr auto MARGIN = 500;

struct EventConfig
{
	EventConfig() = default;
	EventConfig(unsigned int key, unsigned int joy) : keyID(key), joyID(joy) {}
	unsigned int keyID = 0;
	unsigned int joyID = 0;
};

enum class PlayWith
{
	KEYBOARD, GAMEPAD
};

enum Joystick
{
	A = 10 + MARGIN, 
	B = 11 + MARGIN, 
	X = 12 + MARGIN, 
	Y = 13 + MARGIN, 
	HOME = 14 + MARGIN, 
	LB = 8 + MARGIN, 
	RB = 9 + MARGIN,
	L3 = 6 + MARGIN, 
	R3 = 7 + MARGIN, 
	SELECT = 5 + MARGIN, 
	START = 4 + MARGIN, 
	DPAD_UP = 0 + MARGIN, 
	DPAD_DOWN = 1 + MARGIN,
	DPAD_LEFT = 2 + MARGIN, 
	DPAD_RIGHT = 3 + MARGIN, 
	LT = 15 + MARGIN, 
	RT = 16 + MARGIN, 
	AXE1_UP = 17 + MARGIN,
	AXE1_DOWN = 18 + MARGIN, 
	AXE1_LEFT = 19 + MARGIN, 
	AXE1_RIGHT = 20 + MARGIN, 
	AXE2_UP = 21 + MARGIN, 
	AXE2_DOWN = 22 + MARGIN,
	AXE2_LEFT = 23 + MARGIN, 
	AXE2_RIGHT = 24 + MARGIN, 
	NOTHING = 25 + MARGIN
};

class GameWindow;

class InputSystem
{
public:
	InputSystem(int &screenWidth, int &screenHeight);
	~InputSystem();

	void Update(float deltaTime);
	void UpdateJoystickAxis(int axe, int value, SDL_Window *window);
	void UpdateJoystickHats(unsigned int value);

	void PressKey(unsigned int keyID);
	void ReleaseKey(unsigned int keyID);

	bool IsKeyDown(unsigned int keyID);
	bool IsKeyPressed(unsigned int keyID);
	bool IsEventDown(unsigned int eventID);
	bool IsEventPressed(unsigned int eventID);

	void ReleaseEvent(unsigned int eventID);

	bool IsJoyConnect() const { return m_joystick != nullptr; }

	void AddEvent(unsigned int eventID, unsigned int keyID, unsigned int joyID)
	{
		m_eventConfig[eventID] = EventConfig(keyID, joyID);
	}

	void UpdateMapping(unsigned int eventID, PlayWith util);

	std::string GetMapping(unsigned int eventID, PlayWith util);
	std::string GetMapping(unsigned int eventID);

	void UpdateConfig();
	void SaveConfig();
	void ClearConfig();
	bool LoadConfigFile();
	bool SaveConfigFile();

	void SetMouseCoords(float x, float y);
	void SetPlayWith(PlayWith util) { m_playWith = util; }

	glm::vec2 GetMouseCoords() const { return m_mouseCoords; }
	PlayWith GetPlayWith() const { return m_playWith; }
	EventConfig GetEventKey(unsigned int eventID) { return m_eventConfig[eventID]; }

private:
	InputSystem() = delete;
	InputSystem(const InputSystem&) = delete;
	InputSystem(InputSystem&&) = delete;
	InputSystem& operator=(const InputSystem&) = delete;
	InputSystem& operator=(InputSystem&&) = delete;

	bool wasKeyDown(unsigned int keyID);
	std::string getMapping(unsigned int eventID, PlayWith util, std::unordered_map<unsigned int, EventConfig> &map);

	std::unordered_map<unsigned int, EventConfig> m_eventConfig;
	std::unordered_map<unsigned int, EventConfig> m_eventConfigTemp;
	std::unordered_map<unsigned int, bool> m_keyMap;
	std::unordered_map<unsigned int, bool> m_previousKeyMap;
	glm::vec2 m_mouseCoords = glm::vec2(0);	
	glm::vec2 m_joyAxeValue = glm::vec2(0);	
	PlayWith m_playWith = PlayWith::KEYBOARD;
	SDL_Joystick *m_joystick = nullptr;
	int &m_screenWidth;
	int &m_screenHeight;
	float m_axisTimeLapse = 0.0f;
};