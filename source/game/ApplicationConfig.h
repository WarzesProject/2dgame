#pragma once

enum class RenderType
{
#if RENDER_VULKAN
	Vulkan,
#endif
#if RENDER_OPENGL
	OpenGL,
#endif
};

struct ApplicationConfig
{
	// window config
	int width = 640;
	int height = 480;
	const char *title = "Test";
	bool fullscreen = false;
	bool windowedFullscreen = true;
	bool resizable = false;
	bool highDPI = false;

	// render config
	RenderType renderType = RenderType::OpenGL;
	float MaxFPS = 60.0f;
};