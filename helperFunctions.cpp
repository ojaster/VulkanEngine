////
////  helperFunctions.cpp
////  FIrst_Project
////
////  Created by Данил on 4.9.2021.
////  Copyright © 2021 Daniil. All rights reserved.
////
//
#include "helperFunctions.hpp"
//
//

bool QueueFamilyIndices::isComplete() {
    return graphicsFamily.has_value() && presentFamily.has_value();
}


QueueFamilyIndices findQueueFamilies(VkPhysicalDevice &device,VkSurfaceKHR &surface) {
    QueueFamilyIndices indices;

    uint32_t queueFamilyCount = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);

    std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
    vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());

    int i = 0;
    for (const auto& queueFamily : queueFamilies) {
        if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
            indices.graphicsFamily = i;
        }

        VkBool32 presentSupport = false;
        vkGetPhysicalDeviceSurfaceSupportKHR(device, i, surface, &presentSupport);

        if (presentSupport) {
            indices.presentFamily = i;
        }

        if (indices.isComplete()) {
            break;
        }

        i++;
    }

    return indices;
}


bool checkDeviceExtensionSupport(VkPhysicalDevice device,std::vector<const char*> &deviceExtensions) {
    uint32_t extensionCount;
    vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, nullptr);

    std::vector<VkExtensionProperties> availableExtensions(extensionCount);
    vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, availableExtensions.data());

    std::set<std::string> requiredExtensions(deviceExtensions.begin(), deviceExtensions.end());

    for (const auto& extension : availableExtensions) {
        requiredExtensions.erase(extension.extensionName);
    }

    return requiredExtensions.empty();
}

bool isDeviceSuitable(VkPhysicalDevice device,VkSurfaceKHR &surface,std::vector<const char*> &deviceExtensions) {
    QueueFamilyIndices indices = findQueueFamilies(device,surface);

    bool extensionsSupported = checkDeviceExtensionSupport(device,deviceExtensions);

    bool swapChainAdequate = false;
    if (extensionsSupported) {
        SwapChainSupportDetails swapChainSupport = querySwapChainSupport(device,surface);
        swapChainAdequate = !swapChainSupport.formats.empty() && !swapChainSupport.presentModes.empty();
    }

    VkPhysicalDeviceFeatures supportedFeatures;
    vkGetPhysicalDeviceFeatures(device, &supportedFeatures);

    return indices.isComplete() && extensionsSupported && swapChainAdequate  && supportedFeatures.samplerAnisotropy;
}


VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats) {
    for (const auto& availableFormat : availableFormats) {
        if (availableFormat.format == VK_FORMAT_B8G8R8A8_SRGB && availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
            return availableFormat;
        }
    }

    return availableFormats[0];
}


VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes) {
    for (const auto& availablePresentMode : availablePresentModes) {
        if (availablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR) {
            return availablePresentMode;
        }
    }
    return VK_PRESENT_MODE_FIFO_KHR;
}

VkImageView createImageView(VkImage image, VkFormat format, VkImageAspectFlags aspectFlags, uint32_t mipLevels,VkDevice &logicalDevice) {
    VkImageViewCreateInfo viewInfo{};
    viewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
    viewInfo.image = image;
    viewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
    viewInfo.format = format;
    viewInfo.subresourceRange.aspectMask = aspectFlags;
    viewInfo.subresourceRange.baseMipLevel = 0;
    viewInfo.subresourceRange.levelCount = mipLevels;
    viewInfo.subresourceRange.baseArrayLayer = 0;
    viewInfo.subresourceRange.layerCount = 1;

    VkImageView imageView;
    if (vkCreateImageView(logicalDevice, &viewInfo, nullptr, &imageView) != VK_SUCCESS) {
        throw std::runtime_error("failed to create texture image view!");
    }
    return imageView;
}
//
bool Vertex::operator==(const Vertex& other) const {
    return pos == other.pos && color == other.color;
}
//
//
VkShaderModule createShaderModule(const std::vector<char>& code,VkDevice &logicalDevice) {
    VkShaderModuleCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    createInfo.codeSize = code.size();
    createInfo.pCode = reinterpret_cast<const uint32_t*>(code.data());

    VkShaderModule shaderModule;
    if (vkCreateShaderModule(logicalDevice, &createInfo, nullptr, &shaderModule) != VK_SUCCESS) {
        throw std::runtime_error("failed to create shader module!");
    }

    return shaderModule;
}


//
void Vertex2::copy(Vertex2 vertex1){
    pos = vertex1.pos;
    color = vertex1.color;
    texCoord = vertex1.texCoord;
}

//border
void createVertexBuffersT(VkBuffer &buffer,VmaAllocation &vertexBufferMem,std::vector<Vertex2> &vertices,VmaAllocator &allocator){
    VkDeviceSize bufferSize = sizeof(vertices[0]) * vertices.size();

    VkBufferCreateInfo bufferInfo = {};
    bufferInfo.size = bufferSize;
    bufferInfo.usage = VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
    bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
    bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;

    VmaAllocationCreateInfo allocInfo = {};
    allocInfo.usage = VMA_MEMORY_USAGE_CPU_TO_GPU;
    allocInfo.requiredFlags = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;

    if(vmaCreateBuffer(allocator, &bufferInfo, &allocInfo, &buffer, &vertexBufferMem, nullptr) != VK_SUCCESS){
        throw std::runtime_error("failed to create vertex buffer");
    }
    void *data;
    vmaMapMemory(allocator, vertexBufferMem, &data);
        memcpy(data, vertices.data(), (size_t) bufferSize);
    vmaUnmapMemory(allocator, vertexBufferMem);

}
void createIndexBufferBeta(VkDevice &logicalDevice,VkBuffer &indexBuffer,VmaAllocator &allocator,VkQueue &graphicsQueue,int index,VmaAllocation &allocation) {
    VkDeviceSize bufferSize = sizeof(indices[0]) * indices.size();

    VkBufferCreateInfo bufferInfo = { VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO };
    bufferInfo.size = bufferSize;
    bufferInfo.usage = VK_BUFFER_USAGE_INDEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT;

    VmaAllocationCreateInfo allocInfo = {};
    allocInfo.usage = VMA_MEMORY_USAGE_GPU_ONLY;
    allocInfo.requiredFlags = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;

    vmaCreateBuffer(allocator, &bufferInfo, &allocInfo, &indexBuffer, &allocation, nullptr);

    void *data;
    vmaMapMemory(allocator, allocation, &data);
    memcpy(data, indices.data(), (size_t) bufferSize);
    vmaUnmapMemory(allocator, allocation);

//    vmaBindBufferMemory(allocator, allocation, indexBuffer);
}
void createUniformBuffersBeta(std::vector<VkBuffer> &uniformBuffers,VmaAllocator &allocator,VkDevice &logicalDevice,VkPhysicalDevice &physicalDevice,int index,std::vector<VmaAllocation> &allocations){
    for(int i = 0; i<uniformBuffers.size(); i++){
        VkDeviceSize bufferSize = sizeof(UniformBufferObject);

        VkBufferCreateInfo bufferInfo = { VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO };
        bufferInfo.size = bufferSize;
        bufferInfo.usage = VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT;

        VmaAllocationCreateInfo allocInfo = {};
        allocInfo.usage = VMA_MEMORY_USAGE_GPU_ONLY;
        allocInfo.requiredFlags = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;

        vmaCreateBuffer(allocator, &bufferInfo, &allocInfo, &uniformBuffers[i], &allocations[i], nullptr);
   //     vmaBindBufferMemory(allocator, allocation, uniformBuffers[i]);
    }

}

void createIndexBuffer(VkBuffer &indexBuffer,VmaAllocation &indexBufferMemory,VmaAllocator &allocator,VkCommandPool &pool,VkDevice &logicalDevice, VkQueue &graphicsQueue) {
    VkDeviceSize bufferSize = sizeof(indices[0]) * indices.size();

    VkBufferCreateInfo bufferinfo = {};
    bufferinfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    bufferinfo.size = bufferSize;
    bufferinfo.usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT;
    bufferinfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

    VmaAllocationCreateInfo allocInfo = {};
    allocInfo.requiredFlags = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;
    allocInfo.usage = VMA_MEMORY_USAGE_GPU_ONLY;

    VkBuffer stagingBuffer;
    VmaAllocation staginBufferMem;
    if(vmaCreateBuffer(allocator, &bufferinfo, &allocInfo, &stagingBuffer, &staginBufferMem, nullptr) != VK_SUCCESS){
        throw std::runtime_error("failed to create index buffer");
    }// prob need staging buffer
    void *data;
    vmaMapMemory(allocator, staginBufferMem, &data);
        memcpy(data, indices.data(), (size_t)bufferSize);
    vmaUnmapMemory(allocator, staginBufferMem);


    bufferinfo.usage = VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT;
    allocInfo.requiredFlags = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;
    allocInfo.usage = VMA_MEMORY_USAGE_GPU_ONLY;
    if(vmaCreateBuffer(allocator, &bufferinfo, &allocInfo, &indexBuffer, &indexBufferMemory, nullptr) != VK_SUCCESS){
        throw std::runtime_error("failed to create index buffer");
    }// prob need staging buffer

    copyBuffer(stagingBuffer, indexBuffer, bufferSize, pool, logicalDevice, graphicsQueue);

    vmaDestroyBuffer(allocator, stagingBuffer, staginBufferMem);
//
//    VkBuffer stagingBuffer;
//    VkDeviceMemory stagingBufferMemory;
//    createBuffer(logicalDevice, stagingBuffer, physicalDevice, stagingBufferMemory, bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
//
//    void* data;
//    vkMapMemory(logicalDevice, stagingBufferMemory, 0, bufferSize, 0, &data);
//    memcpy(data, indices.data(), (size_t) bufferSize);
//    vkUnmapMemory(logicalDevice, stagingBufferMemory);
//
//    createBuffer(logicalDevice, indexBuffer, physicalDevice, indexBufferMemory, bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
//
//    copyBuffer(stagingBuffer, indexBuffer, bufferSize, pool, logicalDevice, graphicsQueue);
//
//    vkDestroyBuffer(logicalDevice, stagingBuffer, nullptr);
//    vkFreeMemory(logicalDevice, stagingBufferMemory, nullptr);
}
void createUniformBuffers(std::vector<VkBuffer> &uniformBuffers, std::vector<VmaAllocation> &uniformBufferMem,VmaAllocator &allocator){
    for(int i = 0; i<uniformBuffers.size(); i++){
        VkBufferCreateInfo bufferInfo = {};
        bufferInfo.size = sizeof(UniformBufferObject);
        bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
        bufferInfo.usage = VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT;
        bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

        VmaAllocationCreateInfo allocInfo = {};
        allocInfo.requiredFlags = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;
        allocInfo.usage = VMA_MEMORY_USAGE_CPU_TO_GPU;

        if(vmaCreateBuffer(allocator, &bufferInfo, &allocInfo, &uniformBuffers[i], &uniformBufferMem[i], nullptr) != VK_SUCCESS){
            throw std::runtime_error("failed to create uniform buffer");
        }
//        createBuffer(logicalDevice, uniformBuffers[i], physicalDevice, uniformBufferMem[i], sizeof(UniformBufferObject), VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
    }

}
void createDescriptorPoolT(VkDevice &logicalDevice,uint32_t size,VkDescriptorPool &descriptorPool){

    VkDescriptorPoolSize poolSize = {};
    poolSize.type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    poolSize.descriptorCount = size;

    VkDescriptorPoolSize poolSize2 = {};
    poolSize2.type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    poolSize2.descriptorCount = size;

    std::vector<VkDescriptorPoolSize> poolSizes{poolSize,poolSize2};

    VkDescriptorPoolCreateInfo createInfo = {};
    createInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
    createInfo.poolSizeCount = poolSizes.size();
    createInfo.maxSets = size;
    createInfo.pPoolSizes = poolSizes.data();

    if(vkCreateDescriptorPool(logicalDevice, &createInfo, nullptr, &descriptorPool) != VK_SUCCESS){
        throw std::runtime_error("failed to create descriptor pool");
    }
}

void createTextureImage(VkDevice &logicalDevice,VkPhysicalDevice &physicalDevice,VkImage &textureImage, VkDeviceMemory &textureImageMem,VkCommandPool &pool,VkQueue &graphicsQueue,char * texture) {
    int texWidth, texHeight, texChannels;
    stbi_uc* pixels = stbi_load(texture, &texWidth, &texHeight, &texChannels, STBI_rgb_alpha);

    VkDeviceSize imageSize = texWidth * texHeight * 4;

    if (!pixels) {
        throw std::runtime_error("failed to load texture image!");
    }
    VkBuffer stagingBuffer;
    VkDeviceMemory stagingBufferMem;

    createBuffer(logicalDevice, stagingBuffer, physicalDevice, stagingBufferMem, imageSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
    void* data;
    vkMapMemory(logicalDevice, stagingBufferMem, 0, imageSize, 0, &data);
        memcpy(data, pixels, static_cast<size_t>(imageSize));
    vkUnmapMemory(logicalDevice, stagingBufferMem);
    stbi_image_free(pixels);
    createImage(textureImage, textureImageMem, texWidth, texHeight, logicalDevice, physicalDevice, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
    transitionImageLayout(logicalDevice, pool, graphicsQueue, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_UNDEFINED, textureImage);
    copyBufferToImage(logicalDevice, pool, graphicsQueue, texWidth, texHeight, stagingBuffer, textureImage);

    transitionImageLayout(logicalDevice, pool, graphicsQueue, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, textureImage);

    vkDestroyBuffer(logicalDevice, stagingBuffer, nullptr);
    vkFreeMemory(logicalDevice, stagingBufferMem, nullptr);
}

void createTextureImageView(VkImage &textureImage,VkDevice &logicalDevice,VkImageView &textureImageView){
    VkImageViewCreateInfo viewInfo{};
    viewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
    viewInfo.image = textureImage;
    viewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
    viewInfo.format = VK_FORMAT_R8G8B8A8_SRGB;
    viewInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    viewInfo.subresourceRange.baseMipLevel = 0;
    viewInfo.subresourceRange.levelCount = 1;
    viewInfo.subresourceRange.baseArrayLayer = 0;
    viewInfo.subresourceRange.layerCount = 1;

    if (vkCreateImageView(logicalDevice, &viewInfo, nullptr, &textureImageView) != VK_SUCCESS) {
        throw std::runtime_error("failed to create texture image view!");
    }
}

void createImageSampler(VkDevice &logicalDevice,VkSampler &sampler){
    VkSamplerCreateInfo samplerInfo{};
    samplerInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;

    samplerInfo.magFilter = VK_FILTER_NEAREST;
    samplerInfo.minFilter = VK_FILTER_NEAREST;
    samplerInfo.addressModeU = VK_SAMPLER_ADDRESS_MODE_REPEAT;
    samplerInfo.addressModeV = VK_SAMPLER_ADDRESS_MODE_REPEAT;
    samplerInfo.addressModeW = VK_SAMPLER_ADDRESS_MODE_REPEAT;
    samplerInfo.anisotropyEnable = VK_FALSE;
    samplerInfo.maxAnisotropy = 1.f;
    samplerInfo.borderColor = VK_BORDER_COLOR_INT_OPAQUE_BLACK;
    samplerInfo.unnormalizedCoordinates = VK_FALSE;
    samplerInfo.compareEnable = VK_FALSE;
    samplerInfo.compareOp = VK_COMPARE_OP_ALWAYS;
    samplerInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
    samplerInfo.mipLodBias = 0.0f;
    samplerInfo.minLod = 0.0f;
    samplerInfo.maxLod = 0.0f;

    if(vkCreateSampler(logicalDevice, &samplerInfo, nullptr, &sampler) != VK_SUCCESS){
        throw std::runtime_error("failed to create sampler");
    }

}

void createDescriptorSets2(VkDevice &logicalDevice,std::vector<VkDescriptorSet> &descSets, VkDescriptorPool &descPool,VkDescriptorSetLayout &layout,std::vector<VkBuffer> &uniforBuffers,VkImageView &textureImageView,VkSampler &textureSampler){// one way to set new texture is to recreate recources for the textures

    for(int i = 0; i<descSets.size(); i++){
        VkDescriptorSetAllocateInfo allocInfo = {};
        allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
        allocInfo.descriptorPool = descPool;
        allocInfo.descriptorSetCount = 1;
        allocInfo.pSetLayouts = &layout;// might case and issue but i doudt

        if(vkAllocateDescriptorSets(logicalDevice, &allocInfo, &descSets[i]) != VK_SUCCESS){
            throw std::runtime_error("failed to create descriptor sets");
        }

        VkDescriptorBufferInfo bufferInfo{};
        bufferInfo.buffer = uniforBuffers[i];
        bufferInfo.offset = 0;
        bufferInfo.range = sizeof(UniformBufferObject);

        VkDescriptorImageInfo imageInfo{};
        imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
        imageInfo.imageView = textureImageView;
        imageInfo.sampler = textureSampler;


        std::vector<VkWriteDescriptorSet> descriptorWrites(2);

        descriptorWrites[0].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
        descriptorWrites[0].dstSet = descSets[i];
        descriptorWrites[0].dstBinding = 0;
        descriptorWrites[0].dstArrayElement = 0;
        descriptorWrites[0].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
        descriptorWrites[0].descriptorCount = 1;
        descriptorWrites[0].pBufferInfo = &bufferInfo;

        descriptorWrites[1].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
        descriptorWrites[1].dstSet = descSets[i];
        descriptorWrites[1].dstBinding = 1;
        descriptorWrites[1].dstArrayElement = 0;
        descriptorWrites[1].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
        descriptorWrites[1].descriptorCount = 1;
        descriptorWrites[1].pImageInfo = &imageInfo;

        /*The pBufferInfo field is used for descriptors that refer to buffer data, pImageInfo is used for descriptors that refer to
         image data, and pTexelBufferView is used for descriptors that refer to buffer views. Our descriptor is based on buffers, so
         we're using pBufferInfo.
         */
        vkUpdateDescriptorSets(logicalDevice, descriptorWrites.size(), descriptorWrites.data(), 0, nullptr);
    }

}

void copyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size, VkCommandPool &pool,VkDevice &logicalDevice,VkQueue &graphicsQueue) {
    VkCommandBuffer commandBuffer = beginSingletimeCommand(logicalDevice,pool);

    VkBufferCopy copyBuff = {};
    copyBuff.size = size;
    vkCmdCopyBuffer(commandBuffer, srcBuffer, dstBuffer, 1, &copyBuff);

    endSingleTimeCommand(commandBuffer, graphicsQueue, logicalDevice, pool);

}

void createBuffer(VkDevice &logicalDevice,VkBuffer &buffer,VkPhysicalDevice &physicalDevice,VkDeviceMemory &vertexBufferMem,VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties){
    VkBufferCreateInfo bufferInfo = {};
    bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    bufferInfo.size = size;
    bufferInfo.usage = usage;
    bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;


    if(vkCreateBuffer(logicalDevice, &bufferInfo, nullptr, &buffer) != VK_SUCCESS){//not bakcped up by actual memory
        throw std::runtime_error("failed to create vertex buffer");
    }
    VkMemoryRequirements memReq;
    vkGetBufferMemoryRequirements(logicalDevice, buffer, &memReq);

    VkMemoryAllocateInfo allocInfo = {};
    allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    allocInfo.allocationSize = memReq.size;
    allocInfo.memoryTypeIndex = findMemoryType(memReq.memoryTypeBits, properties, physicalDevice);
    if(vkAllocateMemory(logicalDevice, &allocInfo, nullptr, &vertexBufferMem) != VK_SUCCESS){//back up buffer with memory
        throw std::runtime_error("failed to allocate buffer memory");
    }

    vkBindBufferMemory(logicalDevice, buffer, vertexBufferMem, 0);
}

void createImage(VkImage &image, VkDeviceMemory &imageMem,int &texWidth,int &texHeight,VkDevice &logicalDevice,VkPhysicalDevice &physicalDevice, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags properties){
    VkImageCreateInfo createInfo = {};
    createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
    createInfo.flags = 0;
    createInfo.imageType = VK_IMAGE_TYPE_2D;
    createInfo.format = format;
    createInfo.extent.width = static_cast<uint32_t>(texWidth);
    createInfo.extent.height = static_cast<uint32_t>(texHeight);
    createInfo.extent.depth = 1;
    createInfo.mipLevels = 1;
    createInfo.arrayLayers = 1;
    createInfo.samples = VK_SAMPLE_COUNT_1_BIT;
    createInfo.tiling = tiling;
    createInfo.usage = usage;
    createInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
    createInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;

    if(vkCreateImage(logicalDevice, &createInfo, nullptr, &image) != VK_SUCCESS){
        throw std::runtime_error("failed to create image");
    }
    VkMemoryRequirements memReq;
    vkGetImageMemoryRequirements(logicalDevice, image, &memReq);

    VkMemoryAllocateInfo allocInfo = {};
    allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    allocInfo.allocationSize = memReq.size;
    allocInfo.memoryTypeIndex = findMemoryType(memReq.memoryTypeBits, properties, physicalDevice);

    if(vkAllocateMemory(logicalDevice, &allocInfo, nullptr, &imageMem) != VK_SUCCESS){
        throw std::runtime_error("failed to allocate memory");
    }
    vkBindImageMemory(logicalDevice, image, imageMem, 0);
}

uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties,VkPhysicalDevice &physicalDevice) {
       VkPhysicalDeviceMemoryProperties memProperties;
       vkGetPhysicalDeviceMemoryProperties(physicalDevice, &memProperties);

       for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++) {
           if ((typeFilter & (1 << i)) && (memProperties.memoryTypes[i].propertyFlags & properties) == properties) {
               return i;
           }
       }

       throw std::runtime_error("failed to find suitable memory type!");
   }

void endSingleTimeCommand(VkCommandBuffer &commandBuffer,VkQueue &graphicsQueue,VkDevice &logicalDevice,VkCommandPool &pool){
    vkEndCommandBuffer(commandBuffer);

    VkSubmitInfo submitInfo = {};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &commandBuffer;

    vkQueueSubmit(graphicsQueue, 1, &submitInfo, nullptr);
    vkQueueWaitIdle(graphicsQueue);

    vkFreeCommandBuffers(logicalDevice, pool, 1, &commandBuffer);

}
VkCommandBuffer beginSingletimeCommand(VkDevice &logicalDevice,VkCommandPool &pool){
    VkCommandBufferAllocateInfo allocInfo = {};
    allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocInfo.commandPool = pool;
    allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    allocInfo.commandBufferCount = 1;

    VkCommandBuffer commandBuffer;

    if(vkAllocateCommandBuffers(logicalDevice, &allocInfo, &commandBuffer) != VK_SUCCESS){
        throw std::runtime_error("failed to allocate command buffer");
    }
    VkCommandBufferBeginInfo beginInfo = {};
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
    vkBeginCommandBuffer(commandBuffer, &beginInfo);
    return commandBuffer;
}

void transitionImageLayout(VkDevice &logicalDevice,VkCommandPool &pool,VkQueue &graphicsQueue,VkImageLayout newLayout, VkImageLayout oldLayout,VkImage &image){
    VkCommandBuffer commandBuffer = beginSingletimeCommand(logicalDevice, pool);

    VkImageMemoryBarrier barrier = {};
    barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
    barrier.srcAccessMask = 0;
    barrier.dstAccessMask = 0;
    barrier.newLayout = newLayout;
    barrier.oldLayout = oldLayout;
    barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    barrier.image = image;
    barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    barrier.subresourceRange.baseMipLevel = 0;
    barrier.subresourceRange.levelCount = 1;
    barrier.subresourceRange.baseArrayLayer = 0;
    barrier.subresourceRange.layerCount = 1;

    VkPipelineStageFlags sourceStage;
    VkPipelineStageFlags destinationStage;

    if (oldLayout == VK_IMAGE_LAYOUT_UNDEFINED && newLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL) {
        barrier.srcAccessMask = 0;
        barrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;

        sourceStage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
        destinationStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
    } else if (oldLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL && newLayout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL) {
        barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
        barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

        sourceStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
        destinationStage = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
    } else {
        throw std::invalid_argument("unsupported layout transition!");
    }

    vkCmdPipelineBarrier(commandBuffer, sourceStage, destinationStage, 0, 0, nullptr, 0, nullptr, 1, &barrier);

    endSingleTimeCommand(commandBuffer, graphicsQueue, logicalDevice, pool);
}

void copyBufferToImage(VkDevice &logicalDevice, VkCommandPool &pool,VkQueue &graphicsQueue,int &width, int &height,VkBuffer &buffer,VkImage &image){
    VkCommandBuffer commandBuffer = beginSingletimeCommand(logicalDevice, pool);
    VkBufferImageCopy region = {};
    region.bufferOffset = 0;
    region.bufferRowLength = 0;
    region.bufferImageHeight = 0;

    region.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    region.imageSubresource.mipLevel = 0;
    region.imageSubresource.baseArrayLayer = 0;
    region.imageSubresource.layerCount = 1;


    region.imageOffset = VkOffset3D{0,0,0};
    region.imageExtent = VkExtent3D{static_cast<uint32_t>(width),static_cast<uint32_t>(height),1};

    vkCmdCopyBufferToImage(commandBuffer, buffer, image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &region);

    endSingleTimeCommand(commandBuffer, graphicsQueue, logicalDevice, pool);
}


float deviceToNormalized(float coords, uint32_t axis){
    float halfSize = axis/2.f;
    return float((coords/halfSize)-1);
}

void createAtlas(VkDevice &logicalDevice,VkPhysicalDevice &physicalDevice, VkImage &textureImage, VkDeviceMemory &textureImageMem, VkCommandPool &commandPool,VkQueue &graphicsQueue, char *texture, VkSampler &textureSampler, VkImageView &textureImageView){
    createTextureImage(logicalDevice, physicalDevice, textureImage, textureImageMem, commandPool, graphicsQueue, texture);
    createTextureImageView(textureImage,logicalDevice,textureImageView);
    createImageSampler(logicalDevice, textureSampler);
}
void destroyAtlas(VkDevice &logicalDevice, VkImage &textureImage, VkDeviceMemory &textureImageMem, VkSampler &textureSampler, VkImageView &textureImageView){
    vkDestroyImage(logicalDevice, textureImage, nullptr);
    vkDestroySampler(logicalDevice, textureSampler, nullptr);
    vkDestroyImageView(logicalDevice, textureImageView, nullptr);
    vkFreeMemory(logicalDevice, textureImageMem, nullptr);
}


void check_vk_result(VkResult err){
    if (err == 0)
        return;
    fprintf(stderr, "[vulkan] Error: VkResult = %d\n", err);
    if (err < 0)
        abort();
}

VkFormat findSupportedFormat(const std::vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features,VkPhysicalDevice &physicalDevice) {
    for (VkFormat format : candidates) {
        VkFormatProperties props;
        vkGetPhysicalDeviceFormatProperties(physicalDevice, format, &props);
        if (tiling == VK_IMAGE_TILING_LINEAR && (props.linearTilingFeatures & features) == features) {
            return format;
        } else if (tiling == VK_IMAGE_TILING_OPTIMAL && (props.optimalTilingFeatures & features) == features) {
            return format;
        }
    }
    throw std::runtime_error("failed to find supported format!");
}

VkFormat findDepthFormat(VkPhysicalDevice &physicalDevice) {
    return findSupportedFormat(
        {VK_FORMAT_D32_SFLOAT, VK_FORMAT_D32_SFLOAT_S8_UINT, VK_FORMAT_D24_UNORM_S8_UINT},
        VK_IMAGE_TILING_OPTIMAL,
        VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT,physicalDevice
    );
}

SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice &device,VkSurfaceKHR &surface) {
    SwapChainSupportDetails details;

    vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, surface, &details.capabilities);

    uint32_t formatCount;
    vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount, nullptr);

    if (formatCount != 0) {
        details.formats.resize(formatCount);
        vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount, details.formats.data());
    }

    uint32_t presentModeCount;
    vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &presentModeCount, nullptr);

    if (presentModeCount != 0) {
        details.presentModes.resize(presentModeCount);
        vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &presentModeCount, details.presentModes.data());
    }

    return details;
}


