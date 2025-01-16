//
//  loadFiles.cpp
//  FIrst_Project
//
//  Created by Данил on 3.9.2021.
//  Copyright © 2021 Daniil. All rights reserved.
//
#define STB_IMAGE_IMPLEMENTATION
#include "loadFiles.hpp"

std::vector<char> readFile(const std::string& filename){
    std::ifstream file(filename, std::ios::ate | std::ios::binary);

    if (!file.is_open()) {
        throw std::runtime_error("failed to open file!");
    }

    size_t fileSize = (size_t) file.tellg();
    std::vector<char> buffer(fileSize);

    file.seekg(0);
    file.read(buffer.data(), fileSize);

    file.close();

    return buffer;
}

void TileMap::loadMap(char *mapDir,char *imageDir, vkStruct &vulkan){
    if(!tmxMap.load(mapDir)){
        throw std::runtime_error("failed to open map file");
    }
    image = imageDir;
    initializeMap(vulkan);
}
TileMap::TileMap(){}

void TileMap::initializeMap(vkStruct &vulkan){
    tmx::Vector2u vec = tmxMap.getTileCount();
    map.resize(vec.y, std::vector<int>(vec.x));
    int tileIndex = 0;
    const auto& layers = tmxMap.getLayers();
    std::cout<<"MAP"<<std::endl;
    for(const auto& layer : layers){
        if(layer->getType() == tmx::Layer::Type::Object){
//            const auto& objectLayer = layer->getLayerAs<tmx::ObjectGroup>();
//            const auto& objects = objectLayer.getObjects();
//            for(const auto& object : objects){
//                    //do stuff with object properties
//            }
            
            }else if(layer->getType() == tmx::Layer::Type::Tile){
                    const auto& tileLayer = layer->getLayerAs<tmx::TileLayer>();
                    //read out tile layer properties etc...
                    const auto& tiles = tileLayer.getTiles();
                    for(int i = 0; i<vec.y; i++){
                        for(int j = 0; j<vec.x; j++){
                            std::cout<<tiles[tileIndex].ID<<" ";
                            map[i][j] = tiles[tileIndex].ID;
                            tileIndex++;
                        }
                        std::cout<<std::endl;
                    }
                }
            }
    std::cout<<"MAP END"<<std::endl;
//        std::cout<<"hehe"<<std::endl;
//            const auto& tilesets = tmxMap.getTilesets();
//            for(const auto& tileset : tilesets){
//                //read out tile set properties, load textures etc...
//            }
    createSpriteSheet(vulkan);
}
std::vector<std::vector<int>>& TileMap::getMap(){
    return map;
}
void TileMap::createSpriteSheet(vkStruct &vulkan){
    tmx::Vector2u tileSize = tmxMap.getTileSize();
    tileMapDimensions  = tmxMap.getTileCount();

    int texWidth, texHeight, texChannels;
    stbi_load(image, &texWidth, &texHeight, &texChannels, STBI_rgb_alpha);
    
    textureCoordCreator(tileSize.x, tileSize.x, texWidth, texHeight, texObjects);
    
    device = vulkan.logicalDevice;
    
    createAtlas(vulkan.logicalDevice, vulkan.physicalDevice, texImage, texImageMem, vulkan.commandPool, vulkan.graphicsQueue, image, texSampler, texImageView);
    
    
    
}

tmx::Vector2u TileMap::getMapDimensions(){
    return tileMapDimensions;
}
imageRes &TileMap::getImages(){
    imageRes res = {texImage,texImageMem,texImageView,texSampler};
    return res;
}
TileMap::~TileMap(){
    destroyAtlas(device, texImage, texImageMem, texSampler, texImageView);
}
std::vector<textureObj> &TileMap::getTexObjects(){
    return texObjects;
}
