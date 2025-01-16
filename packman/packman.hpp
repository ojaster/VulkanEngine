//
//  packman.hpp
//  FIrst_Project
//
//  Created by Данил on 4.9.2021.
//  Copyright © 2021 Daniil. All rights reserved.
//

#ifndef packman_hpp
#define packman_hpp

#include <stdio.h>
#include "objects.hpp"
#include "initImGui.hpp"//include vulkanh and windowh
#include "physics.hpp"
#include "audioSystem.hpp"


class packman{
public:
    void game(initVulkanH &vulkan,windowH &window,std::vector<const char*> deviceExtensions,std::vector<const char*> layerExtension,void (*updateSequance)(GLFWwindow *,float &, float &,bool &, bool &,bool &, bool &,double &));
private:
    void drawFrame();
    void beginCommandBuffers(std::vector<VkCommandBuffer> &commandBuffers,VkRenderPass &renderPass,std::vector<VkFramebuffer> &frameBuffers,infoForViews &info,VkPipeline &pipeline,VkPipelineLayout &layout,std::vector<ObjT> &stuff);
    void createMapFromTmx(int map[21][21],infoForViews &info,std::vector<textureObj> &objectsTex,std::vector<ObjT> &returnObj,int *indices);
    void recreateObjects(std::vector<ObjT> &stuff,initVulkanH &vulkan);
};
void createGraphicsPipeline(initVulkanH *vulkan);
void createDescriptorSetLayout(initVulkanH *vulkan);
void updateUniformBufferT(VkDevice &logicalDevice, std::vector<VmaAllocation> &uniforBufferMem,infoForViews &info,uint32_t &currentImage,bool num,VmaAllocator &allocator);
void updateUniformBuffer(VkDevice &logicalDevice, std::vector<VmaAllocation> &uniforBufferMem,infoForViews &info,uint32_t &currentImage,float up,double &deltaTime,float &left,ObjT &packman,bool change,VmaAllocator &allocator,Rect &rectangle);
void animationP(bool notRight,bool right, bool down,bool notDown,textureObj &packmanClosed,textureObj &packmanOpen,ObjT &packman,double &prev);
void colResolve(Rect &wallRect, Rect &player,bool &notRight, bool &right, bool &down, bool &notDown,float &up, float &left,float offsetX, float offsetY);
void animation(textureObj &frame1,ObjT &object);
void updateUniformBufferTA(VkDevice &logicalDevice, std::vector<VmaAllocation> &uniforBufferMem,infoForViews &info,uint32_t &currentImage,bool num,VmaAllocator &allocator,ObjT &object);
void ghostUpdateUniformBuffer(std::vector<VmaAllocation> &uniforBufferMem,uint32_t &currentImage,float &up,float &left,ObjT &packman,bool change,VmaAllocator &allocator,Rect &rectangle);
void portalCol(Rect &player, Rect *portals,float &xMov,float xOffset);
class Ghost{
public:
    float pMoveX;
    float pMoveY;
    bool pUp;
    bool pDown;
    bool pLeft;
    bool pRight;
    ObjT ghost;
    
    
    bool cantUP = false;
    bool cantDown = false;
    bool cantLeft = false;
    bool cantRight = false;
    
    void initialize(ObjT &ghostI);
    void update(float speedY, float speedX, double deltaTime);
    Rect &getGhostB();
    void ghostAi(int &delayInSec,int &oldBox,int frameLimit = 60);
    
    float getOffsetX();
    float getOffsetY();
    
    void setGhostOutTrue();
    
    void setCantFalse();
    
    void decideCant();
    
private:
    Rect ghostB;
    float pOffsetX;
    float pOffsetY;
    bool pOut;
    
    bool ghostInIntersection = false;
    
    void ghostGetOut(double deltaTime);
    
};
void wallColGhosts(std::vector<ObjT> &walls, Ghost &blinky, Ghost &pinkGhost, Ghost &blueGhost, Ghost &orangeGhost);
#endif /* packman_hpp */
