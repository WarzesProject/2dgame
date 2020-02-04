#pragma once

class Application;

enum class ScreenState
{
	NONE,
	RUNNING,
	EXIT_GAME,
	CHANGE_NEXT,
	CHANGE_PREVIOUS
};

class IGameScreen
{
public:
	virtual ~IGameScreen() = default;

	// Called at beginning and end of application
	virtual void Build() = 0;
	virtual void Destroy() = 0;

	// Called when a screen enters and exits focus
	virtual void OnEntry() = 0;
	virtual void OnExit() = 0;

	// Called in the main game loop
	virtual void Update() = 0;
	virtual void Draw() = 0;

	// Return the index of the next or previous screen when changing screens
	virtual int GetNextScreenIndex() const = 0;
	virtual int GetPreviousScreenIndex() const = 0;

	int GetScreenIndex() const
	{
		return m_screenIndex;
	}

	ScreenState GetGameState() const
	{
		return m_currentState;
	}

	void SetApplication(Application *app)
	{
		m_app = app;
	}
	void SetGameState(ScreenState state)
	{
		if ( m_screenIndex == 0 && state == ScreenState::CHANGE_PREVIOUS )
			m_currentState = ScreenState::EXIT_GAME;
		else
			m_currentState = state;
	}
	void SetScreenIndex(int index)
	{
		m_screenIndex = index;
	}

protected:
	int m_screenIndex = -1;	
	ScreenState m_currentState = ScreenState::NONE;
	Application *m_app = nullptr;
};