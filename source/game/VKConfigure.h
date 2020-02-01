#pragma once

const VkFormat PIXEL_FORMAT_PRECEDENCE[] =
{
	VK_FORMAT_B8G8R8A8_UNORM
};
const int N_PIXEL_FORMAT_PRECEDENCES = N_ELEMENTS_IN_ARRAY(PIXEL_FORMAT_PRECEDENCE);

const VkColorSpaceKHR COLOR_SPACE_PRECEDENCE[] = 
{
	VK_COLOR_SPACE_SRGB_NONLINEAR_KHR
};
const int N_COLOR_SPACE_PRECEDENCES = N_ELEMENTS_IN_ARRAY(COLOR_SPACE_PRECEDENCE);

const VkPresentModeKHR PRESENT_MODE_PRECEDENCE[] =
{
	VK_PRESENT_MODE_MAILBOX_KHR,
	VK_PRESENT_MODE_IMMEDIATE_KHR,	// this precedes and is supposedly preferable to
	VK_PRESENT_MODE_FIFO_KHR		// this, which "unfortunately some drivers currently don't properly support"
};
const int N_PRESENT_MODE_PRECEDENCES = N_ELEMENTS_IN_ARRAY(PRESENT_MODE_PRECEDENCE);
const int N_PRECEDENCES = N_PIXEL_FORMAT_PRECEDENCES + N_COLOR_SPACE_PRECEDENCES + N_PRESENT_MODE_PRECEDENCES;

constexpr const char* INSTANCE_EXTENSION_NAMES[] =
{
	VK_EXT_DEBUG_REPORT_EXTENSION_NAME
};
const int N_INSTANCE_EXTENSION_NAMES = N_ELEMENTS_IN_ARRAY(INSTANCE_EXTENSION_NAMES);

const bool REQUIRE_INSTANCE_EXTENSION[] =
{
	false
};

constexpr const char* INSTANCE_LAYER_NAMES[] =
{
	"VK_LAYER_LUNARG_standard_validation",
	"VK_LAYER_LUNARG_assistant_layer"
};
const int N_INSTANCE_LAYER_NAMES = N_ELEMENTS_IN_ARRAY(INSTANCE_LAYER_NAMES);

const bool REQUIRE_INSTANCE_LAYER[] =
{
	false,
	false
};

constexpr const char* DEVICE_EXTENSION_NAMES[] =
{
	VK_KHR_SWAPCHAIN_EXTENSION_NAME
};
const int N_DEVICE_EXTENSION_NAMES = N_ELEMENTS_IN_ARRAY(DEVICE_EXTENSION_NAMES);

const bool REQUIRE_DEVICE_EXTENSION[] =
{
	true
};

#define stringAllRequiredNames(kind)							\
	inline std::string stringAllREQUIRED_##kind##_NAMES() {		\
		std::string allnames;									\
		std::string comma = "";									\
		for (int iKind = 0; iKind < N_##kind##_NAMES; ++iKind)	\
			if (REQUIRE_##kind[iKind]) {						\
				allnames += comma + kind##_NAMES[iKind];		\
				comma = ", ";									\
			}													\
		return allnames;										\
	}
stringAllRequiredNames(INSTANCE_EXTENSION)
stringAllRequiredNames(INSTANCE_LAYER)
stringAllRequiredNames(DEVICE_EXTENSION)

inline void SanityCheckVulkanConfiguration()
{
	assert(N_INSTANCE_EXTENSION_NAMES == N_ELEMENTS_IN_ARRAY(REQUIRE_INSTANCE_EXTENSION));
	assert(N_INSTANCE_LAYER_NAMES == N_ELEMENTS_IN_ARRAY(REQUIRE_INSTANCE_LAYER));
	assert(N_DEVICE_EXTENSION_NAMES == N_ELEMENTS_IN_ARRAY(REQUIRE_DEVICE_EXTENSION));
}