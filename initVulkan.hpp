//
//  initVulkan.hpp
//  FIrst_Project
//
//  Created by Данил on 2.9.2021.
//  Copyright © 2021 Daniil. All rights reserved.
//

#ifndef initVulkan_hpp
#define initVulkan_hpp
#include "helperFunctions.hpp"
#include <iostream>
#include <vector>
#include "initWindow.hpp"
#include <vulkan/vk_mem_alloc.h>
#include <set>
#include "loadFiles.hpp"
//currently doesn't support msaa
//msaa should be optional
static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageType, const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, void* pUserData);

void DestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger, const VkAllocationCallbacks* pAllocator);

bool checkValidationLayerSupport(std::vector<const char*> &validationLayers);

std::vector<const char*> getRequiredExtensions(bool layers);

bool checkDeviceCapabilities(VkPhysicalDevice &physicalDevice,VkSurfaceKHR &surface,SwapChainSupportDetails &caps);

void recordCommands(VkRenderPass &renderPass,VkFramebuffer &frameBuffer,infoForViews &info,VkCommandBuffer &commandBuffer);

void populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo);

VkResult CreateDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT* pDebugMessenger);

bool queueSupport(VkPhysicalDevice &physicaldevice, uint32_t &index, uint32_t &index2,VkSurfaceKHR &surface);

VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities,GLFWwindow *window);

class initVulkanH{
public:
    initVulkanH(bool validationLayers);
    void destroy();
    void run(GLFWwindow *window,std::vector<const char*> &deviceExtensions,std::vector<const char*> &validationLayers, void (*createPipeline)(initVulkanH *),void (*createSetLayout)(initVulkanH *));
    void drawFrame(windowH &window,void (*createPipeline)(initVulkanH *),void (*createSetLayout)(initVulkanH *),uint32_t &imageIndex,std::vector<VkCommandBuffer> &cmdBuffers,VkResult &recreate);
    std::vector<VkCommandBuffer> &getCommandBuffers();
    VkRenderPass &getRenderPass();
    std::vector<VkFramebuffer> &getFrameBuffers();
    infoForViews &getInfo();
    VkPipeline &getPipeline();
    VkPipelineLayout &getLayout();

    VkDevice &getDevice();
    VkSampleCountFlagBits getMsaa();
    VkDescriptorSetLayout &getSetLayout();

    VmaAllocator &getAllocator();
    VkPhysicalDevice &getPhysicalDevice();

    VkCommandPool &getCommandPool();
    VkQueue &getGraphicsQueue();
    VkQueue &getPresentQueue();
//    void initializeGui(ImGui_ImplVulkanH_Window* wd);
    
    VkInstance &getInstance();
    
    VkSurfaceKHR &getSurface();

    uint32_t *getIndices();
    
    std::vector<VkFence> &getInFlightFences();
    
    uint32_t getCurrentFrame();
    
    std::vector<VkFence> &getImagesInFlight();
    
    std::vector<VkSemaphore> &getImageAvailableSemaphores();
    std::vector<VkSemaphore> &getRenderFinishedSemaphores();
    
    VkSwapchainKHR &getSwapChain();
    
    void recreateSwapChain(GLFWwindow *window,void (*createPipeline)(initVulkanH *));
private:
    /*
     VkDevice &logicalDevice,infoForViews &info,VkSampleCountFlagBits msaaSamples,VkDescriptorSetLayout &descriptorSetLayout,VkPipelineLayout &pipelineLayout,VkRenderPass &renderPass, VkPipeline &graphicsPipeline
     */
    //
    //parameters
    //

    //other
    const int MAX_FRAMES_IN_FLIGHT = 2;
    VkSampleCountFlagBits msaaSamples = VK_SAMPLE_COUNT_1_BIT;
    bool layers;
    uint32_t mipLevels;
    SwapChainSupportDetails capabilities;
    uint32_t indices[2];

    VkInstance instance;
    VkAllocationCallbacks *pAllocator = nullptr;
    VkDebugUtilsMessengerEXT debugMessenger;

    //surface
    VkSurfaceKHR surface;

    //devices
    VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
    VkDevice logicalDevice;

    //vma allocator
    VmaAllocator allocator;

    //queues
    VkQueue graphicsQueue;
    VkQueue presentQueue;

    //swapchain
    VkSwapchainKHR swapChain;
        //swapchain objects
        std::vector<VkImage> swapChainImages;
        infoForViews info;
        std::vector<VkImageView> swapChainImageViews;
        std::vector<VkFramebuffer> swapChainFramebuffers;

    //renderpass
    VkRenderPass renderPass;

    //pipeline
    VkDescriptorSetLayout descriptorSetLayout;
    VkPipelineLayout pipelineLayout;
    VkPipeline graphicsPipeline;

    //command objects
    VkCommandPool commandPool;
    std::vector<VkCommandBuffer> commandBuffers;

    //sync objects
    std::vector<VkSemaphore> imageAvailableSemaphores;
    std::vector<VkSemaphore> renderFinishedSemaphores;
    std::vector<VkFence> inFlightFences;
    std::vector<VkFence> imagesInFlight;
    size_t currentFrame = 0;


//    //color recources
//    VkImage colorImage;
//    VmaAllocation colorImageMemory;
//    VkImageView colorImageView;
//
//    //depth recources
    VkImage depthImage;
    VkDeviceMemory depthImageMemory;//will change to vma
    VkImageView depthImageView;

    //
    //methods to initialize vulkan
    //
    void initInstance(std::vector<const char*> &validationLayers);
    void setupDebugMessenger();

    void pickPhysicalDevice(std::vector<const char*> &deviceExtensions);//queus can be picked here
    void createLogicalDevice(std::vector<const char*> &deviceExtensions,std::vector<const char*> &validationLayers);

    void createVmaAllocator();

    void createSurface(GLFWwindow *window);

    //also creates images
    void createSwapChain(GLFWwindow *window);
    void createFramebuffers();
    void createImageViews();

    void createRenderPass();

    void createDescriptorSetLayout();
    void createGraphicsPipeline();

    void createCommandPool();

//    //color recources
//    void createColorResources();
//    void destroyColorRecources();


    void createCommandBuffers();

    void createSyncObjects();
    void destroySyncObjects();

    void cleanupSwapChain();

    void createGuiDescriptorPool(VkDescriptorPool &descPool);
    void createGuiRenderPass(VkRenderPass &renderPass);
    
//    void SetupGuiVulkanWindow(ImGui_ImplVulkanH_Window* wd, VkSurfaceKHR surface, int width, int height);
    
    
    //    //depth recources
    void createDepthResources();
    void destroyDepthRecources();
    
};

#endif /* initVulkan_hpp */
