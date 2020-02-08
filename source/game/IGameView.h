#pragma once

class Application;

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
	virtual ~IGameView() = default;

	// Called at beginning and end of application
	virtual void Build() = 0;
	virtual void Destroy() = 0;

	// Called when a view enters and exits focus
	virtual void OnEntry() = 0;
	virtual void OnExit() = 0;

	// Called in the main game loop
	virtual void Update() = 0;
	virtual void Draw() = 0;

	// Return the index of the next or previous view when changing view
	virtual int GetPreviousViewIndex() const = 0;
	virtual int GetNextViewIndex() const = 0;

	void SetGameState(GameViewState state)
	{
		if( m_viewIndex == 0 && state == GameViewState::ChangePrevious )
			m_currentState = GameViewState::ExitGame;
		else
			m_currentState = state;
	}

	int GetIndex() const { return m_viewIndex; }
	GameViewState GetGameState() const { return m_currentState; }
	void SetApplication(Application *app) { m_app = app; }
	void SetViewIndex(int index) { m_viewIndex = index; }

protected:
	int m_viewIndex = -1;
	GameViewState m_currentState = GameViewState::None;
	Application *m_app = nullptr;
};