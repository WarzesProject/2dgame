#pragma once

#define SE_STRINGIZE( _n ) SE_STRINGIZE_2( _n )
#define SE_STRINGIZE_2( _n ) #_n
#define TODO( _msg ) __pragma(message("" __FILE__ "(" SE_STRINGIZE(__LINE__) "): TODO: " _msg))

#include "BuildOptions.h"

#pragma warning( disable : 4514 )
#pragma warning( disable : 4820 )
#pragma warning( disable : 5045 )
//#pragma warning( disable : 4996 )

#pragma warning( push, 1 )

//[=========================================================================]
//[ STL Header                                                              ]
//[=========================================================================]
#include <exception>
#include <chrono>
#include <memory>
#include <algorithm>
#include <optional>
#include <iostream>
#include <fstream>
#include <string>
#include <string_view>
#include <vector>
#include <set>
#include <map>
#include <unordered_map>

//[=========================================================================]
//[ 3rd-party                                                               ]
//[=========================================================================]
#define WIN32_LEAN_AND_MEAN 
#define NOMINMAX
#include <SDKDDKVer.h>

#include <SDL2-2.0.10/SDL.h>
#include <SDL2-2.0.10/SDL_syswm.h>
#include <SDL2-2.0.10/SDL_mixer.h>
#include <SDL2-2.0.10/SDL_ttf.h>

#if RENDER_VULKAN
#	include <SDL2-2.0.10/SDL_vulkan.h>
#endif

#if RENDER_VULKAN
#	define VK_USE_PLATFORM_WIN32_KHR
#	include <vulkan/vulkan.hpp>
#endif

#if RENDER_OPENGL
#	include <glad/glad.h>
#endif

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm/glm.hpp>
#include <glm/glm/gtc/matrix_transform.hpp>
//#include <glm/glm/gtx/hash.hpp>

#include <stb/stb_image.h>

//#include <tinyobjloader/tiny_obj_loader.h>

//[=========================================================================]
//[ Core Header                                                             ]
//[=========================================================================]
#include "Debug.h"

// TODO: rename and rebase
#define N_ELEMENTS_IN_ARRAY(ELEMENTS)	sizeof(ELEMENTS) / sizeof(ELEMENTS[0])

#pragma warning( pop )