////
////  main.cpp
////  FIrst_Project
////
////  Created by Данил on 2.9.2021.
////  Copyright © 2021 Daniil. All rights reserved.
////
////#include <stdio.h>
////#include "objects.hpp"
////#include "initVulkan.hpp"
//#include "packman.hpp"
//std::vector<const char*> validationLayers = {
//    "VK_LAYER_KHRONOS_validation"
//};
//
//std::vector<const char*> deviceExtensions = {
//    VK_KHR_SWAPCHAIN_EXTENSION_NAME
//};
////#include "initWindow.hpp"
//void update(int a){
//    std::cout<<"dick and balls "<<a<<std::endl;
//}
//void getupdate(GLFWwindow *window,float &up, float &left,bool &notDown, bool &down,bool &right, bool &notRight,double &deltaTime){
//    float speedY = 0.5;//same here -0.5
//    float speedx = 0.5;//reversed values because in packman with vulkan value is -0.5
//    if(glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS && !notDown){
//        notDown = true;
//        down = false;
//        right = false;
//        notRight = false;
//        
//        //order
//      //  changeDir = true;
//    }
//    if(glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS && !down){
//        down = true;
//        notDown = false;
//        right = false;
//        notRight = false;
//        
//        //changeDir = true;
//    }
//    if(glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS && !right){
//        right = true;
//        down = false;
//        notDown = false;
//        notRight = false;
//        
////            changeDir = true;
//    }
//    if(glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS && !notRight){
//        notRight = true;
//        down = false;
//        right = false;
//        notDown = false;
//        
////            changeDir = true;
//    }
//    if(down) {
//        up -=speedY * deltaTime;
//    }
//    if(notDown){
//        up +=speedY * deltaTime;
//    }
//    if(notRight){
//        left -=speedx * deltaTime;
//    }
//    if(right){
//        left +=speedx * deltaTime;
//    }
//}
//int main(){
//    
//    packman k;
//    windowH window(800,600,"hehe");
//
//    initVulkanH vulkan(true);
//    std::srand(time(NULL));
//    try {
//        k.game(vulkan, window,deviceExtensions,validationLayers,getupdate);// problem with fullscreen mode and it is apps fault, not my api
//    } catch (const std::exception& e) {
//        std::cout<<e.what()<<std::endl;
//        return 0;
//    }
//}
