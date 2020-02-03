#pragma once

#include "DebugRenderer.h"

enum  class ShapeType
{
	CIRCLE, BOX
};

class Shape
{
public:
	Shape(float x, float y, float w, float h, ShapeType type);
	void Init(float x, float y, float w, float h, ShapeType type);

	friend std::ostream& operator<<(std::ostream& os, const Shape& s);

	static bool SimpleCollision(glm::vec2& c1, float x1, float y1, glm::vec2& c2, float x2, float y2, glm::vec2& col);
	static bool SimpleCollision(glm::vec2& c1, float x1, float y1, glm::vec2& c2, float x2, float y2);
	static bool PointIntriangle(glm::vec2& pt, glm::vec2& t1, glm::vec2& t2, glm::vec2& t3);
	static bool InSameSide(glm::vec2& pt1, glm::vec2& t1, glm::vec2& t2);
	static bool CircleContains(float x, float y, float r, glm::vec2& pt);
	static bool BoxContains(float x, float y, float w, float h, glm::vec2& pt);

	bool CollideWith(Shape& s, glm::vec2& collisionDepth);
	bool CircleCollideCircle(Shape& c, glm::vec2& collisionDepth);
	bool CircleCollideBox(Shape& c, glm::vec2& collisionDepth);
	bool CollideTile(glm::vec2& centertilePos, float width);
	bool Contains(glm::vec2& p);
	void Draw(DebugRenderer& debugRenderer, ColorRGBA8 color);

	void SetPosition(const glm::vec2& pos) { position.x = destRect.x = pos.x; position.y = destRect.y = pos.y; }
	void SetPosition(float x, float y) { position.x = destRect.x = x; position.y = destRect.y = y; }
	void SetDestRect(glm::vec4& dest) { destRect = dest; position.x = dest.x; position.y = dest.y; }
	void IncrPosition(glm::vec2& incr);
	void IncrPosition(float x, float y);
	void IncrPositionX(float x);
	void IncrPositionY(float y);

	static ShapeType GetTypeByName(const std::string& type);
	std::string GetTypeName();
	glm::vec2 GetDimension() { return glm::vec2(destRect.z, destRect.w); }
	glm::vec2 GetCenter() { return glm::vec2(GetCenterX(), GetCenterY()); }
	glm::vec2 GetBL() { return glm::vec2(destRect.x, destRect.y); }
	glm::vec2 GetTL() { return glm::vec2(destRect.x, destRect.y + destRect.w); }
	glm::vec2 GetTR() { return glm::vec2(destRect.x + destRect.z, destRect.y + destRect.w); }
	glm::vec2 GetBR() { return glm::vec2(destRect.x + destRect.z, destRect.y); }
	float GetWidth() { return destRect.z; }
	float GetHeight() { return destRect.w; }
	float GetCenterWidth() { return (destRect.z / 2.0f); }
	float GetCenterHeight() { return (destRect.w / 2.0f); }
	float GetCenterX() { return destRect.x + (destRect.z / 2.0f); }
	float GetCenterY() { return destRect.y + (destRect.w / 2.0f); }
	float GetLX() { return destRect.x; }
	float GetRX() { return destRect.x + destRect.z; }
	float GetBY() { return destRect.y; }
	float GetTY() { return destRect.y + destRect.w; }

	ShapeType type;
	glm::vec2 position;		/* Position in earth coordinates */
	glm::vec4 destRect;
};