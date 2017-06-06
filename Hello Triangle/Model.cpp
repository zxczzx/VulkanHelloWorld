#include "Model.h"
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#define STB_IMAGE_IMPLEMENTATION
#define TINYOBJLOADER_IMPLEMENTATION
#include <tiny_obj_loader.h>
#include <unordered_map>


Model::Model(VDeleter<VkDevice>& pDevice, uint32_t uMemoryType, VkCommandPool& vCommandPool, VkQueue& grapQueue) : MODEL_PATH("models/cube.obj"), TEXTURE_PATH("textures/container2.png"), 
	device(pDevice), memoryType(uMemoryType), commandPool(vCommandPool), graphicsQueue(grapQueue)
{
}

Model::~Model()
{
}

void Model::createBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VDeleter<VkBuffer>& buffer, VDeleter<VkDeviceMemory>& bufferMemory)
{
	VkBufferCreateInfo bufferInfo = {};
	bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
	bufferInfo.size = size;
	bufferInfo.usage = usage;
	bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

	if (vkCreateBuffer(device, &bufferInfo, nullptr, buffer.replace()) != VK_SUCCESS)
	{
		throw std::runtime_error("failed to create vertex buffer!");
	}

	VkMemoryRequirements memRequirements;
	vkGetBufferMemoryRequirements(device, buffer, &memRequirements);

	VkMemoryAllocateInfo allocInfo = {};
	allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
	allocInfo.allocationSize = memRequirements.size;
	allocInfo.memoryTypeIndex = memoryType;

	if (vkAllocateMemory(device, &allocInfo, nullptr, bufferMemory.replace()) != VK_SUCCESS)
	{
		throw std::runtime_error("failed to allocate vertex buffer memory!");
	}

	vkBindBufferMemory(device, buffer, bufferMemory, 0);
}

void Model::copyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size)
{
	VkCommandBuffer commandBuffer = beginSingleTimeCommands();

	VkBufferCopy copyRegion = {};
	copyRegion.size = size;
	vkCmdCopyBuffer(commandBuffer, srcBuffer, dstBuffer, 1, &copyRegion);

	endSingleTimeCommands(commandBuffer);
}

VkCommandBuffer Model::beginSingleTimeCommands()
{
	VkCommandBufferAllocateInfo allocInfo = {};
	allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
	allocInfo.commandPool = commandPool;
	allocInfo.commandBufferCount = 1;

	VkCommandBuffer commandBuffer;
	vkAllocateCommandBuffers(device, &allocInfo, &commandBuffer);

	VkCommandBufferBeginInfo beginInfo = {};
	beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
	beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

	vkBeginCommandBuffer(commandBuffer, &beginInfo);

	return commandBuffer;
}

void Model::endSingleTimeCommands(VkCommandBuffer commandBuffer)
{
	vkEndCommandBuffer(commandBuffer);

	VkSubmitInfo submitInfo = {};
	submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
	submitInfo.commandBufferCount = 1;
	submitInfo.pCommandBuffers = &commandBuffer;

	vkQueueSubmit(graphicsQueue, 1, &submitInfo, VK_NULL_HANDLE);
	vkQueueWaitIdle(graphicsQueue);

	vkFreeCommandBuffers(device, commandPool, 1, &commandBuffer);
}

//void Model::loadModel()
//{
//	tinyobj::attrib_t attrib;
//	std::vector<tinyobj::shape_t> shapes;
//	std::vector<tinyobj::material_t> materials;
//	std::string err;
//
//	if (!LoadObj(&attrib, &shapes, &materials, &err, MODEL_PATH.c_str()))
//	{
//		throw std::runtime_error(err);
//	}
//
//	std::unordered_map<Vertex, uint32_t> uniqueVertices = {};
//
//	for (const auto& shape : shapes)
//	{
//		for (const auto& index : shape.mesh.indices)
//		{
//			Vertex vertex = {};
//
//			vertex.pos = {
//				attrib.vertices[3 * index.vertex_index + 0],
//				attrib.vertices[3 * index.vertex_index + 1],
//				attrib.vertices[3 * index.vertex_index + 2]
//			};
//
//			vertex.texCoord = {
//				attrib.texcoords[2 * index.texcoord_index + 0],
//				1.0f - attrib.texcoords[2 * index.texcoord_index + 1]
//			};
//
//			vertex.color = { 1.0f, 1.0f, 1.0f };
//
//			if (uniqueVertices.count(vertex) == 0)
//			{
//				uniqueVertices[vertex] = static_cast<uint32_t>(vertices.size());
//				vertices.push_back(vertex);
//			}
//
//			indices.push_back(uniqueVertices[vertex]);
//		}
//	}
//}
//
//void Model::createVertexBuffer()
//{
//	VkDeviceSize bufferSize = sizeof(vertices[0]) * vertices.size();
//
//	VDeleter<VkBuffer> stagingBuffer{ device, vkDestroyBuffer };
//	VDeleter<VkDeviceMemory> stagingBufferMemory{ device, vkFreeMemory };
//	createBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory);
//
//	void* data;
//	vkMapMemory(device, stagingBufferMemory, 0, bufferSize, 0, &data);
//	memcpy(data, vertices.data(), (size_t)bufferSize);
//	vkUnmapMemory(device, stagingBufferMemory);
//
//	createBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, vertexBuffer, vertexBufferMemory);
//
//	copyBuffer(stagingBuffer, vertexBuffer, bufferSize);
//}
