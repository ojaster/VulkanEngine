//
//  initWindow.cpp
//  FIrst_Project
//
//  Created by Данил on 2.9.2021.
//  Copyright © 2021 Daniil. All rights reserved.
//

#include "initWindow.hpp"
static bool framebufferResized = false;

windowH::windowH(const uint32_t width, const uint32_t height, const char *title){
    createWindow(width,height,title);
}
    
windowH::~windowH(){
    glfwTerminate();
    glfwDestroyWindow(window);
}


void windowH::createWindow(const uint32_t width, const uint32_t height, const char *title){
    glfwInit();
        
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
//    glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);
    if(glfwVulkanSupported()){
        std::cout<<"vulkan supported"<<std::endl;
    }
    
//    GLFWmonitor* monitor = glfwGetPrimaryMonitor();
//    const GLFWvidmode* mode = glfwGetVideoMode(monitor);
//    glfwWindowHint(GLFW_RED_BITS, mode->redBits);
//    glfwWindowHint(GLFW_GREEN_BITS, mode->greenBits);
//    glfwWindowHint(GLFW_BLUE_BITS, mode->blueBits);
//    glfwWindowHint(GLFW_REFRESH_RATE, mode->refreshRate);
    glfwSwapInterval(1);// makes drop and drag window smooth
    bool fullscreen = false;
    GLFWmonitor* monitor = fullscreen ? glfwGetPrimaryMonitor() : NULL;
    
    window = glfwCreateWindow(width, height, title, monitor, nullptr);
    glfwSetWindowUserPointer(window, this);
//    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);


    glfwSetFramebufferSizeCallback(window, framebufferResizeCallback);
}
GLFWwindow *windowH::getWindow(){
    return window;

}
void windowH::displayFps(int &fps,double &previousTime){
    double currentTime = glfwGetTime();
    fps++;
       // If a second has passed.
       if ( currentTime - previousTime >= 1.0 )
       {
           std::cout<<fps<<std::endl;
           fps = 0;
           previousTime = currentTime;
       }
}
void windowH::framebufferResizeCallback(GLFWwindow* window, int width, int height) {
    framebufferResized = true;
}
bool windowH::getFrameBufferResize(){
    return framebufferResized;
}
void windowH::setFrameBufferResizeFalse(){
    framebufferResized = false;
}
