#pragma once

class ScreenManager;

class __declspec(novtable) IGame
{
public:
	virtual ~IGame() = default;

	virtual void OnInit(ScreenManager *screenMgr) = 0;
};