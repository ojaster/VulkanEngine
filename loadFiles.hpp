//
//  loadFiles.hpp
//  FIrst_Project
//
//  Created by Данил on 3.9.2021.
//  Copyright © 2021 Daniil. All rights reserved.
//

#ifndef loadFiles_hpp
#define loadFiles_hpp
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <set>
//#include <tinyobjloader-master/examples/viewer/stb_image.h> why does it have bad gates or what
#include "Map.hpp"
#include "Layer.hpp"
#include "TileLayer.hpp"
#include "ObjectGroup.hpp"
#include "objects.hpp"
#include "helperFunctions.hpp"
#include "ImageLayer.hpp"

struct vkStruct{
    VkDevice &logicalDevice;
    VkPhysicalDevice &physicalDevice;
    VkCommandPool &commandPool;
    VkQueue &graphicsQueue;
};

struct imageRes{
    const VkImage &texImage;
    const VkDeviceMemory &texImageMem;
    const VkImageView &texImageView;
    const VkSampler &texSampler;
};

std::vector<char> readFile(const std::string& filename);
class TileMap{//i've got so many plans on this i will automate sprite sheet coordinat creation with this
public:
    void loadMap(char *mapDir,char *imageDir,vkStruct &vulkan);
    TileMap();
    std::vector<std::vector<int>>& getMap();
    tmx::Vector2u getMapDimensions();
    imageRes &getImages();
    ~TileMap();
    std::vector<textureObj> &getTexObjects();
private:
    void initializeMap(vkStruct &vulkan);
    void createSpriteSheet(vkStruct &vulkan);
    
    tmx::Map tmxMap;
    std::vector<textureObj> texObjects;
    
    std::vector<std::vector<int>> map;
    char * image;
    
    VkImage texImage;
    VkDeviceMemory texImageMem;
    VkImageView texImageView;
    VkSampler texSampler;
    
    VkDevice device;
    
    tmx::Vector2u tileMapDimensions;//tile map width in tiles //tile map height in tiles
};

#endif /* loadFiles_hpp */
