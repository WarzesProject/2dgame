#pragma once

enum class GameViewState
{
	None,
	Running,
	ExitGame,
	ChangeNext,
	ChangePrevious
};

class __declspec(novtable) IGameView
{
public:
	virtual ~IGameView();
};