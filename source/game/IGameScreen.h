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

	virtual void Build() = 0;
	virtual void Destroy() = 0;

	virtual void OnEntry() = 0;

	virtual void OnExit() = 0;


	virtual void Update() = 0;

	virtual void Draw() = 0;

	virtual int GetNextScreenIndex() const = 0;
	virtual int GetPreviousScreenIndex() const = 0;
	int GetScreenIndex() const
	{
		return m_screenIndex;
	}

	ScreenState GetGameState() const
	{
		return m_state;
	}

	void SetApplication(Application *app)
	{
		m_app = app;
	}
	void SetGameState(ScreenState state)
	{
		if ( m_screenIndex == 0 && state == ScreenState::CHANGE_PREVIOUS )
			m_state = ScreenState::EXIT_GAME;
		else
			m_state = state;
	}
	void SetScreenIndex(int index)
	{
		m_screenIndex = index;
	}

protected:
	int m_screenIndex = -1;	
	ScreenState m_state = ScreenState::NONE;
	Application *m_app = nullptr;
};