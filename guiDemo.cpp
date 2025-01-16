////
////  guiDemo.cpp
////  FIrst_Project
////
////  Created by Данил on 25.9.2021.
////  Copyright © 2021 Daniil. All rights reserved.
////
//
//#include <stdio.h>
//#include "initImGui.hpp"
//
//std::vector<const char*> validationLayers = {
//    "VK_LAYER_KHRONOS_validation"
//};
//
//std::vector<const char*> deviceExtensions = {
//    VK_KHR_SWAPCHAIN_EXTENSION_NAME
//};
//
//void createGraphicsPipeline(initVulkanH *vulkan){
//    infoForViews info = vulkan->getInfo();
//    VkDevice &logicalDevice = vulkan->getDevice();
//    auto vertShader = readFile("shaders/vert.spv");
//    auto fragShader = readFile("shaders/frag.spv");
//
//    VkShaderModule vert;
//    VkShaderModuleCreateInfo vertInfo = {};
//    vertInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
//    vertInfo.codeSize = static_cast<uint32_t>(vertShader.size());
//    vertInfo.pCode = reinterpret_cast<uint32_t *>(vertShader.data());
//    if(vkCreateShaderModule(logicalDevice, &vertInfo, nullptr, &vert) != VK_SUCCESS){
//        throw std::runtime_error("failed to create shader module");
//    }
//
//    VkShaderModule frag;
//    VkShaderModuleCreateInfo fragInfo = {};
//    fragInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
//    fragInfo.codeSize = static_cast<uint32_t>(fragShader.size());
//    fragInfo.pCode = reinterpret_cast<uint32_t *>(fragShader.data());
//    if(vkCreateShaderModule(logicalDevice, &fragInfo, nullptr, &frag) != VK_SUCCESS){
//        throw std::runtime_error("failed to create shader module");
//    }
//
//
//    auto bindingDescription = Vertex2::getBindingDescription();
//    auto attributeDescriptions = Vertex2::getAttributeDescriptions();
//
//    VkPipelineVertexInputStateCreateInfo vertCreateInfo = {};
//    //createVertexInput(vertCreateInfo);
//    vertCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
//    vertCreateInfo.vertexBindingDescriptionCount = 1;
//    vertCreateInfo.vertexAttributeDescriptionCount = static_cast<uint32_t>(attributeDescriptions.size());
//    vertCreateInfo.pVertexBindingDescriptions = &bindingDescription;
//    vertCreateInfo.pVertexAttributeDescriptions = attributeDescriptions.data();
//
//    VkPipelineInputAssemblyStateCreateInfo assemblyCreateInfo = {};
//    //createAssemblyInput(assemblyCreateInfo);
//    assemblyCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
//    assemblyCreateInfo.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
//    assemblyCreateInfo.primitiveRestartEnable = VK_FALSE;
//
//    VkViewport viewPort = {};
//    VkRect2D scissor = {};
//    VkPipelineViewportStateCreateInfo viewPortCreateInfo = {};
//    //createViewPort(viewPortCreateInfo, info,viewPort,scissor);
//    viewPortCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
//    viewPort.x = 0;
//    viewPort.y = 0;
//    viewPort.width = info.extent.width;
//    viewPort.height = info.extent.height;
//    viewPort.minDepth = 0;
//    viewPort.maxDepth = 1;
//
//    scissor.offset = {0,0};
//    scissor.extent = info.extent;
//
//    viewPortCreateInfo.viewportCount = 1;
//    viewPortCreateInfo.pViewports = &viewPort;
//    viewPortCreateInfo.scissorCount = 1;
//    viewPortCreateInfo.pScissors = &scissor;
//
//
//    VkPipelineRasterizationStateCreateInfo rasterizationCreateInfo = {};
//    //createRasterization(rasterizationCreateInfo);
//    rasterizationCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
//    rasterizationCreateInfo.depthClampEnable = VK_FALSE;
//    rasterizationCreateInfo.rasterizerDiscardEnable = VK_FALSE;// only false if multi sampling on
//    rasterizationCreateInfo.polygonMode = VK_POLYGON_MODE_FILL;
//    rasterizationCreateInfo.cullMode = VK_CULL_MODE_BACK_BIT;
//    rasterizationCreateInfo.frontFace = VK_FRONT_FACE_CLOCKWISE;
//    rasterizationCreateInfo.depthBiasEnable = VK_FALSE;
//    rasterizationCreateInfo.lineWidth = 1.0f;
//
//    VkPipelineColorBlendStateCreateInfo colorBlendCreateInfo = {};
//    //createColorBlend(colorBlendCreateInfo);
//    colorBlendCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
//    colorBlendCreateInfo.logicOpEnable = VK_FALSE;
//    colorBlendCreateInfo.attachmentCount = 1;
//    colorBlendCreateInfo.logicOp = VK_LOGIC_OP_COPY;
//
//    VkPipelineColorBlendAttachmentState colorBlendAttachment{};
//       colorBlendAttachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT |  VK_COLOR_COMPONENT_A_BIT;
//       colorBlendAttachment.blendEnable = VK_TRUE;
//       colorBlendAttachment.srcColorBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA;
//       colorBlendAttachment.dstColorBlendFactor =  VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
//       colorBlendAttachment.colorBlendOp = VK_BLEND_OP_ADD;
//       colorBlendAttachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
//       colorBlendAttachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
//       colorBlendAttachment.alphaBlendOp = VK_BLEND_OP_ADD;
//
//    colorBlendCreateInfo.pAttachments = &colorBlendAttachment;
//
//  //  createPipelineLayout(layout,logicalDevice);
//    VkPipelineLayoutCreateInfo layoutCreateInfo = {};
//    layoutCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
//    layoutCreateInfo.setLayoutCount = 1;
//    layoutCreateInfo.pSetLayouts = &vulkan->getSetLayout();
//    if(vkCreatePipelineLayout(logicalDevice, &layoutCreateInfo, nullptr, &vulkan->getLayout()) != VK_SUCCESS){
//        throw std::runtime_error("failed to create pipline layout");
//    }
//
//    VkPipelineShaderStageCreateInfo vertStage = {};
//    vertStage.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
//    vertStage.stage = VK_SHADER_STAGE_VERTEX_BIT;
//    vertStage.module = vert;
//    vertStage.pName = "main";
//
//    VkPipelineShaderStageCreateInfo fragStage = {};
//    fragStage.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
//    fragStage.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
//    fragStage.module = frag;
//    fragStage.pName = "main";
//
//    VkPipelineShaderStageCreateInfo stages[2] = {vertStage,fragStage};
//
//            VkPipelineMultisampleStateCreateInfo multisampling{};
//            multisampling.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
//            multisampling.sampleShadingEnable = VK_FALSE;
//            multisampling.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
//
//    VkGraphicsPipelineCreateInfo createInfo = {};
//    createInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
//    createInfo.stageCount = 2;
//    createInfo.pStages = stages;
//    createInfo.pVertexInputState = &vertCreateInfo;
//    createInfo.pInputAssemblyState = &assemblyCreateInfo;
//    createInfo.pViewportState = &viewPortCreateInfo;
//    createInfo.pRasterizationState = &rasterizationCreateInfo;
//    createInfo.pColorBlendState = &colorBlendCreateInfo;
//    createInfo.pMultisampleState = &multisampling;
//    createInfo.layout = vulkan->getLayout();
//    createInfo.renderPass = vulkan->getRenderPass();
//    createInfo.subpass = 0;//.
//    createInfo.basePipelineHandle = VK_NULL_HANDLE;//,
//
//    if(vkCreateGraphicsPipelines(logicalDevice, nullptr, 1, &createInfo, nullptr, &vulkan->getPipeline()) != VK_SUCCESS){
//        throw std::runtime_error("failed to create graphics pipeline");
//    }
//    vkDestroyShaderModule(logicalDevice, vert, nullptr);
//    vkDestroyShaderModule(logicalDevice, frag, nullptr);
//}
//
//void createDescriptorSetLayout(initVulkanH *vulkan){
//    VkDescriptorSetLayoutBinding setLayout = {};
//    setLayout.binding = 0;
//    setLayout.descriptorCount = 1;
//    setLayout.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
//    setLayout.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;
//
//    VkDescriptorSetLayoutBinding samplerLayoutBinding{};
//    samplerLayoutBinding.binding = 1;
//    samplerLayoutBinding.descriptorCount = 1;
//    samplerLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
//    samplerLayoutBinding.pImmutableSamplers = nullptr;
//    samplerLayoutBinding.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;
//
//    std::vector<VkDescriptorSetLayoutBinding> bindings = {setLayout,samplerLayoutBinding};
//
//    VkDescriptorSetLayoutCreateInfo createInfo = {};
//    createInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
//    createInfo.bindingCount = static_cast<uint32_t>(bindings.size());
//    createInfo.pBindings = bindings.data();
//
//
//    if(vkCreateDescriptorSetLayout(vulkan->getDevice(), &createInfo, nullptr, &vulkan->getSetLayout()) != VK_SUCCESS){
//        throw std::runtime_error("failed to create descriptor set layout");
//    }
//}
//
//int main(){
//    windowH window(800,600,"demo");
//    initVulkanH vulkan(true);
////    Gui demoGui;
//    GuiB demoGui;
//    double lastTime = glfwGetTime();
//    float frameLimit = 30;
//    try {
//        vulkan.run(window.getWindow(), deviceExtensions, validationLayers, createGraphicsPipeline, createDescriptorSetLayout);
//        demoGui.init_imguiNV(window.getWindow(),vulkan);
//        while(!glfwWindowShouldClose(window.getWindow())){
//            glfwPollEvents();
//            demoGui.drawGuiNV(window.getWindow(), vulkan);
//            while (glfwGetTime() < lastTime + 1.0/frameLimit) {}
//            lastTime += 1.0/frameLimit;
//        }
//    } catch (const std::exception& e) {
//            std::cout<<e.what()<<std::endl;
//            return 0;
//    }
//    demoGui.cleanUpNV(vulkan);
//    vulkan.destroy();
//}
