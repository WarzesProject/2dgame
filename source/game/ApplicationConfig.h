#pragma once

enum class RenderType
{
	Vulkan,
	OpenGL
};

struct ApplicationConfig
{
	// window config
	int width = 640;
	int height = 480;
	const char *title = "Test";

	// render config
	RenderType renderType = RenderType::Vulkan;
};