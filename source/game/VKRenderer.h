#pragma once

#include "Renderer.h"
#include "VKPlatformExtension.h"

class VKRenderer final : public Renderer
{
public:
	VKRenderer(SDL_Window *window);
	~VKRenderer() final;

private:
	void querySupportedVulkanExtensions();

	void displayFoundVulkanExtensions()
	{
		auto nExtensions = m_nVulkanExtensions;
		const char* appended = m_nAdditionalExtensions > 0 ? " (requests appended)" : "";
		SDL_Log("Platform (%s) reports Vulkan INSTANCE EXTENSIONS: %d %s", m_namePlatform, nExtensions, appended);
		for( size_t iExt = 0; iExt < nExtensions; ++iExt )
		{
			const char *sdlReportedExtension = m_supportedVulkanExtensions[iExt];
			const char *actuallySupported = m_extendedPlatform.IsSupported(sdlReportedExtension) ? " (supported)" : "";
			SDL_Log(" %2lu. %s%s", iExt + 1, sdlReportedExtension, actuallySupported);
		}
	}

	SDL_Window *m_window = nullptr;
	PlatformExtension m_extendedPlatform;
	const char **m_supportedVulkanExtensions;
	uint32_t m_nVulkanExtensions;
	uint32_t m_nAdditionalExtensions;
	const char *m_namePlatform;
};