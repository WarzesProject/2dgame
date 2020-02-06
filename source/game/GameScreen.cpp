#include "stdafx.h"
#include "GameScreen.h"
#include "Application.h"
#include "EventConfig.h"
//-----------------------------------------------------------------------------
constexpr float DESIRED_FPS = 60.0f;
constexpr float MS_PER_SECOND = 1000.0f;
constexpr float DESIRED_FRAMETIME = MS_PER_SECOND / DESIRED_FPS;
constexpr float MAX_DELTA_TIME = 1.0f;
constexpr int MAX_PHYSICS_STEPS = 6;

const float PLAYER_SPEED = 3.0f;
const float HUMAN_SPEED = 1.0f;
const float ZOMBIE_SPEED = 1.0f;
//-----------------------------------------------------------------------------
GameScreen::GameScreen() : m_currentLevel(m_particleEngine)
{
	m_screenIndex = 0;
	m_previousTicks = (float)SDL_GetTicks();

	m_inPause = false;
	m_gameState = GameState::PLAY;
	m_renderer.setDebug(false);
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
	initSystems();
	initLevel();
	m_inPause = false;
}
//-----------------------------------------------------------------------------
void GameScreen::OnExit()
{
	m_renderer.dispose();
}
//-----------------------------------------------------------------------------
void GameScreen::Update()
{
	checkInput();

	if ( !m_inPause )
	{
		int i = 0;
		float newTicks, frameTime, totalDeltaTime;

		newTicks = (float)SDL_GetTicks();
		frameTime = newTicks - m_previousTicks;
		m_previousTicks = newTicks;
		totalDeltaTime = frameTime / DESIRED_FRAMETIME;

		while ( totalDeltaTime > 0.0f && i < MAX_PHYSICS_STEPS )
		{
			float deltaTime = (totalDeltaTime > MAX_DELTA_TIME) ? MAX_DELTA_TIME : totalDeltaTime;

			m_entityController.updateEntities(m_currentLevel, deltaTime);
			m_particleEngine.Update(deltaTime);

			totalDeltaTime -= deltaTime;
			i++;
		}

		m_camera.SetPosition(m_currentLevel.getPlayer().getShape().position);
		m_camera.Update();
	}
}
//-----------------------------------------------------------------------------
void GameScreen::Draw()
{
	m_renderer.render(m_currentLevel, m_camera, m_particleEngine);
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
void GameScreen::checkInput()
{
	if ( m_currentLevel.input->IsEventDown(MyEvent::PAUSE) )
	{
		m_currentLevel.input->ReleaseEvent(MyEvent::PAUSE);
		if ( m_inPause )
		{
			m_inPause = false;
		}
		else
		{
			ResourceManager::GetSound("Sounds/ui_click.ogg").Play();
			m_inPause = true;
		}
	}
}

void GameScreen::initSystems()
{
	m_screenWidth = m_app->GetScreenWidth();
	m_screenHeight = m_app->GetScreenHeight();

	m_camera.Init(m_screenWidth, m_screenHeight);
	m_camera.SetPosition(glm::vec2(m_screenWidth / 2, m_screenHeight / 2));

	m_renderer.init(m_screenWidth, m_screenHeight);
}

void GameScreen::initLevel()
{
	//m_currentLevel.update();
	//m_renderer.clearAmbientColor(m_currentLevel.ambientColor);
}