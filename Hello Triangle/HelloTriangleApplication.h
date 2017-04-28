#pragma once
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <vector>
#include "VDeleter.h"
#include "Utilities.h"

class HelloTriangleApplication 
{
public:
	HelloTriangleApplication();

	void run();

	const std::vector<const char*> validationLayers =
	{
		"VK_LAYER_LUNARG_standard_validation"
	};

	const std::vector<const char*> deviceExtensions =
	{
		VK_KHR_SWAPCHAIN_EXTENSION_NAME
	};

	const std::vector<Vertex> vertices = {
		{ { 0.0f, -0.5f },	{ 1.0f, 1.0f, 1.0f } },
		{ { 0.5f, 0.5f },	{ 0.0f, 1.0f, 0.0f } },
		{ { -0.5f, 0.5f },	{ 0.0f, 0.0f, 1.0f } }
	};

private:
	const int WIDTH;
	const int HEIGHT;
	GLFWwindow* window;

	VDeleter<VkInstance> instance{ vkDestroyInstance };
	VDeleter<VkSurfaceKHR> surface{ instance, vkDestroySurfaceKHR };
	VDeleter<VkDevice> device{ vkDestroyDevice };
	VDeleter<VkDebugReportCallbackEXT> callback { instance, Utilities::DestroyDebugReportCallbackEXT };
	VDeleter<VkSwapchainKHR> swapChain{ device, vkDestroySwapchainKHR };
	std::vector<VDeleter<VkImageView>> swapChainImageViews;
	VDeleter<VkPipelineLayout> pipelineLayout{ device, vkDestroyPipelineLayout };
	VDeleter<VkRenderPass> renderPass{ device, vkDestroyRenderPass };
	VDeleter<VkPipeline> graphicsPipeline{ device, vkDestroyPipeline };
	std::vector<VDeleter<VkFramebuffer>> swapChainFramebuffers;
	VDeleter<VkCommandPool> commandPool{ device, vkDestroyCommandPool };
	VDeleter<VkSemaphore> imageAvailableSemaphore{ device, vkDestroySemaphore };
	VDeleter<VkSemaphore> renderFinishedSemaphore{ device, vkDestroySemaphore };
	VDeleter<VkBuffer> vertexBuffer{ device, vkDestroyBuffer };
	VDeleter<VkDeviceMemory> vertexBufferMemory{ device, vkFreeMemory };

	std::vector<VkCommandBuffer> commandBuffers;
	std::vector<VkImage> swapChainImages;
	VkFormat swapChainImageFormat;
	VkExtent2D swapChainExtent;
	VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
	VkQueue graphicQueue;
	VkQueue presentQueue;

	void initWindow();
	int rateDeviceSuitability(VkPhysicalDevice device);
	void pickPhysicalDevice();
	QueueFamiliyIndices findQueueFamilies(VkPhysicalDevice device);
	bool isDeviceSuitable(VkPhysicalDevice device);
	bool checkDeviceExtensionSupport(VkPhysicalDevice device);
	VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
	VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR> availablePresentModes);
	VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities);
	SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice device);
	uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);

	static void onWindowResized(GLFWwindow* window, int width, int height);
	void recreateSwapChain();

	void createLogicalDevice();
	void createSurface();
	void createSwapChain();
	void createImageViews();
	void createShaderModule(const std::vector<char>& code, VDeleter<VkShaderModule>& shaderModule);
	void createGraphicPipeline();
	void createRenderPass();
	void createFramebuffers();
	void createCommandPools();
	void createCommandBuffers();
	void createSemaphores();
	void createVertexBuffer();
	void initVulkan();
	void drawFrame();
	void mainLoop();
	void createInstance();
	void setupDebugCallback();

	static std::vector<const char*> getRequiredExtensions();
	bool checkValidationLayerSupport();
	static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(VkDebugReportFlagsEXT flags, VkDebugReportObjectTypeEXT objType, uint64_t obj, size_t location, int32_t code,
		const char* layerPrefix, const char* msg, void* userData);
};