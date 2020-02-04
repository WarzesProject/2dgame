#include "stdafx.h"
#include "SpriteBatch.h"

//-----------------------------------------------------------------------------
enum Corner
{
	BL = 0,		// Bottom left
	BR = 1,		// Bottom right
	TR = 2,		// Top right
	TL = 3		// Top left
};
//-----------------------------------------------------------------------------
Glyph::Glyph(const glm::vec4 &destRect, const glm::vec4& uvRect, GLuint textureID, float depth, const ColorRGBA8 &color, float angle)
{
	this->textureID = textureID;
	this->depth = depth;

	glm::vec2 halfDims(destRect.z / 2.0f, destRect.w / 2.0f);

	// Get points centered at origin
	glm::vec2 tl(-halfDims.x, halfDims.y); // top left point
	glm::vec2 bl(-halfDims.x, -halfDims.y); // bottom left point
	glm::vec2 br(halfDims.x, -halfDims.y); // bottom right point
	glm::vec2 tr(halfDims.x, halfDims.y); // top right point

	// Rotate the points
	tl = rotatePoint(tl, angle) + halfDims;
	bl = rotatePoint(bl, angle) + halfDims;
	br = rotatePoint(br, angle) + halfDims;
	tr = rotatePoint(tr, angle) + halfDims;

	this->topLeft.color = color;
	this->topLeft.SetPosition(destRect.x + tl.x, destRect.y + tl.y);
	this->topLeft.SetUV(uvRect.x, uvRect.y + uvRect.w);

	this->bottomLeft.color = color;
	this->bottomLeft.SetPosition(destRect.x + bl.x, destRect.y + bl.y);
	this->bottomLeft.SetUV(uvRect.x, uvRect.y);

	this->bottomRight.color = color;
	this->bottomRight.SetPosition(destRect.x + br.x, destRect.y + br.y);
	this->bottomRight.SetUV(uvRect.x + uvRect.z, uvRect.y);

	this->topRight.color = color;
	this->topRight.SetPosition(destRect.x + tr.x, destRect.y + tr.y);
	this->topRight.SetUV(uvRect.x + uvRect.z, uvRect.y + uvRect.w);
}
//-----------------------------------------------------------------------------
Glyph::Glyph(const glm::vec2 &position, const glm::vec2 &dimensions, const glm::vec4& uvRect, GLuint textureID, float depth, const ColorRGBA8 &color, float angle)
{
	this->textureID = textureID;
	this->depth = depth;

	glm::vec2 halfDims(dimensions.x / 2.0f, dimensions.y / 2.0f);

	// Get points centered at origin
	glm::vec2 tl(-halfDims.x, halfDims.y); // top left point
	glm::vec2 bl(-halfDims.x, -halfDims.y); // bottom left point
	glm::vec2 br(halfDims.x, -halfDims.y); // bottom right point
	glm::vec2 tr(halfDims.x, halfDims.y); // top right point

	// Rotate the points
	tl = rotatePoint(tl, angle) + halfDims;
	bl = rotatePoint(bl, angle) + halfDims;
	br = rotatePoint(br, angle) + halfDims;
	tr = rotatePoint(tr, angle) + halfDims;

	this->topLeft.color = color;
	this->topLeft.SetPosition(position.x + tl.x, position.y + tl.y);
	this->topLeft.SetUV(uvRect.x, uvRect.y + uvRect.w);

	this->bottomLeft.color = color;
	this->bottomLeft.SetPosition(position.x + bl.x, position.y + bl.y);
	this->bottomLeft.SetUV(uvRect.x, uvRect.y);

	this->bottomRight.color = color;
	this->bottomRight.SetPosition(position.x + br.x, position.y + br.y);
	this->bottomRight.SetUV(uvRect.x + uvRect.z, uvRect.y);

	this->topRight.color = color;
	this->topRight.SetPosition(position.x + tr.x, position.y + tr.y);
	this->topRight.SetUV(uvRect.x + uvRect.z, uvRect.y + uvRect.w);
}//-----------------------------------------------------------------------------
Glyph::Glyph(const glm::vec4& destRet, const glm::vec4& uvRect, GLuint textureID, float depth, const ColorRGBA8 &color)
{
	this->textureID = textureID;
	this->depth = depth;

	this->topLeft.color = color;
	this->topLeft.SetPosition(destRet.x, destRet.y + destRet.w);
	this->topLeft.SetUV(uvRect.x, uvRect.y + uvRect.w);

	this->bottomLeft.color = color;
	this->bottomLeft.SetPosition(destRet.x, destRet.y);
	this->bottomLeft.SetUV(uvRect.x, uvRect.y);

	this->bottomRight.color = color;
	this->bottomRight.SetPosition(destRet.x + destRet.z, destRet.y);
	this->bottomRight.SetUV(uvRect.x + uvRect.z, uvRect.y);

	this->topRight.color = color;
	this->topRight.SetPosition(destRet.x + destRet.z, destRet.y + destRet.w);
	this->topRight.SetUV(uvRect.x + uvRect.z, uvRect.y + uvRect.w);
}
//-----------------------------------------------------------------------------
Glyph::Glyph(const glm::vec2 &position, const glm::vec2 &dimensions, const glm::vec4& uvRect, GLuint textureID, float depth, const ColorRGBA8 &color)
{
	this->textureID = textureID;
	this->depth = depth;

	this->topLeft.color = color;
	this->topLeft.SetPosition(position.x, position.y + dimensions.y);
	this->topLeft.SetUV(uvRect.x, uvRect.y + uvRect.w);

	this->bottomLeft.color = color;
	this->bottomLeft.SetPosition(position.x, position.y);
	this->bottomLeft.SetUV(uvRect.x, uvRect.y);

	this->bottomRight.color = color;
	this->bottomRight.SetPosition(position.x + dimensions.x, position.y);
	this->bottomRight.SetUV(uvRect.x + uvRect.z, uvRect.y);

	this->topRight.color = color;
	this->topRight.SetPosition(position.x + dimensions.x, position.y + dimensions.y);
	this->topRight.SetUV(uvRect.x + uvRect.z, uvRect.y + uvRect.w);
}
//-----------------------------------------------------------------------------
Glyph::Glyph(const std::vector<glm::vec2> &points, const glm::vec4& uvRect, GLuint textureID, float depth, const ColorRGBA8 &color)
{
	this->textureID = textureID;
	this->depth = depth;

	this->topLeft.color = color;
	this->topLeft.SetPosition(points[TL].x, points[TL].y);
	this->topLeft.SetUV(uvRect.x, uvRect.y + uvRect.w);

	this->bottomLeft.color = color;
	this->bottomLeft.SetPosition(points[BL].x, points[BL].y);
	this->bottomLeft.SetUV(uvRect.x, uvRect.y);

	this->bottomRight.color = color;
	this->bottomRight.SetPosition(points[BR].x, points[BR].y);
	this->bottomRight.SetUV(uvRect.x + uvRect.z, uvRect.y);

	this->topRight.color = color;
	this->topRight.SetPosition(points[TR].x, points[TR].y);
	this->topRight.SetUV(uvRect.x + uvRect.z, uvRect.y + uvRect.w);
}
//-----------------------------------------------------------------------------
glm::vec2 Glyph::rotatePoint(glm::vec2 position, float angle)
{
	glm::vec2 newPoint;

	newPoint.x = position.x * cos(angle) - position.y * sin(angle);
	newPoint.y = position.x * sin(angle) + position.y * cos(angle);
	return newPoint;
}
//-----------------------------------------------------------------------------
void SpriteBatch::Init()
{
	createVertexArray();
}
//-----------------------------------------------------------------------------
void SpriteBatch::Dispose()
{
	if ( m_vao != 0 )
	{
		glDeleteVertexArrays(1, &m_vao);
		m_vao = 0;
	}
	if ( m_vbo != 0 )
	{
		glDeleteBuffers(1, &m_vbo);
		m_vbo = 0;
	}
}
//-----------------------------------------------------------------------------
void SpriteBatch::Begin(GlyphSortType sortType)
{
	m_sortType = sortType;
	m_renderBatches.clear();
	m_glyphs.clear();
}
//-----------------------------------------------------------------------------
void SpriteBatch::End()
{
	m_glyphsPointers.resize(m_glyphs.size());

	for ( size_t i = 0; i < m_glyphs.size(); i++ )
	{
		m_glyphsPointers[i] = &m_glyphs[i];
	}

	sortGlyph();
	createRenderBatches();
}
//-----------------------------------------------------------------------------
void SpriteBatch::Draw(const glm::vec4& destRet, const glm::vec4& uvRect, GLuint textureID, float depth, const ColorRGBA8 &color)
{
	m_glyphs.emplace_back(destRet, uvRect, textureID, depth, color);
}
//-----------------------------------------------------------------------------
void SpriteBatch::Draw(const glm::vec2 &position, const glm::vec2 &dimensions, const glm::vec4& uvRect, GLuint textureID, float depth, const ColorRGBA8 &color)
{
	m_glyphs.emplace_back(position, dimensions, uvRect, textureID, depth, color);
}
//-----------------------------------------------------------------------------
void SpriteBatch::Draw(const glm::vec4& destRet, const glm::vec4& uvRect, GLuint textureID, float depth, const ColorRGBA8 &color, float angle)
{
	m_glyphs.emplace_back(destRet, uvRect, textureID, depth, color, angle);
}
//-----------------------------------------------------------------------------
void SpriteBatch::Draw(const glm::vec2 &position, const glm::vec2 &dimensions, const glm::vec4& uvRect, GLuint textureID, float depth, const ColorRGBA8 &color, float angle)
{
	m_glyphs.emplace_back(position, dimensions, uvRect, textureID, depth, color, angle);
}
//-----------------------------------------------------------------------------
void SpriteBatch::Draw(const std::vector<glm::vec2> &points, const glm::vec4& uvRect, GLuint textureID, float depth, const ColorRGBA8 &color)
{
	m_glyphs.emplace_back(points, uvRect, textureID, depth, color);
}
//-----------------------------------------------------------------------------
void SpriteBatch::Draw(const glm::vec4& destRet, const glm::vec4& uvRect, GLuint textureID, float depth, const ColorRGBA8 &color, const glm::vec2 &direction)
{
	// Here, we use dot product to compute the angle between 
	// the origine direction and current direction.
	// we now that right and direction vector are normalized
	// formula : a . b = ||a|| ||b|| cos teta, ||a|| = 1 and ||b|| = 1, so ||a|| * ||b|| = 1

	const glm::vec2 right(1.0f, 0.0f);  // Origine direction
	float angle = acos(glm::dot(right, direction));
	// acos return result in [0, pi]. So, for the angles great than 180 in degree or pi in radiant,
	// we keep the negative of angle
	if ( direction.y < 0.0f )
		angle = -angle;

	m_glyphs.emplace_back(destRet, uvRect, textureID, depth, color, angle);
}
//-----------------------------------------------------------------------------
void SpriteBatch::RenderBatch()
{
	glBindVertexArray(m_vao);

	for ( size_t i = 0; i < m_renderBatches.size(); i++ )
	{
		glBindTexture(GL_TEXTURE_2D, m_renderBatches[i].textureID);
		glDrawArrays(GL_TRIANGLES, m_renderBatches[i].offset, m_renderBatches[i].numVertices);
	}

	glBindVertexArray(0);
}
//-----------------------------------------------------------------------------
void SpriteBatch::createVertexArray()
{
	if ( m_vao == 0 )
	{
		glGenVertexArrays(1, &m_vao); // generate vertex arrays
	}
	glBindVertexArray(m_vao);


	if ( m_vbo == 0 )
	{
		glGenBuffers(1, &m_vbo); // generate buffer object
	}
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);

	// this is the position attribute pointer
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex2D), (void*)offsetof(Vertex2D, position));
	// this is the color attribute pointer
	glVertexAttribPointer(1, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(Vertex2D), (void*)offsetof(Vertex2D, color));
	// this is the uv attribute pointer
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex2D), (void*)offsetof(Vertex2D, uv));

	glBindVertexArray(0);
}
//-----------------------------------------------------------------------------
void SpriteBatch::createRenderBatches()
{
	if ( m_glyphsPointers.empty() )
	{
		return;
	}

	std::vector<Vertex2D> vertices(m_glyphsPointers.size() * 6);
	int offset = 0;
	int cv = 0; // current vertex

	m_renderBatches.emplace_back(offset, 6, m_glyphsPointers[0]->textureID);
	vertices[cv++] = m_glyphsPointers[0]->topLeft;
	vertices[cv++] = m_glyphsPointers[0]->bottomLeft;
	vertices[cv++] = m_glyphsPointers[0]->bottomRight;
	vertices[cv++] = m_glyphsPointers[0]->bottomRight;
	vertices[cv++] = m_glyphsPointers[0]->topRight;
	vertices[cv++] = m_glyphsPointers[0]->topLeft;
	offset += 6;

	for ( size_t i = 1; i < m_glyphsPointers.size(); i++ )
	{
		if ( m_glyphsPointers[i]->textureID != m_glyphsPointers[i - 1]->textureID )
		{
			m_renderBatches.emplace_back(offset, 6, m_glyphsPointers[i]->textureID);
		}
		else
		{
			m_renderBatches.back().numVertices += 6;
		}

		vertices[cv++] = m_glyphsPointers[i]->topLeft;
		vertices[cv++] = m_glyphsPointers[i]->bottomLeft;
		vertices[cv++] = m_glyphsPointers[i]->bottomRight;
		vertices[cv++] = m_glyphsPointers[i]->bottomRight;
		vertices[cv++] = m_glyphsPointers[i]->topRight;
		vertices[cv++] = m_glyphsPointers[i]->topLeft;
		offset += 6;
	}

	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex2D), nullptr, GL_DYNAMIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, vertices.size() * sizeof(Vertex2D), vertices.data());

	glBindBuffer(GL_ARRAY_BUFFER, 0);
}
//-----------------------------------------------------------------------------
void SpriteBatch::sortGlyph()
{
	switch ( m_sortType )
	{
	case GlyphSortType::BACK_TO_FRONT:
		std::stable_sort(m_glyphsPointers.begin(), m_glyphsPointers.end(), compareBackToFront);
		break;
	case GlyphSortType::FRONT_TO_BACK:
		std::stable_sort(m_glyphsPointers.begin(), m_glyphsPointers.end(), compareFrontToBack);
		break;
	case GlyphSortType::TEXTURE:
		std::stable_sort(m_glyphsPointers.begin(), m_glyphsPointers.end(), compareTexture);
		break;
	}
}
//-----------------------------------------------------------------------------
bool SpriteBatch::compareBackToFront(Glyph* a, Glyph* b)
{
	return (a->textureID < b->textureID);
}
//-----------------------------------------------------------------------------
bool SpriteBatch::compareFrontToBack(Glyph* a, Glyph* b)
{
	return (a->textureID > b->textureID);
}
//-----------------------------------------------------------------------------
bool SpriteBatch::compareTexture(Glyph* a, Glyph* b)
{
	return (a->depth < b->depth);
}
//-----------------------------------------------------------------------------