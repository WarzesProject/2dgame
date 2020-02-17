#pragma once

enum class TextureFilter
{
	Point,
	Linear
};

struct ApplicationConfig
{
	// app config
	struct App
	{
		bool enableConsole = true;
	} app;

	// window config
	struct Window
	{
		int width = 640;
		int height = 480;
		const char *title = "Test";
		bool fullscreen = false;
		bool windowedFullscreen = true;
		bool resizable = false;
		bool highDPI = false;
	} window;

	// graphics config
	struct Graphics
	{		
		float maxFPS = 60.0f;

		TextureFilter textureFilter = TextureFilter::Point;
	} graphics;

	// audio config
	struct Audio
	{

	} audio;

	struct Resources
	{
		const char *coreFolder = "";
	} resources;
};