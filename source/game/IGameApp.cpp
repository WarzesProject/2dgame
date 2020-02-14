#include "stdafx.h"
#include "IGameApp.h"
//-----------------------------------------------------------------------------
bool IGameApp::privateInit(Application *app)
{
	m_app = app;
	m_viewManager.Init(m_app);

	Init();

	m_currentView = m_viewManager.GetCurrentGameView();
	if( !m_currentView )
		return false;

	m_currentView->OnEntry();
	m_currentView->SetGameState(GameViewState::Running);

	return true;
}
//-----------------------------------------------------------------------------
void IGameApp::privateDraw()
{
	if ( m_currentView && m_currentView->GetGameState() == GameViewState::Running )
		m_currentView->Draw();
}
//-----------------------------------------------------------------------------
bool IGameApp::privateUpdate()
{
	if ( m_currentView )
	{
		switch ( m_currentView->GetGameState() )
		{
		case GameViewState::Running:
			m_currentView->Update();
			break;
		case GameViewState::ChangeNext:
			m_currentView->OnExit();
			m_currentView = m_viewManager.MoveNext();
			if ( m_currentView )
			{
				m_currentView->OnEntry();
				m_currentView->SetGameState(GameViewState::Running);				
			}
			break;
		case GameViewState::ChangePrevious:
			m_currentView->OnExit();
			m_currentView = m_viewManager.MovePrevious();
			if ( m_currentView )
			{
				m_currentView->OnEntry();
				m_currentView->SetGameState(GameViewState::Running);
			}
			break;
		case GameViewState::None:
		case GameViewState::ExitGame:
			return false;
		}
	}
	else
		return false;

	return true;
}
//-----------------------------------------------------------------------------
bool IGameApp::privateQuit()
{
	TODO("тут можно спрашивать хочет ли игрок закрыть");
	m_currentView->SetGameState(GameViewState::ChangePrevious);
	return true;
}
//-----------------------------------------------------------------------------
void IGameApp::privateClose()
{
	if( m_currentView )
		m_currentView->OnExit();
	m_viewManager.Destroy();
}
//-----------------------------------------------------------------------------