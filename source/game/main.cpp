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
		config.resources.coreFolder = "";

		Application app(config);

		auto g = Application::Get();


		app.Run(std::make_unique<Game>());
	}
	catch ( const ExceptionSDL &exc )
	{
		std::cerr << exc.what() << std::endl;
		system("PAUSE");
	}
	catch ( const ExceptionSDLAudio &exc )
	{
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Fatal Error", exc.what(), nullptr);
	}
	catch ( const ExceptionApp &exc )
	{
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Fatal Error", exc.what(), nullptr);
	}
	catch( const Exception &exc )
	{
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Fatal Error", exc.what(), nullptr);		
	}
	return 0;
}
//-----------------------------------------------------------------------------