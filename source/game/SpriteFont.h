#pragma once

#include "Vertex.h"

struct Texture;
class SpriteBatch;

struct CharGlyph
{
	char character;
	glm::vec4 uvRect;
	glm::vec2 size;
};

#define FIRST_PRINTABLE_CHAR ((char)32)
#define LAST_PRINTABLE_CHAR ((char)126)

// Different justification modes.
enum class Justification
{
	LEFT, MIDDLE, RIGHT, MIDDLEX, RIGHTX, MIDDLEY, RIGHTY
};

class SpriteFont
{
public:
	void Init(std::string_view font, int size);
	void Init(std::string_view font, int size, char cs, char ce);
	void Dispose();

	int GetFontHeight() const{ return m_fontHeight; }

	glm::vec2 Measure(const char *s);

	void Justify(const char *s, glm::vec2 &position, const glm::vec2 &scaling, Justification just);
	void Justify(const char *s, glm::vec4 &destRect, Justification just);

	void Draw(SpriteBatch &batch, const char *s, const glm::vec2 &position, const glm::vec2 &scaling, float depth, ColorRGBA8 tint, Justification just = Justification::LEFT);
	void Draw(SpriteBatch &batch, const char *s, const glm::vec4 &destRect, float depth, ColorRGBA8 tint, Justification just = Justification::LEFT);

private:
	static std::vector<int>* createRows(glm::ivec4 *rects, int rectsLength, int r, int padding, int &w);

	int m_regStart, m_regLength;
	CharGlyph *m_glyphs;
	int m_fontHeight;
	unsigned int m_texID;
};