#include "stdafx.h"
#include "Vertex.h"
//-----------------------------------------------------------------------------
vk::VertexInputBindingDescription Vertex2::GetBindingDescription()
{
	vk::VertexInputBindingDescription bindingDescription = {};
	bindingDescription.binding = 0;
	bindingDescription.stride = sizeof(Vertex2);
	bindingDescription.inputRate = vk::VertexInputRate::eVertex;

	return bindingDescription;
}
//-----------------------------------------------------------------------------
std::array<vk::VertexInputAttributeDescription, 4> Vertex2::GetAttributeDescriptions()
{
	std::array<vk::VertexInputAttributeDescription, 4> attributeDescriptions = {};

	attributeDescriptions[0].binding = 0;
	attributeDescriptions[0].location = 0;
	attributeDescriptions[0].format = vk::Format::eR32G32Sfloat;
	attributeDescriptions[0].offset = offsetof(Vertex2, pos);

	attributeDescriptions[1].binding = 0;
	attributeDescriptions[1].location = 1;
	attributeDescriptions[1].format = vk::Format::eR32G32B32A32Sfloat;
	attributeDescriptions[1].offset = offsetof(Vertex2, color);

	attributeDescriptions[2].binding = 0;
	attributeDescriptions[2].location = 2;
	attributeDescriptions[2].format = vk::Format::eR32G32Sfloat;
	attributeDescriptions[2].offset = offsetof(Vertex2, texCoord);

	attributeDescriptions[3].binding = 0;
	attributeDescriptions[3].location = 3;
	attributeDescriptions[3].format = vk::Format::eR32G32B32A32Sint;
	attributeDescriptions[3].offset = offsetof(Vertex2, data);

	return attributeDescriptions;
}
//-----------------------------------------------------------------------------
vk::VertexInputBindingDescription Vertex3::GetBindingDescription()
{
	vk::VertexInputBindingDescription bindingDescription = {};
	bindingDescription.binding = 0;
	bindingDescription.stride = sizeof(Vertex3);
	bindingDescription.inputRate = vk::VertexInputRate::eVertex;

	return bindingDescription;
}
//-----------------------------------------------------------------------------
std::array<vk::VertexInputAttributeDescription, 3> Vertex3::GetAttributeDescriptions()
{
	std::array<vk::VertexInputAttributeDescription, 3> attributeDescriptions = {};

	attributeDescriptions[0].binding = 0;
	attributeDescriptions[0].location = 0;
	attributeDescriptions[0].format = vk::Format::eR32G32B32Sfloat;
	attributeDescriptions[0].offset = offsetof(Vertex3, pos);

	attributeDescriptions[1].binding = 0;
	attributeDescriptions[1].location = 1;
	attributeDescriptions[1].format = vk::Format::eR32G32B32Sfloat;
	attributeDescriptions[1].offset = offsetof(Vertex3, color);

	attributeDescriptions[2].binding = 0;
	attributeDescriptions[2].location = 2;
	attributeDescriptions[2].format = vk::Format::eR32G32Sfloat;
	attributeDescriptions[2].offset = offsetof(Vertex3, texCoord);

	return attributeDescriptions;
}
//-----------------------------------------------------------------------------