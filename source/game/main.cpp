#include "stdafx.h"
#include "Debug.h"
#include "Application.h"
//-----------------------------------------------------------------------------
#pragma comment( lib, "SDL2.lib" )
#pragma comment( lib, "SDL2main.lib" )
//-----------------------------------------------------------------------------
int main([[maybe_unused]] int argc, [[maybe_unused]] char **args)
{
	try
	{
		Application app;
		app.Run();
	}
	catch( const Exception &exc )
	{
		SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, exc.what());
	}

	return 0;
}
//-----------------------------------------------------------------------------