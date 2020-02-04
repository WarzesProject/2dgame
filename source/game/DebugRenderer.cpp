#include "stdafx.h"
#include "DebugRenderer.h"
//-----------------------------------------------------------------------------
constexpr float PI = 3.14159265359f;
//-----------------------------------------------------------------------------
constexpr const char *VERTEX_SRC = R"(
	#version 130

	in vec2 vertexPosition;
	in vec4 vertexColor;

	out vec4 fragmentColor;
	out vec2 fragmentPosition;

	uniform mat4 cameraMatrix;

	void main()
	{
		//Set the x,y position on the screen
		gl_Position.xy = (cameraMatrix * vec4(vertexPosition, 0.0, 1.0)).xy;
		gl_Position.z = 0.0;
		//Indicate that the coordinates are normalized
		gl_Position.w = 1.0;

		fragmentColor = vertexColor;
		fragmentPosition = vertexPosition;
	}
)";

//-----------------------------------------------------------------------------
constexpr const char *FRAGMENT_SRC = R"(
	#version 130

	in vec4 fragmentColor;
	in vec2 fragmentPosition;

	out vec4 color;

	void main() 
	{	
		color = fragmentColor;
	}
)";
//-----------------------------------------------------------------------------
DebugRenderer::~DebugRenderer()
{
	Dispose();
}
//-----------------------------------------------------------------------------
void DebugRenderer::Init()
{
	// Shader init
	m_program.CompileShadersFromSource(VERTEX_SRC, FRAGMENT_SRC);
	m_program.AddAttribute("vertexPosition");
	m_program.AddAttribute("vertexColor");
	m_program.LinkShaders();

	// Set up buffers
	glGenVertexArrays(1, &m_vao);
	glGenBuffers(1, &m_vbo);
	glGenBuffers(1, &m_ibo);

	glBindVertexArray(m_vao);
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ibo);

	// Set up attributes
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);

	// this is the position attribute pointer
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(DebugVertex), (void*)offsetof(DebugVertex, position));
	// this is the color attribute pointer
	glVertexAttribPointer(1, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(DebugVertex), (void*)offsetof(DebugVertex, color));

	glBindVertexArray(0);
}
//-----------------------------------------------------------------------------
void DebugRenderer::End()
{
	const GLsizeiptr verticesSize = static_cast<GLsizeiptr>(m_vertices.size() * sizeof(DebugVertex));
	const GLsizeiptr indicesSize = static_cast<GLsizeiptr>(m_indices.size() * sizeof(GLuint));

	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	glBufferData(GL_ARRAY_BUFFER, verticesSize, nullptr, GL_DYNAMIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, verticesSize, m_vertices.data());
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indicesSize, nullptr, GL_DYNAMIC_DRAW);
	glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, indicesSize, m_indices.data());
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	m_numElements = m_indices.size();
	m_indices.clear();
	m_vertices.clear();
}
//-----------------------------------------------------------------------------
glm::vec2 rotatePoint(const glm::vec2 &position, float angle)
{
	glm::vec2 newPoint;
	newPoint.x = position.x * cos(angle) - position.y * sin(angle);
	newPoint.y = position.x * sin(angle) + position.y * cos(angle);
	return newPoint;
}
//-----------------------------------------------------------------------------
void DebugRenderer::DrawLine(const glm::vec2 &a, const glm::vec2 &b, const ColorRGBA8 &color)
{
	size_t i = m_vertices.size();
	m_vertices.resize(m_vertices.size() + 2);

	m_vertices[i].position = a;
	m_vertices[i].color = color;
	m_vertices[i + 1].position = b;
	m_vertices[i + 1].color = color;

	m_indices.push_back(i);
	m_indices.push_back(i + 1);
}
//-----------------------------------------------------------------------------
void DebugRenderer::DrawPolygon(const std::vector<glm::vec2> &points, const ColorRGBA8 &color)
{
	// Set up vertexes
	size_t start = m_vertices.size();
	size_t size = points.size();
	m_vertices.resize(m_vertices.size() + size);

	for( size_t i = 0; i < size; i++ )
	{
		m_vertices[start + i].position = points[i];
		m_vertices[start + i].color = color;
	}

	// Set up indices
	m_indices.reserve(m_indices.size() + size * 2);
	for( size_t i = 0; i < size - 1; i++ )
	{
		m_indices.push_back(start + i);
		m_indices.push_back(start + i + 1);
	}

	m_indices.push_back(start + size - 1);
	m_indices.push_back(start);
}
//-----------------------------------------------------------------------------
void DebugRenderer::DrawPolygon(const std::vector<glm::vec2> &points, const ColorRGBA8 &color, float angle)
{
	// Set up vertexes
	size_t start = m_vertices.size();
	size_t size = points.size();
	m_vertices.resize(m_vertices.size() + size);

	for( size_t i = 0; i < size; i++ )
	{
		m_vertices[start + i].position = rotatePoint(points[i], angle);
		m_vertices[start + i].color = color;
	}

	// Set up indices
	m_indices.reserve(m_indices.size() + size * 2);
	for( size_t i = 0; i < size - 1; i++ )
	{
		m_indices.push_back(start + i);
		m_indices.push_back(start + i + 1);
	}

	m_indices.push_back(start + size - 1);
	m_indices.push_back(start);
}
//-----------------------------------------------------------------------------
void DebugRenderer::DrawBox(const glm::vec4 &destRect, const ColorRGBA8 &color)
{
	size_t i = m_vertices.size();
	m_vertices.resize(m_vertices.size() + 4);

	m_vertices[i].position.x = destRect.x;
	m_vertices[i].position.y = destRect.y + destRect.w;
	m_vertices[i + 1].position.x = destRect.x;
	m_vertices[i + 1].position.y = destRect.y;
	m_vertices[i + 2].position.x = destRect.x + destRect.z;
	m_vertices[i + 2].position.y = destRect.y;
	m_vertices[i + 3].position.x = destRect.x + destRect.z;
	m_vertices[i + 3].position.y = destRect.y + destRect.w;

	for( size_t j = i; j < i + 4; j++ )
	{
		m_vertices[j].color = color;
	}

	m_indices.reserve(m_indices.size() + 8);

	m_indices.push_back(i);
	m_indices.push_back(i + 1);

	m_indices.push_back(i + 1);
	m_indices.push_back(i + 2);

	m_indices.push_back(i + 2);
	m_indices.push_back(i + 3);

	m_indices.push_back(i + 3);
	m_indices.push_back(i);
}
//-----------------------------------------------------------------------------
void DebugRenderer::DrawBox(const glm::vec2 &position, const glm::vec2 &dimensions, const ColorRGBA8 &color)
{
	size_t i = m_vertices.size();
	m_vertices.resize(m_vertices.size() + 4);

	m_vertices[i].position.x = position.x;
	m_vertices[i].position.y = position.y + dimensions.y;
	m_vertices[i + 1].position.x = position.x;
	m_vertices[i + 1].position.y = position.y;
	m_vertices[i + 2].position.x = position.x + dimensions.x;
	m_vertices[i + 2].position.y = position.y;
	m_vertices[i + 3].position.x = position.x + dimensions.x;
	m_vertices[i + 3].position.y = position.y + dimensions.y;

	for( size_t j = i; j < i + 4; j++ )
		m_vertices[j].color = color;

	m_indices.reserve(m_indices.size() + 8);

	m_indices.push_back(i);
	m_indices.push_back(i + 1);

	m_indices.push_back(i + 1);
	m_indices.push_back(i + 2);

	m_indices.push_back(i + 2);
	m_indices.push_back(i + 3);

	m_indices.push_back(i + 3);
	m_indices.push_back(i);
}
//-----------------------------------------------------------------------------
void DebugRenderer::DrawBox(const glm::vec4 &destRect, const ColorRGBA8 &color, float angle)
{
	size_t i = m_vertices.size();
	m_vertices.resize(m_vertices.size() + 4);

	glm::vec2 halfDims(destRect.z / 2.0f, destRect.w / 2.0f);

	// Get points centered at origin
	glm::vec2 tl(-halfDims.x, halfDims.y); // top left point
	glm::vec2 bl(-halfDims.x, -halfDims.y); // bottom left point
	glm::vec2 br(halfDims.x, -halfDims.y); // bottom right point
	glm::vec2 tr(halfDims.x, halfDims.y); // top right point

	glm::vec2 positionOffset(destRect.x, destRect.y);

	// Rotate the points
	m_vertices[i].position = rotatePoint(tl, angle) + halfDims + positionOffset;
	m_vertices[i + 1].position = rotatePoint(bl, angle) + halfDims + positionOffset;
	m_vertices[i + 2].position = rotatePoint(br, angle) + halfDims + positionOffset;
	m_vertices[i + 3].position = rotatePoint(tr, angle) + halfDims + positionOffset;

	for( size_t j = i; j < i + 4; j++ )
		m_vertices[j].color = color;

	m_indices.reserve(m_indices.size() + 8);

	m_indices.push_back(i);
	m_indices.push_back(i + 1);

	m_indices.push_back(i + 1);
	m_indices.push_back(i + 2);

	m_indices.push_back(i + 2);
	m_indices.push_back(i + 3);

	m_indices.push_back(i + 3);
	m_indices.push_back(i);
}
//-----------------------------------------------------------------------------
void DebugRenderer::DrawBox(const glm::vec2 &position, const glm::vec2 &dimensions, const ColorRGBA8 &color, float angle)
{
	size_t i = m_vertices.size();
	m_vertices.resize(m_vertices.size() + 4);

	glm::vec2 halfDims(dimensions.x / 2.0f, dimensions.y / 2.0f);

	// Get points centered at origin
	glm::vec2 tl(-halfDims.x, halfDims.y);  // top left point
	glm::vec2 bl(-halfDims.x, -halfDims.y); // bottom left point
	glm::vec2 br(halfDims.x, -halfDims.y);  // bottom right point
	glm::vec2 tr(halfDims.x, halfDims.y);   // top right point

	glm::vec2 positionOffset(position.x, position.y);

	// Rotate the points
	m_vertices[i].position = rotatePoint(tl, angle) + halfDims + positionOffset;
	m_vertices[i + 1].position = rotatePoint(bl, angle) + halfDims + positionOffset;
	m_vertices[i + 2].position = rotatePoint(br, angle) + halfDims + positionOffset;
	m_vertices[i + 3].position = rotatePoint(tr, angle) + halfDims + positionOffset;

	for( size_t j = i; j < i + 4; j++ )
		m_vertices[j].color = color;

	m_indices.reserve(m_indices.size() + 8);

	m_indices.push_back(i);
	m_indices.push_back(i + 1);

	m_indices.push_back(i + 1);
	m_indices.push_back(i + 2);

	m_indices.push_back(i + 2);
	m_indices.push_back(i + 3);

	m_indices.push_back(i + 3);
	m_indices.push_back(i);
}
//-----------------------------------------------------------------------------
void DebugRenderer::DrawBox(const glm::vec4 &destRect, const ColorRGBA8 &color, const glm::vec2 &direction)
{
	const glm::vec2 right(1.0f, 0.0f);  // Origine direction
	float angle = acos(glm::dot(right, direction));
	if( direction.y < 0.0f )
		angle = -angle;

	DrawBox(destRect, color, angle);
}
//-----------------------------------------------------------------------------
void DebugRenderer::DrawCircle(const glm::vec2 &center, const ColorRGBA8 &color, float radius)
{
	const int NUM_VERTICES = (radius * 4 > 100) ? 100 : (int)radius * 4;

	// Set up vertexes
	size_t start = m_vertices.size();
	m_vertices.resize(m_vertices.size() + NUM_VERTICES);

	for( int i = 0; i < NUM_VERTICES; i++ )
	{
		float angle = ((float)i / NUM_VERTICES) * PI * 2.0f;
		m_vertices[start + i].position.x = cos(angle) * radius + center.x;
		m_vertices[start + i].position.y = sin(angle) * radius + center.y;
		m_vertices[start + i].color = color;
	}

	// Set up indices
	m_indices.reserve(m_indices.size() + NUM_VERTICES * 2);
	for( int i = 0; i < NUM_VERTICES - 1; i++ )
	{
		m_indices.push_back(start + i);
		m_indices.push_back(start + i + 1);
	}

	m_indices.push_back(start + NUM_VERTICES - 1);
	m_indices.push_back(start);
}
//-----------------------------------------------------------------------------
void DebugRenderer::Render(const glm::mat4 &projectionMatrix, float lineWith)
{
	m_program.Use();
	{
		GLint cmLocation = m_program.GetUniformLocation("cameraMatrix");
		glUniformMatrix4fv(cmLocation, 1, GL_FALSE, &(projectionMatrix[0][0]));

		glLineWidth(lineWith);
		glBindVertexArray(m_vao);
		glDrawElements(GL_LINES, static_cast<GLsizei>(m_numElements), GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
	}
	m_program.Unuse();
}
//-----------------------------------------------------------------------------
void DebugRenderer::Dispose()
{
	if( m_vao )
		glDeleteVertexArrays(1, &m_vao);
	if( m_vbo )
		glDeleteBuffers(1, &m_vbo);
	if( m_ibo )
		glDeleteBuffers(1, &m_ibo);

	m_program.Dispose();
}
//-----------------------------------------------------------------------------