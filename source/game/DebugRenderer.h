#pragma once

#include "Vertex.h"
#include "GLSLProgram.h"

class DebugRenderer
{
public:
	DebugRenderer();
	~DebugRenderer();

	void Init();
	void End();

	void DrawLine(const glm::vec2& a, const glm::vec2& b, const ColorRGBA8& color);
	void DrawPolygon(const std::vector<glm::vec2>& points, const ColorRGBA8& color);
	void DrawPolygon(const std::vector<glm::vec2>& points, const ColorRGBA8& color, float angle);

	void DrawBox(const glm::vec4& destRect, const ColorRGBA8& color);
	void DrawBox(const glm::vec4& destRect, const ColorRGBA8& color, float angle);
	void DrawBox(const glm::vec4& destRect, const ColorRGBA8& color, const glm::vec2& direction);
	void DrawBox(const glm::vec2& position, const glm::vec2& dimensions, const ColorRGBA8& color);
	void DrawBox(const glm::vec2& position, const glm::vec2& dimensions, const ColorRGBA8& color, float angle);

	void DrawCircle(const glm::vec2& center, const ColorRGBA8& color, float radius);

	void Render(const glm::mat4& projectionMatrix, float lineWith);

	void Dispose();

	struct DebugVertex
	{
		glm::vec2 position;
		ColorRGBA8 color;
	};

private:
	GLSLProgram m_program;
	std::vector<DebugVertex> m_vertices;
	std::vector<GLuint> m_indices;
	GLuint m_vbo = 0, m_vao = 0, m_ibo = 0;	
	int m_numElements = 0;
};