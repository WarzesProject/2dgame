#include "stdafx.h"
#include "GameScreen.h"
#include "Application.h"
#include "TileSheet.h"

GLSLProgram m_colorProgram;
SpriteBatch m_spriteBatch;
Texture texture;
ColorRGBA8 color;
SpriteFont spriteFont;


Texture texture2;
TileSheet tiletexture;

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
	const int screenWidth = m_app->GetScreenWidth();
	const int screenHeight = m_app->GetScreenHeight();
	m_camera.Init(screenWidth, screenHeight);
	m_camera.SetPosition(glm::vec2(screenWidth / 2.0f, screenHeight / 2.0f));

	m_colorProgram.CompileShadersFromFile("../data/shaders/colorShading.vert", "../data/shaders/colorShading.frag");
	m_colorProgram.AddAttribute("vertexPosition");
	m_colorProgram.AddAttribute("vertexColor");
	m_colorProgram.AddAttribute("vertexUV");
	m_colorProgram.LinkShaders();

	m_spriteBatch.Init();

	texture = ResourceManager::GetTexture("../data/sprite/logo1.png");
	texture2 = ResourceManager::GetTexture("../data/sprite/blue_ninja.png");

	tiletexture.Init(texture2, glm::ivec2(10, 2));

	spriteFont = ResourceManager::GetFont("../data/fonts/chintzy.ttf", 40);

	color.r = 255;
	color.g = 255;
	color.b = 255;
	color.a = 255;
}
//-----------------------------------------------------------------------------
void GameScreen::OnExit()
{
	//ResourceManager::GetMusic("../data/musics/XYZ.ogg").Stop();
}
//-----------------------------------------------------------------------------
void GameScreen::Update()
{
	SDL_Event ev;
	while ( SDL_PollEvent(&ev) )
		m_app->OnSDLEvent(ev);

	const float CAMERA_SPEED = 2.0f;
	const float SCALE_SPEED = 0.1f;

	auto eventHandler = m_app->GetEventHandler();

	if ( eventHandler.IsKeyDown(SDLK_w) )
		m_camera.SetPosition(m_camera.GetPosition() + glm::vec2(0.0f, CAMERA_SPEED));

	if ( eventHandler.IsKeyDown(SDLK_s) )
		m_camera.SetPosition(m_camera.GetPosition() + glm::vec2(0.0f, -CAMERA_SPEED));

	if ( eventHandler.IsKeyDown(SDLK_a) )
		m_camera.SetPosition(m_camera.GetPosition() + glm::vec2(-CAMERA_SPEED, 0.0f));

	if ( eventHandler.IsKeyDown(SDLK_d) )
		m_camera.SetPosition(m_camera.GetPosition() + glm::vec2(CAMERA_SPEED, 0.0f));

	if ( eventHandler.IsKeyDown(SDLK_q) )
		m_camera.SetScale(m_camera.GetScale() + SCALE_SPEED);

	if ( eventHandler.IsKeyDown(SDLK_e) )
		m_camera.SetScale(m_camera.GetScale() - SCALE_SPEED);

	//if ( eventHandler.IsKeyDown(SDL_BUTTON_LEFT) )
	//{
	//    glm::vec2 mouseCoords = eventHandler.GetMouseCoords();
	//    mouseCoords = m_camera.ConvertScreenToWorld(mouseCoords);

	//    glm::vec2 playerPosition(0.0f);
	//    glm::vec2 direction = mouseCoords - playerPosition;
	//    direction = glm::normalize(direction);

	//    _sprite.emplace_back(playerPosition, direction, 5.00f, 1000);
	//}

	m_camera.Update();
}
//-----------------------------------------------------------------------------
void GameScreen::Draw()
{
	glClearDepth(1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.2f, 0.6f, 1.0f, 1.0f);

	m_colorProgram.Use();

	glActiveTexture(GL_TEXTURE0);
	GLint textureLocation = m_colorProgram.GetUniformLocation("mySampler");
	glUniform1i(textureLocation, 0);

	//Set the camera matrix
	GLint pLocation = m_colorProgram.GetUniformLocation("P");
	glm::mat4 cameraMatrix = m_camera.GetCameraMatrix();

	glUniformMatrix4fv(pLocation, 1, GL_FALSE, &(cameraMatrix[0][0]));

	m_spriteBatch.Begin();
	{
		// позиция спрайта
		glm::vec4 dest(0.0f, 0.0f, 200.0f, 200.0f);
		glm::vec4 uv(0.0f, 0.0f, 1.0f, 1.0f);
		m_spriteBatch.Draw(dest, uv, texture.id, 0.0f, color);
	}
	m_spriteBatch.End();
	m_spriteBatch.RenderBatch();

	const ColorRGBA8 fontColor(255, 0, 0, 255);
	m_spriteBatch.Begin();
	spriteFont.Draw(m_spriteBatch, "Hell", glm::vec2(100.0f, 480 - 32.0f), glm::vec2(1.0f), 0.0f, fontColor);
	m_spriteBatch.End();
	m_spriteBatch.RenderBatch();
	
	
	m_spriteBatch.Begin();
	{
		glm::vec4 dest(300.0f, 0.0f, 200.0f, 200.0f);
		glm::vec4 uvRect = tiletexture.GetUV(0);
		m_spriteBatch.Draw(dest, uvRect, tiletexture.texture.id, 0.0f, color);
	}
	m_spriteBatch.End();
	m_spriteBatch.RenderBatch();

	glBindTexture(GL_TEXTURE_2D, 0);
	m_colorProgram.Unuse();
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