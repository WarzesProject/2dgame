#pragma once

#include "Tile.h"
#include "Camera2D.h"

class IGRenderer
{
protected:
	ColorRGBA8 m_debugColor;
};

class TileRenderer : public IGRenderer
{
public:
	TileRenderer();
	void Render(SpriteBatch& spriteBatch, Camera2D& camera, std::vector<Tile>& tiles);
	void Render(DebugRenderer& debugRenderer, Camera2D& camera, std::vector<Tile>& tiles);
};

class GameRenderer
{
public:
	void Init(float screenWidth, float screenHeight);
	void ClearAmbientColor(const ColorRGBA8& color);
	void Render(Camera2D& camera);
	void Dispose();
	void SetDebug(bool debug) { m_debug = debug; }
	void SetLineWidth(float lineWidth) { m_lineWidth = lineWidth; }

private:
	bool m_debug;
	float m_lineWidth;
};