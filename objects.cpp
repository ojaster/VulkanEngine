//
//  objects.cpp
//  FIrst_Project
//
//  Created by Данил on 4.9.2021.
//  Copyright © 2021 Daniil. All rights reserved.
//

#include "objects.hpp"
ObjT::ObjT(std::vector<Vertex2> vertices2,char* tex){
        vertices.resize(vertices2.size());
        for(int i = 0; i<vertices2.size(); i++){
            vertices[i].copy(vertices2[i]);
        }
        texture = tex;
    }
ObjT::ObjT(char* tex){texture = tex;}
ObjT::ObjT(){}
void ObjT::operator =(ObjT &object){
    texture = object.texture;
    vertices = object.vertices;
    descSets = object.descSets;
    uniformBuffers = object.uniformBuffers;
    uniformBufferMem = object.uniformBufferMem;
    vertexBuffer = object.vertexBuffer;
    vertexBufferMem = object.vertexBufferMem;
    indexBuffer = object.indexBuffer;
    IndexBufferMem = object.IndexBufferMem;
    pool = object.pool;
    textureImage = object.textureImage;
    textureImageMem = object.textureImageMem;
    textureImageView = object.textureImageView;
    textureSampler = object.textureSampler;
    graphicsPipeline = object.graphicsPipeline;
    setLayout = object.setLayout;
    layout = object.layout;
    packman = object.packman;
    atlas = object.atlas;
    id = object.id;
}

void ObjT::copy(std::vector<Vertex> vertices2,char * tex){
    vertices.resize(vertices2.size()*2);

    vertices[0].pos = vertices2[0].pos;// top left

    vertices[1].pos.x = vertices2[1].pos.x;
    vertices[1].pos.y = vertices2[0].pos.y;// top right

    vertices[2].pos.x = vertices2[1].pos.x;
    vertices[2].pos.y = vertices2[1].pos.y; // bottom right

    vertices[3].pos.x = vertices2[0].pos.x;
    vertices[3].pos.y = vertices2[1].pos.y; // bottom left

    for(int i =0; i<vertices.size(); i++){
        if(i <= 1){
            vertices[i].color = vertices2[0].color;// top vertices equal vertices2[0].color
        }else{
            vertices[i].color = vertices2[1].color;// bottom vertices equal vertices2[1].color
        }
    }
    texture = tex;
}

void ObjT::spriteSheet(VkImageView &atView, VkImage &atImage, VkSampler &atSampler,VkDeviceMemory &atMem){
    textureImageView = atView;
    textureImage = atImage;
    textureSampler = atSampler;
    textureImageMem = atMem;
    atlas = true;
}


void ObjT::destroyObjForRecreate(VkDevice &logicalDevice,VmaAllocator &allocator){
    vkDestroyDescriptorPool(logicalDevice, pool, nullptr);
    for(int i = 0; i<uniformBuffers.size(); i++){
        vmaDestroyBuffer(allocator, uniformBuffers[i], uniformBufferMem[i]);
    }
}

void ObjT::destroyObj(VkDevice &logicalDevice,VmaAllocator &allocator){
    vkDeviceWaitIdle(logicalDevice);
    for(int i = 0; i<uniformBuffers.size(); i++){
        vmaDestroyBuffer(allocator, uniformBuffers[i], uniformBufferMem[i]);
    }
    vkDestroyDescriptorPool(logicalDevice, pool, nullptr);
    vmaDestroyBuffer(allocator, indexBuffer, IndexBufferMem);
    vmaDestroyBuffer(allocator, vertexBuffer, vertexBufferMem);

    if(!atlas){
        vkDestroyImage(logicalDevice, textureImage, nullptr);
        vkDestroyImageView(logicalDevice, textureImageView, nullptr);
        vkDestroySampler(logicalDevice, textureSampler, nullptr);
        vkFreeMemory(logicalDevice, textureImageMem, nullptr);
    }
}
void ObjT::createObj(VkDevice &logicalDevice,VkPhysicalDevice &physicalDevice,VkCommandPool &commandPool,VkQueue &graphicsQueue,uint32_t size,infoForViews &info,VkRenderPass &renderPass,VkDescriptorSetLayout &tSetLayout,VkPipelineLayout &tLayout, VkPipeline &tPipeline,VmaAllocator &allocator){

    setLayout = tSetLayout;
    layout = tLayout;
    graphicsPipeline = tPipeline;


    createVertexBuffersT(vertexBuffer,vertexBufferMem,vertices,allocator);

    createIndexBuffer(indexBuffer, IndexBufferMem, allocator,commandPool,logicalDevice,graphicsQueue);

    uniformBuffers.resize(size);
    uniformBufferMem.resize(size);

    createUniformBuffers(uniformBuffers, uniformBufferMem, allocator);


    createDescriptorPoolT(logicalDevice, size, pool);

    descSets.resize(size);

    if(!atlas){
        createTextureImage(logicalDevice, physicalDevice, textureImage, textureImageMem, commandPool, graphicsQueue, texture);
        createTextureImageView(textureImage,logicalDevice,textureImageView);
        createImageSampler(logicalDevice, textureSampler);
    }
    createDescriptorSets2(logicalDevice, descSets, pool, setLayout, uniformBuffers, textureImageView, textureSampler);
}
void ObjT::createObjForRecreate(VkDevice &logicalDevice,VkPhysicalDevice &physicalDevice,uint32_t size,infoForViews &info,VkRenderPass &renderPass,VmaAllocator &allocator,VkDescriptorSetLayout &tSetLayout,VkPipelineLayout &tLayout, VkPipeline &tPipeline){

    setLayout = tSetLayout;
    layout = tLayout;
    graphicsPipeline = tPipeline;

    uniformBuffers.resize(size);
    uniformBufferMem.resize(size);
    createUniformBuffers(uniformBuffers, uniformBufferMem, allocator);

    createDescriptorPoolT(logicalDevice, size, pool);

    descSets.resize(size);
    createDescriptorSets2(logicalDevice, descSets, pool, setLayout, uniformBuffers, textureImageView, textureSampler);
}

void recordObj(VkBuffer &vertexBuffer,VkBuffer &indexBuffer,VkDescriptorSet &descSet,VkPipelineLayout &layout,VkCommandBuffer &commandBuffer){
    VkBuffer vertexBuffers[] = {vertexBuffer};
    VkDeviceSize offsets[] = {0};
    vkCmdBindVertexBuffers(commandBuffer, 0, 1, vertexBuffers, offsets);

    vkCmdBindIndexBuffer(commandBuffer, indexBuffer, 0, VK_INDEX_TYPE_UINT16);

    vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, layout, 0,1, &descSet, 0, nullptr);
    vkCmdDrawIndexed(commandBuffer, static_cast<uint32_t>(indices.size()), 1, 0, 0, 0);
}

void textureObj::intialize(glm::vec2 leftTop,glm::vec2 botRight){
    cords[0] = leftTop;
    cords[1].x = botRight.x;
    cords[1].y = leftTop.y;
        
    cords[2] = botRight;
        
    cords[3].x = leftTop.x;
    cords[3].y = botRight.y;
}

void texPosVert(std::vector<Vertex2> &vertices,textureObj texObj){
    vertices[0].texCoord = texObj.cords[0];
    vertices[1].texCoord = texObj.cords[1];
    vertices[2].texCoord = texObj.cords[2];
    vertices[3].texCoord = texObj.cords[3];
}

void texPosVert90deg(std::vector<Vertex2> &vertices,textureObj texObj){
    vertices[0].texCoord = texObj.cords[1];
    vertices[1].texCoord = texObj.cords[2];
    vertices[2].texCoord = texObj.cords[3];
    vertices[3].texCoord = texObj.cords[0];
}

void texPosVert90degR(std::vector<Vertex2> &vertices,textureObj texObj){
    vertices[0].texCoord = texObj.cords[3];
    vertices[1].texCoord = texObj.cords[0];
    vertices[2].texCoord = texObj.cords[1];
    vertices[3].texCoord = texObj.cords[2];
}

void texPosVert180deg(std::vector<Vertex2> &vertices,textureObj texObj){
    vertices[0].texCoord = texObj.cords[2];
    vertices[1].texCoord = texObj.cords[3];
    vertices[2].texCoord = texObj.cords[0];
    vertices[3].texCoord = texObj.cords[1];
}
void texPosVert180degR(std::vector<Vertex2> &vertices,textureObj texObj){
    vertices[0].texCoord = texObj.cords[1];
    vertices[1].texCoord = texObj.cords[0];
    vertices[2].texCoord = texObj.cords[3];
    vertices[3].texCoord = texObj.cords[2];
}

void textureCoordCreator(float tileWidth,float tileHeight, float tileMapWidth,float tileMapHeight,std::vector<textureObj> &objectsTex){//tile height and width = 32, picture width and heigh = 192
    glm::vec2 offsetIn = {0.001,0.001};
    glm::vec2 offsetOut = {-0.001,-0.001};
    glm::vec2 leftTop = {0,0};
    glm::vec2 rightBottom = {tileWidth/tileMapWidth, tileHeight/tileMapHeight};
    int size = tileMapWidth/tileWidth * tileMapHeight/tileHeight;
    
    objectsTex.resize(size);
    int nextLine = (tileMapWidth/tileWidth)-1;//probably should change// already changed
    for(int i = 0; i<size; i++){
        objectsTex[i].intialize(leftTop+offsetIn, rightBottom+offsetOut);
        if(i != nextLine){
            leftTop = {leftTop.x+tileWidth/tileMapWidth, leftTop.y};
          //  cout<<leftTop.x+32.f/192.f<<endl;
            rightBottom = {leftTop.x+tileWidth/tileMapWidth, rightBottom.y};
        }else{
            nextLine += sqrt(size);
            leftTop = {0, leftTop.y + tileHeight/tileMapHeight};
            rightBottom = {leftTop.x+tileWidth/tileMapWidth, leftTop.y + tileHeight/tileMapHeight};
        }
    }
}

void createMapFromTmx(int sizeX, int sizeY,std::vector<std::vector<int>> &map,infoForViews &info,std::vector<textureObj> &objectsTex,std::vector<ObjT> &returnObj){//should change should be in helper function and here it should be specifically modifieable for any game
    glm::vec3 black(0.f,0.f,0.f);
    glm::vec3 white(1.f,1.f,1.f);

    uint32_t WIDTH = info.extent.width;
    uint32_t HEIGHT = info.extent.height;

    float tileMapWidth = WIDTH/sizeX;
    float tileMapHeight = HEIGHT/sizeY;

    
    float depth = 0.f;
    ObjT temporary; // one object at a time
    temporary.vertices.resize(4);
    for(int i = 0; i<4; i++){
        temporary.vertices[i].pos.z = depth;
        temporary.vertices[i].color = black;
    }

    for(int i = 0; i<sizeY; i++){//height
        for(int j = 0; j<sizeX; j++){//width
            if(map[i][j] != 0){
                float packManX = j*tileMapWidth;
                float packManY = i*tileMapHeight;
                temporary.vertices[0].pos = {deviceToNormalized(packManX,WIDTH),deviceToNormalized(packManY,HEIGHT),depth};

                temporary.vertices[1].pos.x = deviceToNormalized(packManX + tileMapWidth,WIDTH);
                temporary.vertices[1].pos.y = deviceToNormalized(packManY,HEIGHT);// top right

                temporary.vertices[2].pos.x = deviceToNormalized(packManX + tileMapWidth, WIDTH);
                temporary.vertices[2].pos.y = deviceToNormalized(packManY+tileMapHeight, HEIGHT); // bottom right

                temporary.vertices[3].pos.x = deviceToNormalized(packManX,WIDTH);
                temporary.vertices[3].pos.y = deviceToNormalized(packManY+tileMapHeight, HEIGHT); // bottom left
                temporary.id = map[i][j]-1;
                texPosVert(temporary.vertices, objectsTex[map[i][j]-1]);
                returnObj.push_back(temporary);
            }
        }
    }
}

void animation(textureObj &frame1,ObjT &object){
    for(int i = 0; i<object.vertices.size(); i++){
        object.vertices[i].texCoord = frame1.cords[i];
    }
}

