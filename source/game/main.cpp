#include "stdafx.h"
#include "Application.h"
#include "Game.h"
//-----------------------------------------------------------------------------
#pragma comment( lib, "SDL2.lib" )
#pragma comment( lib, "SDL2main.lib" )
#pragma comment( lib, "SDL2_mixer.lib" )
#if RENDER_VULKAN
#	pragma comment( lib, "vulkan-1.lib" )
#endif
//-----------------------------------------------------------------------------
int main([[maybe_unused]] int argc, [[maybe_unused]] char **args)
{
	try
	{
		ApplicationConfig config;
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