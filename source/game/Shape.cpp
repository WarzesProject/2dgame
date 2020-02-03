#include "stdafx.h"
#include "Shape.h"
//-----------------------------------------------------------------------------
Shape::Shape(float x, float y, float width, float height, ShapeType type) :
	position(x, y), destRect(x, y, width, height), type(type)
{
}
//-----------------------------------------------------------------------------
void Shape::Init(float x, float y, float w, float h, ShapeType type)
{
	destRect.x = position.x = x;
	destRect.y = position.y = y;
	destRect.z = w;
	destRect.w = h;
	type = type;
}
//-----------------------------------------------------------------------------
std::ostream& operator<<(std::ostream& os, const Shape& s)
{
	os << ((s.type == ShapeType::BOX) ? "BOX" : "CIRCLE")
		<< "["
		<< s.destRect.x << ", " << s.destRect.y << ", "
		<< s.destRect.z << ", " << s.destRect.w
		<< "]";

	return os;
}
//-----------------------------------------------------------------------------
std::string Shape::GetTypeName()
{
	return (type == ShapeType::BOX) ? "BOX" : "CIRCLE";
}
//-----------------------------------------------------------------------------
ShapeType Shape::GetTypeByName(const std::string& type)
{
	return (type == "BOX") ? ShapeType::BOX : ShapeType::CIRCLE;
}
//-----------------------------------------------------------------------------
void Shape::Draw(DebugRenderer& debugRenderer, ColorRGBA8 color)
{
	if( type == ShapeType::BOX )
		debugRenderer.DrawBox(destRect, color, 0.0f);
	else
		debugRenderer.DrawCircle(GetCenter(), color, GetCenterWidth());
}
//-----------------------------------------------------------------------------
bool Shape::Contains(glm::vec2& p)
{
	return (
		p.x > destRect.x && p.x < destRect.x + destRect.z &&
		p.y > destRect.y && p.y < destRect.y + destRect.w
		);
}
//-----------------------------------------------------------------------------
bool Shape::CollideWith(Shape& s, glm::vec2& collisionDepth)
{
	if( s.type == ShapeType::CIRCLE && type == ShapeType::CIRCLE )
		return CircleCollideCircle(s, collisionDepth);
	else
		return CircleCollideBox(s, collisionDepth);
}
//-----------------------------------------------------------------------------
void Shape::IncrPosition(glm::vec2& incr)
{
	IncrPositionX(incr.x);
	IncrPositionY(incr.y);
}
//-----------------------------------------------------------------------------
void Shape::IncrPosition(float x, float y)
{
	IncrPositionX(x);
	IncrPositionY(y);
}
//-----------------------------------------------------------------------------
void Shape::IncrPositionX(float x)
{
	position.x += x;
	destRect.x = position.x;
}
//-----------------------------------------------------------------------------
void Shape::IncrPositionY(float y)
{
	position.y += y;
	destRect.y = position.y;
}
//-----------------------------------------------------------------------------
bool Shape::CircleCollideCircle(Shape& s, glm::vec2& collisionDepth)
{
	float minDist = GetCenterWidth() + s.GetCenterWidth();
	glm::vec2 distVec = GetCenter() - s.GetCenter();
	float collision = minDist - glm::length(distVec);

	if( collision > 0 )
	{
		if( collision == minDist )
			collisionDepth = glm::normalize(glm::vec2(1.0f, 1.0f)) * collision;
		else
			collisionDepth = glm::normalize(distVec) * collision;
		return true;
	}

	return false;
}
//-----------------------------------------------------------------------------
bool Shape::CircleCollideBox(Shape& s, glm::vec2& collisionDepth)
{
	float minDistX = GetCenterWidth() + s.GetCenterWidth();
	float minDistY = GetCenterHeight() + s.GetCenterHeight();

	glm::vec2 distVec = GetCenter() - s.GetCenter();
	float collisionX = minDistX - abs(distVec.x);
	float collisionY = minDistY - abs(distVec.y);

	if( collisionX > 0 && collisionY > 0 )
	{
		if( collisionX < collisionY )
		{
			collisionDepth.x = (distVec.x < 0) ? -collisionX : collisionX;
		}
		else
		{
			collisionDepth.y = (distVec.y < 0) ? -collisionY : collisionY;
		}

		return true;
	}

	return false;
}
//-----------------------------------------------------------------------------
bool Shape::CollideTile(glm::vec2& centertilePos, float width)
{
	float minDistX = GetCenterWidth() + (width / 2.0f);
	float minDistY = GetCenterHeight() + (width / 2.0f);

	glm::vec2 distVec = GetCenter() - centertilePos;
	float collisionX = minDistX - abs(distVec.x);
	float collisionY = minDistY - abs(distVec.y);

	if( collisionX > 0 && collisionY > 0 )
	{
		if( collisionX < collisionY )
		{
			if( distVec.x < 0 )
			{
				IncrPositionX(-collisionX);
			}
			else
			{
				IncrPositionX(collisionX);
			}
		}
		else
		{
			if( distVec.y < 0 )
			{
				IncrPositionY(-collisionY);
			}
			else
			{
				IncrPositionY(collisionY);
			}
		}
		return true;
	}

	return false;
}
//-----------------------------------------------------------------------------
bool Shape::SimpleCollision(glm::vec2& c1, float x1, float y1, glm::vec2& c2, float x2, float y2, glm::vec2& col)
{
	float minDistX = x1 + x2;
	float minDistY = y1 + y2;

	glm::vec2 distVec = c1 - c2;
	col.x = minDistX - abs(distVec.x);
	col.y = minDistY - abs(distVec.y);

	if( col.x > 0 && col.y > 0 )
	{
		return true;
	}

	return false;
}
//-----------------------------------------------------------------------------
bool Shape::SimpleCollision(glm::vec2& c1, float x1, float y1, glm::vec2& c2, float x2, float y2)
{
	float minDistX = x1 + x2;
	float minDistY = y1 + y2;

	glm::vec2 distVec = c1 - c2;
	float collisionX = minDistX - abs(distVec.x);
	float collisionY = minDistY - abs(distVec.y);

	if( collisionX > 0 && collisionY > 0 )
	{
		return true;
	}

	return false;
}
//-----------------------------------------------------------------------------
bool Shape::PointIntriangle(glm::vec2& pt, glm::vec2& t1, glm::vec2& t2, glm::vec2& t3)
{
	bool b1 = InSameSide(pt, t1, t2);
	bool b2 = InSameSide(pt, t2, t3);
	bool b3 = InSameSide(pt, t3, t1);
	return b1 && b2 && b3;
}
//-----------------------------------------------------------------------------
bool Shape::InSameSide(glm::vec2& pt, glm::vec2& t1, glm::vec2& t2)
{
	float v = ((t2.y - t1.y) * (pt.x - t1.x) + (-t2.x + t1.x) * (pt.y - t1.y));
	return v >= 0.0f;
}
//-----------------------------------------------------------------------------
bool Shape::CircleContains(float x, float y, float r, glm::vec2& pt)
{
	return sqrt(pow(x - pt.x, 2) + pow(y - pt.y, 2)) <= r;
}
//-----------------------------------------------------------------------------
bool Shape::BoxContains(float x, float y, float w, float h, glm::vec2& pt)
{
	return (pt.x > x && pt.x < x + w && pt.y > y && pt.y < y + h);
}
//-----------------------------------------------------------------------------