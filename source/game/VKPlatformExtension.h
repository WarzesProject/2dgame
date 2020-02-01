#pragma once

#include "VKConfigure.h"

// TODO: rename VKPlatformExtension?
class PlatformExtension
{
public:
	PlatformExtension()
	{
		SanityCheckVulkanConfiguration();

		vkEnumerateInstanceExtensionProperties(nullptr, &m_nAllExtensions, nullptr);

		m_allExtensions = new VkExtensionProperties[m_nAllExtensions];
		vkEnumerateInstanceExtensionProperties(nullptr, &m_nAllExtensions, m_allExtensions);

		showAllExtensions();

		for( int iReq = 0; iReq < N_INSTANCE_EXTENSION_NAMES; ++iReq )
		{
			const char *name = INSTANCE_EXTENSION_NAMES[iReq];
			bool unmatched = true;
			for( uint32_t iExtns = 0; iExtns < m_nAllExtensions; ++iExtns )
			{
				if( strcmp(name, m_allExtensions[iExtns].extensionName) == 0 )
				{
					m_grantedExtensionNames.emplace_back(name);
					unmatched = false;
					break;
				}
			}
			if( unmatched )
			{
				if( REQUIRE_INSTANCE_EXTENSION[iReq] )
					Throw("Required Instance Extension NOT Supported: " + std::string(name));
				else
					SDL_Log("Requested Extension NOT Supported: %s", name);
			}
		}
	}

	~PlatformExtension()
	{
		delete m_allExtensions;
	}

	bool IsSupported(const char *extensionName)
	{
		for( size_t iExtension = 0; iExtension < m_nAllExtensions; ++iExtension )
			if( strcmp(extensionName, m_allExtensions[iExtension].extensionName) == 0 )
				return true;
		return false;
	}

	bool IsDebugReportSupported()
	{
		return IsSupported(VK_EXT_DEBUG_REPORT_EXTENSION_NAME);
	}

	uint32_t RequestedExtensionsAvailable() { return (uint32_t)m_grantedExtensionNames.size(); }
	const char** RequestedExtensionNames() { return m_grantedExtensionNames.data(); }

private:
	void showAllExtensions()
	{
		SDL_Log("Vulkan reports Extensions Supported: %d", m_nAllExtensions);

		char prefix[8];
		for( uint32_t iExtension = 0; iExtension < m_nAllExtensions; )
		{
			const char *eachExtensionName = m_allExtensions[iExtension++].extensionName;
			snprintf(prefix, 8, " %2u. ", iExtension);
			const char* postfix = "";
			for( const char *requestedName : m_grantedExtensionNames )
			{
				if( strcmp(requestedName, eachExtensionName) == 0 )
				{
					sprintf_s(prefix, " --> ");
					postfix = " <-- requested";
					break;
				}
			}
			SDL_Log("%s%s%s", prefix, eachExtensionName, postfix);
		}
	}

	VkExtensionProperties *m_allExtensions;
	uint32_t m_nAllExtensions = 0;
	std::vector<const char*> m_grantedExtensionNames;
};