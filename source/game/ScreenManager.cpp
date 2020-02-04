#include "stdafx.h"
#include "ScreenManager.h"
#include "IGameScreen.h"
//-----------------------------------------------------------------------------
void ScreenManager::Init(Application *app)
{
	m_app = app;
}
//-----------------------------------------------------------------------------
IGameScreen* ScreenManager::MoveNext()
{
	IGameScreen *currentScreen = GetCurrentScreen();
	if ( currentScreen->GetNextScreenIndex() != SCREEN_INDEX_NO_SCREEN )
		m_currentScreenIndex = currentScreen->GetNextScreenIndex();

	return GetCurrentScreen();
}
//-----------------------------------------------------------------------------
IGameScreen* ScreenManager::MovePrevious()
{
	IGameScreen *currentScreen = GetCurrentScreen();
	if ( currentScreen->GetPreviousScreenIndex() != SCREEN_INDEX_NO_SCREEN )
		m_currentScreenIndex = currentScreen->GetPreviousScreenIndex();

	return GetCurrentScreen();
}
//-----------------------------------------------------------------------------
void ScreenManager::AddScreen(IGameScreen* screen)
{
	screen->Build();
	screen->SetApplication(m_app);
	screen->SetScreenIndex(m_screens.size());
	m_screens.push_back(screen);
}
//-----------------------------------------------------------------------------
void ScreenManager::SetScreen(int screenIndex)
{
	m_currentScreenIndex = screenIndex;
}
//-----------------------------------------------------------------------------
void ScreenManager::Destroy()
{
	for ( size_t i = 0; i < m_screens.size(); i++ )
	{
		m_screens[i]->Destroy();
	}

	m_screens.clear();
	m_currentScreenIndex = SCREEN_INDEX_NO_SCREEN;
}
//-----------------------------------------------------------------------------
IGameScreen* ScreenManager::GetCurrentScreen()
{
	if ( m_currentScreenIndex != SCREEN_INDEX_NO_SCREEN )
		return m_screens[m_currentScreenIndex];
	else
		return nullptr;
}
//-----------------------------------------------------------------------------