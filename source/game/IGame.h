#pragma once

class ScreenManager;
class EventHandler;

class __declspec(novtable) IGame
{
public:
	virtual ~IGame() = default;

	virtual void OnInit(ScreenManager *screenMgr, EventHandler *eventHandler) = 0;
};