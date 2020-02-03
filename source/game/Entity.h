#pragma once

#include "Shape.h"
#include "TileSheet.h"

class Entity
{
public:

private:
	Shape m_shape;
	std::string m_name;
	TileSheet m_tileSheet;
	ColorRGBA8 m_color;
	glm::vec2 m_direction;
};