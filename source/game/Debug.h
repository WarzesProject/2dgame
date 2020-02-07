#pragma once

class Exception : public std::exception
{
public:
	Exception(std::string_view str) : std::exception(str.data()) {}
};

template<typename T>
constexpr auto Throw(T ex) { return throw Exception(ex); }

inline void ThrowSDLError(const char *str)
{
	Throw(str + std::string(SDL_GetError()));
}

inline void ThrowSDLMixError(const char *str)
{
	Throw(str + std::string(Mix_GetError()));
}