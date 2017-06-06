#pragma once
#define GLFW_INCLUDE_VULKAN
#include <vector>
//#include "Utilities.h"
#include "VDeleter.h"
#include <GLFW/glfw3.h>

class Model
{
public:
	Model(VDeleter<VkDevice>& device, uint32_t uMemoryType, VkCommandPool& vCommandPool, VkQueue& grapQueue);
	~Model();

	void createBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VDeleter<VkBuffer>& buffer, VDeleter<VkDeviceMemory>& bufferMemory);
	void copyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);
	VkCommandBuffer beginSingleTimeCommands();
	void endSingleTimeCommands(VkCommandBuffer commandBuffer);

private:
	VDeleter<VkDevice> device;
	uint32_t memoryType;
	VkCommandPool commandPool;
	VkQueue graphicsQueue;

	VDeleter<VkBuffer> vertexBuffer{ device, vkDestroyBuffer };
	VDeleter<VkDeviceMemory> vertexBufferMemory{ device, vkFreeMemory };
	VDeleter<VkBuffer> indexBuffer{ device, vkDestroyBuffer };
	VDeleter<VkDeviceMemory> indexMemoryBuffer{ device, vkFreeMemory };

	VDeleter<VkBuffer> uniformStagingBuffer{ device, vkDestroyBuffer };
	VDeleter<VkDeviceMemory> uniformStagingBufferMemory{ device, vkFreeMemory };
	VDeleter<VkBuffer> uniformBuffer{ device, vkDestroyBuffer };
	VDeleter<VkDeviceMemory> uniformBufferMemory{ device, vkFreeMemory };

//	std::vector<Vertex> vertices;
	std::vector<uint32_t> indices;

	const std::string MODEL_PATH;
	const std::string TEXTURE_PATH;

	void loadModel();

	void createVertexBuffer();
	void createIndexBuffer();
	void createUniformBuffer();

	void createDescriptorPool();
	void createDescriptorSet();
	void createCommandBuffers();

};

