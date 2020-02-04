#pragma once

#include "Vertex.h"

// Determines how we should sort the glyphs
enum class GlyphSortType
{
	NONE, 
	FRONT_TO_BACK, 
	BACK_TO_FRONT, 
	TEXTURE
};

class Glyph
{
public:
	Glyph(const glm::vec4& destRet, const glm::vec4& uvRect, GLuint textureID, float depth, const ColorRGBA8 &color, float angle);
	Glyph(const glm::vec2 &position, const glm::vec2 &dimensions, const glm::vec4& uvRect, GLuint textureID, float depth, const ColorRGBA8 &color, float angle);

	Glyph(const glm::vec4& destRet, const glm::vec4& uvRect, GLuint textureID, float depth, const ColorRGBA8 &color);

	Glyph(const glm::vec2 &position, const glm::vec2 &dimensions, const glm::vec4& uvRect, GLuint textureID, float depth, const ColorRGBA8 &color);

	Glyph(const std::vector<glm::vec2> &points, const glm::vec4& uvRect, GLuint textureID, float depth, const ColorRGBA8 &color);

	GLuint textureID;
	float depth;
	Vertex2D topLeft;
	Vertex2D bottomLeft;
	Vertex2D bottomRight;
	Vertex2D topRight;

private:
	glm::vec2 rotatePoint(glm::vec2 point, float angle);
};

class RenderBatch
{
public:
	RenderBatch(GLuint offset, GLuint numVertices, GLuint textureID)
		: offset(offset)
		, numVertices(numVertices)
		, textureID(textureID)
	{
	};
	GLuint offset;
	GLuint numVertices;
	GLuint textureID;
};

class SpriteBatch
{
public:
	void Init();

	void Dispose();

	void Begin(GlyphSortType sortType = GlyphSortType::TEXTURE);
	void End();

	void Draw(const glm::vec4& destRet, const glm::vec4& uvRect, GLuint textureID, float depth, const ColorRGBA8 &color);
	void Draw(const glm::vec2 &position, const glm::vec2 &dimensions, const glm::vec4& uvRect, GLuint textureID, float depth, const ColorRGBA8 &color);

	void Draw(const glm::vec4& destRet, const glm::vec4& uvRect, GLuint texture, float depth, const ColorRGBA8 &color, float angle);
	void Draw(const glm::vec2 &position, const glm::vec2 &dimensions, const glm::vec4& uvRect, GLuint texture, float depth, const ColorRGBA8 &color, float angle);

	void Draw(const glm::vec4& destRet, const glm::vec4& uvRect, GLuint texture, float depth, const ColorRGBA8 &color, const glm::vec2 &direction);

	void Draw(const std::vector<glm::vec2> &points, const glm::vec4& uvRect, GLuint textureID, float depth, const ColorRGBA8 &color);

	void RenderBatch();

private:
	void createRenderBatches();
	void createVertexArray();
	void sortGlyph();

	static bool compareBackToFront(Glyph* a, Glyph* b);
	static bool compareFrontToBack(Glyph* a, Glyph* b);
	static bool compareTexture(Glyph* a, Glyph* b);

	GlyphSortType m_sortType;
	GLuint m_vbo = 0;
	GLuint m_vao = 0;
	std::vector<Glyph*> m_glyphsPointers;
	std::vector<Glyph> m_glyphs;
	std::vector<::RenderBatch> m_renderBatches;
};