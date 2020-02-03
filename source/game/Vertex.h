#pragma once

struct Position
{
	float x = 0.0f;
	float y = 0.0f; 
};

struct  ColorRGBA8
{
	ColorRGBA8() = default;
	ColorRGBA8(GLubyte r, GLubyte g, GLubyte b, GLubyte a) : r(r), g(g), b(b), a(a)	{}
	
	void SetColor(GLubyte rr, GLubyte gg, GLubyte bb, GLubyte aa)
	{
		r = rr;
		g = gg;
		b = bb;
		a = aa;
	}

	GLubyte r = 0;
	GLubyte g = 0;
	GLubyte b = 0;
	GLubyte a = 0;
};

struct UV
{
	float u = 0.0f;
	float v = 0.0f;
};

struct Vertex2D
{
	void SetColor(GLubyte r, GLubyte g, GLubyte b, GLubyte a)
	{
		color.r = r;
		color.g = g;
		color.b = b;
		color.a = a;
	}

	void SetPosition(float x, float y)
	{
		position.x = x;
		position.y = y;
	}

	void SetUV(float u, float v)
	{
		uv.u = u;
		uv.v = v;
	}

	Position position;
	ColorRGBA8 color;
	UV uv;
};