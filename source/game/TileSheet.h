#pragma once

#include "Texture.h"

class TileSheet
{
public:
	void Init(const Texture &texture, const glm::ivec2 &dims, bool invX = false, bool invY = false)
	{
		this->texture = texture;
		this->dims = dims;
		if ( invX )
		{
			this->invX = 1.0f;
			this->invZ = -1.0f;
		}
		else
		{
			this->invX = 0.0f;
			this->invZ = 1.0f;
		}
		if ( invY )
		{
			this->invY = 1.0f;
			this->invW = -1.0f;
		}
		else
		{
			this->invY = 0.0f;
			this->invW = 1.0f;
		}
	}

	glm::vec4& GetUV(int index)
	{
		const int xTile = index % dims.x;
		const int yTile = index / dims.x;

		uvs.x = (xTile + invX) / (float)dims.x;
		uvs.y = (yTile + invY) / (float)dims.y;
		uvs.z = invZ / dims.x;
		uvs.w = invW / dims.y;

		return uvs;
	}
	
	glm::vec4& GetUV(int xTile, int yTile)
	{
		uvs.x = (xTile + invX) / (float)dims.x;
		uvs.y = (yTile + invY) / (float)dims.y;
		uvs.z = invZ / dims.x;
		uvs.w = invW / dims.y;

		return uvs;
	}

	Texture texture;
	glm::ivec2 dims;
	glm::vec4 uvs;
	float invX = 0, invY = 0, invZ = 1, invW = 1;
};