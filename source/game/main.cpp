#include "stdafx.h"
#include "Application.h"
#include "Game.h"
//-----------------------------------------------------------------------------
#pragma comment( lib, "SDL2.lib" )
#pragma comment( lib, "SDL2main.lib" )
#pragma comment( lib, "SDL2_mixer.lib" )
#pragma comment( lib, "SDL2_ttf.lib" )
#if RENDER_VULKAN
#	pragma comment( lib, "vulkan-1.lib" )
#endif
#if RENDER_OPENGL
#	pragma comment( lib, "OpenGL32.lib" )
#endif

//-----------------------------------------------------------------------------
int main([[maybe_unused]] int argc, [[maybe_unused]] char **args)
{
	try
	{
		ApplicationConfig config;
		config.width = 1500;
		config.height = 800;


		Application app(config);
		app.Run(std::make_unique<Game>());
	}
	catch( const Exception &exc )
	{
		SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, exc.what());
	}

	return 0;
}
//-----------------------------------------------------------------------------