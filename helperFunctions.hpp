//
//  helperFunctions.hpp
//  FIrst_Project
//
//  Created by Данил on 4.9.2021.
//  Copyright © 2021 Daniil. All rights reserved.
//

#ifndef helperFunctions_hpp
#define helperFunctions_hpp

#include <stdio.h>
#define GLM_FORCE_ALIGNED_GENTYPES//this is so fucking important i can't say it is not,holy 3 days and this is the error
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/hash.hpp>
#include <vulkan/vk_mem_alloc.h>
#include <array>
#include <vector>
#include <set>
#include <tinyobjloader-master/examples/viewer/stb_image.h>
#include <iostream>

struct infoForViews{
    VkSurfaceFormatKHR format;
    VkExtent2D extent;
};

template<typename T>
class optionalH{
    T unit;
    bool is = false;
public:
    bool has_value(){
        return is;
    }
    void operator =(T unit2){
        unit = unit2;
        is = true;
    }
    uint32_t value(){
        return unit;
    }
};

struct SwapChainSupportDetails {
    VkSurfaceCapabilitiesKHR capabilities;
    std::vector<VkSurfaceFormatKHR> formats;
    std::vector<VkPresentModeKHR> presentModes;
};
//
//
struct QueueFamilyIndices {
    optionalH<uint32_t> graphicsFamily;
    optionalH<uint32_t> presentFamily;

    bool isComplete();
};


QueueFamilyIndices findQueueFamilies(VkPhysicalDevice &device,VkSurfaceKHR &surface);


bool checkDeviceExtensionSupport(VkPhysicalDevice device,std::vector<const char*> &deviceExtensions);


bool isDeviceSuitable(VkPhysicalDevice device,VkSurfaceKHR &surface,std::vector<const char*> &deviceExtensions);

VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);

VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes);


VkImageView createImageView(VkImage image, VkFormat format, VkImageAspectFlags aspectFlags, uint32_t mipLevels,VkDevice &logicalDevice);

//
struct Vertex {
    glm::vec3 pos;
    glm::vec3 color;

    static VkVertexInputBindingDescription getBindingDescription() {
        VkVertexInputBindingDescription bindingDescription{};
        bindingDescription.binding = 0;
        bindingDescription.stride = sizeof(Vertex);
        bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

        return bindingDescription;
    }
    static std::vector<VkVertexInputAttributeDescription> getAttributeDescriptions() {
        std::vector<VkVertexInputAttributeDescription> attributeDescriptions(2);
        attributeDescriptions[0].binding = 0;
        attributeDescriptions[0].location = 0;
        attributeDescriptions[0].format = VK_FORMAT_R32G32_SFLOAT;
        attributeDescriptions[0].offset = offsetof(Vertex, pos);

        attributeDescriptions[1].binding = 0;
        attributeDescriptions[1].location = 1;
        attributeDescriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;
        attributeDescriptions[1].offset = offsetof(Vertex, color);

        return attributeDescriptions;
    }
    void copy(Vertex vertex1);

    bool operator==(const Vertex& other) const;
};
//
//
VkShaderModule createShaderModule(const std::vector<char>& code,VkDevice &logicalDevice);


//
struct Vertex2 {
    glm::vec3 pos;
    glm::vec3 color;
    glm::vec2 texCoord;

    static VkVertexInputBindingDescription getBindingDescription() {
        VkVertexInputBindingDescription bindingDescription{};
        bindingDescription.binding = 0;
        bindingDescription.stride = sizeof(Vertex2);
        bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

        return bindingDescription;
    }
    static std::vector<VkVertexInputAttributeDescription> getAttributeDescriptions() {
        std::vector<VkVertexInputAttributeDescription> attributeDescriptions(3);
        attributeDescriptions[0].binding = 0;
        attributeDescriptions[0].location = 0;
        attributeDescriptions[0].format = VK_FORMAT_R32G32B32_SFLOAT;//previous VK_FORMAT_R32G32_SFLOAT , new VK_FORMAT_R32G32B32_SFLOAT
        attributeDescriptions[0].offset = offsetof(Vertex2, pos);

        attributeDescriptions[1].binding = 0;
        attributeDescriptions[1].location = 1;
        attributeDescriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;
        attributeDescriptions[1].offset = offsetof(Vertex2, color);

        attributeDescriptions[2].binding = 0;
        attributeDescriptions[2].location = 2;
        attributeDescriptions[2].format = VK_FORMAT_R32G32_SFLOAT;
        attributeDescriptions[2].offset = offsetof(Vertex2, texCoord);

        return attributeDescriptions;
    }
    void copy(Vertex2 vertex1);
};
//
void createVertexBuffersT(VkBuffer &buffer,VmaAllocation &vertexBufferMem,std::vector<Vertex2> &vertices,VmaAllocator &allocator);

void createIndexBufferBeta(VkDevice &logicalDevice,VkBuffer &indexBuffer,VmaAllocator &allocator,VkQueue &graphicsQueue,int index,VmaAllocation &allocation);

//void createUniformBuffersBeta(std::vector<VkBuffer> &uniformBuffers,VmaAllocator &allocator,VkDevice &logicalDevice,VkPhysicalDevice &physicalDevice,int index,std::vector<VmaAllocation> &allocations);

void createIndexBuffer(VkBuffer &indexBuffer,VmaAllocation &indexBufferMemory,VmaAllocator &allocator,VkCommandPool &pool,VkDevice &logicalDevice, VkQueue &graphicsQueue);

void createUniformBuffers(std::vector<VkBuffer> &uniformBuffers, std::vector<VmaAllocation> &uniformBufferMem,VmaAllocator &allocator);

void createDescriptorPoolT(VkDevice &logicalDevice,uint32_t size,VkDescriptorPool &descriptorPool);

void createTextureImage(VkDevice &logicalDevice,VkPhysicalDevice &physicalDevice,VkImage &textureImage, VkDeviceMemory &textureImageMem,VkCommandPool &pool,VkQueue &graphicsQueue,char * texture);

struct UniformBufferObject {
    glm::mat4 model;
    glm::mat4 view;
    glm::mat4 proj;
};
//
const std::vector<uint16_t> indices = {
    0, 1, 2, 2, 3, 0
};
//
void createTextureImageView(VkImage &textureImage,VkDevice &logicalDevice,VkImageView &textureImageView);

void createImageSampler(VkDevice &logicalDevice,VkSampler &sampler);

void createDescriptorSets2(VkDevice &logicalDevice,std::vector<VkDescriptorSet> &descSets, VkDescriptorPool &descPool,VkDescriptorSetLayout &layout,std::vector<VkBuffer> &uniforBuffers,VkImageView &textureImageView,VkSampler &textureSampler);
//
void copyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size, VkCommandPool &pool,VkDevice &logicalDevice,VkQueue &graphicsQueue);

void createBuffer(VkDevice &logicalDevice,VkBuffer &buffer,VkPhysicalDevice &physicalDevice,VkDeviceMemory &vertexBufferMem,VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties);

void createImage(VkImage &image, VkDeviceMemory &imageMem,int &texWidth,int &texHeight,VkDevice &logicalDevice,VkPhysicalDevice &physicalDevice, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags properties);

void createTextureImage(VkDevice &logicalDevice,VkPhysicalDevice &physicalDevice,VkImage &textureImage, VkDeviceMemory &textureImageMem,VkCommandPool &pool,VkQueue &graphicsQueue,char * texture) ;

void createTextureImageView(VkImage &textureImage,VkDevice &logicalDevice,VkImageView &textureImageView);

uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties,VkPhysicalDevice &physicalDevice);
//
void endSingleTimeCommand(VkCommandBuffer &commandBuffer,VkQueue &graphicsQueue,VkDevice &logicalDevice,VkCommandPool &pool);

VkCommandBuffer beginSingletimeCommand(VkDevice &logicalDevice,VkCommandPool &pool);
//
void transitionImageLayout(VkDevice &logicalDevice,VkCommandPool &pool,VkQueue &graphicsQueue,VkImageLayout newLayout, VkImageLayout oldLayout,VkImage &image);

void copyBufferToImage(VkDevice &logicalDevice, VkCommandPool &pool,VkQueue &graphicsQueue,int &width, int &height,VkBuffer &buffer,VkImage &image);

float deviceToNormalized(float coords, uint32_t axis);

void createAtlas(VkDevice &logicalDevice,VkPhysicalDevice &physicalDevice, VkImage &textureImage, VkDeviceMemory &textureImageMem, VkCommandPool &commandPool,VkQueue &graphicsQueue, char *texture, VkSampler &textureSampler, VkImageView &textureImageView);

void destroyAtlas(VkDevice &logicalDevice, VkImage &textureImage, VkDeviceMemory &textureImageMem, VkSampler &textureSampler, VkImageView &textureImageView);

void check_vk_result(VkResult err);

VkFormat findSupportedFormat(const std::vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features,VkPhysicalDevice &physicalDevice);

VkFormat findDepthFormat(VkPhysicalDevice &physicalDevice);

SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice &device,VkSurfaceKHR &surface);



#endif /* helperFunctions_hpp */
