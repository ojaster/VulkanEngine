//
//  initImGui.cpp
//  FIrst_Project
//
//  Created by Данил on 23.9.2021.
//  Copyright © 2021 Daniil. All rights reserved.
//

//#define IMGUI_DISABLE_OBSOLETE_FUNCTIONS
#include "initImGui.hpp"
void dearGui::init_imgui(){
    if (!glfwVulkanSupported())
    {
        throw std::runtime_error("GLFW: Vulkan Not Supported\n");
    }
    VkResult err;

    int w, h;
    glfwGetFramebufferSize(window.getWindow(), &w, &h);

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsClassic();
    {
        VkDescriptorPoolSize pool_sizes[] =
        {
            { VK_DESCRIPTOR_TYPE_SAMPLER, 1000 },
            { VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1000 },
            { VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE, 1000 },
            { VK_DESCRIPTOR_TYPE_STORAGE_IMAGE, 1000 },
            { VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER, 1000 },
            { VK_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER, 1000 },
            { VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1000 },
            { VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, 1000 },
            { VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC, 1000 },
            { VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC, 1000 },
            { VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT, 1000 }
        };
        VkDescriptorPoolCreateInfo pool_info = {};
        pool_info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
        pool_info.flags = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT;
        pool_info.maxSets = 1000 * IM_ARRAYSIZE(pool_sizes);
        pool_info.poolSizeCount = (uint32_t)IM_ARRAYSIZE(pool_sizes);
        pool_info.pPoolSizes = pool_sizes;
        err = vkCreateDescriptorPool(vulkan->getDevice(), &pool_info, nullptr, &g_DescriptorPool);
        check_vk_result(err);
    }

    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForVulkan(window.getWindow(), true);
    ImGui_ImplVulkan_InitInfo init_info = {};
    init_info.Instance = vulkan->getInstance();
    init_info.PhysicalDevice = vulkan->getPhysicalDevice();
    init_info.Device = vulkan->getDevice();
    init_info.QueueFamily = vulkan->getIndices()[0];
    init_info.Queue = vulkan->getGraphicsQueue();
    init_info.PipelineCache = VK_NULL_HANDLE;
    init_info.DescriptorPool = g_DescriptorPool;
    init_info.Allocator = nullptr;
    init_info.MinImageCount = 3;
    init_info.ImageCount = imageCount;
    init_info.CheckVkResultFn = check_vk_result;
    ImGui_ImplVulkan_Init(&init_info, vulkan->getRenderPass());// so far compatible with vulkan.getRenderPass();

    // Load Fonts
    // - If no fonts are loaded, dear imgui will use the default font. You can also load multiple fonts and use ImGui::PushFont()/PopFont() to select them.
    // - AddFontFromFileTTF() will return the ImFont* so you can store it if you need to select the font among multiple.
    // - If the file cannot be loaded, the function will return NULL. Please handle those errors in your application (e.g. use an assertion, or display an error and quit).
    // - The fonts will be rasterized at a given size (w/ oversampling) and stored into a texture when calling ImFontAtlas::Build()/GetTexDataAsXXXX(), which ImGui_ImplXXXX_NewFrame below will call.
    // - Read 'docs/FONTS.md' for more instructions and details.
    // - Remember that in C/C++ if you want to include a backslash \ in a string literal you need to write a double backslash \\ !
    //io.Fonts->AddFontDefault();
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/Roboto-Medium.ttf", 16.0f);
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/Cousine-Regular.ttf", 15.0f);
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/DroidSans.ttf", 16.0f);
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/ProggyTiny.ttf", 10.0f);
    //ImFont* font = io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\ArialUni.ttf", 18.0f, NULL, io.Fonts->GetGlyphRangesJapanese());
    //IM_ASSERT(font != NULL);

    // Upload Fonts
    {
        VkCommandBuffer command_buffer = beginSingletimeCommand(vulkan->getDevice(), vulkan->getCommandPool());

        ImGui_ImplVulkan_CreateFontsTexture(command_buffer);
        
        endSingleTimeCommand(command_buffer, vulkan->getGraphicsQueue(), vulkan->getDevice(), vulkan->getCommandPool());

        err = vkDeviceWaitIdle(vulkan->getDevice());
        check_vk_result(err);
        ImGui_ImplVulkan_DestroyFontUploadObjects();
    }

//    // Our state
//    
//    std::vector<Vertex2> vertices(4);
//    vertices[0].copy({{-1.0f, -0.5f}, {1.0f, 0.0f, 0.0f}, {1.f,0.f}});
//    vertices[1].copy({{-0.5f, -0.5f}, {0.0f, 1.0f, 1.0f},  {0.f,0.f}});
//    vertices[2].copy({{-0.5f, 0.0f}, {0.0f, 0.0f, 1.0f},  {0.f,1.f}});
//    vertices[3].copy({{-1.0f, 0.0f}, {0.7f, 1.0f, 0.3f},  {1.f,1.f}});
//    ObjT spinStuff(vertices,"textures/texture.jpg");
//    infoForViews info = {wd->SurfaceFormat,{uint32_t(w),uint32_t(h)}};
//    spinStuff.createObj(vulkan.getDevice(), vulkan.getPhysicalDevice(), vulkan.getCommandPool(), vulkan.getGraphicsQueue(), wd->ImageCount, info, wd->RenderPass, vulkan.getSetLayout(), vulkan.getLayout(), vulkan.getPipeline(), vulkan.getAllocator());
//
//    // Main loop
//    float lastTime = glfwGetTime();
//    float fps = 30;
}
void dearGui::drawGui(void (*createGraphicsPipeline)(initVulkanH *),std::vector<ObjT> &objects,void (*beginCommandBuffers)(std::vector<ObjT> &, initVulkanH *, VkCommandBuffer &,uint32_t &frameIndex),std::vector<UniformBufferObject> &uboh,uint32_t &frameIndex){
    // Resize swap chain?
    if (g_SwapChainRebuild)//no proper vulkan object enlargment maybe because it's unique recources like pipeline, setlayout and layout aren't recreated
    {
        vkQueueWaitIdle(vulkan->getGraphicsQueue());
        for(int i = 0; i<objects.size(); i++){
            objects[i].destroyObjForRecreate(vulkan->getDevice(), vulkan->getAllocator());// recreating recources should be done manually
        }
        int width, height;
        glfwGetFramebufferSize(window.getWindow(), &width, &height);
        if (width > 0 && height > 0)
        {
            ImGui_ImplVulkan_SetMinImageCount(imageCount);
            vulkan->recreateSwapChain(window.getWindow(), createGraphicsPipeline);
            g_SwapChainRebuild = false;
        }
        for(int i = 0; i<objects.size(); i++){
            objects[i].createObjForRecreate(vulkan->getDevice(), vulkan->getPhysicalDevice(), 3, vulkan->getInfo(), vulkan->getRenderPass(), vulkan->getAllocator(), vulkan->getSetLayout(), vulkan->getLayout(), vulkan->getPipeline());// althought because we give links of vulkan objects to imgui we shall not recreate vulkan like we would otherwise
        }
    }//do something later

    // Start the Dear ImGui frame
    ImGui_ImplVulkan_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    // 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
    if (show_demo_window)
        ImGui::ShowDemoWindow(&show_demo_window);

    // 2. Show a simple window that we create ourselves. We use a Begin/End pair to created a named window.
    {
        static float f = 0.0f;
        static int counter = 0;

        ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.

        ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
        ImGui::Checkbox("Demo Window", &show_demo_window);      // Edit bools storing our window open/close state
        ImGui::Checkbox("Another Window", &show_another_window);

        ImGui::SliderFloat("float", &f, -10.0f, 10.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
        ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color

        if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
            counter++;
        ImGui::SameLine();
        ImGui::Text("counter = %d", counter);

        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
        ImGui::End();
        uboh[0].model = uboh[0].model * glm::transpose(glm::mat4{1,0,0,0,
                                  0,1,0,0,
                                  0,0,1,f,
                                  0,0,0,1});//just a test, not permanent cool it works it is sick
    }

    // 3. Show another simple window.
    if (show_another_window)
    {
        ImGui::Begin("Another Window", &show_another_window);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
        ImGui::Text("Hello from another window!");
        if (ImGui::Button("Close Me"))
            show_another_window = false;
        ImGui::End();
    }

    // Rendering
    ImGui::Render();
    ImDrawData* draw_data = ImGui::GetDrawData();
    const bool is_minimized = (draw_data->DisplaySize.x <= 0.0f || draw_data->DisplaySize.y <= 0.0f);
    if (!is_minimized)
    {
        clearValues[1].depthStencil = {1.0f, 0};
        clearValues[0].color.float32[0] = clear_color.x * clear_color.w;
        clearValues[0].color.float32[1] = clear_color.y * clear_color.w;
        clearValues[0].color.float32[2] = clear_color.z * clear_color.w;
        clearValues[0].color.float32[3] = clear_color.w;
        applyUniformBuffer(objects, frameIndex, *vulkan, uboh);//have to update recources by myself
        FrameRender(draw_data, beginCommandBuffers, objects,frameIndex);// working but i see nothing, day later and i it works well now i need to find how to add handles to manipulate my objects live
        FramePresent(*vulkan,frameIndex);//if imgui is included it replaces my vulkan app to handle present
        
        //i think if i want to manipulate my objects live, it will mean that imgui will take resposibility of recreating my object when needed and it's destruction
    }
}
dearGui::dearGui(initVulkanH &vulkanh, windowH &windowh) : vulkan(&vulkanh), window(windowh){}
void dearGui::destroyGui(){
    err = vkDeviceWaitIdle(vulkan->getDevice());
    check_vk_result(err);
    vkDestroyDescriptorPool(vulkan->getDevice(), g_DescriptorPool, nullptr);
//    ImGui_ImplVulkanH_DestroyWindow(vulkan->getInstance(), vulkan->getDevice(), &g_MainWindowData, nullptr);
    ImGui_ImplVulkan_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}
void dearGui::FramePresent(initVulkanH &vulkan, uint32_t &imageIndex){
    if (g_SwapChainRebuild)
        return;
    VkSemaphore render_complete_semaphore = vulkan.getRenderFinishedSemaphores()[currentFrame];
    VkPresentInfoKHR info = {};
    info.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
    info.waitSemaphoreCount = 1;
    info.pWaitSemaphores = &render_complete_semaphore;
    info.swapchainCount = 1;
    info.pSwapchains = &vulkan.getSwapChain();
    info.pImageIndices = &imageIndex;
    err = vkQueuePresentKHR(vulkan.getPresentQueue(), &info);
    if (err == VK_ERROR_OUT_OF_DATE_KHR || err == VK_SUBOPTIMAL_KHR)
    {
        g_SwapChainRebuild = true;
        return;
    }
    check_vk_result(err);
    currentFrame = (currentFrame + 1) % 2; // Now we can use the next set of semaphores
}

void dearGui::glfw_error_callback(int error, const char* description){
    fprintf(stderr, "Glfw Error %d: %s\n", error, description);
}

void dearGui::FrameRender(ImDrawData* draw_data,void (*beginCommandBuffers)(std::vector<ObjT> &, initVulkanH *, VkCommandBuffer &,uint32_t &frameIndex),std::vector<ObjT> &objects,uint32_t &frameIndex){
    VkResult err;

    VkSemaphore image_acquired_semaphore  = vulkan->getImageAvailableSemaphores()[currentFrame];
    VkSemaphore render_complete_semaphore = vulkan->getRenderFinishedSemaphores()[currentFrame];
//    std::cout<<image_acquired_semaphore<<std::endl;
    err = vkAcquireNextImageKHR(vulkan->getDevice(), vulkan->getSwapChain(), UINT64_MAX, image_acquired_semaphore, VK_NULL_HANDLE, &frameIndex);
    if (err == VK_ERROR_OUT_OF_DATE_KHR || err == VK_SUBOPTIMAL_KHR)
    {
        g_SwapChainRebuild = true;
        return;
    }
    check_vk_result(err);

    {
        err = vkWaitForFences(vulkan->getDevice(), 1, &vulkan->getInFlightFences()[currentFrame], VK_TRUE, UINT64_MAX);    // wait indefinitely instead of periodically checking
        check_vk_result(err);

        err = vkResetFences(vulkan->getDevice(), 1, &vulkan->getInFlightFences()[currentFrame]);
        check_vk_result(err);
    }
    {
//        err = vkResetCommandPool(vulkan->getDevice(), vulkan->getCommandPool(), 0);
        vkResetCommandBuffer(vulkan->getCommandBuffers()[frameIndex], VK_COMMAND_BUFFER_RESET_RELEASE_RESOURCES_BIT);
        check_vk_result(err);
        VkCommandBufferBeginInfo info = {};
        info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
        info.flags |= VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
        err = vkBeginCommandBuffer(vulkan->getCommandBuffers()[frameIndex], &info);
        check_vk_result(err);
    }
    {
        VkRenderPassBeginInfo info = {};
        info.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
        info.renderPass = vulkan->getRenderPass();
        info.framebuffer = vulkan->getFrameBuffers()[frameIndex];
        info.renderArea.extent = vulkan->getInfo().extent;
        info.clearValueCount = static_cast<uint32_t>(clearValues.size());;
        info.pClearValues = clearValues.data();
        vkCmdBeginRenderPass(vulkan->getCommandBuffers()[frameIndex], &info, VK_SUBPASS_CONTENTS_INLINE);
    }

    // Record dear imgui primitives into command buffer
    beginCommandBuffers(objects, vulkan, vulkan->getCommandBuffers()[frameIndex],frameIndex);// order of rendering determines wheter my spinstuff renders behind or infront of the imgui windows
    ImGui_ImplVulkan_RenderDrawData(draw_data, vulkan->getCommandBuffers()[frameIndex]);

    // Submit command buffer
    vkCmdEndRenderPass(vulkan->getCommandBuffers()[frameIndex]);
    {
        VkPipelineStageFlags wait_stage = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
        VkSubmitInfo info = {};
        info.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
        info.waitSemaphoreCount = 1;
        info.pWaitSemaphores = &image_acquired_semaphore;
        info.pWaitDstStageMask = &wait_stage;
        info.commandBufferCount = 1;
        info.pCommandBuffers = &vulkan->getCommandBuffers()[frameIndex];
        info.signalSemaphoreCount = 1;
        info.pSignalSemaphores = &render_complete_semaphore;

        err = vkEndCommandBuffer(vulkan->getCommandBuffers()[frameIndex]);
        check_vk_result(err);
        err = vkQueueSubmit(vulkan->getGraphicsQueue(), 1, &info, vulkan->getInFlightFences()[currentFrame]);
        check_vk_result(err);
    }
}

//void dearGui::SetupVulkanWindow(ImGui_ImplVulkanH_Window* wd, VkSurfaceKHR surface, int width, int height,initVulkanH &vulkan){
//    wd->Surface = surface;
//
//    // Check for WSI support
//    VkBool32 res;
//    vkGetPhysicalDeviceSurfaceSupportKHR(vulkan.getPhysicalDevice(), vulkan.getIndices()[0], wd->Surface, &res);
//    if (res != VK_TRUE)
//    {
//        fprintf(stderr, "Error no WSI support on physical device 0\n");
//        exit(-1);
//    }
//
//    // Select Surface Format
//    const VkFormat requestSurfaceImageFormat[] = { VK_FORMAT_B8G8R8A8_SRGB };
//    const VkColorSpaceKHR requestSurfaceColorSpace = VK_COLOR_SPACE_SRGB_NONLINEAR_KHR;
//    wd->SurfaceFormat = ImGui_ImplVulkanH_SelectSurfaceFormat(vulkan.getPhysicalDevice(), wd->Surface, requestSurfaceImageFormat, (size_t)IM_ARRAYSIZE(requestSurfaceImageFormat), requestSurfaceColorSpace);
//
//    wd->PresentMode = VK_PRESENT_MODE_FIFO_KHR;
//    //printf("[vulkan] Selected PresentMode = %d\n", wd->PresentMode);
//
//    // Create SwapChain, RenderPass, Framebuffer, etc.
//    ImGui_ImplVulkanH_CreateOrResizeWindow(vulkan.getInstance(), vulkan.getPhysicalDevice(), vulkan.getDevice(), wd, vulkan.getIndices()[0], nullptr, width, height, 3);
//}
void dearGui::applyUniformBuffer(std::vector<ObjT> &obj,uint32_t &currentImage,initVulkanH &vulkan,std::vector<UniformBufferObject> &uboh){
    for(int i = 0; i<obj.size(); i++){
        UniformBufferObject ubo{};
        ubo.model = uboh[i].model;
        ubo.view = uboh[i].view;
        ubo.proj = uboh[i].proj;

        void *data;
        vmaMapMemory(vulkan.getAllocator(), obj[i].uniformBufferMem[currentImage], &data);
            memcpy(data, &ubo, sizeof(ubo));
        vmaUnmapMemory(vulkan.getAllocator(), obj[i].uniformBufferMem[currentImage]);
    }
}
