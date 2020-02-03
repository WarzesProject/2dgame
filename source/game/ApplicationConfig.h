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

	// render config
	RenderType renderType = RenderType::OpenGL;
};