#pragma once

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
	float MaxFPS = 60.0f;
};