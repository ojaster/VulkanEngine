//
//  objects.hpp
//  FIrst_Project
//
//  Created by Данил on 4.9.2021.
//  Copyright © 2021 Daniil. All rights reserved.
//

#ifndef objects_hpp
#define objects_hpp

#include <stdio.h>
#include <vector>
#include "helperFunctions.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/hash.hpp>
#include <vulkan/vk_mem_alloc.h>

class ObjT{
public:
    char * texture;
    std::vector<Vertex2> vertices;
    ObjT(std::vector<Vertex2> vertices2,char* tex);
    ObjT(char* tex);
    ObjT();
    void operator =(ObjT &object);
    bool atlas = false;
    int id;

    void copy(std::vector<Vertex> vertices2,char * tex);



    std::vector<VkDescriptorSet> descSets;
    std::vector<VkBuffer> uniformBuffers;
    std::vector<VmaAllocation> uniformBufferMem;
    VkBuffer vertexBuffer;
    VmaAllocation vertexBufferMem;
    VkBuffer indexBuffer;
    VmaAllocation IndexBufferMem;
    VkDescriptorPool pool;

    VkImage textureImage;//fixed problem
    VkDeviceMemory textureImageMem;//fixed problem
    VkImageView textureImageView;//fixed problem
    VkSampler textureSampler;//fixed problem

    VkPipeline graphicsPipeline;//problem
    VkDescriptorSetLayout setLayout;//problem
    VkPipelineLayout layout;//problem
    bool packman = false;
    void spriteSheet(VkImageView &atView, VkImage &atImage, VkSampler &atSampler,VkDeviceMemory &atMem);


    void destroyObjForRecreate(VkDevice &logicalDevice,VmaAllocator &allocator);

    void destroyObj(VkDevice &logicalDevice,VmaAllocator &allocator);

    void createObj(VkDevice &logicalDevice,VkPhysicalDevice &physicalDevice,VkCommandPool &commandPool,VkQueue &graphicsQueue,uint32_t size,infoForViews &info,VkRenderPass &renderPass,VkDescriptorSetLayout &tSetLayout,VkPipelineLayout &tLayout, VkPipeline &tPipeline,VmaAllocator &allocator);

    void createObjForRecreate(VkDevice &logicalDevice,VkPhysicalDevice &physicalDevice,uint32_t size,infoForViews &info,VkRenderPass &renderPass,VmaAllocator &allocator,VkDescriptorSetLayout &tSetLayout,VkPipelineLayout &tLayout, VkPipeline &tPipeline);
};
void recordObj(VkBuffer &vertexBuffer,VkBuffer &indexBuffer,VkDescriptorSet &descSet,VkPipelineLayout &layout,VkCommandBuffer &commandBuffer);
struct textureObj{
    glm::vec2 cords[4];
    void intialize(glm::vec2 leftTop,glm::vec2 botRight);
};
void texPosVert(std::vector<Vertex2> &vertices,textureObj texObj);
void texPosVert90deg(std::vector<Vertex2> &vertices,textureObj texObj);
void texPosVert90degR(std::vector<Vertex2> &vertices,textureObj texObj);
void texPosVert180deg(std::vector<Vertex2> &vertices,textureObj texObj);
void texPosVert180degR(std::vector<Vertex2> &vertices,textureObj texObj);

void textureCoordCreator(float tileWidth,float tileHeight, float tileMapWidth,float tileMapHeight,std::vector<textureObj> &objectsTex);

void createMapFromTmx(int sizeX, int sizeY,std::vector<std::vector<int>> &map,infoForViews &info,std::vector<textureObj> &objectsTex,std::vector<ObjT> &returnObj);

void animation(textureObj &frame1,ObjT &object);

#endif /* objects_hpp */
