#pragma once

struct Vertex2
{
	static vk::VertexInputBindingDescription GetBindingDescription();
	static std::array<vk::VertexInputAttributeDescription, 4> GetAttributeDescriptions();

	glm::vec2 pos;
	glm::vec4 color;
	glm::vec2 texCoord;

	union
	{
		struct
		{
			int32_t blendMode;
		};
		int32_t data[4];
	};
};

struct Vertex3
{
	static vk::VertexInputBindingDescription GetBindingDescription();

	static std::array<vk::VertexInputAttributeDescription, 3> GetAttributeDescriptions();

	bool operator==(const Vertex3 &other) const
	{
		return pos == other.pos && color == other.color && texCoord == other.texCoord;
	}

	glm::vec3 pos;
	glm::vec3 color;
	glm::vec2 texCoord;
};

namespace std
{
	template<> struct hash<Vertex3>
	{
		size_t operator()(Vertex3 const &vertex) const
		{
			return ((hash<glm::vec3>()(vertex.pos) ^ (hash<glm::vec3>()(vertex.color) << 1)) >> 1) ^ (hash<glm::vec2>()(vertex.texCoord) << 1);
		}
	};
}