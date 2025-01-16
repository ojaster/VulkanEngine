//
//  initWindow.hpp
//  FIrst_Project
//
//  Created by Данил on 2.9.2021.
//  Copyright © 2021 Daniil. All rights reserved.
//

#ifndef initWindow_hpp
#define initWindow_hpp
#include <stdio.h>

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <iostream>
class windowH{
public:
    windowH(const uint32_t width, const uint32_t height, const char *title);
    ~windowH();
    GLFWwindow *getWindow();
    void displayFps(int &fps,double &previousTime);
    bool getFrameBufferResize();
    void setFrameBufferResizeFalse();
private:
    //
    //parameters
    //
    GLFWwindow *window;

    //
    //methods
    //

    void createWindow(const uint32_t width, const uint32_t height, const char *title);
    static void framebufferResizeCallback(GLFWwindow* window, int width, int height);
};

#endif /* initWindow_hpp */
