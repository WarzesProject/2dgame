#pragma once

#include "Texture.h"

class Sprite
{
public:
	~Sprite();

	void Init(float x, float y, float width, float height, const std::string texturePath);

	void Draw();

private:
	float m_x = 0.0f;
	float m_y = 0.0f;
	float m_width = 0.0f;
	float m_height = 0.0f;
	GLuint m_vboID = 0;
	Texture m_texture;
};