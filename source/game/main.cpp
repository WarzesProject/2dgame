#include "stdafx.h"
#include "Application.h"
#include "Game.h"
//-----------------------------------------------------------------------------
#pragma comment( lib, "SDL2.lib" )
#pragma comment( lib, "SDL2main.lib" )
#pragma comment( lib, "SDL2_mixer.lib" )
#pragma comment( lib, "SDL2_ttf.lib" )
//-----------------------------------------------------------------------------
int main([[maybe_unused]] int argc, [[maybe_unused]] char **args)
{
	try
	{
		ApplicationConfig config;
		config.window.width = 1024;
		config.window.height = 768;
        config.window.title = "Game";
		config.graphics.maxFPS = 60;
		config.resources.coreFolder = "../";
        
		Application app(config);
		app.Run(std::make_unique<Game>());
	}
	catch( const Exception &exc )
	{
        std::cerr << exc.what() << std::endl;
        system("PAUSE");
	}

	return 0;
}
//-----------------------------------------------------------------------------