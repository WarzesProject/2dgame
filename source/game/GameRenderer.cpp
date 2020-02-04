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

}
//-----------------------------------------------------------------------------
void GameRenderer::ClearAmbientColor(const ColorRGBA8 &color)
{
	glClearColor(color.r / 255.0f, color.g / 255.0f, color.b / 255.0f, color.a / 255.0f);
}
//-----------------------------------------------------------------------------
void GameRenderer::Render(Camera2D& camera)
{
	glm::vec4 box;
	camera.SetScreenBox(box);	
}
//-----------------------------------------------------------------------------