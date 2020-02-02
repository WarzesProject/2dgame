#pragma once

#include "Renderer.h"
#include "VKCore.h"

struct Vertex
{
	glm::vec3 pos;
	glm::vec3 color;
	glm::vec2 texCoord;

	static vk::VertexInputBindingDescription GetBindingDescription()
	{
		vk::VertexInputBindingDescription bindingDescription = {};

		bindingDescription.binding = 0;
		bindingDescription.stride = sizeof(Vertex);
		bindingDescription.inputRate = vk::VertexInputRate::eVertex;

		return bindingDescription;
	}

	static std::array<vk::VertexInputAttributeDescription, 3> GetAttributeDescriptions()
	{
		std::array<vk::VertexInputAttributeDescription, 3> attributeDescriptions = {};

		attributeDescriptions[0].binding = 0;
		attributeDescriptions[0].location = 0;
		attributeDescriptions[0].format = vk::Format::eR32G32B32Sfloat;
		attributeDescriptions[0].offset = offsetof(Vertex, pos);

		attributeDescriptions[1].binding = 0;
		attributeDescriptions[1].location = 1;
		attributeDescriptions[1].format = vk::Format::eR32G32B32Sfloat;
		attributeDescriptions[1].offset = offsetof(Vertex, color);

		attributeDescriptions[2].binding = 0;
		attributeDescriptions[2].location = 2;
		attributeDescriptions[2].format = vk::Format::eR32G32Sfloat;
		attributeDescriptions[2].offset = offsetof(Vertex, texCoord);

		return attributeDescriptions;
	}

	bool operator==(const Vertex &other) const
	{
		return pos == other.pos && color == other.color && texCoord == other.texCoord;
	}
};

namespace std
{
	template<> struct hash<Vertex>
	{
		size_t operator()(Vertex const &vertex) const
		{
			return ((hash<glm::vec3>()(vertex.pos) ^ (hash<glm::vec3>()(vertex.color) << 1)) >> 1) ^ (hash<glm::vec2>()(vertex.texCoord) << 1);
		}
	};
}

struct UniformBufferObject
{
	glm::mat4 model;
	glm::mat4 view;
	glm::mat4 proj;
};

struct QueueFamilyIndices
{
	bool IsComplete()
	{
		return graphicsFamily.has_value() && presentFamily.has_value();
	}

	std::optional<uint32_t> graphicsFamily;
	std::optional<uint32_t> presentFamily;
};

struct SwapchainSupportDetails
{
	vk::SurfaceCapabilitiesKHR capabilities;
	std::vector<vk::SurfaceFormatKHR> formats;
	std::vector<vk::PresentModeKHR> presentModes;
};

class VKRenderer final : public Renderer
{
public:
	VKRenderer(SDL_Window *window);
	~VKRenderer() final;

	void Resize() final;
	void Draw() final;

private:
	void createInstance();
	void setupDebugMessenger();
	void createSurface();
	void pickPhysicalDevice();
	bool isDeviceSuitable(vk::PhysicalDevice device);
	QueueFamilyIndices findQueueFamilies(vk::PhysicalDevice device);
	bool checkDeviceExtensionSupport(vk::PhysicalDevice device);
	SwapchainSupportDetails querySwapchainSupport(vk::PhysicalDevice device);
	vk::SampleCountFlagBits getMaxUsableSampleCount();
	void createLogicalDevice();
	void createSwapchain();
	vk::SurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<vk::SurfaceFormatKHR>& availableFormats);
	vk::PresentModeKHR chooseSwapPresentMode(const std::vector<vk::PresentModeKHR>& availablePresentModes);
	vk::Extent2D chooseSwapExtent(const vk::SurfaceCapabilitiesKHR& capabilities);
	void createImageViews();
	vk::UniqueImageView createImageView(vk::Image image, vk::Format format, vk::ImageAspectFlags aspectFlags, uint32_t mipLevels);
	void createRenderPass();
	vk::Format findDepthFormat();
	vk::Format findSupportedFormat(const std::vector<vk::Format>& candidates, vk::ImageTiling tiling, vk::FormatFeatureFlags features);
	void createDescriptorSetLayout();
	void createGraphicsPipeline();
	vk::UniqueShaderModule createShaderModule(const std::vector<char>& code);
	void createCommandPool();
	void createColorResources();
	void createImage(uint32_t width, uint32_t height, uint32_t m_mipLevels, vk::SampleCountFlagBits numSamples, vk::Format format, vk::ImageTiling tiling, vk::ImageUsageFlags usage, vk::MemoryPropertyFlags properties, vk::UniqueImage& image, vk::UniqueDeviceMemory& imageMemory);
	uint32_t findMemoryType(uint32_t typeFilter, vk::MemoryPropertyFlags properties);
	void createDepthResources();
	void createFramebuffers();
	void createTextureImage();
	void createBuffer(vk::DeviceSize size, vk::BufferUsageFlags usage, vk::MemoryPropertyFlags properties, vk::UniqueBuffer& buffer, vk::UniqueDeviceMemory& bufferMemory);
	void transitionImageLayout(vk::Image image, vk::Format format, vk::ImageLayout oldLayout, vk::ImageLayout newLayout, uint32_t m_mipLevels);
	std::vector<vk::UniqueCommandBuffer> beginSingleTimeCommands();
	void endSingleTimeCommands(std::vector<vk::UniqueCommandBuffer>& m_commandBuffers);
	void copyBufferToImage(vk::Buffer buffer, vk::Image image, uint32_t width, uint32_t height);
	void generateMipmaps(vk::Image image, vk::Format imageFormat, int32_t texWidth, int32_t texHeight, uint32_t m_mipLevels);
	void createTextureImageView();
	void createTextureSampler();
	void loadModel();
	void createVertexBuffer();
	void copyBuffer(vk::Buffer srcBuffer, vk::Buffer dstBuffer, vk::DeviceSize size);
	void createIndexBuffer();
	void createUniformBuffers();
	void createDescriptorPool();
	void createDescriptorSets();
	void createCommandBuffers();
	void createSyncObjects();
	void recreateSwapchain();
	void updateUniformBuffer(uint32_t currentImage);
	bool hasStencilComponent(vk::Format format);

	SDL_Window *m_window = nullptr;
	vk::UniqueInstance m_instance;
	vk::UniqueDebugUtilsMessengerEXT m_debugMessenger;
	vk::UniqueSurfaceKHR m_surface;
	vk::PhysicalDevice m_physicalDevice;
	vk::SampleCountFlagBits m_msaaSamples = vk::SampleCountFlagBits::e1;
	vk::UniqueDevice m_device;
	vk::Queue m_graphicsQueue;
	vk::Queue m_presentQueue;
	vk::UniqueSwapchainKHR m_swapchain;
	std::vector<vk::Image> m_swapchainImages;
	vk::Format m_swapchainImageFormat;
	vk::Extent2D m_swapchainExtent;
	std::vector<vk::UniqueImageView> m_swapchainImageViews;
	std::vector<vk::UniqueFramebuffer> m_swapchainFramebuffers;
	vk::UniqueRenderPass m_renderPass;
	vk::UniqueDescriptorSetLayout m_descriptorSetLayout;
	vk::UniquePipelineLayout m_pipelineLayout;
	std::vector<vk::UniquePipeline> m_graphicsPipelines;
	vk::UniqueCommandPool m_commandPool;

	vk::UniqueImage m_colorImage;
	vk::UniqueDeviceMemory m_colorImageMemory;
	vk::UniqueImageView m_colorImageView;

	vk::UniqueImage m_depthImage;
	vk::UniqueDeviceMemory m_depthImageMemory;
	vk::UniqueImageView m_depthImageView;

	uint32_t m_mipLevels;
	vk::UniqueImage m_textureImage;
	vk::UniqueDeviceMemory m_textureImageMemory;
	vk::UniqueImageView m_textureImageView;
	vk::UniqueSampler m_textureSampler;

	std::vector<Vertex> m_vertices;
	std::vector<uint32_t> m_indices;
	vk::UniqueBuffer m_vertexBuffer;
	vk::UniqueDeviceMemory m_vertexBufferMemory;
	vk::UniqueBuffer m_indexBuffer;
	vk::UniqueDeviceMemory m_indexBufferMemory;

	std::vector<vk::UniqueBuffer> m_uniformBuffers;
	std::vector<vk::UniqueDeviceMemory> m_uniformBuffersMemory;

	vk::UniqueDescriptorPool m_descriptorPool;
	std::vector<vk::DescriptorSet> m_descriptorSets;

	std::vector<vk::UniqueCommandBuffer> m_commandBuffers;

	std::vector<vk::UniqueSemaphore> m_imageAvailableSemaphores;
	std::vector<vk::UniqueSemaphore> m_renderFinishedSemaphores;
	std::vector<vk::UniqueFence> m_inFlightFences;
	std::vector<size_t> m_imagesInFlight;

	size_t m_currentFrame = 0;
};