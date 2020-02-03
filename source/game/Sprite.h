#pragma once

#include "Texture.h"

class Sprite
{
public:
	Sprite();
	~Sprite();

	void Init(float x, float y, float width, float height, const std::string texturePath);

	void Draw();

private:
	float m_x;
	float m_y;
	float m_width;
	float m_height;
	GLuint m_vboID;
	Texture m_texture;
};