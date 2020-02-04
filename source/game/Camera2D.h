﻿#pragma once

class Camera2D
{
public:
	void Init(float screenWidth, float screenHeight);

	void Update();

	glm::vec2 ConvertScreenToWorld(glm::vec2 screenCoords);

	bool IsBoxInView(const glm::vec4 &destRect);
	bool IsBoxInView(const glm::vec2 &position);
	bool IsBoxInView(const glm::vec2 &position, const glm::vec2 &dimension);

	void OffsetPosition(const glm::vec2 &offset)
	{
		m_position += offset; m_needsMatrixUpdate = true;
	}

	void OffsetScale(float offset)
	{
		m_scale += offset; 
		if ( m_scale < 0.001f ) 
			m_scale = 0.001f; 
		m_needsMatrixUpdate = true;
	}

	void SetScreenBox(glm::vec4 &box);
	void SetScale(float scale)
	{
		m_scale = scale; m_needsMatrixUpdate = true;
	}
	void SetPosition(const glm::vec2 &position)
	{
		m_position = position; m_needsMatrixUpdate = true;
	}

	double GetScreenLenght() const;
	float GetScale() const
	{
		return m_scale;
	}
	float GetScreenWidth() const
	{
		return m_screenWidth;
	}
	float GetScreenHeight() const
	{
		return m_screenHeight;
	}
	const glm::vec2& GetPosition() const
	{
		return m_position;
	}
	const glm::mat4& GetCameraMatrix() const
	{
		return m_cameraMatrix;
	}
	float GetAspectRatio() const
	{
		return m_screenWidth / m_screenHeight;
	}

private:
	float m_screenWidth = 100.0f;
	float m_screenHeight = 100.0f;
	float m_scale = 1.0f;
	glm::vec2 m_position = { 0.0f, 0.0f };
	glm::mat4 m_cameraMatrix = glm::mat4(1.0f);
	glm::mat4 m_orthoMatrix = glm::mat4(1.0f);
	bool m_needsMatrixUpdate = true;
};