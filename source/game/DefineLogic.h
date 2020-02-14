#pragma once

#include "SpriteBatch.h"
#include "TileSheet.h"
#include "DebugRenderer.h"
#include "ResourceManager.h"
#include "AudioSystem.h"
#include "InputSystem.h"
#include "Vertex.h"

constexpr glm::vec4 UVRect(0.0f, 0.0f, 1.0f, 1.0f);
constexpr ColorRGBA8 WhiteColor(255, 255, 255, 255);

constexpr int TILE_WIDTH = 32;
constexpr int ITEM_SIZE = 32;
constexpr int MAX_MOVEMENT = 40;

enum class MovingState
{
	UP = 0,
	DOWN = 1,
	LEFT = 2,
	RIGHT = 3,
};