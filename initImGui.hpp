//
//  initImGui.hpp
//  FIrst_Project
//
//  Created by Данил on 23.9.2021.
//  Copyright © 2021 Daniil. All rights reserved.
//

#ifndef initImGui_hpp
#define initImGui_hpp

#include <stdio.h>
#include "initVulkan.hpp"

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_vulkan.h"

#include "initWindow.hpp"
#include "objects.hpp"
#include <array>

#if defined(_MSC_VER) && (_MSC_VER >= 1900) && !defined(IMGUI_DISABLE_WIN32_FUNCTIONS)
#pragma comment(lib, "legacy_stdio_definitions")
#endif

//#define IMGUI_UNLIMITED_FRAME_RATE
#ifdef _DEBUG
#define IMGUI_VULKAN_DEBUG_REPORT
#endif

#ifdef IMGUI_VULKAN_DEBUG_REPORT
static VKAPI_ATTR VkBool32 VKAPI_CALL debug_report(VkDebugReportFlagsEXT flags, VkDebugReportObjectTypeEXT objectType, uint64_t object, size_t location, int32_t messageCode, const char* pLayerPrefix, const char* pMessage, void* pUserData)
{
    (void)flags; (void)object; (void)location; (void)messageCode; (void)pUserData; (void)pLayerPrefix; // Unused arguments
    fprintf(stderr, "[vulkan] Debug report from ObjectType: %i\nMessage: %s\n\n", objectType, pMessage);
    return VK_FALSE;
}
#endif // IMGUI_VULKAN_DEBUG_REPORT

class dearGui{
public:
    void init_imgui();
    void drawGui(void (*createGraphicsPipeline)(initVulkanH *),std::vector<ObjT> &objects,void (*beginCommandBuffers)(std::vector<ObjT> &, initVulkanH *, VkCommandBuffer &,uint32_t &frameIndex),std::vector<UniformBufferObject> &uboh,uint32_t &frameIndex);
    dearGui(initVulkanH &vulkanh, windowH &windowh);
    void destroyGui();
private:
    void applyUniformBuffer(std::vector<ObjT> &obj,uint32_t &currentImage,initVulkanH &vulkan,std::vector<UniformBufferObject> &uboh);
    
    void FramePresent(initVulkanH &vulkan,uint32_t &imageIndex);

    void glfw_error_callback(int error, const char* description);
    
    void FrameRender(ImDrawData* draw_data,void (*beginCommandBuffers)(std::vector<ObjT> &, initVulkanH *, VkCommandBuffer &,uint32_t &frameIndex),std::vector<ObjT> &objects,uint32_t &frameIndex);
    
    void SetupVulkanWindow(VkSurfaceKHR surface, int width, int height,initVulkanH &vulkan);
    
    VkDescriptorPool         g_DescriptorPool = VK_NULL_HANDLE;
    bool                     g_SwapChainRebuild = false;
    
    std::array<VkClearValue, 2> clearValues{};
    windowH window;
    initVulkanH *vulkan;
    int imageCount = 3;
    int currentFrame = 0;
    
    bool show_demo_window = true;
    bool show_another_window = false;
    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
    VkResult err;
};


#endif /* initImGui_hpp */
