#include "stdafx.h"
#include "GameScreen.h"
#include "Application.h"
//-----------------------------------------------------------------------------
GameScreen::GameScreen()
{
	m_screenIndex = 0;
}
//-----------------------------------------------------------------------------
GameScreen::~GameScreen()
{
}
//-----------------------------------------------------------------------------
void GameScreen::Build()
{

}
//-----------------------------------------------------------------------------
void GameScreen::Destroy()
{

}
//-----------------------------------------------------------------------------
void GameScreen::OnEntry()
{
	m_camera.Init(m_app->GetScreenWidth(), m_app->GetScreenHeight());
	m_camera.SetScale(1.0f);
	m_camera.SetPosition(glm::vec2(m_app->GetScreenWidth() / 2, m_app->GetScreenHeight() / 2));
	ResourceManager::GetMusic("../data/musics/XYZ.ogg").Play();

	m_renderer.Init(m_app->GetScreenWidth(), m_app->GetScreenHeight());
}
//-----------------------------------------------------------------------------
void GameScreen::OnExit()
{
	ResourceManager::GetMusic("../data/musics/XYZ.ogg").Stop();
}
//-----------------------------------------------------------------------------
void GameScreen::Update()
{
	m_camera.Update();

	SDL_Event ev;
	while( SDL_PollEvent(&ev) )
		m_app->OnSDLEvent(ev);
}
//-----------------------------------------------------------------------------
void GameScreen::Draw()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.2f, 0.6f, 1.0f, 1.0f);
	m_renderer.Render(m_camera);
}
//-----------------------------------------------------------------------------
int GameScreen::GetNextScreenIndex() const
{
	return -1;
}
//-----------------------------------------------------------------------------
int GameScreen::GetPreviousScreenIndex() const
{
	return -1;
}
//-----------------------------------------------------------------------------