#include "stdafx.h"
#include "LevelManager.h"
#include "ResourceManager.h"
#include "DefineLogic.h"
//-----------------------------------------------------------------------------
LevelManager::LevelManager(std::string_view fileName)
{
	const glm::vec4 uvRect(0.0f, 0.0f, 1.0f, 1.0f);
	const ColorRGBA8 whiteColor = { 255,255,255,255 };

	std::ifstream file;
	file.open(fileName.data());
	if ( file.fail() )
		Throw("Failed to open " + std::string(fileName.data()));

	std::string tmp;
	std::vector<std::string> tempLevelData;
	while ( std::getline(file, tmp) )
		tempLevelData.push_back(tmp);
	m_levelData.resize(tempLevelData.size());
	for ( size_t i = 0; i < tempLevelData.size(); i++ )
		m_levelData[i] = tempLevelData[tempLevelData.size() - 1 - i];

	m_spriteBatch.Init();
	m_spriteBatch.Begin();

	for ( size_t y = 0; y < m_levelData.size(); y++ )
	{
		for ( size_t x = 0; x < m_levelData[y].size(); x++ )
		{
			char tile = m_levelData[y][x];
			glm::vec4 destRect(x * TILE_WIDTH, y * TILE_WIDTH, TILE_WIDTH, TILE_WIDTH);

			switch ( tile )
			{
			case 'R':
				m_spriteBatch.Draw(destRect, uvRect, ResourceManager::GetTexture("../test/Assets/Floor/red_bricks.png").id, 0.0f, whiteColor);
				break;
			case 'G':
				m_spriteBatch.Draw(destRect, uvRect, ResourceManager::GetTexture("../test/Assets/Floor/glass.png").id, 0.0f, whiteColor);
				break;
			case 'L':
				m_spriteBatch.Draw(destRect, uvRect, ResourceManager::GetTexture("../test/Assets/Floor/light_bricks.png").id, 0.0f, whiteColor);
				break;
			case '@':
				m_startPlayerPosition.x = (float)(x * TILE_WIDTH);
				m_startPlayerPosition.y = (float)(y * TILE_WIDTH);
				m_levelData[y][x] = '.';
				m_spriteBatch.Draw(destRect, uvRect, ResourceManager::GetTexture("../test/Assets/Floor/groundTile.png").id, 0.0f, whiteColor);
				break;
			case '.':
				m_spriteBatch.Draw(destRect, uvRect, ResourceManager::GetTexture("../test/Assets/Floor/groundTile.png").id, 0.0f, whiteColor);
				break;
			default:
				SDL_Log(("Unexpected symbol " + std::to_string(tile) + std::string(" at (") + std::to_string(x) + " , " + std::to_string(y) + ")").c_str());
				break;
			}
		}
	}

	m_spriteBatch.End();
}
//-----------------------------------------------------------------------------
void LevelManager::Draw()
{
	m_spriteBatch.RenderBatch();
}
//-----------------------------------------------------------------------------
int LevelManager::GetWidth() const
{
	return (int)m_levelData[0].length();
}
//-----------------------------------------------------------------------------
int LevelManager::GetHeight() const
{
	return m_levelData.size();
}
//-----------------------------------------------------------------------------
glm::vec2 LevelManager::GetStartPlayerPosition() const
{
	return m_startPlayerPosition;
}
//-----------------------------------------------------------------------------
const std::vector<std::string>& LevelManager::GetLevelData() const
{
	return m_levelData;
}
//-----------------------------------------------------------------------------
const char LevelManager::GetSymbol(int x, int y) const
{
	return m_levelData[y][x];
}
//-----------------------------------------------------------------------------
glm::vec2 LevelManager::GetCameraPos(const glm::vec2 &cameraPos, const glm::vec2 &screenSize, const float scale)
{
	const glm::vec2 playerPos = cameraPos;
	glm::vec2 returnPos = cameraPos;

	const glm::vec2 halfCameraDims = glm::vec2(screenSize.x / (2.0f * scale), screenSize.y / (2.0f * scale));

	const glm::vec2 LeftDownDistance = playerPos;
	const glm::vec2 RightUpDistance = glm::vec2(GetWidth() * (float)TILE_WIDTH, GetHeight() * (float)TILE_WIDTH) - playerPos;

	if ( (LeftDownDistance.x - halfCameraDims.x) < 0.0f )
		returnPos.x = halfCameraDims.x;

	if ( (LeftDownDistance.y - halfCameraDims.y) < 0.0f )
		returnPos.y = halfCameraDims.y;

	if ( (RightUpDistance.x - halfCameraDims.x) < 0.0f )
		returnPos.x = GetWidth() * (float)TILE_WIDTH - halfCameraDims.x;

	if ( (RightUpDistance.y - halfCameraDims.y) < 0.0f )
		returnPos.y = GetHeight() * (float)TILE_WIDTH - halfCameraDims.y;
	
	return returnPos;
}
//-----------------------------------------------------------------------------