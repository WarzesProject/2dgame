#pragma once

class Exception : public std::exception
{
public:
	Exception(std::string_view str) 
		: std::exception(str.data())
	{
	}
};

class ExceptionApp : public std::exception
{
public:
	ExceptionApp(std::string_view str)
		: std::exception(str.data())
	{
	}
};

// исключение в случае ошибок SDL
class ExceptionSDL : public ExceptionApp
{
public:
	ExceptionSDL(std::string_view str) 
		: ExceptionApp(str.data() + std::string(SDL_GetError()))
	{
	}
};

// исключение в случае ошибок SDL Audio
class ExceptionSDLAudio : public ExceptionApp
{
public:
	ExceptionSDLAudio(std::string_view str)
		: ExceptionApp(str.data() + std::string(Mix_GetError()))
	{
	}
};

template<typename T>
constexpr auto Throw(T ex) { return throw Exception(ex); }

template<typename T>
constexpr auto ThrowSDLError(T ex) { return throw ExceptionSDL(ex); }

template<typename T>
constexpr auto ThrowSDLMixError(T ex) { return throw ExceptionSDLAudio(ex); }