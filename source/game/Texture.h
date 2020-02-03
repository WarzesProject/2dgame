#pragma once

struct Texture
{
	std::string filePath;
	int width;
	int height;
#if RENDER_OPENGL
	GLuint id;
#endif
};