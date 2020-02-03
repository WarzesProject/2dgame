#include "stdafx.h"
#include "GameRenderer.h"
#include "ResourceManager.h"
//-----------------------------------------------------------------------------
TileRenderer::TileRenderer()
{
	m_debugColor = ColorRGBA8(255, 0, 0, 50);
}
//-----------------------------------------------------------------------------
void TileRenderer::Render(SpriteBatch& spriteBatch, Camera2D& camera, std::vector<Tile>& tiles)
{
	for( size_t i = 0; i < tiles.size(); i++ )
	{
		Tile& t = tiles[i];

		if( camera.IsBoxInView(t.getDestRect()) )
		{
			t.Draw(spriteBatch);
		}
	}
}
//-----------------------------------------------------------------------------
void TileRenderer::Render(DebugRenderer& debugRenderer, Camera2D& camera, std::vector<Tile>& tiles)
{
	for( size_t i = 0; i < tiles.size(); i++ )
	{
		Tile& t = tiles[i];

		if( camera.IsBoxInView(t.getDestRect()) )
		{
			t.Draw(debugRenderer, m_debugColor);
		}
	}
}
//-----------------------------------------------------------------------------
void GameRenderer::Init(float screenWidth, float screenHeight)
{
	m_debugColor = ColorRGBA8(255, 0, 0, 255);
	m_lineWidth = 2.0f;
	m_debug = false;

	// Compile color shader
	m_textureProgram.CompileShaders("../data/shaders/colorShading.vert", "../data/shaders/colorShading.frag");
	m_textureProgram.AddAttribute("vertexPosition");
	m_textureProgram.AddAttribute("vertexColor");
	m_textureProgram.AddAttribute("vertexUV");
	m_textureProgram.LinkShaders();

	// Compile light shader
	m_lightProgram.CompileShaders("../data/shaders/lightShading.vert", "../data/shaders/lightShading.frag");
	m_lightProgram.AddAttribute("vertexPosition");
	m_lightProgram.AddAttribute("vertexColor");
	m_lightProgram.AddAttribute("vertexUV");
	m_lightProgram.LinkShaders();

	m_spriteBatch.Init();
	m_lightSpriteBatch.Init();
	m_fontSpriteBatch.Init();
	m_debugRenderer.Init();

	/*TEMP*/
	for( int x = 0; x < 100; x++ )
		for( int y = 0; y < 100; y++ )
			{
				Tile t;
				t.Init(ResourceManager::GetTexture("../data/sprite/space-00-03.png"), x, y, true);
				temptiles.push_back(t);
			}
	/*TEMP*/
}
//-----------------------------------------------------------------------------
void GameRenderer::ClearAmbientColor(const ColorRGBA8& color)
{
	glClearColor(color.r / 255.0f, color.g / 255.0f, color.b / 255.0f, color.a / 255.0f);
}
//-----------------------------------------------------------------------------
void GameRenderer::Render(Camera2D& camera)
{
	glm::vec4 box;
	camera.SetScreenBox(box);

	//ClearAmbientColor(level.ambientColor);
	glClear(GL_COLOR_BUFFER_BIT);
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	//glBlendFunc(GL_DST_COLOR, GL_ZERO);

	m_textureProgram.Use();
	{
		glActiveTexture(GL_TEXTURE0);

		// Make sure the shader uses texture 0
		GLint textureLocation = m_textureProgram.GetUniformLocation("mySampler");
		glUniform1i(textureLocation, 0);

		// set the camera matrix
		GLint cmLocation = m_textureProgram.GetUniformLocation("cameraMatrix");
		glm::mat4 cameraMatrix = camera.GetCameraMatrix();
		glUniformMatrix4fv(cmLocation, 1, GL_FALSE, &(cameraMatrix[0][0]));

		m_spriteBatch.Begin();
		m_tileRenderer.Render(m_spriteBatch, camera, temptiles);
		m_spriteBatch.End();
		m_spriteBatch.RenderBatch();

	}
	m_textureProgram.Unuse();

	
}
