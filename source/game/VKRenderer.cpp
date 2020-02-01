#include "stdafx.h"
#include "VKRenderer.h"
//-----------------------------------------------------------------------------
VKRenderer::VKRenderer(SDL_Window *window)
	: m_window(window)
{
	querySupportedVulkanExtensions();
	displayFoundVulkanExtensions();	
}
//-----------------------------------------------------------------------------
VKRenderer::~VKRenderer()
{
	delete m_supportedVulkanExtensions;
}
//-----------------------------------------------------------------------------
void VKRenderer::querySupportedVulkanExtensions()
{
	uint32_t nExtensionsReturnedBySDL;
	if( !SDL_Vulkan_GetInstanceExtensions(m_window, &nExtensionsReturnedBySDL, nullptr) )
		ThrowSDLError("Fail to query NUMBER of Vulkan Instance Extensions: ");

	m_nAdditionalExtensions = m_extendedPlatform.RequestedExtensionsAvailable();
	m_nVulkanExtensions = nExtensionsReturnedBySDL + m_nAdditionalExtensions;

	m_supportedVulkanExtensions = new const char*[m_nVulkanExtensions];
	if( !SDL_Vulkan_GetInstanceExtensions(m_window, &nExtensionsReturnedBySDL, m_supportedVulkanExtensions) )
		Throw("Fail querying " + std::to_string(m_nVulkanExtensions) + " Vulkan Instance Extension names: " + std::string(SDL_GetError()));

	for( size_t iAdd = 0, iConcat = nExtensionsReturnedBySDL; iAdd < m_nAdditionalExtensions && iConcat < m_nVulkanExtensions; ++iAdd, ++iConcat )
		m_supportedVulkanExtensions[iConcat] = m_extendedPlatform.RequestedExtensionNames()[iAdd];
}
//-----------------------------------------------------------------------------