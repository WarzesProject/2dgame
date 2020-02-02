#pragma once

#pragma warning( disable : 4514 )
#pragma warning( disable : 4820 )

#pragma warning( push, 1 )

//[=========================================================================]
//[ STL Header                                                              ]
//[=========================================================================]
#include <exception>
#include <chrono>
#include <memory>
#include <optional>
#include <iostream>
#include <fstream>
#include <string>
#include <string_view>
#include <vector>
#include <set>

//[=========================================================================]
//[ 3rd-party                                                               ]
//[=========================================================================]
#include <SDL2-2.0.10/SDL.h>
#include <SDL2-2.0.10/SDL_syswm.h>
#include <SDL2-2.0.10/SDL_vulkan.h>

#include <vulkan/vulkan.hpp>
#define VK_USE_PLATFORM_WIN32_KHR
#define PLATFORM_SURFACE_EXTENSION_NAME VK_KHR_WIN32_SURFACE_EXTENSION_NAME

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm/glm.hpp>
#include <glm/glm/gtc/matrix_transform.hpp>
#include <glm/glm/gtx/hash.hpp>

#include <stb/stb_image.h>

#include <tinyobjloader/tiny_obj_loader.h>

//[=========================================================================]
//[ Core Header                                                             ]
//[=========================================================================]
#include "Debug.h"

// TODO: rename and rebase
#define N_ELEMENTS_IN_ARRAY(ELEMENTS)	sizeof(ELEMENTS) / sizeof(ELEMENTS[0])

#pragma warning( pop )