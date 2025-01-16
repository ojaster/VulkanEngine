//
//  initVulkan.cpp
//  FIrst_Project
//
//  Created by Данил on 2.9.2021.
//  Copyright © 2021 Daniil. All rights reserved.
//
#define VMA_IMPLEMENTATION
//#define IMGUI_DISABLE_OBSOLETE_FUNCTIONS
//#define IMGUI_IMPL_VULKAN_NO_PROTOTYPES
#include "initVulkan.hpp"
initVulkanH::initVulkanH(bool validationLayers) : layers(validationLayers){}



//    //color recources
//    void createColorResources();
//    void destroyColorRecources();
//
//
//    //depth recources
//    void createDepthResources();
//    void destroyDepthRecources()

void initVulkanH::run(GLFWwindow *window,std::vector<const char*> &deviceExtensions,std::vector<const char*> &validationLayers, void (*createPipeline)(initVulkanH *),void (*createSetLayout)(initVulkanH *)){
    initInstance(validationLayers);
    if(layers){
        setupDebugMessenger();
    }
    createSurface(window);//checked
    pickPhysicalDevice(deviceExtensions);//checked
    createLogicalDevice(deviceExtensions,validationLayers);//checked change here triger layers, point 1
    createVmaAllocator();
    
    createSwapChain(window);//checked
    createImageViews();//checked
    createRenderPass();//checked
//    createGuiRenderPass(renderPass);
//    createDescriptorSetLayout();
    createSetLayout(this);
//    createGraphicsPipeline();
    createPipeline(this);
    createCommandPool();//checked fixed device error
    createDepthResources();
    createFramebuffers();//checked
    createCommandBuffers();//created
    createSyncObjects();
}

//    //color recources
//    void createColorResources();
//    void destroyColorRecources();
//
//
//    //depth recources
//    void createDepthResources();
//    void destroyDepthRecources();
void  initVulkanH::cleanupSwapChain() {
    for (auto framebuffer : swapChainFramebuffers) {
        vkDestroyFramebuffer(logicalDevice, framebuffer, nullptr);
    }
    
    destroyDepthRecources();

    vkFreeCommandBuffers(logicalDevice, commandPool, static_cast<uint32_t>(commandBuffers.size()), commandBuffers.data());

    vkDestroyPipeline(logicalDevice, graphicsPipeline, nullptr);
    vkDestroyPipelineLayout(logicalDevice, pipelineLayout, nullptr);
    vkDestroyRenderPass(logicalDevice, renderPass, nullptr);

    for (auto imageView : swapChainImageViews) {
        vkDestroyImageView(logicalDevice, imageView, nullptr);
    }

    vkDestroySwapchainKHR(logicalDevice, swapChain, nullptr);
}
void initVulkanH::destroy(){
    vkQueueWaitIdle(presentQueue);
    cleanupSwapChain();

    vkDestroyDescriptorSetLayout(logicalDevice, descriptorSetLayout, nullptr);

    for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
        vkDestroySemaphore(logicalDevice, renderFinishedSemaphores[i], nullptr);
        vkDestroySemaphore(logicalDevice, imageAvailableSemaphores[i], nullptr);
        vkDestroyFence(logicalDevice, inFlightFences[i], nullptr);
    }

    vkDestroyCommandPool(logicalDevice, commandPool, nullptr);
    
    vmaDestroyAllocator(allocator);

    vkDestroyDevice(logicalDevice, nullptr);

    if (layers) {
        DestroyDebugUtilsMessengerEXT(instance, debugMessenger, nullptr);
    }

    vkDestroySurfaceKHR(instance, surface, nullptr);
    vkDestroyInstance(instance, nullptr);
    

}
void initVulkanH::initInstance(std::vector<const char*> &validationLayers){
    if (layers && !checkValidationLayerSupport(validationLayers)) {
        throw std::runtime_error("validation layers requested, but not available!");
    }
    
    VkApplicationInfo appInfo{};
    appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.pApplicationName = "AuApp";
    appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.pEngineName = "AuEngine";
    appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.apiVersion = VK_API_VERSION_1_0;
    
    VkInstanceCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    createInfo.pApplicationInfo = &appInfo;
    
    auto extensions = getRequiredExtensions(layers);
    createInfo.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
    createInfo.ppEnabledExtensionNames = extensions.data();
    
    VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo;
    if (layers) {
        createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
        createInfo.ppEnabledLayerNames = validationLayers.data();
    
        populateDebugMessengerCreateInfo(debugCreateInfo);
        createInfo.pNext = (VkDebugUtilsMessengerCreateInfoEXT*) &debugCreateInfo;
    } else {
        createInfo.enabledLayerCount = 0;
    
        createInfo.pNext = nullptr;
    }
    
    if (vkCreateInstance(&createInfo, nullptr, &instance) != VK_SUCCESS) {
        throw std::runtime_error("failed to create instance!");
    }
}

void initVulkanH::setupDebugMessenger(){

    VkDebugUtilsMessengerCreateInfoEXT createInfo;
    populateDebugMessengerCreateInfo(createInfo);

    if (CreateDebugUtilsMessengerEXT(instance, &createInfo, nullptr, &debugMessenger) != VK_SUCCESS) {
        throw std::runtime_error("failed to set up debug messenger!");
    }
}
    
void initVulkanH::pickPhysicalDevice(std::vector<const char*> &deviceExtensions){
    uint32_t physicalDeviceCount;
    vkEnumeratePhysicalDevices(instance, &physicalDeviceCount, nullptr);
    std::vector<VkPhysicalDevice> physicalDevices(physicalDeviceCount);
    vkEnumeratePhysicalDevices(instance, &physicalDeviceCount, physicalDevices.data());
    std::set<std::string> requiredExtensions(deviceExtensions.begin(), deviceExtensions.end());

    for(int i = 0; i<physicalDevices.size(); i++){
        uint32_t extensionCount = 0;
        vkEnumerateDeviceExtensionProperties(physicalDevices[i], nullptr, &extensionCount, nullptr);
        std::vector<VkExtensionProperties> extensions(extensionCount);
        vkEnumerateDeviceExtensionProperties(physicalDevices[i], nullptr, &extensionCount, extensions.data());
        for(int j = 0; j<extensions.size(); j++){
            if(requiredExtensions.erase(extensions[j].extensionName)){
                if(queueSupport(physicalDevices[i],indices[0],indices[1],surface)){
                    if(checkDeviceCapabilities(physicalDevices[i], surface,capabilities)){
                        physicalDevice = physicalDevices[i];
                        return;
                    }
                    break;
                }
                break;
            }
        }
    }
    throw std::runtime_error("extension or queue families");

}

void initVulkanH::createLogicalDevice(std::vector<const char*> &deviceExtensions,std::vector<const char*> &validationLayers){
    VkDeviceCreateInfo createinfo = {};
    createinfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    float priority = 1.0f;
    std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
    std::set<uint32_t> uniqueQueueFamilies = {indices[0], indices[1]};

    float queuePriority = 1.0f;
        for (uint32_t queueFamily : uniqueQueueFamilies) {
            VkDeviceQueueCreateInfo queueCreateInfo{};
            queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
            queueCreateInfo.queueFamilyIndex = queueFamily;
            queueCreateInfo.queueCount = 1;
            queueCreateInfo.pQueuePriorities = &queuePriority;
            queueCreateInfos.push_back(queueCreateInfo);
    }

    createinfo.queueCreateInfoCount = queueCreateInfos.size();
    createinfo.pQueueCreateInfos = queueCreateInfos.data();

    createinfo.enabledExtensionCount = 1;
    createinfo.ppEnabledExtensionNames = deviceExtensions.data();

    if(layers){
        createinfo.enabledLayerCount = 1;
        createinfo.ppEnabledLayerNames = validationLayers.data();
    }

    if(vkCreateDevice(physicalDevice, &createinfo, nullptr, &logicalDevice) != VK_SUCCESS){
        throw std::runtime_error("failed to create logical device");
    }

    vkGetDeviceQueue(logicalDevice, indices[0], 0, &graphicsQueue);
    vkGetDeviceQueue(logicalDevice, indices[1], 0, &presentQueue);
}
    
void initVulkanH::createVmaAllocator(){
    VmaAllocatorCreateInfo allocatorInfo = {};
    allocatorInfo.vulkanApiVersion = VK_API_VERSION_1_0;
    allocatorInfo.physicalDevice = physicalDevice;
    allocatorInfo.device = logicalDevice;
    allocatorInfo.instance = instance;

    if(vmaCreateAllocator(&allocatorInfo, &allocator) != VK_SUCCESS){
        throw std::runtime_error("failed to create vma allocator");
    }
}
    
void initVulkanH::createSurface(GLFWwindow *window){
    if (glfwCreateWindowSurface(instance, window, nullptr, &surface) != VK_SUCCESS) {
        throw std::runtime_error("failed to create window surface!");
    }
}
    
void initVulkanH::createSwapChain(GLFWwindow *window){
    struct SwapChainSupportDetails capabilities = querySwapChainSupport(physicalDevice, surface);
    VkSurfaceFormatKHR format;
    VkPresentModeKHR mode;
    format = capabilities.formats[0];
    for(int i = 0; i<capabilities.formats.size(); i++){
        if(capabilities.formats[i].format == VK_FORMAT_B8G8R8A8_SRGB && capabilities.formats[i].colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR){
            format = capabilities.formats[i];
        }
    }
    mode = VK_PRESENT_MODE_IMMEDIATE_KHR;// immediate mode fixes issues but is jerky
    for(int i = 0; i<capabilities.formats.size(); i++){
        if(capabilities.presentModes[i] == VK_PRESENT_MODE_MAILBOX_KHR){
            mode = capabilities.presentModes[i];
        }
    }

    VkExtent2D extents = chooseSwapExtent(capabilities.capabilities, window);

    VkSwapchainCreateInfoKHR createInfo = {};
    createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
    createInfo.surface = surface;
    createInfo.minImageCount = capabilities.capabilities.minImageCount + 1;
    if(capabilities.capabilities.maxImageCount > 0 && (capabilities.capabilities.minImageCount + 1) > capabilities.capabilities.maxImageCount){
        createInfo.minImageCount = capabilities.capabilities.maxImageCount;
    }
    createInfo.imageFormat = format.format;
    createInfo.imageColorSpace = format.colorSpace;
    createInfo.imageExtent = extents;
    createInfo.imageArrayLayers = 1;// always one unless you want to create stereoscopic images
    createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
    createInfo.oldSwapchain = swapChain;
    if(indices[0] == indices[1]){
        createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
    }else{
        createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
        createInfo.queueFamilyIndexCount = 2;
        createInfo.pQueueFamilyIndices = indices;
    }
    createInfo.preTransform = capabilities.capabilities.currentTransform;
    createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
    createInfo.presentMode = mode;
    createInfo.clipped = VK_TRUE;
    createInfo.oldSwapchain = VK_NULL_HANDLE;

    if(vkCreateSwapchainKHR(logicalDevice, &createInfo, nullptr, &swapChain)!=VK_SUCCESS){
        throw std::runtime_error("failed to create swap chain");
    }

    uint32_t imagecount;
    vkGetSwapchainImagesKHR(logicalDevice, swapChain, &imagecount, nullptr);
    swapChainImages.resize(imagecount);
    vkGetSwapchainImagesKHR(logicalDevice, swapChain, &imagecount, swapChainImages.data());
    info.extent = extents;
    info.format = format;
}
void initVulkanH::createFramebuffers(){
    swapChainFramebuffers.resize(swapChainImageViews.size());
    for(int i = 0; i<swapChainImageViews.size(); i++){
        std::array<VkImageView, 2> attachments = {
            swapChainImageViews[i],
            depthImageView
        };
        VkFramebufferCreateInfo createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
        createInfo.renderPass = renderPass;
        createInfo.attachmentCount = static_cast<uint32_t>(attachments.size());
        createInfo.pAttachments = attachments.data();
        createInfo.width = info.extent.width;
        createInfo.height = info.extent.height;
        createInfo.layers = 1;
        if(vkCreateFramebuffer(logicalDevice, &createInfo, nullptr, &swapChainFramebuffers[i]) != VK_SUCCESS){
            throw std::runtime_error("failed to create frame buffers");
        }
    }
}
void initVulkanH::createImageViews(){
    swapChainImageViews.resize(swapChainImages.size());
    for(int i = 0; i<swapChainImages.size(); i++){
        VkImageViewCreateInfo createInfo = {};
        createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
        createInfo.image = swapChainImages[i];
        createInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
        createInfo.format = info.format.format;
        createInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;//channels rgb and alpha VK_COMPONENT_SWIZZLE_IDENTITY is default
        createInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;// def should play with them
        createInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
        createInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
        createInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;// should research on my own
        createInfo.subresourceRange.baseMipLevel = 0;
        createInfo.subresourceRange.levelCount = 1;
        createInfo.subresourceRange.baseArrayLayer = 0;
        createInfo.subresourceRange.layerCount = 1;


        if(vkCreateImageView(logicalDevice, &createInfo, nullptr, &swapChainImageViews[i]) != VK_SUCCESS){
            throw std::runtime_error("failed to create image view");
        }
    }
}
    
void initVulkanH::createRenderPass(){//z buffer error here,solution is to use two different renderpasses in imgui
    VkAttachmentDescription colorAtt = {};
    colorAtt.format = info.format.format;
    colorAtt.samples = VK_SAMPLE_COUNT_1_BIT;
    colorAtt.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
    colorAtt.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
    colorAtt.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    colorAtt.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;// change present_src to VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL if gui won't work
    colorAtt.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    colorAtt.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;


    VkAttachmentReference attRef = {};
    attRef.attachment = 0;
    attRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

    VkSubpassDescription subPassDesc = {};
   // subPassDesc.flags = VK_SUBPASS_DESCRIPTION_SHADER_RESOLVE_BIT_QCOM;
    subPassDesc.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
    subPassDesc.colorAttachmentCount = 1;
    subPassDesc.pColorAttachments = &attRef;


    VkRenderPassCreateInfo createInfo = {};
    createInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
    createInfo.attachmentCount = 1;
    createInfo.pAttachments = &colorAtt;
    createInfo.subpassCount = 1;
    createInfo.pSubpasses = &subPassDesc;

    VkSubpassDependency dependencyPass = {};
    dependencyPass.srcSubpass = VK_SUBPASS_EXTERNAL;
    dependencyPass.dstSubpass = 0;
    dependencyPass.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
    dependencyPass.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
    dependencyPass.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
    dependencyPass.srcAccessMask = 0;


    createInfo.dependencyCount = 1;
    createInfo.pDependencies = &dependencyPass;
    {
        VkAttachmentDescription depthAttachment{};
        depthAttachment.format = findDepthFormat(physicalDevice);
        depthAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
        depthAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
        depthAttachment.storeOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
        depthAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
        depthAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
        depthAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
        depthAttachment.finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

        VkAttachmentReference depthAttachmentRef{};
        depthAttachmentRef.attachment = 1;
        depthAttachmentRef.layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
        subPassDesc.pDepthStencilAttachment = &depthAttachmentRef;

        dependencyPass.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
        dependencyPass.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
        dependencyPass.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;

        std::array<VkAttachmentDescription, 2> attachments = {colorAtt, depthAttachment};
        createInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
        createInfo.attachmentCount = static_cast<uint32_t>(attachments.size());
        createInfo.pAttachments = attachments.data();
        createInfo.subpassCount = 1;
        createInfo.pSubpasses = &subPassDesc;
        createInfo.dependencyCount = 1;
        createInfo.pDependencies = &dependencyPass;
    }//to use it with imgui gotta use different renderpasses
    

    if(vkCreateRenderPass(logicalDevice, &createInfo, nullptr, &renderPass) != VK_SUCCESS){
        throw std::runtime_error("failed to create renderpass");
    }
}
    
void initVulkanH::createDescriptorSetLayout(){
    VkDescriptorSetLayoutBinding uboLayoutBinding{};
    uboLayoutBinding.binding = 0;
    uboLayoutBinding.descriptorCount = 1;
    uboLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    uboLayoutBinding.pImmutableSamplers = nullptr;
    uboLayoutBinding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;

    VkDescriptorSetLayoutBinding samplerLayoutBinding{};
    samplerLayoutBinding.binding = 1;
    samplerLayoutBinding.descriptorCount = 1;
    samplerLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    samplerLayoutBinding.pImmutableSamplers = nullptr;
    samplerLayoutBinding.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;

    std::vector<VkDescriptorSetLayoutBinding> bindings = {uboLayoutBinding, samplerLayoutBinding};
    VkDescriptorSetLayoutCreateInfo layoutInfo{};
    layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
    layoutInfo.bindingCount = static_cast<uint32_t>(bindings.size());
    layoutInfo.pBindings = bindings.data();

    if (vkCreateDescriptorSetLayout(logicalDevice, &layoutInfo, nullptr, &descriptorSetLayout) != VK_SUCCESS) {
        throw std::runtime_error("failed to create descriptor set layout!");
    }
}
void initVulkanH::createGraphicsPipeline(){
    auto vertShaderCode = readFile("shaders/playingAround/triangleVert.spv");
    auto fragShaderCode = readFile("shaders/triangleFrag.spv");

    VkShaderModule vertShaderModule = createShaderModule(vertShaderCode,logicalDevice);
    VkShaderModule fragShaderModule = createShaderModule(fragShaderCode,logicalDevice);

    VkPipelineShaderStageCreateInfo vertShaderStageInfo{};
    vertShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    vertShaderStageInfo.stage = VK_SHADER_STAGE_VERTEX_BIT;
    vertShaderStageInfo.module = vertShaderModule;
    vertShaderStageInfo.pName = "main";

    VkPipelineShaderStageCreateInfo fragShaderStageInfo{};
    fragShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    fragShaderStageInfo.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
    fragShaderStageInfo.module = fragShaderModule;
    fragShaderStageInfo.pName = "main";

    VkPipelineShaderStageCreateInfo shaderStages[] = {vertShaderStageInfo, fragShaderStageInfo};

    VkPipelineVertexInputStateCreateInfo vertexInputInfo{};
    vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;

    auto bindingDescription = Vertex2::getBindingDescription();
    auto attributeDescriptions = Vertex2::getAttributeDescriptions();

    vertexInputInfo.vertexBindingDescriptionCount = 1;
    vertexInputInfo.vertexAttributeDescriptionCount = static_cast<uint32_t>(attributeDescriptions.size());
    vertexInputInfo.pVertexBindingDescriptions = &bindingDescription;
    vertexInputInfo.pVertexAttributeDescriptions = attributeDescriptions.data();

    VkPipelineInputAssemblyStateCreateInfo inputAssembly{};
    inputAssembly.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
    inputAssembly.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
    inputAssembly.primitiveRestartEnable = VK_FALSE;

    VkViewport viewport{};
    viewport.x = 0.0f;
    viewport.y = 0.0f;
    viewport.width = (float) info.extent.width;
    viewport.height = (float) info.extent.height;
    viewport.minDepth = 0.0f;
    viewport.maxDepth = 1.0f;

    VkRect2D scissor{};
    scissor.offset = {0, 0};
    scissor.extent = info.extent;

    VkPipelineViewportStateCreateInfo viewportState{};
    viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
    viewportState.viewportCount = 1;
    viewportState.pViewports = &viewport;
    viewportState.scissorCount = 1;
    viewportState.pScissors = &scissor;

    VkPipelineRasterizationStateCreateInfo rasterizer{};
    rasterizer.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
    rasterizer.depthClampEnable = VK_FALSE;
    rasterizer.rasterizerDiscardEnable = VK_FALSE;
    rasterizer.polygonMode = VK_POLYGON_MODE_FILL;
    rasterizer.lineWidth = 1.0f;
    rasterizer.cullMode = VK_CULL_MODE_BACK_BIT;
    rasterizer.frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE;
    rasterizer.depthBiasEnable = VK_FALSE;

    VkPipelineMultisampleStateCreateInfo multisampling{};
    multisampling.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
    multisampling.sampleShadingEnable = VK_FALSE;
    multisampling.rasterizationSamples = msaaSamples;

    VkPipelineDepthStencilStateCreateInfo depthStencil{};
    depthStencil.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
    depthStencil.depthTestEnable = VK_TRUE;
    depthStencil.depthWriteEnable = VK_TRUE;
    depthStencil.depthCompareOp = VK_COMPARE_OP_LESS;
    depthStencil.depthBoundsTestEnable = VK_FALSE;
    depthStencil.stencilTestEnable = VK_FALSE;

    VkPipelineColorBlendAttachmentState colorBlendAttachment{};
    colorBlendAttachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
    colorBlendAttachment.blendEnable = VK_FALSE;

    VkPipelineColorBlendStateCreateInfo colorBlending{};
    colorBlending.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
    colorBlending.logicOpEnable = VK_FALSE;
    colorBlending.logicOp = VK_LOGIC_OP_COPY;
    colorBlending.attachmentCount = 1;
    colorBlending.pAttachments = &colorBlendAttachment;
    colorBlending.blendConstants[0] = 0.0f;
    colorBlending.blendConstants[1] = 0.0f;
    colorBlending.blendConstants[2] = 0.0f;
    colorBlending.blendConstants[3] = 0.0f;

    VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
    pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
    pipelineLayoutInfo.setLayoutCount = 1;
    pipelineLayoutInfo.pSetLayouts = &descriptorSetLayout;

    if (vkCreatePipelineLayout(logicalDevice, &pipelineLayoutInfo, nullptr, &pipelineLayout) != VK_SUCCESS) {
        throw std::runtime_error("failed to create pipeline layout!");
    }

    VkGraphicsPipelineCreateInfo pipelineInfo{};
    pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
    pipelineInfo.stageCount = 2;
    pipelineInfo.pStages = shaderStages;
    pipelineInfo.pVertexInputState = &vertexInputInfo;
    pipelineInfo.pInputAssemblyState = &inputAssembly;
    pipelineInfo.pViewportState = &viewportState;
    pipelineInfo.pRasterizationState = &rasterizer;
    pipelineInfo.pMultisampleState = &multisampling;
    pipelineInfo.pDepthStencilState = &depthStencil;
    pipelineInfo.pColorBlendState = &colorBlending;
    pipelineInfo.layout = pipelineLayout;
    pipelineInfo.renderPass = renderPass;
    pipelineInfo.subpass = 0;
    pipelineInfo.basePipelineHandle = VK_NULL_HANDLE;

    VkPipelineCreationFeedbackCreateInfoEXT feedBackInfo = {};
    feedBackInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_CREATION_FEEDBACK_CREATE_INFO_EXT;
    VkPipelineCreationFeedbackEXT feedBack = {};
    feedBack.flags = VK_PIPELINE_CREATION_FEEDBACK_VALID_BIT_EXT;
    VkPipelineCreationFeedbackEXT feedBacks[2] = {};
    feedBackInfo.pPipelineCreationFeedback = &feedBack;
    feedBackInfo.pipelineStageCreationFeedbackCount = 2;
    feedBacks[0].flags = VK_PIPELINE_CREATION_FEEDBACK_VALID_BIT_EXT;
    feedBacks[1].flags = VK_PIPELINE_CREATION_FEEDBACK_VALID_BIT_EXT;
    feedBacks[0].duration = 1;
    feedBacks[1].duration = 1;
    feedBackInfo.pPipelineStageCreationFeedbacks = feedBacks;


    pipelineInfo.pNext = &feedBackInfo;

    if (vkCreateGraphicsPipelines(logicalDevice, VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &graphicsPipeline) != VK_SUCCESS) {
        throw std::runtime_error("failed to create graphics pipeline!");
    }
    std::cout<<"flag:"<<feedBack.flags<<std::endl;
    std::cout<<"duration:"<<feedBack.duration<<std::endl;
    std::cout<<std::endl;
    for(int i = 0; i <2; i++){
        std::cout<<"flags:"<<feedBacks[i].flags<<std::endl;
        std::cout<<"duration:"<<feedBacks[i].duration<<std::endl;
    }

    vkDestroyShaderModule(logicalDevice, fragShaderModule, nullptr);
    vkDestroyShaderModule(logicalDevice, vertShaderModule, nullptr);
}
    
void initVulkanH::createCommandPool(){
    VkCommandPoolCreateInfo createInfo = {};
    createInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    createInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
    createInfo.queueFamilyIndex = indices[0];
    if(vkCreateCommandPool(logicalDevice, &createInfo, nullptr, &commandPool) != VK_SUCCESS){
        throw std::runtime_error("failed to create command pool");
    }
}

//void initVulkanH::createColorResources(){}
//void initVulkanH::destroyColorRecources(){}
//
void initVulkanH::createDepthResources() {
    VkFormat depthFormat = findDepthFormat(physicalDevice);
    int width =info.extent.width;
    int height =info.extent.height;
    createImage(depthImage, depthImageMemory, width, height, logicalDevice, physicalDevice, depthFormat, VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
    depthImageView = createImageView(depthImage, depthFormat, VK_IMAGE_ASPECT_DEPTH_BIT, 1,logicalDevice);
}
void initVulkanH::destroyDepthRecources(){
    vkDestroyImageView(logicalDevice, depthImageView, nullptr);
    vkDestroyImage(logicalDevice, depthImage, nullptr);
    vkFreeMemory(logicalDevice, depthImageMemory, nullptr);
}
    
void initVulkanH::createCommandBuffers(){
    commandBuffers.resize(swapChainImages.size());
    VkCommandBufferAllocateInfo createInfo = {};
    createInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    createInfo.commandPool = commandPool;
    createInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    createInfo.commandBufferCount = commandBuffers.size();
    if(vkAllocateCommandBuffers(logicalDevice, &createInfo, commandBuffers.data()) != VK_SUCCESS){
        throw std::runtime_error("failed to create commandBuffers");
    }
}

void initVulkanH::createSyncObjects(){
    imageAvailableSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
    renderFinishedSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
    inFlightFences.resize(MAX_FRAMES_IN_FLIGHT);
    imagesInFlight.resize(swapChainImages.size(), VK_NULL_HANDLE);

    VkSemaphoreCreateInfo semaphoreInfo{};
    semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

    VkFenceCreateInfo fenceInfo{};
    fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
    fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

    for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
        if (vkCreateSemaphore(logicalDevice, &semaphoreInfo, nullptr, &imageAvailableSemaphores[i]) != VK_SUCCESS ||
            vkCreateSemaphore(logicalDevice, &semaphoreInfo, nullptr, &renderFinishedSemaphores[i]) != VK_SUCCESS ||
            vkCreateFence(logicalDevice, &fenceInfo, nullptr, &inFlightFences[i]) != VK_SUCCESS) {
            throw std::runtime_error("failed to create synchronization objects for a frame!");
        }
    }
}
void initVulkanH::destroySyncObjects(){
    for(int i = 0; i<MAX_FRAMES_IN_FLIGHT; i++){
        vkDestroySemaphore(logicalDevice, imageAvailableSemaphores[i], pAllocator);
        vkDestroySemaphore(logicalDevice, renderFinishedSemaphores[i], pAllocator);
    }
}

void initVulkanH::drawFrame(windowH &window,void (*createPipeline)(initVulkanH *),void (*createSetLayout)(initVulkanH *), uint32_t &imageIndex,std::vector<VkCommandBuffer> &cmdBuffers,VkResult &recreate) {
    vkWaitForFences(logicalDevice, 1, &inFlightFences[currentFrame], VK_TRUE, UINT64_MAX);
    
    VkResult result = vkAcquireNextImageKHR(logicalDevice, swapChain, UINT64_MAX, imageAvailableSemaphores[currentFrame], VK_NULL_HANDLE, &imageIndex);
    recreate = result;
    if(result == VK_ERROR_OUT_OF_DATE_KHR){
        std::cout<<"out of date"<<std::endl;
    }

    if (result == VK_ERROR_OUT_OF_DATE_KHR) {
        recreateSwapChain(window.getWindow(),createPipeline);
        std::cout<<"recreate1"<<std::endl;
        return;
    } else if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
        throw std::runtime_error("failed to acquire swap chain image!");
    }

//    updateUniformBuffer(imageIndex,cameraPos,cameraUp,cameraFront,zoom);

    if (imagesInFlight[imageIndex] != VK_NULL_HANDLE) {
        vkWaitForFences(logicalDevice, 1, &imagesInFlight[imageIndex], VK_TRUE, UINT64_MAX);
    }
    imagesInFlight[imageIndex] = inFlightFences[currentFrame];

    VkSubmitInfo submitInfo{};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

    VkSemaphore waitSemaphores[] = {imageAvailableSemaphores[currentFrame]};
    VkPipelineStageFlags waitStages[] = {VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};
    submitInfo.waitSemaphoreCount = 1;
    submitInfo.pWaitSemaphores = waitSemaphores;
    submitInfo.pWaitDstStageMask = waitStages;

    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &cmdBuffers[imageIndex];

    VkSemaphore signalSemaphores[] = {renderFinishedSemaphores[currentFrame]};
    submitInfo.signalSemaphoreCount = 1;
    submitInfo.pSignalSemaphores = signalSemaphores;

    vkResetFences(logicalDevice, 1, &inFlightFences[currentFrame]);

    if (vkQueueSubmit(graphicsQueue, 1, &submitInfo, inFlightFences[currentFrame]) != VK_SUCCESS) {
        throw std::runtime_error("failed to submit draw command buffer!");
    }

    VkPresentInfoKHR presentInfo{};
    presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;

    presentInfo.waitSemaphoreCount = 1;
    presentInfo.pWaitSemaphores = signalSemaphores;

    VkSwapchainKHR swapChains[] = {swapChain};
    presentInfo.swapchainCount = 1;
    presentInfo.pSwapchains = swapChains;

    presentInfo.pImageIndices = &imageIndex;

    result = vkQueuePresentKHR(presentQueue, &presentInfo);

    if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || window.getFrameBufferResize()) {
        std::cout<<"recreate2"<<std::endl;
        if(window.getFrameBufferResize()){
            std::cout<<"frame buffer resized"<<std::endl;
        }
        window.setFrameBufferResizeFalse();
        recreateSwapChain(window.getWindow(),createPipeline);
        recreate = VK_ERROR_OUT_OF_DATE_KHR;
    } else if (result != VK_SUCCESS) {
        throw std::runtime_error("failed to present swap chain image!");
    }
    currentFrame = (currentFrame + 1) % MAX_FRAMES_IN_FLIGHT;
//    ImGui_ImplVulkan_RenderDrawData(ImGui::GetDrawData(), commandBuffers[currentFrame]);//not here
}
void initVulkanH::recreateSwapChain(GLFWwindow *window,void (*createPipeline)(initVulkanH *)){
    int width = 0, height = 0;
    glfwGetFramebufferSize(window, &width, &height);
    while (width == 0 || height == 0) {
        glfwGetFramebufferSize(window, &width, &height);
        glfwWaitEvents();
    }
    vkDeviceWaitIdle(logicalDevice);

    cleanupSwapChain();

    createSwapChain(window);
    createImageViews();
    createRenderPass();
    createPipeline(this);
    createDepthResources();
    createFramebuffers();
    createCommandBuffers();//checked
    //missing begin command buffers
    
//    ImGui_ImplVulkan_SetMinImageCount(swapChainImages.size());
//    QueueFamilyIndices queueFamily = findQueueFamilies(physicalDevice, surface);
//    ImGui_ImplVulkanH_Window g_MainWindowData;
//    ImGui_ImplVulkanH_CreateOrResizeWindow(instance, physicalDevice, logicalDevice, &g_MainWindowData,
//                                   queueFamily.graphicsFamily.value(), nullptr, width, height, swapChainImages.size());
//    g_MainWindowData.FrameIndex = 0;//will uncomment only when im gui will be implemented
}

std::vector<VkCommandBuffer> &initVulkanH::getCommandBuffers(){
    return commandBuffers;
}

VkRenderPass &initVulkanH::getRenderPass(){
    return renderPass;
}

std::vector<VkFramebuffer> &initVulkanH::getFrameBuffers(){
    return swapChainFramebuffers;
}

infoForViews &initVulkanH::getInfo(){
    return info;
}

VkPipeline &initVulkanH::getPipeline(){
    return graphicsPipeline;
}

VkPipelineLayout &initVulkanH::getLayout(){
    return pipelineLayout;
}

VkDevice &initVulkanH::getDevice(){
    return logicalDevice;
}

VkSampleCountFlagBits initVulkanH::getMsaa(){
    return msaaSamples;
}

VkDescriptorSetLayout &initVulkanH::getSetLayout(){
    return descriptorSetLayout;
}

VmaAllocator &initVulkanH::getAllocator(){
    return allocator;
}

VkPhysicalDevice &initVulkanH::getPhysicalDevice(){
    return physicalDevice;
}

VkCommandPool &initVulkanH::getCommandPool(){
    return commandPool;
}
VkQueue &initVulkanH::getGraphicsQueue(){
    return graphicsQueue;
}
VkQueue &initVulkanH::getPresentQueue(){
    return presentQueue;
}

//void initVulkanH::createGuiDescriptorPool(VkDescriptorPool &descPool){
//    VkDescriptorPoolSize pool_sizes[] =
//    {
//        { VK_DESCRIPTOR_TYPE_SAMPLER, 1000 },
//        { VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1000 },
//        { VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE, 1000 },
//        { VK_DESCRIPTOR_TYPE_STORAGE_IMAGE, 1000 },
//        { VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER, 1000 },
//        { VK_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER, 1000 },
//        { VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1000 },
//        { VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, 1000 },
//        { VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC, 1000 },
//        { VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC, 1000 },
//        { VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT, 1000 }
//    };
//
//    VkDescriptorPoolCreateInfo pool_info = {};
//    pool_info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
//    pool_info.flags = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT;
//    pool_info.maxSets = 1000 * IM_ARRAYSIZE(pool_sizes);
//    pool_info.poolSizeCount = (uint32_t)IM_ARRAYSIZE(pool_sizes);
//    pool_info.pPoolSizes = pool_sizes;
//
//    if(vkCreateDescriptorPool(logicalDevice, &pool_info, nullptr, &descPool) != VK_SUCCESS){
//        throw std::runtime_error("failed to create descriptor pool");
//    }
//}
//void initVulkanH::initializeGui(ImGui_ImplVulkanH_Window* wd){
//
//    SetupGuiVulkanWindow(wd, surface, info.extent.width, info.extent.height);
//    ImGui_ImplVulkan_InitInfo init_info = {};
//    init_info.Instance = instance;
//    init_info.PhysicalDevice = physicalDevice;
//    init_info.Device = logicalDevice;
//    QueueFamilyIndices queueFamily = findQueueFamilies(physicalDevice, surface);
//    init_info.QueueFamily = queueFamily.graphicsFamily.value();// author says that random number, will do fine if queue is not set(i guess, not sure).
//    init_info.Queue = graphicsQueue;
//    init_info.PipelineCache = VK_NULL_HANDLE;// not defining, not necesarry objects works fine
//
//    VkDescriptorPool descriptorPool;
//    createGuiDescriptorPool(descriptorPool);
//    init_info.DescriptorPool = descriptorPool;
//    init_info.Allocator = nullptr;
//    init_info.MinImageCount = swapChainImages.size();// doesn't seem to be used
//    init_info.ImageCount = wd->ImageCount;
//    init_info.CheckVkResultFn = nullptr;// error check logics, don't need it for now
//    VkRenderPass renderPassGui;
////    createGuiRenderPass(renderPassGui);
//    ImGui_ImplVulkan_Init(&init_info, wd->RenderPass);
//
////    VkCommandBuffer command_buffer = beginSingletimeCommand(logicalDevice, commandPool);
////    ImGui_ImplVulkan_CreateFontsTexture(command_buffer);
////    endSingleTimeCommand(command_buffer, graphicsQueue, logicalDevice, commandPool);
//
//    VkResult err;
//    {
//        // Use any command queue
//        VkCommandPool command_pool = wd->Frames[wd->FrameIndex].CommandPool;
//        VkCommandBuffer command_buffer = wd->Frames[wd->FrameIndex].CommandBuffer;
//
//        err = vkResetCommandPool(logicalDevice, command_pool, 0);
//        check_vk_result(err);
//        VkCommandBufferBeginInfo begin_info = {};
//        begin_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
//        begin_info.flags |= VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
//        err = vkBeginCommandBuffer(command_buffer, &begin_info);
//        check_vk_result(err);
//
//        ImGui_ImplVulkan_CreateFontsTexture(command_buffer);
//
//        VkSubmitInfo end_info = {};
//        end_info.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
//        end_info.commandBufferCount = 1;
//        end_info.pCommandBuffers = &command_buffer;
//        err = vkEndCommandBuffer(command_buffer);
//        check_vk_result(err);
//        err = vkQueueSubmit(graphicsQueue, 1, &end_info, VK_NULL_HANDLE);
//        check_vk_result(err);
//
//        err = vkDeviceWaitIdle(logicalDevice);
//        check_vk_result(err);
//        ImGui_ImplVulkan_DestroyFontUploadObjects();
//    }
//}

void initVulkanH::createGuiRenderPass(VkRenderPass &renderPass){
    VkAttachmentDescription colorAttachment = {};
    colorAttachment.format = info.format.format;
    colorAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
    colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_LOAD;
    colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
    colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    colorAttachment.initialLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
    colorAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

    VkAttachmentDescription depthAttachment = {};
    depthAttachment.format = findDepthFormat(physicalDevice);
    depthAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
    depthAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
    depthAttachment.storeOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    depthAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    depthAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    depthAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    depthAttachment.finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

    VkAttachmentReference colorAttachmentRef = {};
    colorAttachmentRef.attachment = 0;
    colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

    VkAttachmentReference depthAttachmentRef = {};
    depthAttachmentRef.attachment = 1;
    depthAttachmentRef.layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

    VkSubpassDescription subpass = {};
    subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
    subpass.colorAttachmentCount = 1;
    subpass.pColorAttachments = &colorAttachmentRef;
    subpass.pDepthStencilAttachment = &depthAttachmentRef;

    VkSubpassDependency dependency = {};
    dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
    dependency.dstSubpass = 0;
    dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
    dependency.srcAccessMask = 0;
    dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
    dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_READ_BIT | VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

    std::array<VkAttachmentDescription, 2> attachments = {colorAttachment, depthAttachment};
    VkRenderPassCreateInfo renderPassInfo = {};
    renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
    renderPassInfo.attachmentCount = static_cast<uint32_t>(attachments.size());
    renderPassInfo.pAttachments = attachments.data();
    renderPassInfo.subpassCount = 1;
    renderPassInfo.pSubpasses = &subpass;
//        renderPassInfo.dependencyCount = 1;
//        renderPassInfo.pDependencies = &dependency;

    if (vkCreateRenderPass(logicalDevice, &renderPassInfo, nullptr, &renderPass) != VK_SUCCESS) {
        throw std::runtime_error("failed to create render pass!");
    }
}
//void initVulkanH::SetupGuiVulkanWindow(ImGui_ImplVulkanH_Window* wd, VkSurfaceKHR surface, int width, int height){
//    wd->Surface = surface;
//
//    // Check for WSI support
//    VkBool32 res;
////    vkGetPhysicalDeviceSurfaceSupportKHR(physicalDevice, g_QueueFamily, wd->Surface, &res);
////    if (res != VK_TRUE)
////    {
////        fprintf(stderr, "Error no WSI support on physical device 0\n");
////        exit(-1);
////    }
//
//    // Select Surface Format
//    const VkFormat requestSurfaceImageFormat[] = { VK_FORMAT_B8G8R8A8_UNORM, VK_FORMAT_R8G8B8A8_UNORM, VK_FORMAT_B8G8R8_UNORM, VK_FORMAT_R8G8B8_UNORM };
//    const VkColorSpaceKHR requestSurfaceColorSpace = VK_COLORSPACE_SRGB_NONLINEAR_KHR;
//    wd->SurfaceFormat = ImGui_ImplVulkanH_SelectSurfaceFormat(physicalDevice, wd->Surface, requestSurfaceImageFormat, (size_t)IM_ARRAYSIZE(requestSurfaceImageFormat), requestSurfaceColorSpace);
//
//    // Select Present Mode
//#ifdef IMGUI_UNLIMITED_FRAME_RATE
//    VkPresentModeKHR present_modes[] = { VK_PRESENT_MODE_MAILBOX_KHR, VK_PRESENT_MODE_IMMEDIATE_KHR, VK_PRESENT_MODE_FIFO_KHR };
//#else
//    VkPresentModeKHR present_modes[] = { VK_PRESENT_MODE_FIFO_KHR };
//#endif
//    wd->PresentMode = ImGui_ImplVulkanH_SelectPresentMode(physicalDevice, wd->Surface, &present_modes[0], IM_ARRAYSIZE(present_modes));
//    //printf("[vulkan] Selected PresentMode = %d\n", wd->PresentMode);
//
//    // Create SwapChain, RenderPass, Framebuffer, etc.
//    IM_ASSERT(swapChainImages.size() >= 2);
//    ImGui_ImplVulkanH_CreateOrResizeWindow(instance, physicalDevice, logicalDevice, wd, indices[0], nullptr, width, height, swapChainImages.size());
//}

VkInstance &initVulkanH::getInstance(){
    return instance;
}

VkSurfaceKHR &initVulkanH::getSurface(){
    return surface;
}
uint32_t *initVulkanH::getIndices(){
    return indices;
}

std::vector<VkFence> &initVulkanH::getInFlightFences(){
    return inFlightFences;
}
uint32_t initVulkanH::getCurrentFrame(){
    return currentFrame;
}
std::vector<VkFence> &initVulkanH::getImagesInFlight(){
    return imagesInFlight;
}

std::vector<VkSemaphore> &initVulkanH::getImageAvailableSemaphores(){
    return imageAvailableSemaphores;
}
std::vector<VkSemaphore> &initVulkanH::getRenderFinishedSemaphores(){
    return renderFinishedSemaphores;
}
VkSwapchainKHR &initVulkanH::getSwapChain(){
    return swapChain;
}

void DestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger, const VkAllocationCallbacks* pAllocator) {
    auto func = (PFN_vkDestroyDebugUtilsMessengerEXT) vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");
    if (func != nullptr) {
        func(instance, debugMessenger, pAllocator);
    }
}

bool checkValidationLayerSupport(std::vector<const char*> &validationLayers) {
    uint32_t layerCount;
    vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

    std::vector<VkLayerProperties> availableLayers(layerCount);
    vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

    for (const char* layerName : validationLayers) {
        bool layerFound = false;

        for (const auto& layerProperties : availableLayers) {
            if (strcmp(layerName, layerProperties.layerName) == 0) {
                layerFound = true;
                break;
            }
        }
        if (!layerFound) {
            return false;
        }
    }
    return true;
}

std::vector<const char*> getRequiredExtensions(bool layers) {
    uint32_t glfwExtensionCount = 0;
    const char** glfwExtensions;
    glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

    std::vector<const char*> extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);

    if (layers) {
        extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
    }

    return extensions;
}

bool checkDeviceCapabilities(VkPhysicalDevice &physicalDevice,VkSurfaceKHR &surface,SwapChainSupportDetails &caps){
    vkGetPhysicalDeviceSurfaceCapabilitiesKHR(physicalDevice, surface, &caps.capabilities);
    uint32_t presentModeCount;
    vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice, surface, &presentModeCount, nullptr);
    caps.presentModes.resize(presentModeCount);
    vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice, surface, &presentModeCount, caps.presentModes.data());
    uint32_t formatCount;
    vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, surface, &formatCount, nullptr);
    caps.formats.resize(formatCount);
    vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, surface, &formatCount, caps.formats.data());

    return !caps.presentModes.empty() && !caps.formats.empty();
}

void recordCommands(VkRenderPass &renderPass,VkFramebuffer &frameBuffer,infoForViews &info,VkCommandBuffer &commandBuffer){
    VkRenderPassBeginInfo beginInfo = {};
    beginInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
    beginInfo.renderPass = renderPass;
    beginInfo.framebuffer = frameBuffer;
    beginInfo.renderArea.offset = {0,0};
    beginInfo.renderArea.extent = info.extent;
    
    std::array<VkClearValue, 2> clearValues{};
    clearValues[0].color = {{0.0f, 0.0f, 1.0f, 1.0f}};
    clearValues[1].depthStencil = {1.0f, 0};

    beginInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
    beginInfo.pClearValues = clearValues.data();

    vkCmdBeginRenderPass(commandBuffer, &beginInfo, VK_SUBPASS_CONTENTS_INLINE);
}

void populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo) {
    createInfo = {};
    createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
    createInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
    createInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
    createInfo.pfnUserCallback = debugCallback;
}

VkResult CreateDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT* pDebugMessenger) {
    auto func = (PFN_vkCreateDebugUtilsMessengerEXT) vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");
    if (func != nullptr) {
        return func(instance, pCreateInfo, pAllocator, pDebugMessenger);
    } else {
        return VK_ERROR_EXTENSION_NOT_PRESENT;
    }
}

bool queueSupport(VkPhysicalDevice &physicaldevice, uint32_t &index, uint32_t &index2,VkSurfaceKHR &surface){
    uint32_t queueCount;
    vkGetPhysicalDeviceQueueFamilyProperties(physicaldevice, &queueCount, nullptr);
    std::vector<VkQueueFamilyProperties> queues(queueCount);
    vkGetPhysicalDeviceQueueFamilyProperties(physicaldevice, &queueCount, queues.data());
    bool graphics = false;
    bool present = false;
    for(int i = 0; i<queueCount; i++){
        if(queues[i].queueFlags & VK_QUEUE_GRAPHICS_BIT){
            index = i;
            graphics = true;
        }
        VkBool32 presentSupport = false;
        vkGetPhysicalDeviceSurfaceSupportKHR(physicaldevice, i, surface, &presentSupport);
        if(presentSupport){
            index2 = i;
            present = true;
        }
    }
    return graphics & present;
}

VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities,GLFWwindow *window) {//magic
    if (capabilities.currentExtent.width != UINT32_MAX) {
        return capabilities.currentExtent;
    } else {
        int width, height;
        glfwGetFramebufferSize(window, &width, &height);

        VkExtent2D actualExtent = {
            static_cast<uint32_t>(width),
            static_cast<uint32_t>(height)
        };

        actualExtent.width = std::max(capabilities.minImageExtent.width, std::min(capabilities.maxImageExtent.width, actualExtent.width));
        actualExtent.height = std::max(capabilities.minImageExtent.height, std::min(capabilities.maxImageExtent.height, actualExtent.height));

        return actualExtent;
    }
}

static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageType, const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, void* pUserData) {
    std::cerr << "validation layer: " << pCallbackData->pMessage << std::endl;

    return VK_FALSE;
}
