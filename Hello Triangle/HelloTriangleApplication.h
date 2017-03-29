#pragma once
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include "VDeleter.h"
#include <vector>

class HelloTriangleApplication
{
public:
	HelloTriangleApplication();
	~HelloTriangleApplication();

	void createInstance();
	void initVulkan();
	void run();

	bool checkValidationLayerSupport();
	std::vector<const char*> getRequiredExtensions();

private:
	void initWindow();
	void mainLoop();

	const int HEIGHT;
	const int WIDTH;

	GLFWwindow* window;
	VDeleter<VkInstance> instance { vkDestroyInstance };

	const std::vector<const char*> validationLayers {
		"VK_LAYER_LUNARG_standard_validation"
	};

#ifndef _DEBUG
	const bool enableValidationLayers = false;
#else
	const bool enableValidationLayers = true;
#endif
};

