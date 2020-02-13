#pragma once

#include "SpriteBatch.h"
#include "TileSheet.h"

class LevelManager
{
public:
	LevelManager(std::string_view fileName);

	void Draw();

	int GetWidth() const;
	int GetHeight() const;
	glm::vec2 GetStartPlayerPosition() const;
	const std::vector<std::string>& GetLevelData() const;
	const char GetSymbol(int x, int y) const;

	glm::vec2 GetCameraPos(const glm::vec2 &cameraPos, const glm::vec2 &screenSize, const float scale);

private:
	std::vector<std::string> m_levelData;

	SpriteBatch m_spriteBatch;
	TileSheet m_texture;

	glm::vec2 m_startPlayerPosition;
};