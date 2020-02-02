#include "stdafx.h"
#include "VKRenderer.h"
#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>
#undef STB_IMAGE_IMPLEMENTATION
#define TINYOBJLOADER_IMPLEMENTATION
#include <tinyobjloader/tiny_obj_loader.h>
#undef TINYOBJLOADER_IMPLEMENTATION
//-----------------------------------------------------------------------------
const std::string MODEL_PATH = "../data/models/cube.obj";
const std::string TEXTURE_PATH = "../data/sprite/no_sprite.png";
const int MAX_FRAMES_IN_FLIGHT = 2;
//-----------------------------------------------------------------------------
const std::vector<const char*> validationLayers =
{
	"VK_LAYER_KHRONOS_validation"
};
const std::vector<const char*> deviceExtensions =
{
	VK_KHR_SWAPCHAIN_EXTENSION_NAME
};
#ifdef NDEBUG
const bool enableValidationLayers = false;
#else
const bool enableValidationLayers = true;
#endif
//-----------------------------------------------------------------------------
VKAPI_ATTR VkResult VKAPI_CALL vkCreateDebugUtilsMessengerEXT(
	VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo,
	const VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT* pDebugMessenger)
{
	auto func = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");
	if( func != nullptr )
		return func(instance, pCreateInfo, pAllocator, pDebugMessenger);
	else
		return VK_ERROR_EXTENSION_NOT_PRESENT;
}
//-----------------------------------------------------------------------------
VKAPI_ATTR void VKAPI_CALL vkDestroyDebugUtilsMessengerEXT(
	VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger,
	VkAllocationCallbacks const* pAllocator)
{
	auto func = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");
	if( func != nullptr )
		func(instance, debugMessenger, pAllocator);
}
//-----------------------------------------------------------------------------
bool checkValidationLayerSupport()
{
	std::vector<vk::LayerProperties> availableLayers = vk::enumerateInstanceLayerProperties();

	for( const char* layerName : validationLayers )
	{
		bool layerFound = false;

		for( const auto& layerProperties : availableLayers )
		{
			if( strcmp(layerName, layerProperties.layerName) )
			{
				layerFound = true;
				break;
			}
		}

		if( !layerFound )
			return false;
	}

	return true;
}
//-----------------------------------------------------------------------------
std::vector<const char*> getRequiredExtensions(SDL_Window *window)
{
	unsigned int extensionCount;
	if( !SDL_Vulkan_GetInstanceExtensions(window, &extensionCount, nullptr) )
		Throw("failed to get required SDL extension count!");

	std::vector<const char*> extensions(extensionCount);
	if( !SDL_Vulkan_GetInstanceExtensions(window, &extensionCount, extensions.data()) )
		Throw("failed to get required SDL extensions!");

	if( enableValidationLayers )
		extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);

	return extensions;
}
//-----------------------------------------------------------------------------
static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(
	VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
	VkDebugUtilsMessageTypeFlagsEXT messageType,
	const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
	void* pUserData)
{

	std::cerr << "validation layer: " << pCallbackData->pMessage << std::endl;
	return VK_FALSE;
}
//-----------------------------------------------------------------------------
vk::DebugUtilsMessengerCreateInfoEXT getDebugUtilsMessengerCreateInfo()
{
	return vk::DebugUtilsMessengerCreateInfoEXT(
		{},
		vk::DebugUtilsMessageSeverityFlagBitsEXT::eVerbose | vk::DebugUtilsMessageSeverityFlagBitsEXT::eWarning | vk::DebugUtilsMessageSeverityFlagBitsEXT::eError,
		vk::DebugUtilsMessageTypeFlagBitsEXT::eGeneral | vk::DebugUtilsMessageTypeFlagBitsEXT::eValidation | vk::DebugUtilsMessageTypeFlagBitsEXT::ePerformance,
		&debugCallback
	);
}
//-----------------------------------------------------------------------------
static std::vector<char> readFile(const std::string& filename)
{
	std::ifstream file(filename, std::ios::ate | std::ios::binary);

	if( !file.is_open() )
		Throw("failed to open file!");

	size_t fileSize = (size_t)file.tellg();
	std::vector<char> buffer(fileSize);

	file.seekg(0);
	file.read(buffer.data(), fileSize);

	file.close();

	return buffer;
}
//-----------------------------------------------------------------------------
VKRenderer::VKRenderer(SDL_Window *window)
	: m_window(window)
{
	createInstance();
	setupDebugMessenger();
	createSurface();
	pickPhysicalDevice();
	createLogicalDevice();
	createSwapchain();
	createImageViews();
	createRenderPass();
	createDescriptorSetLayout();
	createGraphicsPipeline();
	createCommandPool();
	createColorResources();
	createDepthResources();
	createFramebuffers();
	createTextureImage();
	createTextureImageView();
	createTextureSampler();
	loadModel();
	createVertexBuffer();
	createIndexBuffer();
	createUniformBuffers();
	createDescriptorPool();
	createDescriptorSets();
	createCommandBuffers();
	createSyncObjects();
}
//-----------------------------------------------------------------------------
VKRenderer::~VKRenderer()
{
	m_device->waitIdle();
}
//-----------------------------------------------------------------------------
void VKRenderer::Resize()
{
	recreateSwapchain();
}
//-----------------------------------------------------------------------------
void VKRenderer::Draw()
{
	m_device->waitForFences(1, &m_inFlightFences[m_currentFrame].get(), true, UINT64_MAX);

	vk::ResultValue<uint32_t> result = m_device->acquireNextImageKHR(m_swapchain.get(), (uint64_t)UINT64_MAX, m_imageAvailableSemaphores[m_currentFrame].get(), nullptr);

	if( result.result == vk::Result::eErrorOutOfDateKHR )
	{
		recreateSwapchain();
		return;
	}

	if( m_imagesInFlight[result.value] != -1 )
		m_device->waitForFences(1, &m_inFlightFences[m_imagesInFlight[result.value]].get(), true, UINT64_MAX);
	m_imagesInFlight[result.value] = m_currentFrame;

	vk::PipelineStageFlags waitStages = { vk::PipelineStageFlagBits::eColorAttachmentOutput };

	m_device->resetFences(1, &m_inFlightFences[m_currentFrame].get());

	updateUniformBuffer(result.value);

	m_graphicsQueue.submit(
		vk::SubmitInfo(
		(uint32_t)1, &m_imageAvailableSemaphores[m_currentFrame].get(), &waitStages,
			(uint32_t)1, &(m_commandBuffers[result.value].get()),
			(uint32_t)1, &m_renderFinishedSemaphores[m_currentFrame].get()
		), m_inFlightFences[m_currentFrame].get()
	);

	m_presentQueue.presentKHR(
		vk::PresentInfoKHR(
			1, &m_renderFinishedSemaphores[m_currentFrame].get(),
			1, &m_swapchain.get(),
			&result.value, nullptr
		)
	);

	m_currentFrame = (m_currentFrame + 1) % MAX_FRAMES_IN_FLIGHT;
}
//-----------------------------------------------------------------------------
void VKRenderer::createInstance()
{
	if( enableValidationLayers && !checkValidationLayerSupport() )
		Throw("validation layers requested, but not available!");

	vk::ApplicationInfo applicationInfo(
		"Game", VK_MAKE_VERSION(1, 0, 0),
		"No Engine", VK_MAKE_VERSION(1, 0, 0),
		VK_API_VERSION_1_0
	);

	std::vector<const char*> extensions = getRequiredExtensions(m_window);

	size_t layerCount = 0;
	if( enableValidationLayers )
		layerCount = validationLayers.size();

	vk::DebugUtilsMessengerCreateInfoEXT debugMessengerCreateInfo = getDebugUtilsMessengerCreateInfo();
	vk::InstanceCreateInfo instanceCreateInfo(
		{}, &applicationInfo,
		static_cast<uint32_t>(layerCount), validationLayers.data(),
		static_cast<uint32_t>(extensions.size()), extensions.data()
	);
	instanceCreateInfo.pNext = &debugMessengerCreateInfo;

	m_instance = vk::createInstanceUnique(instanceCreateInfo);
}
//-----------------------------------------------------------------------------
void VKRenderer::setupDebugMessenger()
{
	if( !enableValidationLayers )
		return;

	m_debugMessenger = m_instance->createDebugUtilsMessengerEXTUnique(getDebugUtilsMessengerCreateInfo());
}
//-----------------------------------------------------------------------------
void VKRenderer::createSurface()
{
	vk::SurfaceKHR tmpSurface;
	if( !SDL_Vulkan_CreateSurface(m_window, static_cast<VkInstance>(m_instance.get()), reinterpret_cast<VkSurfaceKHR*>(&tmpSurface)) )
		Throw("failed to create SDL surface!");
	m_surface = vk::UniqueSurfaceKHR(tmpSurface, m_instance.get());
}
//-----------------------------------------------------------------------------
void VKRenderer::pickPhysicalDevice()
{
	bool deviceFound = false;
	std::vector<vk::PhysicalDevice> physicalDevices = m_instance->enumeratePhysicalDevices();

	if( physicalDevices.size() == 0 )
		Throw("failed to find GPUs with Vulkan support!");

	for( const auto &device : physicalDevices )
	{
		if( isDeviceSuitable(device) )
		{
			m_physicalDevice = device;
			m_msaaSamples = getMaxUsableSampleCount();
			deviceFound = true;
			break;
		}
	}

	if( !deviceFound )
		Throw("failed to find a suitable GPU!");
}
//-----------------------------------------------------------------------------
bool VKRenderer::isDeviceSuitable(vk::PhysicalDevice device)
{
	QueueFamilyIndices indices = findQueueFamilies(device);

	bool swapchainAdequate = false;
	if( checkDeviceExtensionSupport(device) )
	{
		SwapchainSupportDetails swapchainSupport = querySwapchainSupport(device);
		swapchainAdequate = !swapchainSupport.formats.empty() && !swapchainSupport.presentModes.empty();
	}

	vk::PhysicalDeviceFeatures supportedFeatures = device.getFeatures();

	return indices.IsComplete() && swapchainAdequate && supportedFeatures.samplerAnisotropy;
}
//-----------------------------------------------------------------------------
QueueFamilyIndices VKRenderer::findQueueFamilies(vk::PhysicalDevice device)
{
	QueueFamilyIndices indices;

	std::vector<vk::QueueFamilyProperties> queueFamilies = device.getQueueFamilyProperties();

	int i = 0;
	for( const auto& queueFamily : queueFamilies )
	{
		if( queueFamily.queueFlags & vk::QueueFlagBits::eGraphics )
			indices.graphicsFamily = i;

		if( device.getSurfaceSupportKHR(i, m_surface.get()) )
			indices.presentFamily = i;

		if( indices.IsComplete() )
			break;

		i++;
	}

	return indices;
}
//-----------------------------------------------------------------------------
bool VKRenderer::checkDeviceExtensionSupport(vk::PhysicalDevice device)
{
	std::vector<vk::ExtensionProperties> availableExtensions = device.enumerateDeviceExtensionProperties();

	std::set<std::string> requiredExtensions(deviceExtensions.begin(), deviceExtensions.end());

	for( const auto& extension : availableExtensions )
	{
		requiredExtensions.erase(extension.extensionName);
	}

	return requiredExtensions.empty();
}
//-----------------------------------------------------------------------------
SwapchainSupportDetails VKRenderer::querySwapchainSupport(vk::PhysicalDevice device)
{
	SwapchainSupportDetails details;
	details.capabilities = device.getSurfaceCapabilitiesKHR(m_surface.get());
	details.formats = device.getSurfaceFormatsKHR(m_surface.get());
	details.presentModes = device.getSurfacePresentModesKHR(m_surface.get());
	return details;
}
//-----------------------------------------------------------------------------
vk::SampleCountFlagBits VKRenderer::getMaxUsableSampleCount()
{
	vk::PhysicalDeviceProperties physicalDeviceProperties = m_physicalDevice.getProperties();

	vk::SampleCountFlags counts = physicalDeviceProperties.limits.framebufferColorSampleCounts &
		physicalDeviceProperties.limits.framebufferDepthSampleCounts;
	if( counts & vk::SampleCountFlagBits::e64 ) return vk::SampleCountFlagBits::e64;
	if( counts & vk::SampleCountFlagBits::e32 ) return vk::SampleCountFlagBits::e32;
	if( counts & vk::SampleCountFlagBits::e16 ) return vk::SampleCountFlagBits::e16;
	if( counts & vk::SampleCountFlagBits::e8 ) return vk::SampleCountFlagBits::e8;
	if( counts & vk::SampleCountFlagBits::e4 ) return vk::SampleCountFlagBits::e4;
	if( counts & vk::SampleCountFlagBits::e2 ) return vk::SampleCountFlagBits::e2;

	return vk::SampleCountFlagBits::e1;
}
//-----------------------------------------------------------------------------
void VKRenderer::createLogicalDevice()
{
	QueueFamilyIndices indices = findQueueFamilies(m_physicalDevice);

	std::vector<vk::DeviceQueueCreateInfo> queueCreateInfos;
	std::set<uint32_t> uniqueQueueFamilies = { indices.graphicsFamily.value(), indices.presentFamily.value() };

	float queuePriority = 1.0f;
	for( uint32_t queueFamily : uniqueQueueFamilies )
	{
		queueCreateInfos.push_back(
			vk::DeviceQueueCreateInfo({}, queueFamily, 1, &queuePriority)
		);
	}

	vk::PhysicalDeviceFeatures deviceFeatures = {};
	deviceFeatures.samplerAnisotropy = true;
	deviceFeatures.sampleRateShading = true;

	uint32_t enabledLayerCount = 0;
	if( enableValidationLayers )
		enabledLayerCount = static_cast<uint32_t>(validationLayers.size());

	m_device = m_physicalDevice.createDeviceUnique(
		vk::DeviceCreateInfo(
			{},
			static_cast<uint32_t>(queueCreateInfos.size()), queueCreateInfos.data(),
			enabledLayerCount, validationLayers.data(),
			static_cast<uint32_t>(deviceExtensions.size()), deviceExtensions.data(),
			&deviceFeatures
		)
	);

	m_graphicsQueue = m_device->getQueue(indices.graphicsFamily.value(), 0);
	m_presentQueue = m_device->getQueue(indices.presentFamily.value(), 0);
}
//-----------------------------------------------------------------------------
void VKRenderer::createSwapchain()
{
	SwapchainSupportDetails swapchainSupport = querySwapchainSupport(m_physicalDevice);

	vk::SurfaceFormatKHR surfaceFormat = chooseSwapSurfaceFormat(swapchainSupport.formats);
	vk::PresentModeKHR presentMode = chooseSwapPresentMode(swapchainSupport.presentModes);
	vk::Extent2D extent = chooseSwapExtent(swapchainSupport.capabilities);

	uint32_t imageCount = swapchainSupport.capabilities.minImageCount + 1;
	if( swapchainSupport.capabilities.maxImageCount > 0 && imageCount > swapchainSupport.capabilities.maxImageCount )
		imageCount = swapchainSupport.capabilities.maxImageCount;

	QueueFamilyIndices indices = findQueueFamilies(m_physicalDevice);
	uint32_t queueFamilyIndices[] = { indices.graphicsFamily.value(), indices.presentFamily.value() };

	vk::SharingMode sharingMode = vk::SharingMode::eExclusive;
	if( indices.graphicsFamily != indices.presentFamily )
		sharingMode = vk::SharingMode::eConcurrent;

	m_swapchain = m_device->createSwapchainKHRUnique(
		vk::SwapchainCreateInfoKHR(
			vk::SwapchainCreateFlagsKHR(), m_surface.get(),
			imageCount, surfaceFormat.format,
			surfaceFormat.colorSpace, extent,
			1, vk::ImageUsageFlagBits::eColorAttachment,
			sharingMode, 2, queueFamilyIndices,
			swapchainSupport.capabilities.currentTransform, vk::CompositeAlphaFlagBitsKHR::eOpaque,
			presentMode, true,
			nullptr
		)
	);

	m_swapchainImages = m_device->getSwapchainImagesKHR(m_swapchain.get());
	m_swapchainImageFormat = surfaceFormat.format;
	m_swapchainExtent = extent;
}
//-----------------------------------------------------------------------------
vk::SurfaceFormatKHR VKRenderer::chooseSwapSurfaceFormat(const std::vector<vk::SurfaceFormatKHR>& availableFormats)
{
	for( const auto& availableFormat : availableFormats )
	{
		if( availableFormat.format == vk::Format::eB8G8R8A8Unorm && availableFormat.colorSpace == vk::ColorSpaceKHR::eSrgbNonlinear )
			return availableFormat;
	}

	return availableFormats[0];
}
//-----------------------------------------------------------------------------
vk::PresentModeKHR VKRenderer::chooseSwapPresentMode(const std::vector<vk::PresentModeKHR>& availablePresentModes)
{
	for( const auto& availablePresentMode : availablePresentModes )
	{
		if( availablePresentMode == vk::PresentModeKHR::eMailbox )
			return availablePresentMode;
	}

	return vk::PresentModeKHR::eFifo;
}
//-----------------------------------------------------------------------------
vk::Extent2D VKRenderer::chooseSwapExtent(const vk::SurfaceCapabilitiesKHR & capabilities)
{
	if( capabilities.currentExtent.width != UINT32_MAX )
	{
		return capabilities.currentExtent;
	}
	else
	{
		int width, height;
		SDL_GetWindowSize(m_window, &width, &height);

		vk::Extent2D actualExtent = { (uint32_t)width, (uint32_t)height };

		actualExtent.width = std::clamp(actualExtent.width, capabilities.minImageExtent.width, capabilities.maxImageExtent.width);
		actualExtent.height = std::clamp(actualExtent.height, capabilities.minImageExtent.height, capabilities.maxImageExtent.height);

		return actualExtent;
	}
}
//-----------------------------------------------------------------------------
void VKRenderer::createImageViews()
{
	m_swapchainImageViews.resize(m_swapchainImages.size());

	for( size_t i = 0; i < m_swapchainImages.size(); ++i )
	{
		m_swapchainImageViews[i] = createImageView(m_swapchainImages[i], m_swapchainImageFormat, vk::ImageAspectFlagBits::eColor, 1);
	}
}
//-----------------------------------------------------------------------------
vk::UniqueImageView VKRenderer::createImageView(vk::Image image, vk::Format format, vk::ImageAspectFlags aspectFlags, uint32_t mipLevels)
{
	vk::ImageViewCreateInfo viewInfo = {};
	viewInfo.image = image;
	viewInfo.viewType = vk::ImageViewType::e2D;
	viewInfo.format = format;
	viewInfo.subresourceRange.aspectMask = aspectFlags;
	viewInfo.subresourceRange.baseMipLevel = 0;
	viewInfo.subresourceRange.levelCount = mipLevels;
	viewInfo.subresourceRange.baseArrayLayer = 0;
	viewInfo.subresourceRange.layerCount = 1;

	return m_device->createImageViewUnique(viewInfo);
}
//-----------------------------------------------------------------------------
void VKRenderer::createRenderPass()
{
	vk::AttachmentDescription colorAttachment = {};
	colorAttachment.format = m_swapchainImageFormat;
	colorAttachment.samples = m_msaaSamples;
	colorAttachment.loadOp = vk::AttachmentLoadOp::eClear;
	colorAttachment.storeOp = vk::AttachmentStoreOp::eStore;
	colorAttachment.stencilLoadOp = vk::AttachmentLoadOp::eDontCare;
	colorAttachment.stencilStoreOp = vk::AttachmentStoreOp::eDontCare;
	colorAttachment.initialLayout = vk::ImageLayout::eUndefined;
	colorAttachment.finalLayout = vk::ImageLayout::eColorAttachmentOptimal;

	vk::AttachmentDescription depthAttachment = {};
	depthAttachment.format = findDepthFormat();
	depthAttachment.samples = m_msaaSamples;
	depthAttachment.loadOp = vk::AttachmentLoadOp::eClear;
	depthAttachment.storeOp = vk::AttachmentStoreOp::eDontCare;
	depthAttachment.stencilLoadOp = vk::AttachmentLoadOp::eDontCare;
	depthAttachment.stencilStoreOp = vk::AttachmentStoreOp::eDontCare;
	depthAttachment.initialLayout = vk::ImageLayout::eUndefined;
	depthAttachment.finalLayout = vk::ImageLayout::ePresentSrcKHR;

	vk::AttachmentDescription colorAttachmentResolve = {};
	colorAttachmentResolve.format = m_swapchainImageFormat;
	colorAttachmentResolve.samples = vk::SampleCountFlagBits::e1;
	colorAttachmentResolve.loadOp = vk::AttachmentLoadOp::eDontCare;
	colorAttachmentResolve.storeOp = vk::AttachmentStoreOp::eStore;
	colorAttachmentResolve.stencilLoadOp = vk::AttachmentLoadOp::eDontCare;
	colorAttachmentResolve.stencilStoreOp = vk::AttachmentStoreOp::eDontCare;
	colorAttachmentResolve.initialLayout = vk::ImageLayout::eUndefined;
	colorAttachmentResolve.finalLayout = vk::ImageLayout::ePresentSrcKHR;

	vk::AttachmentReference colorAttachmentRef(0, vk::ImageLayout::eColorAttachmentOptimal);

	vk::AttachmentReference depthAttachmentRef(1, vk::ImageLayout::eDepthStencilAttachmentOptimal);

	vk::AttachmentReference colorAttachmentResolveRef(2, vk::ImageLayout::eColorAttachmentOptimal);

	vk::SubpassDescription subpass;
	subpass.pipelineBindPoint = vk::PipelineBindPoint::eGraphics;
	subpass.colorAttachmentCount = 1;
	subpass.pColorAttachments = &colorAttachmentRef;
	subpass.pDepthStencilAttachment = &depthAttachmentRef;
	subpass.pResolveAttachments = &colorAttachmentResolveRef;

	vk::SubpassDependency dependency(
		(uint32_t)VK_SUBPASS_EXTERNAL, (uint32_t)0,
		vk::PipelineStageFlagBits::eColorAttachmentOutput, vk::PipelineStageFlagBits::eColorAttachmentOutput,
		vk::AccessFlags(), vk::AccessFlagBits::eColorAttachmentRead | vk::AccessFlagBits::eColorAttachmentWrite,
		vk::DependencyFlags()
	);

	std::array<vk::AttachmentDescription, 3> attachments = { colorAttachment, depthAttachment, colorAttachmentResolve };
	m_renderPass = m_device->createRenderPassUnique(
		vk::RenderPassCreateInfo(
			vk::RenderPassCreateFlags(), static_cast<uint32_t>(attachments.size()), attachments.data(), 1, &subpass, 1, &dependency
		)
	);
}
//-----------------------------------------------------------------------------
vk::Format VKRenderer::findDepthFormat()
{
	return findSupportedFormat(
		{ vk::Format::eD32Sfloat, vk::Format::eD32SfloatS8Uint, vk::Format::eD24UnormS8Uint },
		vk::ImageTiling::eOptimal,
		vk::FormatFeatureFlagBits::eDepthStencilAttachment
	);
}
//-----------------------------------------------------------------------------
vk::Format VKRenderer::findSupportedFormat(const std::vector<vk::Format>& candidates, vk::ImageTiling tiling, vk::FormatFeatureFlags features)
{
	for( vk::Format format : candidates )
	{
		vk::FormatProperties props = m_physicalDevice.getFormatProperties(format);

		if( tiling == vk::ImageTiling::eLinear && (props.linearTilingFeatures & features) == features )
		{
			return format;
		}
		else if( tiling == vk::ImageTiling::eOptimal && (props.optimalTilingFeatures & features) == features )
		{
			return format;
		}
	}

	Throw("failed to find supported format!");
}
//-----------------------------------------------------------------------------
void VKRenderer::createDescriptorSetLayout()
{
	vk::DescriptorSetLayoutBinding uboLayoutBinding = {};
	uboLayoutBinding.binding = 0;
	uboLayoutBinding.descriptorType = vk::DescriptorType::eUniformBuffer;
	uboLayoutBinding.descriptorCount = 1;
	uboLayoutBinding.stageFlags = vk::ShaderStageFlagBits::eVertex;
	uboLayoutBinding.pImmutableSamplers = nullptr;

	vk::DescriptorSetLayoutBinding samplerLayoutBinding = {};
	samplerLayoutBinding.binding = 1;
	samplerLayoutBinding.descriptorCount = 1;
	samplerLayoutBinding.descriptorType = vk::DescriptorType::eCombinedImageSampler;
	samplerLayoutBinding.pImmutableSamplers = nullptr;
	samplerLayoutBinding.stageFlags = vk::ShaderStageFlagBits::eFragment;

	std::array<vk::DescriptorSetLayoutBinding, 2> bindings = { uboLayoutBinding, samplerLayoutBinding };
	vk::DescriptorSetLayoutCreateInfo layoutInfo = {};
	layoutInfo.bindingCount = static_cast<uint32_t>(bindings.size());
	layoutInfo.pBindings = bindings.data();

	m_descriptorSetLayout = m_device->createDescriptorSetLayoutUnique(layoutInfo);
}
//-----------------------------------------------------------------------------
void VKRenderer::createGraphicsPipeline()
{
	auto vertShaderCode = readFile("../data/shaders/vert.spv");
	auto fragShaderCode = readFile("../data/shaders/frag.spv");

	vk::UniqueShaderModule vertShaderModule = createShaderModule(vertShaderCode);
	vk::UniqueShaderModule fragShaderModule = createShaderModule(fragShaderCode);

	vk::PipelineShaderStageCreateInfo vertShaderStageInfo(
		vk::PipelineShaderStageCreateFlags(), vk::ShaderStageFlagBits::eVertex, vertShaderModule.get(), "main"
	);
	vk::PipelineShaderStageCreateInfo fragShaderStageInfo(
		vk::PipelineShaderStageCreateFlags(), vk::ShaderStageFlagBits::eFragment, fragShaderModule.get(), "main"
	);

	vk::PipelineShaderStageCreateInfo shaderStages[] = { vertShaderStageInfo, fragShaderStageInfo };

	auto bindingDescription = Vertex::GetBindingDescription();
	auto attributeDescriptions = Vertex::GetAttributeDescriptions();

	vk::PipelineVertexInputStateCreateInfo vertexInputInfo(
		vk::PipelineVertexInputStateCreateFlags(),
		1, &bindingDescription,
		attributeDescriptions.size(), attributeDescriptions.data()
	);

	vk::PipelineInputAssemblyStateCreateInfo inputAssembly(
		vk::PipelineInputAssemblyStateCreateFlags(), vk::PrimitiveTopology::eTriangleList, false
	);

	vk::Viewport viewport(0.0f, 0.0f, (float)m_swapchainExtent.width, (float)m_swapchainExtent.height, 0.0f, 1.0f);

	vk::Rect2D scissor({ 0, 0 }, m_swapchainExtent);

	vk::PipelineViewportStateCreateInfo viewportState(
		vk::PipelineViewportStateCreateFlags(), 1, &viewport, 1, &scissor
	);

	vk::PipelineRasterizationStateCreateInfo rasterizer(
		vk::PipelineRasterizationStateCreateFlags(), false, false,
		vk::PolygonMode::eFill, vk::CullModeFlagBits::eBack, vk::FrontFace::eCounterClockwise,
		false, 0.0f, 0.0f, 0.0f, 1.0f
	);

	vk::PipelineMultisampleStateCreateInfo multisampling(
		vk::PipelineMultisampleStateCreateFlags(), m_msaaSamples,
		true, 0.2f, nullptr, false, false
	);

	vk::PipelineDepthStencilStateCreateInfo depthStencil = {};
	depthStencil.depthTestEnable = true;
	depthStencil.depthWriteEnable = true;
	depthStencil.depthCompareOp = vk::CompareOp::eLess;
	depthStencil.depthBoundsTestEnable = false;
	depthStencil.stencilTestEnable = false;

	vk::PipelineColorBlendAttachmentState colorBlendAttachment;
	colorBlendAttachment.blendEnable = false;
	colorBlendAttachment.colorWriteMask =
		vk::ColorComponentFlagBits::eR | vk::ColorComponentFlagBits::eG |
		vk::ColorComponentFlagBits::eB | vk::ColorComponentFlagBits::eA;

	vk::PipelineColorBlendStateCreateInfo colorBlending(
		vk::PipelineColorBlendStateCreateFlags(), false, vk::LogicOp::eClear, 1, &colorBlendAttachment
	);

	m_pipelineLayout = m_device->createPipelineLayoutUnique(
		vk::PipelineLayoutCreateInfo(
			vk::PipelineLayoutCreateFlags(),
			1, &m_descriptorSetLayout.get(),
			0, nullptr
		)
	);

	vk::GraphicsPipelineCreateInfo pipelineInfo;
	pipelineInfo.stageCount = 2;
	pipelineInfo.pStages = shaderStages;
	pipelineInfo.pVertexInputState = &vertexInputInfo;
	pipelineInfo.pInputAssemblyState = &inputAssembly;
	pipelineInfo.pViewportState = &viewportState;
	pipelineInfo.pRasterizationState = &rasterizer;
	pipelineInfo.pMultisampleState = &multisampling;
	pipelineInfo.pDepthStencilState = &depthStencil;
	pipelineInfo.pColorBlendState = &colorBlending;
	pipelineInfo.layout = m_pipelineLayout.get();
	pipelineInfo.renderPass = m_renderPass.get();
	pipelineInfo.subpass = 0;

	m_graphicsPipelines = m_device->createGraphicsPipelinesUnique({}, pipelineInfo);
}
//-----------------------------------------------------------------------------
vk::UniqueShaderModule VKRenderer::createShaderModule(const std::vector<char>& code)
{
	return m_device->createShaderModuleUnique(
		vk::ShaderModuleCreateInfo(
			vk::ShaderModuleCreateFlags(), code.size(), reinterpret_cast<const uint32_t*>(code.data())
		)
	);
}
//-----------------------------------------------------------------------------
void VKRenderer::createCommandPool()
{
	QueueFamilyIndices queueFamilyIndices = findQueueFamilies(m_physicalDevice);

	m_commandPool = m_device->createCommandPoolUnique(
		vk::CommandPoolCreateInfo(
			vk::CommandPoolCreateFlags(), queueFamilyIndices.graphicsFamily.value()
		)
	);
}
//-----------------------------------------------------------------------------
void VKRenderer::createColorResources()
{
	vk::Format colorFormat = m_swapchainImageFormat;

	createImage(
		m_swapchainExtent.width, m_swapchainExtent.height, 1, m_msaaSamples, colorFormat, vk::ImageTiling::eOptimal,
		vk::ImageUsageFlagBits::eTransientAttachment | vk::ImageUsageFlagBits::eColorAttachment, vk::MemoryPropertyFlagBits::eDeviceLocal,
		m_colorImage, m_colorImageMemory
	);

	m_colorImageView = createImageView(m_colorImage.get(), colorFormat, vk::ImageAspectFlagBits::eColor, 1);
}
//-----------------------------------------------------------------------------
void VKRenderer::createImage(uint32_t width, uint32_t height, uint32_t m_mipLevels, vk::SampleCountFlagBits numSamples, vk::Format format, vk::ImageTiling tiling, vk::ImageUsageFlags usage, vk::MemoryPropertyFlags properties, vk::UniqueImage & image, vk::UniqueDeviceMemory & imageMemory)
{
	vk::ImageCreateInfo imageInfo = {};
	imageInfo.imageType = vk::ImageType::e2D;
	imageInfo.extent.width = width;
	imageInfo.extent.height = height;
	imageInfo.extent.depth = 1;
	imageInfo.mipLevels = 1;
	imageInfo.arrayLayers = 1;
	imageInfo.format = format;
	imageInfo.tiling = tiling;
	imageInfo.initialLayout = vk::ImageLayout::eUndefined;
	imageInfo.usage = usage;
	imageInfo.sharingMode = vk::SharingMode::eExclusive;
	imageInfo.samples = numSamples;
	imageInfo.mipLevels = m_mipLevels;

	image = m_device->createImageUnique(imageInfo);

	vk::MemoryRequirements memRequirements = m_device->getImageMemoryRequirements(image.get());

	vk::MemoryAllocateInfo allocInfo = {};
	allocInfo.allocationSize = memRequirements.size;
	allocInfo.memoryTypeIndex = findMemoryType(memRequirements.memoryTypeBits, properties);

	imageMemory = m_device->allocateMemoryUnique(allocInfo);

	m_device->bindImageMemory(image.get(), imageMemory.get(), 0);
}
//-----------------------------------------------------------------------------
uint32_t VKRenderer::findMemoryType(uint32_t typeFilter, vk::MemoryPropertyFlags properties)
{
	vk::PhysicalDeviceMemoryProperties memProperties = m_physicalDevice.getMemoryProperties();

	for( uint32_t i = 0; i < memProperties.memoryTypeCount; ++i )
	{
		if( typeFilter & (1 << i) && (memProperties.memoryTypes[i].propertyFlags & properties) == properties )
			return i;
	}

	Throw("failed to find suitable memory type!");
}
//-----------------------------------------------------------------------------
void VKRenderer::createDepthResources()
{
	vk::Format depthFormat = findDepthFormat();

	createImage(
		m_swapchainExtent.width, m_swapchainExtent.height, 1, m_msaaSamples, depthFormat, vk::ImageTiling::eOptimal,
		vk::ImageUsageFlagBits::eDepthStencilAttachment, vk::MemoryPropertyFlagBits::eDeviceLocal,
		m_depthImage, m_depthImageMemory
	);

	m_depthImageView = createImageView(m_depthImage.get(), depthFormat, vk::ImageAspectFlagBits::eDepth, 1);
}
//-----------------------------------------------------------------------------
void VKRenderer::createFramebuffers()
{
	m_swapchainFramebuffers.resize(m_swapchainImageViews.size());

	for( size_t i = 0; i < m_swapchainImageViews.size(); ++i )
	{
		std::array<vk::ImageView, 3> attachments = { m_colorImageView.get(), m_depthImageView.get(), m_swapchainImageViews[i].get() };

		m_swapchainFramebuffers[i] = m_device->createFramebufferUnique(
			vk::FramebufferCreateInfo(
				vk::FramebufferCreateFlags(), m_renderPass.get(),
				static_cast<uint32_t>(attachments.size()), attachments.data(),
				m_swapchainExtent.width, m_swapchainExtent.height,
				1
			)
		);
	}
}
//-----------------------------------------------------------------------------
void VKRenderer::createTextureImage()
{
	int texWidth, texHeight, texChannels;
	stbi_uc* pixels = stbi_load(TEXTURE_PATH.c_str(), &texWidth, &texHeight, &texChannels, STBI_rgb_alpha);
	vk::DeviceSize imageSize = static_cast<vk::DeviceSize>(texWidth) * static_cast<vk::DeviceSize>(texHeight) * 4;
	m_mipLevels = static_cast<uint32_t>(std::floor(std::log2(std::max(texWidth, texHeight)))) + 1;

	if( !pixels )
		throw std::runtime_error("failed to load texture image!");

	vk::UniqueBuffer stagingBuffer;
	vk::UniqueDeviceMemory stagingBufferMemory;
	createBuffer(
		imageSize, vk::BufferUsageFlagBits::eTransferSrc,
		vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent,
		stagingBuffer, stagingBufferMemory
	);

	void* data = m_device->mapMemory(stagingBufferMemory.get(), 0, imageSize, vk::MemoryMapFlags());
	memcpy(data, pixels, static_cast<size_t>(imageSize));
	m_device->unmapMemory(stagingBufferMemory.get());

	stbi_image_free(pixels);

	createImage(
		texWidth, texHeight, m_mipLevels, vk::SampleCountFlagBits::e1, vk::Format::eR8G8B8A8Unorm, vk::ImageTiling::eOptimal,
		vk::ImageUsageFlagBits::eTransferSrc | vk::ImageUsageFlagBits::eTransferDst | vk::ImageUsageFlagBits::eSampled,
		vk::MemoryPropertyFlagBits::eDeviceLocal, m_textureImage, m_textureImageMemory
	);

	transitionImageLayout(m_textureImage.get(), vk::Format::eR8G8B8A8Unorm, vk::ImageLayout::eUndefined, vk::ImageLayout::eTransferDstOptimal, m_mipLevels);
	copyBufferToImage(stagingBuffer.get(), m_textureImage.get(), texWidth, texHeight);
	//transitioned to VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL while generating mipmaps

	generateMipmaps(m_textureImage.get(), vk::Format::eR8G8B8A8Unorm, texWidth, texHeight, m_mipLevels);
}
//-----------------------------------------------------------------------------
void VKRenderer::createBuffer(vk::DeviceSize size, vk::BufferUsageFlags usage, vk::MemoryPropertyFlags properties, vk::UniqueBuffer & buffer, vk::UniqueDeviceMemory & bufferMemory)
{
	vk::BufferCreateInfo bufferCreateInfo(
		vk::BufferCreateFlags(), size,
		usage, vk::SharingMode::eExclusive
	);

	buffer = m_device->createBufferUnique(bufferCreateInfo);

	vk::MemoryRequirements memRequirements = m_device->getBufferMemoryRequirements(buffer.get());

	bufferMemory = m_device->allocateMemoryUnique(
		vk::MemoryAllocateInfo(
			memRequirements.size,
			findMemoryType(memRequirements.memoryTypeBits, properties)
		)
	);

	m_device->bindBufferMemory(buffer.get(), bufferMemory.get(), 0);
}
//-----------------------------------------------------------------------------
void VKRenderer::transitionImageLayout(vk::Image image, vk::Format format, vk::ImageLayout oldLayout, vk::ImageLayout newLayout, uint32_t m_mipLevels)
{
	std::vector<vk::UniqueCommandBuffer> m_commandBuffers = beginSingleTimeCommands();

	vk::ImageMemoryBarrier barrier = {};
	barrier.oldLayout = oldLayout;
	barrier.newLayout = newLayout;
	barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
	barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
	barrier.image = image;
	barrier.subresourceRange.aspectMask = vk::ImageAspectFlagBits::eColor;
	barrier.subresourceRange.baseMipLevel = 0;
	barrier.subresourceRange.levelCount = m_mipLevels;
	barrier.subresourceRange.baseArrayLayer = 0;
	barrier.subresourceRange.layerCount = 1;

	vk::PipelineStageFlags sourceStage;
	vk::PipelineStageFlags destinationStage;

	if( oldLayout == vk::ImageLayout::eUndefined && newLayout == vk::ImageLayout::eTransferDstOptimal )
	{
		barrier.srcAccessMask = vk::AccessFlags();
		barrier.dstAccessMask = vk::AccessFlagBits::eTransferWrite;

		sourceStage = vk::PipelineStageFlagBits::eTopOfPipe;
		destinationStage = vk::PipelineStageFlagBits::eTransfer;
	}
	else if( oldLayout == vk::ImageLayout::eTransferDstOptimal && newLayout == vk::ImageLayout::eShaderReadOnlyOptimal )
	{
		barrier.srcAccessMask = vk::AccessFlagBits::eTransferWrite;
		barrier.dstAccessMask = vk::AccessFlagBits::eShaderRead;

		sourceStage = vk::PipelineStageFlagBits::eTransfer;
		destinationStage = vk::PipelineStageFlagBits::eFragmentShader;
	}
	else
	{
		Throw("unsupported layout transition!");
	}

	m_commandBuffers[0]->pipelineBarrier(sourceStage, destinationStage, vk::DependencyFlags(), nullptr, nullptr, barrier);

	endSingleTimeCommands(m_commandBuffers);
}
//-----------------------------------------------------------------------------
std::vector<vk::UniqueCommandBuffer> VKRenderer::beginSingleTimeCommands()
{
	std::vector<vk::UniqueCommandBuffer> m_commandBuffers = m_device->allocateCommandBuffersUnique(
		vk::CommandBufferAllocateInfo(
			m_commandPool.get(), vk::CommandBufferLevel::ePrimary, 1
		)
	);

	m_commandBuffers[0]->begin(
		vk::CommandBufferBeginInfo(
			vk::CommandBufferUsageFlagBits::eOneTimeSubmit, nullptr
		)
	);

	return m_commandBuffers;
}
//-----------------------------------------------------------------------------
void VKRenderer::endSingleTimeCommands(std::vector<vk::UniqueCommandBuffer>& m_commandBuffers)
{
	m_commandBuffers[0]->end();

	vk::SubmitInfo submitInfo = {};
	submitInfo.commandBufferCount = 1;
	submitInfo.pCommandBuffers = &m_commandBuffers[0].get();

	m_graphicsQueue.submit(submitInfo, nullptr);
	m_graphicsQueue.waitIdle();
}
//-----------------------------------------------------------------------------
void VKRenderer::copyBufferToImage(vk::Buffer buffer, vk::Image image, uint32_t width, uint32_t height)
{
	std::vector<vk::UniqueCommandBuffer> m_commandBuffers = beginSingleTimeCommands();

	vk::BufferImageCopy region = {};
	region.bufferOffset = 0;
	region.bufferRowLength = 0;
	region.bufferImageHeight = 0;
	region.imageSubresource.aspectMask = vk::ImageAspectFlagBits::eColor;
	region.imageSubresource.mipLevel = 0;
	region.imageSubresource.baseArrayLayer = 0;
	region.imageSubresource.layerCount = 1;
	region.imageOffset = { 0, 0, 0 };
	region.imageExtent = { width, height, 1 };

	m_commandBuffers[0]->copyBufferToImage(buffer, image, vk::ImageLayout::eTransferDstOptimal, region);

	endSingleTimeCommands(m_commandBuffers);
}
//-----------------------------------------------------------------------------
void VKRenderer::generateMipmaps(vk::Image image, vk::Format imageFormat, int32_t texWidth, int32_t texHeight, uint32_t m_mipLevels)
{
	std::vector<vk::UniqueCommandBuffer> m_commandBuffers = beginSingleTimeCommands();

	vk::FormatProperties formatProperties = m_physicalDevice.getFormatProperties(imageFormat);
	if( !(formatProperties.optimalTilingFeatures & vk::FormatFeatureFlagBits::eSampledImageFilterLinear) )
		throw std::runtime_error("texture image format does not support linear blittings!");

	vk::ImageMemoryBarrier barrier = {};
	barrier.image = image;
	barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
	barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
	barrier.subresourceRange.aspectMask = vk::ImageAspectFlagBits::eColor;
	barrier.subresourceRange.baseArrayLayer = 0;
	barrier.subresourceRange.layerCount = 1;
	barrier.subresourceRange.levelCount = 1;

	int32_t mipWidth = texWidth;
	int32_t mipHeight = texHeight;

	for( uint32_t i = 1; i < m_mipLevels; i++ )
	{
		barrier.subresourceRange.baseMipLevel = i - 1;
		barrier.oldLayout = vk::ImageLayout::eTransferDstOptimal;
		barrier.newLayout = vk::ImageLayout::eTransferSrcOptimal;
		barrier.srcAccessMask = vk::AccessFlagBits::eTransferWrite;
		barrier.dstAccessMask = vk::AccessFlagBits::eTransferRead;

		m_commandBuffers[0]->pipelineBarrier(
			vk::PipelineStageFlagBits::eTransfer, vk::PipelineStageFlagBits::eTransfer, vk::DependencyFlags(),
			nullptr, nullptr, barrier
		);

		vk::ImageBlit blit = {};
		blit.srcOffsets[0] = { 0, 0, 0 };
		blit.srcOffsets[1] = { mipWidth, mipHeight, 1 };
		blit.srcSubresource.aspectMask = vk::ImageAspectFlagBits::eColor;
		blit.srcSubresource.mipLevel = i - 1;
		blit.srcSubresource.baseArrayLayer = 0;
		blit.srcSubresource.layerCount = 1;
		blit.dstOffsets[0] = { 0, 0, 0 };
		blit.dstOffsets[1] = { mipWidth > 1 ? mipWidth / 2 : 1, mipHeight > 1 ? mipHeight / 2 : 1, 1 };
		blit.dstSubresource.aspectMask = vk::ImageAspectFlagBits::eColor;
		blit.dstSubresource.mipLevel = i;
		blit.dstSubresource.baseArrayLayer = 0;
		blit.dstSubresource.layerCount = 1;

		m_commandBuffers[0]->blitImage(
			image, vk::ImageLayout::eTransferSrcOptimal,
			image, vk::ImageLayout::eTransferDstOptimal,
			blit, vk::Filter::eLinear
		);

		barrier.oldLayout = vk::ImageLayout::eTransferSrcOptimal;
		barrier.newLayout = vk::ImageLayout::eShaderReadOnlyOptimal;
		barrier.srcAccessMask = vk::AccessFlagBits::eTransferRead;
		barrier.dstAccessMask = vk::AccessFlagBits::eShaderRead;

		m_commandBuffers[0]->pipelineBarrier(
			vk::PipelineStageFlagBits::eTransfer, vk::PipelineStageFlagBits::eFragmentShader, vk::DependencyFlags(),
			nullptr, nullptr, barrier
		);

		if( mipWidth > 1 ) mipWidth /= 2;
		if( mipHeight > 1 ) mipHeight /= 2;
	}

	barrier.subresourceRange.baseMipLevel = m_mipLevels - 1;
	barrier.oldLayout = vk::ImageLayout::eTransferDstOptimal;
	barrier.newLayout = vk::ImageLayout::eShaderReadOnlyOptimal;
	barrier.srcAccessMask = vk::AccessFlagBits::eTransferWrite;
	barrier.dstAccessMask = vk::AccessFlagBits::eShaderRead;

	m_commandBuffers[0]->pipelineBarrier(
		vk::PipelineStageFlagBits::eTransfer, vk::PipelineStageFlagBits::eFragmentShader, vk::DependencyFlags(),
		nullptr, nullptr, barrier
	);

	endSingleTimeCommands(m_commandBuffers);
}
//-----------------------------------------------------------------------------
void VKRenderer::createTextureImageView()
{
	m_textureImageView = createImageView(m_textureImage.get(), vk::Format::eR8G8B8A8Unorm, vk::ImageAspectFlagBits::eColor, m_mipLevels);
}
//-----------------------------------------------------------------------------
void VKRenderer::createTextureSampler()
{
	vk::SamplerCreateInfo samplerInfo = {};
	samplerInfo.magFilter = vk::Filter::eNearest;
	samplerInfo.minFilter = vk::Filter::eNearest;
	samplerInfo.addressModeU = vk::SamplerAddressMode::eRepeat;
	samplerInfo.addressModeV = vk::SamplerAddressMode::eRepeat;
	samplerInfo.addressModeW = vk::SamplerAddressMode::eRepeat;
	samplerInfo.anisotropyEnable = true;
	samplerInfo.maxAnisotropy = 16;
	samplerInfo.borderColor = vk::BorderColor::eIntOpaqueBlack;
	samplerInfo.unnormalizedCoordinates = false;
	samplerInfo.compareEnable = false;
	samplerInfo.compareOp = vk::CompareOp::eAlways;
	samplerInfo.mipmapMode = vk::SamplerMipmapMode::eLinear;
	samplerInfo.mipLodBias = 0.0f;
	samplerInfo.minLod = 0.0f;
	samplerInfo.maxLod = static_cast<float>(m_mipLevels);

	m_textureSampler = m_device->createSamplerUnique(samplerInfo);
}
//-----------------------------------------------------------------------------
void VKRenderer::loadModel()
{
	tinyobj::attrib_t attrib;
	std::vector<tinyobj::shape_t> shapes;
	std::vector<tinyobj::material_t> materials;
	std::string warn, err;

	if( !tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, MODEL_PATH.c_str()) )
		throw std::runtime_error(warn + err);

	std::unordered_map<Vertex, uint32_t> uniqueVertices = {};

	for( const auto& shape : shapes )
	{
		for( const auto& index : shape.mesh.indices )
		{
			Vertex vertex = {};

			vertex.pos = {
				attrib.vertices[3 * index.vertex_index + 0],
				attrib.vertices[3 * index.vertex_index + 1],
				attrib.vertices[3 * index.vertex_index + 2]
			};

			vertex.texCoord = {
				attrib.texcoords[2 * index.texcoord_index + 0],
				1.0f - attrib.texcoords[2 * index.texcoord_index + 1]
			};

			vertex.color = { 1.0f, 1.0f, 1.0f };

			if( uniqueVertices.count(vertex) == 0 )
			{
				uniqueVertices[vertex] = static_cast<uint32_t>(m_vertices.size());
				m_vertices.push_back(vertex);
			}

			m_indices.push_back(uniqueVertices[vertex]);
		}
	}
}
//-----------------------------------------------------------------------------
void VKRenderer::createVertexBuffer()
{
	vk::DeviceSize bufferSize = sizeof(m_vertices[0]) * m_vertices.size();

	vk::UniqueBuffer stagingBuffer;
	vk::UniqueDeviceMemory stagingBufferMemory;
	createBuffer(
		bufferSize, vk::BufferUsageFlagBits::eTransferSrc,
		vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent,
		stagingBuffer, stagingBufferMemory
	);

	void* data = m_device->mapMemory(stagingBufferMemory.get(), 0, bufferSize, vk::MemoryMapFlags());
	memcpy(data, m_vertices.data(), (size_t)bufferSize);
	m_device->unmapMemory(stagingBufferMemory.get());

	createBuffer(
		bufferSize, vk::BufferUsageFlagBits::eTransferDst | vk::BufferUsageFlagBits::eVertexBuffer,
		vk::MemoryPropertyFlagBits::eDeviceLocal,
		m_vertexBuffer, m_vertexBufferMemory
	);

	copyBuffer(stagingBuffer.get(), m_vertexBuffer.get(), bufferSize);
}
//-----------------------------------------------------------------------------
void VKRenderer::copyBuffer(vk::Buffer srcBuffer, vk::Buffer dstBuffer, vk::DeviceSize size)
{
	std::vector<vk::UniqueCommandBuffer> m_commandBuffers = beginSingleTimeCommands();

	vk::BufferCopy copyRegion(0, 0, size);
	m_commandBuffers[0]->copyBuffer(srcBuffer, dstBuffer, copyRegion);

	endSingleTimeCommands(m_commandBuffers);
}
//-----------------------------------------------------------------------------
void VKRenderer::createIndexBuffer()
{
	vk::DeviceSize bufferSize = sizeof(m_indices[0]) * m_indices.size();

	vk::UniqueBuffer stagingBuffer;
	vk::UniqueDeviceMemory stagingBufferMemory;
	createBuffer(
		bufferSize, vk::BufferUsageFlagBits::eTransferSrc,
		vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent,
		stagingBuffer, stagingBufferMemory
	);

	void* data = m_device->mapMemory(stagingBufferMemory.get(), 0, bufferSize, vk::MemoryMapFlags());
	memcpy(data, m_indices.data(), (size_t)bufferSize);
	m_device->unmapMemory(stagingBufferMemory.get());

	createBuffer(
		bufferSize, vk::BufferUsageFlagBits::eTransferDst | vk::BufferUsageFlagBits::eIndexBuffer,
		vk::MemoryPropertyFlagBits::eDeviceLocal,
		m_indexBuffer, m_indexBufferMemory
	);

	copyBuffer(stagingBuffer.get(), m_indexBuffer.get(), bufferSize);
}
//-----------------------------------------------------------------------------
void VKRenderer::createUniformBuffers()
{
	vk::DeviceSize bufferSize = sizeof(UniformBufferObject);

	m_uniformBuffers.resize(m_swapchainImages.size());
	m_uniformBuffersMemory.resize(m_swapchainImages.size());

	for( size_t i = 0; i < m_swapchainImages.size(); ++i )
	{
		createBuffer(
			bufferSize, vk::BufferUsageFlagBits::eUniformBuffer,
			vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent,
			m_uniformBuffers[i], m_uniformBuffersMemory[i]
		);
	}
}
//-----------------------------------------------------------------------------
void VKRenderer::createDescriptorPool()
{
	std::array<vk::DescriptorPoolSize, 2> poolSizes = {};
	poolSizes[0].type = vk::DescriptorType::eUniformBuffer;
	poolSizes[0].descriptorCount = static_cast<uint32_t>(m_swapchainImages.size());
	poolSizes[1].type = vk::DescriptorType::eCombinedImageSampler;
	poolSizes[1].descriptorCount = static_cast<uint32_t>(m_swapchainImages.size());

	vk::DescriptorPoolCreateInfo poolInfo = {};
	poolInfo.poolSizeCount = poolSizes.size();
	poolInfo.pPoolSizes = poolSizes.data();
	poolInfo.maxSets = static_cast<uint32_t>(m_swapchainImages.size());

	m_descriptorPool = m_device->createDescriptorPoolUnique(poolInfo);
}
//-----------------------------------------------------------------------------
void VKRenderer::createDescriptorSets()
{
	std::vector<vk::DescriptorSetLayout> layouts(m_swapchainImages.size(), m_descriptorSetLayout.get());

	vk::DescriptorSetAllocateInfo allocInfo = {};
	allocInfo.descriptorPool = m_descriptorPool.get();
	allocInfo.descriptorSetCount = static_cast<uint32_t>(m_swapchainImages.size());
	allocInfo.pSetLayouts = layouts.data();

	m_descriptorSets = m_device->allocateDescriptorSets(allocInfo);

	for( size_t i = 0; i < m_swapchainImages.size(); ++i )
	{
		vk::DescriptorBufferInfo bufferInfo = {};
		bufferInfo.buffer = m_uniformBuffers[i].get();
		bufferInfo.offset = 0;
		bufferInfo.range = sizeof(UniformBufferObject);

		vk::DescriptorImageInfo imageInfo = {};
		imageInfo.imageLayout = vk::ImageLayout::eShaderReadOnlyOptimal;
		imageInfo.imageView = m_textureImageView.get();
		imageInfo.sampler = m_textureSampler.get();

		std::array<vk::WriteDescriptorSet, 2> descriptorWrites = {};

		descriptorWrites[0].dstSet = m_descriptorSets[i];
		descriptorWrites[0].dstBinding = 0;
		descriptorWrites[0].dstArrayElement = 0;
		descriptorWrites[0].descriptorType = vk::DescriptorType::eUniformBuffer;
		descriptorWrites[0].descriptorCount = 1;
		descriptorWrites[0].pBufferInfo = &bufferInfo;

		descriptorWrites[1].dstSet = m_descriptorSets[i];
		descriptorWrites[1].dstBinding = 1;
		descriptorWrites[1].dstArrayElement = 0;
		descriptorWrites[1].descriptorType = vk::DescriptorType::eCombinedImageSampler;
		descriptorWrites[1].descriptorCount = 1;
		descriptorWrites[1].pImageInfo = &imageInfo;

		m_device->updateDescriptorSets(descriptorWrites.size(), descriptorWrites.data(), 0, nullptr);
	}
}
//-----------------------------------------------------------------------------
void VKRenderer::createCommandBuffers()
{
	m_commandBuffers.resize(m_swapchainFramebuffers.size());

	m_commandBuffers = m_device->allocateCommandBuffersUnique(
		vk::CommandBufferAllocateInfo(
			m_commandPool.get(), vk::CommandBufferLevel::ePrimary, static_cast<uint32_t>(m_commandBuffers.size())
		)
	);

	for( size_t i = 0; i < m_commandBuffers.size(); ++i )
	{
		m_commandBuffers[i]->begin(
			vk::CommandBufferBeginInfo(
				vk::CommandBufferUsageFlags(), nullptr
			)
		);

		std::array<vk::ClearValue, 2> clearValues;
		clearValues[0].color = vk::ClearColorValue(std::array<float, 4>{0.0f, 0.0f, 0.0f, 1.0f});
		clearValues[1].depthStencil = { 1.0f, 0 };

		vk::RenderPassBeginInfo renderPassBeginInfo(
			m_renderPass.get(), m_swapchainFramebuffers[i].get(), vk::Rect2D({ 0, 0 }, m_swapchainExtent),
			static_cast<uint32_t>(clearValues.size()), clearValues.data()
		);
		m_commandBuffers[i]->beginRenderPass(&renderPassBeginInfo, vk::SubpassContents::eInline);
		m_commandBuffers[i]->bindPipeline(vk::PipelineBindPoint::eGraphics, m_graphicsPipelines[0].get());
		m_commandBuffers[i]->bindVertexBuffers(0, { m_vertexBuffer.get() }, { 0 });
		m_commandBuffers[i]->bindIndexBuffer(m_indexBuffer.get(), 0, vk::IndexType::eUint32);
		m_commandBuffers[i]->bindDescriptorSets(vk::PipelineBindPoint::eGraphics, m_pipelineLayout.get(), 0, 1, &m_descriptorSets[i], 0, nullptr);

		m_commandBuffers[i]->drawIndexed(static_cast<uint32_t>(m_indices.size()), 1, 0, 0, 0);

		m_commandBuffers[i]->endRenderPass();
		m_commandBuffers[i]->end();
	}
}
//-----------------------------------------------------------------------------
void VKRenderer::createSyncObjects()
{
	m_imageAvailableSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
	m_renderFinishedSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
	m_inFlightFences.resize(MAX_FRAMES_IN_FLIGHT);
	m_imagesInFlight.resize(m_swapchainImages.size(), -1);

	for( size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; ++i )
	{
		m_imageAvailableSemaphores[i] = m_device->createSemaphoreUnique(vk::SemaphoreCreateInfo(vk::SemaphoreCreateFlags()));
		m_renderFinishedSemaphores[i] = m_device->createSemaphoreUnique(vk::SemaphoreCreateInfo(vk::SemaphoreCreateFlags()));
		m_inFlightFences[i] = m_device->createFenceUnique(vk::FenceCreateInfo(vk::FenceCreateFlagBits::eSignaled));
	}
}
//-----------------------------------------------------------------------------
void VKRenderer::recreateSwapchain()
{
	m_device->waitIdle();

	createSwapchain();
	createImageViews();
	createRenderPass();
	createGraphicsPipeline();
	createColorResources();
	createDepthResources();
	createFramebuffers();

	m_commandBuffers.clear();
	m_uniformBuffers.clear();
	m_uniformBuffersMemory.clear();

	createCommandPool();
	createUniformBuffers();
	createDescriptorPool();
	createDescriptorSets();
	createCommandBuffers();
}
//-----------------------------------------------------------------------------
void VKRenderer::updateUniformBuffer(uint32_t currentImage)
{
	static auto startTime = std::chrono::high_resolution_clock::now();

	auto currentTime = std::chrono::high_resolution_clock::now();
	float time = std::chrono::duration<float, std::chrono::seconds::period>(currentTime - startTime).count();

	UniformBufferObject ubo = {};
	ubo.model = glm::rotate(glm::mat4(1.0f), time * glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	ubo.view = glm::lookAt(glm::vec3(2.0f, 2.0f, 2.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	ubo.proj = glm::perspective(glm::radians(45.0f), m_swapchainExtent.width / (float)m_swapchainExtent.height, 0.1f, 10.0f);
	ubo.proj[1][1] *= -1;

	void* data = m_device->mapMemory(m_uniformBuffersMemory[currentImage].get(), 0, sizeof(ubo), vk::MemoryMapFlags());
	memcpy(data, &ubo, sizeof(ubo));
	m_device->unmapMemory(m_uniformBuffersMemory[currentImage].get());
}
//-----------------------------------------------------------------------------
bool VKRenderer::hasStencilComponent(vk::Format format)
{
	return format == vk::Format::eD32SfloatS8Uint || format == vk::Format::eD24UnormS8Uint;
}
//-----------------------------------------------------------------------------