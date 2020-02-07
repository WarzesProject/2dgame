#include "stdafx.h"
#include "EventHandler.h"
//-----------------------------------------------------------------------------
constexpr auto FILENAME = "padconfig.conf";
constexpr auto JOY_AXE_DEFAULT_SPEED = 30;
constexpr auto AXIS_TIMELAPSE = 50.0f;
//-----------------------------------------------------------------------------
EventHandler::EventHandler(int &screenWidth, int &screenHeight)
	: m_screenWidth(screenWidth)
	, m_screenHeight(screenHeight)
{
	if ( SDL_NumJoysticks() > 0 )
	{
		SDL_JoystickEventState(SDL_ENABLE);
		m_joystick = SDL_JoystickOpen(0);
	}
}
//-----------------------------------------------------------------------------
EventHandler::~EventHandler()
{
	if ( m_joystick != nullptr )
		SDL_JoystickClose(m_joystick);

	SDL_JoystickEventState(SDL_DISABLE);
}
//-----------------------------------------------------------------------------
void EventHandler::Update(float deltaTime)
{
	if ( SDL_NumJoysticks() > 0 && m_joystick == nullptr )
	{
		SDL_JoystickEventState(SDL_ENABLE);
		m_joystick = SDL_JoystickOpen(0);
	}
	else if ( SDL_NumJoysticks() == 0 && m_joystick != nullptr )
	{
		SDL_JoystickClose(m_joystick);
		m_joystick = nullptr;
		m_playWith = PlayWith::KEYBOARD;
	}

	for ( auto &it : m_keyMap )
		m_previousKeyMap[it.first] = it.second;

	m_axisTimeLapse += deltaTime;
}
//-----------------------------------------------------------------------------
void EventHandler::PressKey(unsigned int keyID)
{
	m_keyMap[keyID] = true;
}
//-----------------------------------------------------------------------------
void EventHandler::ReleaseKey(unsigned int keyID)
{
	m_keyMap[keyID] = false;
}
//-----------------------------------------------------------------------------
void EventHandler::ReleaseEvent(unsigned int eventID)
{
	auto itEvent = m_eventConfig.find(eventID);
	if ( itEvent != m_eventConfig.end() )
	{
		unsigned int keyID = (m_playWith == PlayWith::KEYBOARD) ? itEvent->second.keyID : itEvent->second.joyID;
		ReleaseKey(keyID);
	}
}
//-----------------------------------------------------------------------------
bool EventHandler::IsEventDown(unsigned int eventID)
{
	unsigned int keyID = eventID;
	auto itEvent = m_eventConfig.find(eventID);
	if ( itEvent != m_eventConfig.end() )
		keyID = (m_playWith == PlayWith::KEYBOARD) ? itEvent->second.keyID : itEvent->second.joyID;

	return IsKeyDown(keyID);
}
//-----------------------------------------------------------------------------
bool EventHandler::IsKeyDown(unsigned int keyID)
{
	auto it = m_keyMap.find(keyID);
	if ( it != m_keyMap.end() )
		return it->second;

	return false;
}
//-----------------------------------------------------------------------------
bool EventHandler::wasKeyDown(unsigned int keyID)
{
	auto it = m_previousKeyMap.find(keyID);
	if ( it != m_previousKeyMap.end() )
		return it->second;

	return false;
}
//-----------------------------------------------------------------------------
bool EventHandler::IsEventPressed(unsigned int eventID)
{
	unsigned int keyID = eventID;
	auto itEvent = m_eventConfig.find(eventID);
	if ( itEvent != m_eventConfig.end() )
		keyID = (m_playWith == PlayWith::KEYBOARD) ? itEvent->second.keyID : itEvent->second.joyID;

	return (IsKeyDown(keyID) && !wasKeyDown(keyID));
}
//-----------------------------------------------------------------------------
bool EventHandler::IsKeyPressed(unsigned int keyID)
{
	return (IsKeyDown(keyID) && !wasKeyDown(keyID));
}
//-----------------------------------------------------------------------------
void EventHandler::SetMouseCoords(float x, float y)
{
	m_mouseCoords.x = x;
	m_mouseCoords.y = y;
}
//-----------------------------------------------------------------------------
void EventHandler::UpdateJoystickAxis(int axe, int value, SDL_Window* window)
{
	if ( axe == 0 )
	{
		if ( value < -JOY_DEAD_ZONE )
			PressKey(Joystick::AXE1_LEFT);
		else
			ReleaseKey(Joystick::AXE1_LEFT);
		if ( value > JOY_DEAD_ZONE )
			PressKey(Joystick::AXE1_RIGHT);
		else
			ReleaseKey(Joystick::AXE1_RIGHT);
	}
	else if ( axe == 1 )
	{
		if ( value < -JOY_DEAD_ZONE )
			PressKey(Joystick::AXE1_UP);
		else
			ReleaseKey(Joystick::AXE1_UP);
		if ( value > JOY_DEAD_ZONE )
			PressKey(Joystick::AXE1_DOWN);
		else
			ReleaseKey(Joystick::AXE1_DOWN);
	}
	else if ( axe == 5 )
	{
		if ( value > JOY_DEAD_ZONE )
			PressKey(Joystick::RT);
		else
			ReleaseKey(Joystick::RT);
	}
	else if ( axe == 2 || axe == 4 )
	{

		if ( axe == 2 )
		{
			m_joyAxeValue.x = (float)value;
		}
		else if ( axe == 4 )
		{
			m_joyAxeValue.y = (float)value;
		}

		if ( (m_joyAxeValue.x != 0 || m_joyAxeValue.y != 0) &&
			(m_mouseCoords.x >= 0 && m_mouseCoords.x <= m_screenWidth) &&
			(m_mouseCoords.y >= 0 && m_mouseCoords.y <= m_screenHeight) )
		{
			if ( m_axisTimeLapse > AXIS_TIMELAPSE )
			{
				m_axisTimeLapse = 0;
				m_mouseCoords.x += JOY_AXE_DEFAULT_SPEED * (m_joyAxeValue.x / JOY_AXE_MAX);
				m_mouseCoords.y += JOY_AXE_DEFAULT_SPEED * (m_joyAxeValue.y / JOY_AXE_MAX);
				SDL_WarpMouseInWindow(window, (int)m_mouseCoords.x, (int)m_mouseCoords.y);

				if ( m_mouseCoords.x > m_screenWidth ) m_mouseCoords.x = (float)m_screenWidth;
				if ( m_mouseCoords.y > m_screenHeight ) m_mouseCoords.y = (float)m_screenHeight;
				if ( m_mouseCoords.x < 0 ) m_mouseCoords.x = 0.0f;
				if ( m_mouseCoords.y < 0 ) m_mouseCoords.y = 0.0f;
			}
		}
	}
}
//-----------------------------------------------------------------------------
void EventHandler::UpdateJoystickHats(int value)
{
	ReleaseKey(Joystick::DPAD_LEFT);
	ReleaseKey(Joystick::DPAD_RIGHT);
	ReleaseKey(Joystick::DPAD_UP);
	ReleaseKey(Joystick::DPAD_DOWN);

	PressKey(value);
}
//-----------------------------------------------------------------------------
void EventHandler::UpdateMapping(unsigned int eventID, PlayWith util)
{
	for ( auto& it : m_keyMap )
	{
		if ( IsKeyPressed(it.first) && it.first != SDL_BUTTON_LEFT )
		{
			if ( util == PlayWith::KEYBOARD )
				m_eventConfigTemp[eventID].keyID = it.first;
			else
				m_eventConfigTemp[eventID].joyID = it.first;
		}
	}
}
//-----------------------------------------------------------------------------
void EventHandler::UpdateConfig()
{
	for ( auto &it : m_eventConfig )
		m_eventConfigTemp[it.first] = it.second;
}
//-----------------------------------------------------------------------------
void EventHandler::SaveConfig()
{
	for ( auto &it : m_eventConfigTemp )
		m_eventConfig[it.first] = it.second;
	SaveConfigFile();
}
//-----------------------------------------------------------------------------
bool EventHandler::LoadConfigFile()
{
	std::ifstream file(FILENAME);
	if ( file.fail() )
	{
		perror(FILENAME);
		return false;
	}

	size_t size = 0;
	int Playwith = 0;

	file >> Playwith;
	file >> size;

	for ( size_t i = 0; i < size; i++ )
	{
		unsigned int id, keyID, joyID;
		file >> id >> keyID >> joyID;
		m_eventConfig[id] = EventConfig(keyID, joyID);
	}
	file.close();

	if ( (PlayWith)Playwith == PlayWith::GAMEPAD && m_joystick != nullptr )
		m_playWith = PlayWith::GAMEPAD;
	else
		m_playWith = PlayWith::KEYBOARD;

	UpdateConfig();

	return true;
}
//-----------------------------------------------------------------------------
bool EventHandler::SaveConfigFile()
{
	std::ofstream file(FILENAME);
	if ( file.fail() )
	{
		perror(FILENAME);
		return false;
	}

	file << (int)m_playWith << '\n';
	file << m_eventConfig.size() << '\n';

	for ( auto &it : m_eventConfig )
		file << it.first << ' ' << it.second.keyID << ' ' << it.second.joyID << '\n';

	file.close();
	return true;
}
//-----------------------------------------------------------------------------
void EventHandler::ClearConfig()
{
	m_eventConfigTemp.clear();
}
//-----------------------------------------------------------------------------
std::string EventHandler::GetMapping(unsigned int eventID)
{
	return getMapping(eventID, m_playWith, m_eventConfig);
}
//-----------------------------------------------------------------------------
std::string EventHandler::GetMapping(unsigned int eventID, PlayWith util)
{
	return getMapping(eventID, util, m_eventConfigTemp);
}
//-----------------------------------------------------------------------------
std::string EventHandler::getMapping(unsigned int eventID, PlayWith util, std::unordered_map<unsigned int, EventConfig> &map)
{
	unsigned int keyID;

	if ( util == PlayWith::KEYBOARD )
	{
		keyID = map[eventID].keyID;
		switch ( keyID )
		{
		case SDL_BUTTON_LEFT: return "Mouse Left button";
		case SDL_BUTTON_RIGHT: return "Mouse Right button";
		default:
			return SDL_GetKeyName(keyID);
		}
	}
	else
	{
		keyID = map[eventID].joyID;
		switch ( keyID )
		{
		case Joystick::A: return "A button";
		case Joystick::B: return "B button";
		case Joystick::X: return "X button";
		case Joystick::Y: return "Y button";
		case Joystick::HOME: return "HOME button";
		case Joystick::LB: return "Left bumper";
		case Joystick::RB: return "Right bumper";
		case Joystick::L3: return "Stick Left button";
		case Joystick::R3: return "Stick Right button";
		case Joystick::SELECT: return "Select button";
		case Joystick::START: return "Start button";
		case Joystick::DPAD_UP: return "Directional pad UP";
		case Joystick::DPAD_DOWN: return "Directional pad DOWN";
		case Joystick::DPAD_LEFT: return "Directional pad LEFT";
		case Joystick::DPAD_RIGHT: return "Directional pad RIGHT";
		case Joystick::LT: return "Left trigger";
		case Joystick::RT: return "Right trigger";
		case Joystick::AXE1_UP: return "Left stick UP";
		case Joystick::AXE1_DOWN: return "Left stick DOWN";
		case Joystick::AXE1_LEFT: return "Left stick LEFT";
		case Joystick::AXE1_RIGHT: return "Left stick RIGHT";
		case Joystick::AXE2_UP: return "Right stick UP";
		case Joystick::AXE2_DOWN: return "Right stick DOWN";
		case Joystick::AXE2_LEFT: return "Right stick LEFT";
		case Joystick::AXE2_RIGHT: return "Right stick RIGHT";
		default:
			return "";
		}
	}
}
//-----------------------------------------------------------------------------