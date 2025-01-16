//
//  Miner.cpp
//  FIrst_Project
//
//  Created by Данил on 10.10.2021.
//  Copyright © 2021 Daniil. All rights reserved.
//

#include <stdio.h>
#include "initImGui.hpp"
#include "physics.hpp"
#include "camera.hpp"
#include "loadFiles.hpp"

struct player{
    bool up;
    bool down;
    bool left;
    bool right;
    float speedY;
    float speedX;
    float posX;
    float posY;
    float getOffsetX(){
        return offsetX;
    }
    float getOffsetY(){
        return offsetY;
    }
    player(float pPosx, float pPosy):offsetX(-pPosx),offsetY(-pPosy){
        init();
    }
    player(glm::vec2 pos):offsetX(-pos.x),offsetY(-pos.y){
        init();
    }
private:
    float offsetX;
    float offsetY;
    void init(){
        up = false;
        down = false;
        left = false;
        right = false;
        speedX = 0.5;
        speedY = 0.5;
        posX = 0.f;
        posY = 0.f;
    }
};


static std::vector<const char*> validationLayers = {
    "VK_LAYER_KHRONOS_validation"
};

static std::vector<const char*> deviceExtensions = {
    VK_KHR_SWAPCHAIN_EXTENSION_NAME
};
void createGraphicsPipeline(initVulkanH *vulkan){


    infoForViews info = vulkan->getInfo();
    VkDevice &logicalDevice = vulkan->getDevice();
    auto vertShaderCode = readFile("/Users/daniil/Desktop/vulkan engine recources/shaders/triangleVert.spv");
    auto fragShaderCode = readFile("/Users/daniil/Desktop/vulkan engine recources/shaders/triangleFrag.spv");

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
    rasterizer.frontFace = VK_FRONT_FACE_CLOCKWISE;
    rasterizer.depthBiasEnable = VK_FALSE;

    VkPipelineMultisampleStateCreateInfo multisampling{};
    multisampling.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
    multisampling.sampleShadingEnable = VK_FALSE;
    multisampling.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;

    VkPipelineDepthStencilStateCreateInfo depthStencil{};
    depthStencil.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
    depthStencil.depthTestEnable = VK_TRUE;
    depthStencil.depthWriteEnable = VK_TRUE;
    depthStencil.depthCompareOp = VK_COMPARE_OP_LESS;
//    VK_COMPARE_OP_LESS_OR_EQUAL
    depthStencil.depthBoundsTestEnable = VK_FALSE;
    depthStencil.stencilTestEnable = VK_FALSE;

    VkPipelineColorBlendAttachmentState colorBlendAttachment{};
    colorBlendAttachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;

    colorBlendAttachment.blendEnable = VK_TRUE;
    colorBlendAttachment.srcColorBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA;
    colorBlendAttachment.dstColorBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
    colorBlendAttachment.colorBlendOp = VK_BLEND_OP_ADD;
    colorBlendAttachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA;
    colorBlendAttachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
    colorBlendAttachment.alphaBlendOp = VK_BLEND_OP_SUBTRACT;

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
    pipelineLayoutInfo.pSetLayouts = &vulkan->getSetLayout();

    if (vkCreatePipelineLayout(logicalDevice, &pipelineLayoutInfo, nullptr, &vulkan->getLayout()) != VK_SUCCESS) {
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
    pipelineInfo.layout = vulkan->getLayout();
    pipelineInfo.renderPass = vulkan->getRenderPass();
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

    if (vkCreateGraphicsPipelines(logicalDevice, VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &vulkan->getPipeline()) != VK_SUCCESS) {
        throw std::runtime_error("failed to create graphics pipeline!");
    }
//    std::cout<<"flag:"<<feedBack.flags<<std::endl;
//    std::cout<<"duration:"<<feedBack.duration<<std::endl;
//    std::cout<<std::endl;
//    for(int i = 0; i <2; i++){
//        std::cout<<"flags:"<<feedBacks[i].flags<<std::endl;
//        std::cout<<"duration:"<<feedBacks[i].duration<<std::endl;
//    }

    vkDestroyShaderModule(logicalDevice, fragShaderModule, nullptr);
    vkDestroyShaderModule(logicalDevice, vertShaderModule, nullptr);
}

void createDescriptorSetLayout(initVulkanH *vulkan){
    VkDescriptorSetLayoutBinding setLayout = {};
    setLayout.binding = 0;
    setLayout.descriptorCount = 1;
    setLayout.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    setLayout.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;

    VkDescriptorSetLayoutBinding samplerLayoutBinding{};
    samplerLayoutBinding.binding = 1;
    samplerLayoutBinding.descriptorCount = 1;
    samplerLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    samplerLayoutBinding.pImmutableSamplers = nullptr;
    samplerLayoutBinding.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;

    std::vector<VkDescriptorSetLayoutBinding> bindings = {setLayout,samplerLayoutBinding};

    VkDescriptorSetLayoutCreateInfo createInfo = {};
    createInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
    createInfo.bindingCount = static_cast<uint32_t>(bindings.size());
    createInfo.pBindings = bindings.data();


    if(vkCreateDescriptorSetLayout(vulkan->getDevice(), &createInfo, nullptr, &vulkan->getSetLayout()) != VK_SUCCESS){
        throw std::runtime_error("failed to create descriptor set layout");
    }
}
void beginCommandBuffers(std::vector<ObjT> &objects, initVulkanH &vulkan,std::vector<VkCommandBuffer> &commandBuffers){
    for(int i = 0; i<commandBuffers.size(); i++){
        VkCommandBufferBeginInfo beginInfo = {};
        beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
        if(vkBeginCommandBuffer(commandBuffers[i], &beginInfo) != VK_SUCCESS){
            throw std::runtime_error("failed to begin command buffers");
        }
        recordCommands(vulkan.getRenderPass(), vulkan.getFrameBuffers()[i], vulkan.getInfo(), commandBuffers[i]);
        vkCmdBindPipeline(commandBuffers[i], VK_PIPELINE_BIND_POINT_GRAPHICS, vulkan.getPipeline());
        for(int j = 0; j<objects.size(); j++){
            recordObj(objects[j].vertexBuffer, objects[j].indexBuffer, objects[j].descSets[i], objects[j].layout, commandBuffers[i]);
        }

        vkCmdEndRenderPass(commandBuffers[i]);
        if(vkEndCommandBuffer(commandBuffers[i]) != VK_SUCCESS){
            throw std::runtime_error("failed to end command buffers");
        }
    }
}
glm::mat4 viewMatrixx(glm::lookAt(glm::vec3(0.0f, 0.0f, 2.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f)));
glm::mat4 perspectiveMatrixx;
void updateUniformBufferMap(UniformBufferObject &ubo,initVulkanH &vulkan){

    ubo.model = glm::transpose(glm::mat4(1,0,0,0,
                          0,1,0,0,
                          0,0,1,0,
                          0,0,0,1));
    ubo.view = viewMatrixx;
    ubo.proj = perspectiveMatrixx;
//    ubo.proj[1][1] *= -1;
}
void updateUniformBufferPlayer(UniformBufferObject &ubo,initVulkanH &vulkan,player &m){

    ubo.model = glm::transpose(glm::mat4(1,0,0,m.posX,
                          0,1,0,m.posY,
                          0,0,1,0,
                          0,0,0,1));
    ubo.view = viewMatrixx;
    ubo.proj = perspectiveMatrixx;
//    ubo.proj[1][1] *= -1;

}
void applyUniformBuffer(std::vector<ObjT> &obj,uint32_t &currentImage,initVulkanH &vulkan,std::vector<UniformBufferObject> &uboh){
    for(int i = 0; i<obj.size(); i++){
        UniformBufferObject ubo{};
        ubo.model = uboh[i].model;
        ubo.view = uboh[i].view;
        ubo.proj = uboh[i].proj;

        void *data;
        vmaMapMemory(vulkan.getAllocator(), obj[i].uniformBufferMem[currentImage], &data);
            memcpy(data, &ubo, sizeof(ubo));
        vmaUnmapMemory(vulkan.getAllocator(), obj[i].uniformBufferMem[currentImage]);

        VkDeviceSize bufferSize = sizeof(obj[i].vertices[i]) * obj[i].vertices.size();
        void *data2;
        vmaMapMemory(vulkan.getAllocator(), obj[i].vertexBufferMem, &data2);// forgot to change data to data2
        memcpy(data2, obj[i].vertices.data(), (size_t) bufferSize);
        vmaUnmapMemory(vulkan.getAllocator(), obj[i].vertexBufferMem);
    }
}
void gnomeWalkAnimation(ObjT &gnome,std::vector<textureObj> &texObjects){
    if(gnome.id >= 23){
        gnome.id = 21;
    }else if(gnome.id < 23){
        gnome.id++;
    }
    animation(texObjects[gnome.id], gnome);
}
void gnomeWalkAnimation2(ObjT &gnome,std::vector<textureObj> &texObjects){
    if(gnome.id >= 29){
        gnome.id = 27;
    }else if(gnome.id < 29){
        gnome.id++;
    }
    animation(texObjects[gnome.id], gnome);
}
void gnomeDig(ObjT &gnome,std::vector<textureObj> &texObjects){
    if(gnome.id >= 26){
        gnome.id = 24;
    }else if(gnome.id < 26){
        gnome.id++;
    }
    animation(texObjects[gnome.id], gnome);
}
void gnomeDig2(ObjT &gnome,std::vector<textureObj> &texObjects){
    if(gnome.id >= 32){
        gnome.id = 30;
    }else if(gnome.id < 32){
        gnome.id++;
    }
    animation(texObjects[gnome.id], gnome);
}

void slimeAnimate(ObjT &slime,std::vector<textureObj> &texObjects, int original){
    if(slime.id >= original + 2){
        slime.id = original;
    }else if(slime.id < original + 2){
        slime.id++;
    }
    animation(texObjects[slime.id], slime);
}
void gnomeWalk(ObjT &gnome,player &m,double deltaTime,GLFWwindow *window,bool col,Rect &g){//probably should find some physics library
//    int dir = col ? -1:1;
    int dir = 1;
    float friction = 0.2;

    float acceleration = 0.1;
    float completeStop = 0.01;
    float maxSpeed = 0.5;// not active right now and i don't think i'll need it any time soon

//    glm::vec2 tmp = {0,0};
    float jumpHeight = (abs(g.vel.x)>0.5) ? abs(g.vel.x) : 0.5;
    float jumpAcc = 0.5;
    if(glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS){
        g.vel.y -= sin(jumpHeight)*jumpAcc;
        m.down = true;
        m.up = false;
    }
    if(glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS){
        g.vel.x -= acceleration;
            m.left = true;
            m.right = false;
        }
        if(glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS){
            g.vel.x += acceleration;
            m.right = true;
            m.left = false;
        }
    if(std::abs(g.vel.y) < completeStop){
        g.vel.y = 0;
    }
    if(std::abs(g.vel.x) < completeStop){
        g.vel.x = 0;
    }

//    g.vel.y += tmp.y * deltaTime;
//    g.vel.x += tmp.x * deltaTime;

        g.vel.y *= std::pow(friction, deltaTime);
        g.vel.x *= std::pow(friction, deltaTime);

//    if(std::abs(g.vel.y) > maxSpeed){
//        if(g.vel.y < 0){
//            g.vel.y = -maxSpeed;
//        }else{
//            g.vel.y = maxSpeed;
//        }
//    }
//    if(std::abs(g.vel.x) > maxSpeed){
//        if(g.vel.x < 0){
//            g.vel.x = -maxSpeed;
//        }else{
//            g.vel.x = maxSpeed;
//        }
//    }

    m.posX += g.vel.x * deltaTime;
    m.posY += g.vel.y * deltaTime;
//    std::cout<<g.vel.x<<std::endl;

}

void mainLoop(windowH &window, initVulkanH &vulkan){
    int fps = 60;
    double lastTime = glfwGetTime();

    VkDevice device = vulkan.getDevice();
    VkPhysicalDevice physicalDevice = vulkan.getPhysicalDevice();
    VkCommandPool pool = vulkan.getCommandPool();
    VkQueue queue = vulkan.getGraphicsQueue();

    vkStruct tmp = {device,physicalDevice,pool,queue};
    TileMap maph;
    maph.loadMap("/Users/daniil/Desktop/vulkan engine recources/tileMaps/ActualMiner.tmx","/Users/daniil/Desktop/vulkan engine recources/textures/crazyMiner.png",tmp);

    VkImage texImage = maph.getImages().texImage;
    VkDeviceMemory texImageMem = maph.getImages().texImageMem;
    VkImageView texImageView = maph.getImages().texImageView;
    VkSampler texSampler = maph.getImages().texSampler;

    std::vector<std::vector<int>> map = maph.getMap();

    std::vector<textureObj> texObjects = maph.getTexObjects();
    std::vector<ObjT> objects;
    createMapFromTmx(maph.getMapDimensions().x,maph.getMapDimensions().y, map, vulkan.getInfo(), texObjects, objects);

    for(int i = 0; i<objects.size(); i++){
        objects[i].spriteSheet(texImageView, texImage, texSampler, texImageMem);
        objects[i].createObj(vulkan.getDevice(), vulkan.getPhysicalDevice(), vulkan.getCommandPool(), vulkan.getGraphicsQueue(), 3, vulkan.getInfo(), vulkan.getRenderPass(), vulkan.getSetLayout(), vulkan.getLayout(), vulkan.getPipeline(), vulkan.getAllocator());
    }

    VkResult recreate;
    uint32_t imageIndex = 0;

    double delay = 0;

    player gnome(objects[0].vertices[0].pos);

    double delta = 1.f/fps;
//    for(int i = 0; i<4; i++){
//        objects[0].vertices[i].pos.z = 0.1f;
//    }

    int playerId = 0;
    Rect g(objects[0].vertices);


    int fp;
    double prevTime = glfwGetTime();
    bool col;


//    VkImage texback;
//    VkDeviceMemory texBackMem;
//    VkSampler texBackSamp;
//    VkImageView texBackView;

//    createAtlas(vulkan.getDevice(), vulkan.getPhysicalDevice(), texback, texBackMem, vulkan.getCommandPool(), vulkan.getGraphicsQueue(), "textures/back.png", texBackSamp, texBackView);
    ///idea front back ground tommorow do 14.10.2021
    float depth = -0.1f;
    float dim = 1.5f;


    std::vector<Vertex2> verticesBack(4);
    verticesBack[0].copy({{-dim, -1,depth}, {1.0f, 0.0f, 0.0f}, {1.f,1.f}});
    verticesBack[1].copy({{dim, -1,depth}, {0.0f, 1.0f, 1.0f},  {0.f,1.f}});
    verticesBack[2].copy({{dim, 1,depth}, {0.0f, 0.0f, 1.0f},  {0.f,0.f}});
    verticesBack[3].copy({{-dim, 1,depth}, {0.7f, 1.0f, 0.3f},  {1.f,0.f}});
    textureObj obj;
    obj.intialize({0.f,0.f}, {1.f,1.f});
    texPosVert(verticesBack, obj);

    ObjT backGround(verticesBack,"/Users/daniil/Desktop/vulkan engine recources/tileMaps/blueCrystal.png");
    backGround.id = -1;
    backGround.createObj(vulkan.getDevice(), vulkan.getPhysicalDevice(), vulkan.getCommandPool(), vulkan.getGraphicsQueue(), 3, vulkan.getInfo(), vulkan.getRenderPass(), vulkan.getSetLayout(), vulkan.getLayout(), vulkan.getPipeline(), vulkan.getAllocator());
    objects.push_back(backGround);


//    std::vector<textureObj> frontBacks;
//    textureCoordCreator(64, 64, 128, 128, frontBacks);
//    depth = 0.1f;
//    dim = 1.4f;
//    std::vector<Vertex2> verticesFront(4);
//    verticesFront[3].copy({{-dim, -1,depth}, {1.0f, 0.0f, 0.0f}, {1.f,1.f}});
//    verticesFront[2].copy({{dim, -1,depth}, {0.0f, 1.0f, 1.0f},  {0.f,1.f}});
//    verticesFront[1].copy({{dim, 1,depth}, {0.0f, 0.0f, 1.0f},  {0.f,0.f}});
//    verticesFront[0].copy({{-dim, 1,depth}, {0.7f, 1.0f, 0.3f},  {1.f,0.f}});
//    texPosVert(verticesFront, frontBacks[0]);

//    ObjT frontBack(verticesFront,"textures/frontBack.png");
//    frontBack.id = -2;
//    frontBack.createObj(vulkan.getDevice(), vulkan.getPhysicalDevice(), vulkan.getCommandPool(), vulkan.getGraphicsQueue(), 3, vulkan.getInfo(), vulkan.getRenderPass(), vulkan.getSetLayout(), vulkan.getLayout(), vulkan.getPipeline(), vulkan.getAllocator());
//    objects.push_back(frontBack);

    beginCommandBuffers(objects, vulkan, vulkan.getCommandBuffers());


    std::vector<UniformBufferObject> uboh(objects.size());// plus one because of baciGround

    bool right;

    std::vector<int> slimes;
    std::vector<int> original;
    for(int i = 0; i<objects.size(); i++){
//        std::cout<<objects[i].id<<std::endl;
        if(objects[i].id == 0 || objects[i].id == 3 || objects[i].id == 6 || objects[i].id == 9){
            slimes.push_back(i);
            original.push_back(objects[i].id);
        }
    }

    int index =0;
    cameraH camera(CAMERA_SIDE_SCROLL);
    camera.initializeCamera(glm::vec3(0.0f, 0.0f, 2.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));

    float normalx;
    float normaly;
//    std::vector<b2Body*> floorss;
//    glm::vec2 gravity(0,1);
//    b2World world({gravity.x,gravity.y});
//    world.SetContinuousPhysics(true);
//    b2Body *player;
//    int b2BoxIndex = 0;
//
//    b2PolygonShape groundBox;
//    b2PolygonShape dynamicBox;
//    b2BodyDef tmp;
//    std::vector<int> it;
//    for(int i = 0; i<objects.size(); i++){
//        float x = objects[i].vertices[0].pos.x;
//        float y = objects[i].vertices[0].pos.y;
//        tmp.position.Set(x, y);
//
//
//        if(i == 0){
//            tmp.type = b2_dynamicBody;
//            player = world.CreateBody(&tmp);
//            player->SetType(b2_dynamicBody);
//            dynamicBox.SetAsBox(x, y);
//        }
//        if(objects[i].id == 40){
//            tmp.type = b2_staticBody;
//            floorss.resize(floorss.size()+1);
//            floorss[b2BoxIndex] = world.CreateBody(&tmp);
//            floorss[b2BoxIndex]->SetType(b2_staticBody);
//            groundBox.SetAsBox(x, y);
//            floorss[b2BoxIndex]->CreateFixture(&groundBox, 0.f);
//            it.push_back(i);
//            b2BoxIndex++;
//        }
//    }
//    b2FixtureDef fixtureDef;
//    fixtureDef.shape = &dynamicBox;
//    fixtureDef.density = 1.0f;
//    fixtureDef.friction = 0.3f;
//    player->CreateFixture(&fixtureDef);


    float x = objects[2].vertices[0].pos.x;
    float y = objects[2].vertices[0].pos.y;
    Rect rect(objects[2].vertices);
    b2Vec2 gravity(0.0f, 40.f);
    b2World world(gravity);
//    b2BodyDef groundBodyDef;
//    groundBodyDef.position.Set(x, y);
//    b2Body* groundBody = world.CreateBody(&groundBodyDef);
//    b2PolygonShape groundBox;
////    groundBox.SetAsBox(5.0f, 10.f);
//    /*
//     /// Build vertices to represent an axis-aligned box centered on the local origin.
//     /// @param hx the half-width.
//     /// @param hy the half-height.
//     */
//    groundBox.SetAsBox(rect.size.x/2, rect.size.y/2);
//    groundBody->CreateFixture(&groundBox, 0.0f);
        std::vector<b2Body*> groundBody;
        int b2BoxIndices = 0;
        float oldY;
        b2Vec2 topLeft;
        b2Vec2 downLeft;
        b2Vec2 downRight;
        b2Vec2 topRight;
    bool built = false;
    int multiplier = 1;
        for(int i = 0; i<objects.size(); i++){
            b2BodyDef groundBodyDef;
            b2PolygonShape groundBox;
            x = objects[i].vertices[0].pos.x;
            y = objects[i].vertices[0].pos.y;
            Rect rect(objects[i].vertices);
            if(objects[i].id == 40){
//                if(y != oldY){
//                    oldY = y;
//
//                    if(built && i != 2){
////                        groundBody.resize(groundBody.size() +1);
////                        groundBody[b2BoxIndices] = world.CreateBody(&groundBodyDef);
////                        groundBody[b2BoxIndices]->CreateFixture(&groundBox, 0.0f);
////                        b2BoxIndices++;
//                    }
//
////                    groundBodyDef.type = b2_staticBody;
////                    groundBodyDef.position.Set(x,y);
//////                    topLeft = {rect.pos.x,rect.pos.y};
//////                    downLeft = {rect.pos.x,rect.pos.y+rect.pos.y};
////                    built = false;
////                    multiplier = 1;
//                }else{
////                    float width = rect.size.x/2.f * multiplier;
////                    float height = rect.size.y/2.f;
////                    multiplier++;
////                    groundBox.SetAsBox(width, height);
////                    downRight = {rect.pos.x+rect.size.x,rect.pos.y + rect.size.y};
////                    topRight = {rect.pos.x+rect.size.x, rect.pos.y};
////                    std::vector<b2Vec2> vertices = {topLeft,downLeft,downRight,downLeft};
////                    groundBox.CreateLoop(vertices.data(), 4);
//                }
//
                if(rect.size.x < 0 || rect.size.y < 0){
                    std::cout<<"negative size"<<std::endl;
                }
                //                groundBox.SetOneSided(topLeft, topRight, downRight, downLeft);
    //                groundBox.m_vertex0 = topLeft;
    //                groundBox.m_vertex1 = downLeft;
    //                groundBox.m_vertex2 = topRight;
    //                groundBox.m_vertex3 = downRight;
                //                groundBox.set
//                                groundBox.SetAsBox((rect.size.x)/2.f, (rect.size.y)/2.f);
                groundBodyDef.type = b2_staticBody;
                groundBodyDef.position.Set(x,y);
//                    topLeft = {rect.pos.x,rect.pos.y};
//                    downLeft = {rect.pos.x,rect.pos.y+rect.pos.y};
//                built = false;
//                multiplier = 1;
                float width = rect.size.x/2.f;
                float height = rect.size.y/2.f;
//                multiplier++;
                groundBox.SetAsBox(width, height);

                groundBody.resize(groundBody.size() +1);
                groundBody[b2BoxIndices] = world.CreateBody(&groundBodyDef);
                groundBody[b2BoxIndices]->CreateFixture(&groundBox, 0.0f);
                b2BoxIndices++;

            }else{
                built = true;
            }
        }

    b2BodyDef bodyDef;
    bodyDef.type = b2_dynamicBody;
    bodyDef.bullet = true;// doesn't dipt but still
    x = objects[0].vertices[0].pos.x;
    y = objects[0].vertices[0].pos.y;
    bodyDef.position.Set(x, y);
    b2Body* body = world.CreateBody(&bodyDef);

//    b2PolygonShape dynamicBox;
    Rect rect1(objects[0].vertices);
//    b2Vec2 topLeft = {rect1.pos.x,rect1.pos.y};
//    b2Vec2 topRight = {rect1.pos.x+rect1.size.x, rect1.pos.y};
//    b2Vec2 downRight = {rect1.pos.x+rect1.size.x,rect1.pos.y + rect1.size.y};
//    b2Vec2 downLeft = {rect1.pos.x,rect1.pos.y+rect1.pos.y};
////                groundBox.SetOneSided(topLeft, topRight, downRight, downLeft);
//    dynamicBox.m_vertex0 = topLeft;
//    dynamicBox.m_vertex1 = downLeft;
//    dynamicBox.m_vertex2 = topRight;
//    dynamicBox.m_vertex3 = downRight;

    b2CircleShape circle;// a better solution than fucking around with edges, but still i gotta fuck with edges
//    circle.m_p.Set(2, 3);
    circle.m_radius = 0.09f;
//    dynamicBox.SetAsBox(rect1.size.x/2.f, (rect1.size.y)/2.f);

    b2FixtureDef fixtureDef;
    fixtureDef.shape = &circle;
    fixtureDef.density = 10.f;
    fixtureDef.friction = 0.6f;

    body->CreateFixture(&fixtureDef);

    int32 velocityIterations = 6;
    int32 positionIterations = 2;

    //very big sucess with box2d, can't wait to implement it for the all objects heheheheh, but now i've gotta go and do chores

    while (!glfwWindowShouldClose(window.getWindow())) {
        double aspectRatio = float(vulkan.getInfo().extent.width)/vulkan.getInfo().extent.height;
        perspectiveMatrixx = glm::ortho(-aspectRatio, aspectRatio, -1., 1., 1., 10.);
//        viewMatrixx = glm::lookAt(glm::vec3(0.0f, 0.0f, 2.0f), glm::vec3(gnome.posX, gnome.posY, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f)); //wrong but looks so cool
        camera.updateCamerPos(glm::vec3(gnome.posX - gnome.getOffsetX(), gnome.posY - gnome.getOffsetY(), 2.0f));
        viewMatrixx = camera.getCamera();
//        viewMatrixx = glm::lookAt(glm::vec3(gnome.posX - gnome.getOffsetX(), gnome.posY - gnome.getOffsetY(), 2.0f),   glm::vec3(gnome.posX - gnome.getOffsetX(), gnome.posY - gnome.getOffsetY(), 2.0f), glm::vec3(0.0f, 1.0f, 0.0f));

        //b2 tests here

        delay++;
        if(delay >= fps/3){
            if(gnome.right){
                gnomeWalkAnimation(objects[0],texObjects);
                right = true;
            }else if(gnome.left){
                if(objects[0].id < 27){
                    objects[0].id = 27;
                }
                gnomeWalkAnimation2(objects[0],texObjects);
                right = false;
            }
            if(!gnome.right && !gnome.left){
                if(!right && gnome.down){
                    gnomeDig2(objects[0], texObjects);
                }
                if(right && gnome.down){
                    gnomeDig(objects[0], texObjects);
                }
            }
            for(int i =0; i<slimes.size(); i++){
//                std::cout<<original[i]<<std::endl;
                slimeAnimate(objects[slimes[i]], texObjects, original[i]);
            }

//            animation(frontBacks[index++], objects[objects.size()-1]);
            if(index > 2){
                index = 0;
            }

            delay = 0;
        }

        gnome.right = false;
        gnome.left = false;
            gnomeWalk(objects[playerId],gnome,delta,window.getWindow(),col,g);
            glm::vec4 vector4;
            vector4 = uboh[0].model * glm::vec4(objects[0].vertices[0].pos,1);
            g.pos.x = vector4.x;
            g.pos.y = vector4.y;
        col = false;
        window.displayFps(fp, prevTime);


        {
            world.Step(delta, velocityIterations, positionIterations);
            b2Vec2 vec2 = body->GetPosition();
//            vec2.x += 0.01;
//            vec2.y += 0.01;
            b2Vec2 vel = body->GetLinearVelocity();
            b2Vec2 newVel = b2Vec2{g.vel.x,g.vel.y} + gravity;
            body->SetLinearVelocity({g.vel.x,g.vel.y});
//            body->ApplyLinearImpulse(newVel, body->GetWorldCenter(), true);
//            std::cout<<g.vel.x<<std::endl;
//            std::cout<<"x:"<<vec2.x<<" y:"<<vec2.y<<std::endl;
//            g.vel = {newVel.x,newVel.y};
            gnome.posX = vec2.x + gnome.getOffsetX();
            gnome.posY = vec2.y + gnome.getOffsetY();
            //velocity needs time to rest, it's the explanation for player getting stuck for some time when hitting a block
        }



        for(int i = 0; i<uboh.size(); i++){
            if(i == playerId){
                updateUniformBufferPlayer(uboh[i], vulkan,gnome);
                continue;
            }
//            if(objects[i].id != -1){
                updateUniformBufferMap(uboh[i], vulkan);
//            }
        }



        applyUniformBuffer(objects,imageIndex,vulkan,uboh);

        vulkan.drawFrame(window, createGraphicsPipeline, createDescriptorSetLayout, imageIndex, vulkan.getCommandBuffers(), recreate);
        if(recreate == VK_ERROR_OUT_OF_DATE_KHR){
            for(int i =0; i<objects.size(); i++){
                objects[i].destroyObjForRecreate(vulkan.getDevice(), vulkan.getAllocator());
                objects[i].createObjForRecreate(vulkan.getDevice(), vulkan.getPhysicalDevice(), 3, vulkan.getInfo(), vulkan.getRenderPass(), vulkan.getAllocator(), vulkan.getSetLayout(), vulkan.getLayout(), vulkan.getPipeline());
            }
            beginCommandBuffers(objects, vulkan, vulkan.getCommandBuffers());
        }
        glfwPollEvents();
        while (glfwGetTime() < lastTime + 1.0/fps) {}
        lastTime += 1.0/fps;
    }
    vkQueueWaitIdle(vulkan.getGraphicsQueue());
    for(int i =0; i<objects.size(); i++){
        objects[i].destroyObj(vulkan.getDevice(), vulkan.getAllocator());
    }


}

int main(){// i really should learn about glsl more
    initVulkanH vulkan(true);
    windowH window(1000,1000,"miner");
    try {
        vulkan.run(window.getWindow(), deviceExtensions, validationLayers, createGraphicsPipeline ,createDescriptorSetLayout);
        mainLoop(window, vulkan);
    } catch (const std::exception& e){
        std::cout<<e.what()<<std::endl;
        return EXIT_FAILURE;
    }
    vulkan.destroy();

}
