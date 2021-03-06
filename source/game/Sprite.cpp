#include "stdafx.h"
#include "Sprite.h"
#include "ResourceManager.h"
//-----------------------------------------------------------------------------
Sprite::~Sprite()
{
	if ( m_vboID != 0 )
		glDeleteBuffers(1, &m_vboID);
}
//-----------------------------------------------------------------------------
void Sprite::Init(float x, float y, float width, float height, std::string_view texturePath)
{
	m_x = x;
	m_y = y;
	m_width = width;
	m_height = height;

	m_texture = ResourceManager::GetTexture(texturePath);

	if ( m_vboID == 0 )
		glGenBuffers(1, &m_vboID);

	Vertex2D vertexData[6];

	vertexData[0].SetPosition(x + width, y + height);
	vertexData[0].SetUV(1.0f, 1.0f);

	vertexData[1].SetPosition(x, y + height);
	vertexData[1].SetUV(0.0f, 1.0f);

	vertexData[2].SetPosition(x, y);
	vertexData[2].SetUV(0.0f, 0.0f);

	vertexData[3].SetPosition(x, y);
	vertexData[3].SetUV(0.0f, 0.0f);

	vertexData[4].SetPosition(x + width, y);
	vertexData[4].SetUV(1.0f, 0.0f);

	vertexData[5].SetPosition(x + width, y + height);
	vertexData[5].SetUV(1.0f, 1.0f);

	for ( size_t i = 0; i < 6; i++ )
		vertexData[i].SetColor(255, 255, 255, 255);

	glBindBuffer(GL_ARRAY_BUFFER, m_vboID);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertexData), vertexData, GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
}
//-----------------------------------------------------------------------------
void Sprite::Draw()
{
	//bind the texture
	glBindTexture(GL_TEXTURE_2D, m_texture.ID());

	// bind the buffer object
	glBindBuffer(GL_ARRAY_BUFFER, m_vboID);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);

	// this is the position attribute pointer
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex2D), (void*)offsetof(Vertex2D, position));
	// this is the color attribute pointer
	glVertexAttribPointer(1, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(Vertex2D), (void*)offsetof(Vertex2D, color));
	// this is the uv attribute pointer
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex2D), (void*)offsetof(Vertex2D, uv));

	glDrawArrays(GL_TRIANGLES, 0, 6);

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
}
//-----------------------------------------------------------------------------