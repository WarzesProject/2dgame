#pragma once

class Texture
{
public:
	Texture() = default;
	Texture(int width, int height);
	Texture(std::string_view filePath);
	Texture(const Texture&) = default;
	Texture(Texture&&) = default;

	Texture& operator=(const Texture&) = default;
	Texture& operator=(Texture&&) = default;

	void Create(int width, int height);
	void Load(std::string_view filePath);
	void Close();

	bool IsValid() const { return m_width > 0 && m_height > 0 && m_id != 0; }

	GLuint ID() const { return m_id; }

private:
	std::string m_filePath;
	int m_width = 0;
	int m_height = 0;
	GLuint m_id = 0;
};