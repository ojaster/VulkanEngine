////
////  packman.cpp
////  FIrst_Project
////
////  Created by Данил on 4.9.2021.
////  Copyright © 2021 Daniil. All rights reserved.
////
//#include "packman.hpp"
//#include <unordered_map>
////#include <SFML/Audio/AlResource.hpp>
////tommorow move everyhting from here, make it more generalized and for now not to care about performace, expect if they are vulkan relate and i mean low level
//
//
//glm::mat4 viewMatrixx(glm::lookAt(glm::vec3(0.0f, 0.0f, 2.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f)));
//glm::mat4 perspectiveMatrixx;
//Rect door;
//Rect posOfLeave;
//Rect posOffinish;
//
//int map[21][21] ={//from tiled and it works, a huge sucess
//    0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,
//    0,1,10,4,4,10,4,4,4,10,1,10,4,4,4,10,4,4,10,1,0,
//    0,1,4,1,1,4,1,1,1,4,1,4,1,1,1,4,1,1,4,1,0,
//    0,1,10,4,4,10,4,10,4,10,4,10,4,10,4,10,4,4,10,1,0,
//    0,1,4,1,1,4,1,4,1,1,1,1,1,4,1,4,1,1,4,1,0,
//    0,1,10,4,4,10,1,10,4,10,1,10,4,10,1,10,4,4,10,1,0,
//    0,1,1,1,1,4,1,1,1,4,1,4,1,1,1,4,1,1,1,1,0,
//    0,0,0,0,1,4,1,10,0,10,5,10,0,10,1,4,1,0,0,0,0,
//    0,1,1,1,1,4,1,0,1,1,-1,1,1,0,1,4,1,1,1,1,0,
//    0,9,0,0,0,10,0,10,1,23,17,11,1,10,0,10,0,0,0,9,0,
//    0,1,1,1,1,4,1,0,1,1,1,1,1,0,1,4,1,1,1,1,0,
//    0,0,0,0,1,4,1,10,0,0,0,0,0,10,1,4,1,0,0,0,0,
//    0,1,1,1,1,4,1,0,1,1,1,1,1,0,1,4,1,1,1,1,0,
//    0,1,10,4,4,10,4,10,4,10,1,10,4,10,4,10,4,4,10,1,0,
//    0,1,4,1,1,4,1,1,1,4,1,4,1,1,1,4,1,1,4,1,0,
//    0,1,10,10,1,10,4,10,4,10,29,10,4,10,4,10,1,10,10,1,0,
//    0,1,1,4,1,4,1,4,1,1,1,1,1,4,1,4,1,4,1,1,0,
//    0,1,10,10,4,10,1,10,4,10,1,10,4,10,1,10,4,10,10,1,0,
//    0,1,4,1,1,1,1,1,1,4,1,4,1,1,1,1,1,1,4,1,0,
//    0,1,10,4,4,4,4,4,4,10,4,10,4,4,4,4,4,4,10,1,0,
//    0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0
//};
//
//std::vector<Rect> turnBoxes;
//Rect portals[2];
//
//void packman::game(initVulkanH &vulkan, windowH &window,std::vector<const char*> deviceExtensions,std::vector<const char*> layerExtensions,void (*updateSequance)(GLFWwindow *,float &, float &,bool &, bool &,bool &, bool &,double &)){
//    vulkan.run(window.getWindow(), deviceExtensions, layerExtensions,createGraphicsPipeline,createDescriptorSetLayout);
////    glfwSetFramebufferSizeCallback(window.getWindow(), framebufferResizeCallback);
////    GuiB dearGui;
////    windowH windowTest(800,600,"hehe");
////    dearGui.init_imguiNV(window.getWindow(), vulkan);// somehow affects vulkan app
////    dearGui.init_imguiSpecial(window.getWindow(),vulkan);
//
//    std::vector<textureObj> objectsTex;
//    textureCoordCreator(32.f, 32.f, 192.f, 192.f,objectsTex);
//
//    std::vector<ObjT> stuff;
//    int indices[10];
//    createMapFromTmx(map, vulkan.getInfo(), objectsTex,stuff,indices);
//
////    std::vector<VkCommandBuffer> &commandBuffers = vulkan.getCommandBuffers();
////    VkRenderPass &renderPass = vulkan.getRenderPass();
////    std::vector<VkFramebuffer> &frameBuffers = vulkan.getFrameBuffers();
////    infoForViews &info = vulkan.getInfo();
////    VkPipeline &pipeline = vulkan.getPipeline();
////    VkPipelineLayout &layout = vulkan.getLayout();
////    std::cout<<VK_ERROR_OUT_OF_DATE_KHR<<std::endl;
//
//    //record command buffer, create getter function for command buffers, create recources, add obj class just draw first triangle, get the hang of things, then implement legit packman
//    int fps = 0;
//    double previousTime = glfwGetTime();
//    double lastTime = glfwGetTime();
//    float frameLimit = 60;
//    double aspectRatio = float(vulkan.getInfo().extent.width)/vulkan.getInfo().extent.height;
//    uint32_t imageIndex = 0;
//
//
//    VkImage atlasImage;
//    VkDeviceMemory atlasImageMem;
//    VkSampler atlasImageSampler;
//    VkImageView atlasImageView;
//
//    VkImage atlasImageG;
//    VkDeviceMemory atlasImageMemG;
//    VkSampler atlasImageSamplerG;
//    VkImageView atlasImageViewG;
//
////    ObjT packman;
////    packman = stuff[stuff.size()-1];
//    createAtlas(vulkan.getDevice(), vulkan.getPhysicalDevice(), atlasImage, atlasImageMem, vulkan.getCommandPool(), vulkan.getGraphicsQueue(), "textures/evenBetter.png", atlasImageSampler, atlasImageView);
//
//    float up = 0;
//    float left = 0;
//    bool notDown = false;
//    bool down = false;
//    bool right = false;
//    bool notRight = false;
////    double deltaTime = 1.f/60.f;
//    float depth = 0.f;
//    createAtlas(vulkan.getDevice(), vulkan.getPhysicalDevice(), atlasImageG, atlasImageMemG, vulkan.getCommandPool(), vulkan.getGraphicsQueue(), "textures/gameOver.png", atlasImageSamplerG, atlasImageViewG);
//    ObjT gameOver;
//    gameOver.vertices.resize(4);
//    gameOver.vertices[0].pos = {-0.5,0.5,depth};
//    gameOver.vertices[1].pos = {0.5,0.5,depth};
//    gameOver.vertices[2].pos = {0.5,-0.5,depth};
//    gameOver.vertices[3].pos = {-0.5,-0.5,depth};
//    gameOver.vertices[0].texCoord = {0,0};
//    gameOver.vertices[3].texCoord = {0,1};
//    gameOver.vertices[2].texCoord = {1,1};
//    gameOver.vertices[1].texCoord = {1,0};// texture is too big
//    gameOver.spriteSheet(atlasImageViewG, atlasImageG, atlasImageSamplerG, atlasImageMemG);
//    gameOver.createObj(vulkan.getDevice(), vulkan.getPhysicalDevice(), vulkan.getCommandPool(), vulkan.getGraphicsQueue(), 3, vulkan.getInfo(), vulkan.getRenderPass(),vulkan.getSetLayout(),vulkan.getLayout(),vulkan.getPipeline(),vulkan.getAllocator());
//
//
//    std::vector<ObjT> score(3);
//    std::vector<textureObj> scoreTex;
//    textureCoordCreator(32.f, 32.f, 96.f, 128.f,scoreTex);
////    textureCoordCreator(<#float tileWidth#>, <#float tileHeight#>, <#float tileMapWidth#>, <#float tileMapHeight#>, <#std::vector<textureObj> &objectsTex#>)
//
//    VkImage atlasScoreImage;
//    VkDeviceMemory atlasScoreImageMem;
//    VkImageView atlasScoreImageView;
//    VkSampler atlasScoreImageSampler;
//
//    createAtlas(vulkan.getDevice(), vulkan.getPhysicalDevice(), atlasScoreImage, atlasScoreImageMem, vulkan.getCommandPool(), vulkan.getGraphicsQueue(), "textures/numbers.png", atlasScoreImageSampler, atlasScoreImageView);
//    score[0].vertices.resize(4);
//    score[0].vertices[0].pos = {0.8,1,depth};
//    score[0].vertices[1].pos = {0.9,1,depth};
//    score[0].vertices[2].pos = {score[0].vertices[1].pos.x,0.5,depth};
//    score[0].vertices[3].pos = {0.8,0.5,depth};
//    Rect scorePos(score[0].vertices);
//
//    for(int i = 0; i<score.size(); i++){
//        score[i].vertices.resize(4);
//        score[i].vertices[0].pos = {0.8-scorePos.size.x*i,1,depth};
//        score[i].vertices[1].pos = {0.9-scorePos.size.x*i,1,depth};
//        score[i].vertices[2].pos = {0.9-scorePos.size.x*i,0.89,depth};
//        score[i].vertices[3].pos = {0.8-scorePos.size.x*i,0.89,depth};
//        for(int j = 0; j<4; j++){
//            score[i].vertices[j].texCoord = scoreTex[0].cords[j];
//        }
//        score[i].spriteSheet(atlasScoreImageView, atlasScoreImage, atlasScoreImageSampler, atlasScoreImageMem);
//        score[i].createObj(vulkan.getDevice(), vulkan.getPhysicalDevice(), vulkan.getCommandPool(), vulkan.getGraphicsQueue(), 3, vulkan.getInfo(), vulkan.getRenderPass(),vulkan.getSetLayout(),vulkan.getLayout(),vulkan.getPipeline(),vulkan.getAllocator());
//    }
//
//
//    for(int i =0; i<stuff.size(); i++){
//        stuff[i].spriteSheet(atlasImageView, atlasImage, atlasImageSampler, atlasImageMem);
//        stuff[i].createObj(vulkan.getDevice(), vulkan.getPhysicalDevice(), vulkan.getCommandPool(), vulkan.getGraphicsQueue(), 3, vulkan.getInfo(), vulkan.getRenderPass(),vulkan.getSetLayout(),vulkan.getLayout(),vulkan.getPipeline(),vulkan.getAllocator());
//    }
//    std::vector<ObjT> walls(indices[0]);
//    std::vector<ObjT> food(indices[1]);
//    int index = 0;
//    for(int i = 0; i<walls.size(); i++){
//        walls[i] = stuff[index++];
//    }
//    for(int i = 0; i<food.size(); i++){
//        food[i] = stuff[index++];
//    }
//    Ghost blinky;
//    blinky.initialize(stuff[index++]);
//    blinky.setGhostOutTrue();
//
//    ObjT packman(stuff[index++]);
//
//    Ghost orangeGhost;
//    orangeGhost.initialize(stuff[index++]);
//
//    Ghost pinkGhost;
//    pinkGhost.initialize(stuff[index++]);
//
//    Ghost blueGhost;
//    blueGhost.initialize(stuff[index++]);
//
//    stuff.resize(stuff.size()+1+3);
//    stuff[stuff.size()-4] = gameOver;
//
//    stuff[stuff.size()-3] = score[0];
//    stuff[stuff.size()-2] = score[1];
//    stuff[stuff.size()-1] = score[2];
//
//    //score should be textures and also atlas
//
//    Rect packmanB(packman.vertices);
//
//    float offsetX = -packmanB.pos.x;
//    float offsetY = -packmanB.pos.y;
//
//    VmaAllocator all = vulkan.getAllocator();
//    std::unordered_map<int, bool> skipIndixes;
//    double prev = glfwGetTime();
//    bool an = true;
//    VkResult recreate;
//    auto secondPartD = std::chrono::high_resolution_clock::now();
//    auto firstPartD = std::chrono::high_resolution_clock::now();
//    double deltaTime = 1/frameLimit;
//    int counter = 0;
//    int num1 = 0;
//    int num2 = 0;
//    int num3 = 0;
//
//    bool finishAnimation = false;
//    bool packmanDie = false;
//    int animationCounter = 0;
//    float del = 0.2;
//
//    int packmanPowerUpTimer = frameLimit * 2;
//    int packmanPowerCounter = 0;
//    bool powerUpWorking = false;
//
//    int oldBox = -1;
//
//    int poldBox = -1;
//    int pGhostDelay = 0;
//
//    int ghostDelay = 0;
//
//    int bghostDelay = 0;
//    int boldBox = -1;
//
//    int oghostDelay = 0;
//    int ooldBox = -1;
//
//
//    bool blinkyDead = false;
//    bool pinkDead = false;
//    bool orangeDead = false;
//    bool blueDead = false;
//
//    posOfLeave = blueGhost.ghost.vertices;
//    posOffinish = blinky.getGhostB();
//
//    double past = glfwGetTime();
//    double current = glfwGetTime();
//    perspectiveMatrixx = glm::ortho(-aspectRatio, aspectRatio, 1., -1., 1., 10.);//for some fucking reason shit is flipped so fuck i guess i have to find why it is flipped
//    std::vector<bool>skipPowerUps(turnBoxes.size());
//    skipPowerUps = {false};
//
////    float zoom;x
//
//    audioSystem sys;
////    sys.createReverb();
////    sys.setReverbAttributes();
//    sys.createSoundS("sound/pacman_beginning.mp3");//0
//    sys.createSoundS("sound/pacman_chomp.wav");//1
//    sys.createSoundS("sound/pacman_eatghost.wav");//2
//    sys.createSoundS("sound/pacman_death.wav");//3
//    sys.playSound();
//
//    bool firstPlay = true;
//
//
////    throw std::runtime_error("app works, but don't launch it, gui isn't setup yet");
////    ImGui_ImplVulkanH_Frame
//    beginCommandBuffers(vulkan.getCommandBuffers(), vulkan.getRenderPass(), vulkan.getFrameBuffers(), vulkan.getInfo(), vulkan.getPipeline(), vulkan.getLayout(), stuff);
//    while (!glfwWindowShouldClose(window.getWindow())) {
////        firstPartD = std::chrono::high_resolution_clock::now();
////        deltaTime = std::chrono::duration<double, std::chrono::seconds::period>(firstPartD - secondPartD).count();
////        secondPartD = firstPartD;
//        //window resize problem is here, i prob shouldn't do delta time like that
//        current = glfwGetTime();
//        if(an && !packmanDie){
//            animationP(notRight, right, down, notDown, objectsTex[28], objectsTex[30], packman, prev);
//        }else if(packmanDie && current - past >= del){
//            switch (animationCounter) {
//                case 0:
//                    animation(objectsTex[29], packman);
//                    break;
//                case 1:
//                    animation(objectsTex[31], packman);
//                    break;
//                case 2:
//                    animation(objectsTex[32], packman);
//                    break;
//                case 3:
//                    animation(objectsTex[33], packman);
//                    break;
//                case 4:
//                    animation(objectsTex[34], packman);
//                    break;
//                case 5:
//                    animation(objectsTex[35], packman);
//                    break;
//                case 6:
//                    animation(objectsTex[9], packman);
//                    break;
//                case 7:
//                    animation(objectsTex[15], packman);
//                    finishAnimation = true;
//                    break;
//
//                default:
//                    break;
//            }
//            animationCounter++;
//        }
//        if(current - past >= del){
//            past = current;
//        }
//        an = true;
//        for(int i = 0; i<food.size(); i++){
//            Rect rectangle(food[i].vertices);
//            if(rectCol(packmanB, rectangle) && !skipIndixes[i]){
//                skipIndixes[i] = true;
//                counter++;
//                num1 ++;
//                if(num1 >= 10){
//                    num2++;
//                    num1 = 0;
//                }
//                if(num2 >= 10){
//                    num3++;
//                    num2 = 0;
//                }
//                animation(scoreTex[num1], score[0]);
//                animation(scoreTex[num2], score[1]);
//                animation(scoreTex[num3], score[2]);
//                std::cout<<num3<<num2<<num1<<std::endl;
//                if(!sys.isPlaying()){
//                    sys.playSound(1);
//                }
//            }
//            if(!skipIndixes[i]){
//                updateUniformBufferT(vulkan.getDevice(), food[i].uniformBufferMem, vulkan.getInfo(), imageIndex,0,all);
//            }else{
//                updateUniformBufferT(vulkan.getDevice(), food[i].uniformBufferMem, vulkan.getInfo(), imageIndex,1,all);
//            }
//        }
//
//        for(int i = 0; i<walls.size(); i++){
//            Rect rectangle(walls[i].vertices);
//            if(rectCol(packmanB, rectangle)){
//                colResolve(rectangle, packmanB, notRight, right, down, notDown, up, left,offsetX,offsetY);
//                an = false;
//            }
//            updateUniformBufferT(vulkan.getDevice(), walls[i].uniformBufferMem, vulkan.getInfo(), imageIndex,0,all);
//        }
//        if(!packmanDie){
//            updateSequance(window.getWindow(),up,left,notDown,down,right,notRight,deltaTime);
//        }else{
//            notDown = false;
//            down = false;
//            right = false;
//            notRight = false;
//        }
//        updateUniformBuffer(vulkan.getDevice(), packman.uniformBufferMem, vulkan.getInfo(), imageIndex, up, deltaTime, left, packman, finishAnimation, vulkan.getAllocator(), packmanB);//not optimised copies vertex buffer each frame
//
//        for(int i = 0; i<score.size(); i++){
//            updateUniformBufferTA(vulkan.getDevice(), score[i].uniformBufferMem, vulkan.getInfo(), imageIndex, 0, vulkan.getAllocator(),score[i]);//not optimised copies vertex buffer each frame
//        }
//
//        if(counter >= food.size() || (packmanDie && finishAnimation)){
//            updateUniformBufferT(vulkan.getDevice(), gameOver.uniformBufferMem, vulkan.getInfo(), imageIndex, 0, vulkan.getAllocator());
//        }
//        if((rectCol(packmanB, blinky.getGhostB()) || rectCol(packmanB, pinkGhost.getGhostB()) || rectCol(packmanB, blueGhost.getGhostB()) || rectCol(packmanB, orangeGhost.getGhostB())) && !powerUpWorking && !packmanDie){
//            packmanDie = true;
//            an = false;
//            sys.playSound(3);
//        }
//
//        portalCol(packmanB, portals, left, offsetX);
//        portalCol(blinky.getGhostB(), portals, blinky.pMoveX, blinky.getOffsetX());
//        portalCol(pinkGhost.getGhostB(), portals, pinkGhost.pMoveX, pinkGhost.getOffsetX());
//        portalCol(blueGhost.getGhostB(), portals, blueGhost.pMoveX, blueGhost.getOffsetX());
//        portalCol(orangeGhost.getGhostB(), portals, orangeGhost.pMoveX, orangeGhost.getOffsetX());
//
//        ghostDelay++;
//        pGhostDelay++;
//        bghostDelay++;
//        oghostDelay++;
//
//        pinkGhost.ghostAi(pGhostDelay, poldBox,60);
//        blinky.ghostAi(ghostDelay, oldBox,60);
//        blueGhost.ghostAi(bghostDelay, boldBox,60);
//        orangeGhost.ghostAi(oghostDelay, ooldBox,60);
//
//
//
//        wallColGhosts(walls, blinky, pinkGhost, blueGhost, orangeGhost);
//
//        packmanPowerCounter++;
//        if(packmanPowerCounter >= packmanPowerUpTimer && powerUpWorking){
//            blinkyDead = false;
//            pinkDead = false;
//            orangeDead = false;
//            blueDead = false;
//            powerUpWorking = false;
////            sys.playSound(2);
//            std::cout<<"end of power up"<<std::endl;
//        }
//        blinky.update(0.4, 0.4, deltaTime);
//        ghostUpdateUniformBuffer(blinky.ghost.uniformBufferMem, imageIndex, blinky.pMoveY, blinky.pMoveX, blinky.ghost, blinkyDead, vulkan.getAllocator(), blinky.getGhostB());
//
//        pinkGhost.update(0.5, 0.5, deltaTime);
//        ghostUpdateUniformBuffer(pinkGhost.ghost.uniformBufferMem, imageIndex, pinkGhost.pMoveY, pinkGhost.pMoveX, pinkGhost.ghost, pinkDead, vulkan.getAllocator(), pinkGhost.getGhostB());
//
//        blueGhost.update(0.5, 0.5, deltaTime);
//        ghostUpdateUniformBuffer(blueGhost.ghost.uniformBufferMem, imageIndex, blueGhost.pMoveY, blueGhost.pMoveX, blueGhost.ghost, blueDead, vulkan.getAllocator(), blueGhost.getGhostB());
//
//        orangeGhost.update(0.5, 0.5, deltaTime);
//        ghostUpdateUniformBuffer(orangeGhost.ghost.uniformBufferMem, imageIndex, orangeGhost.pMoveY, orangeGhost.pMoveX, orangeGhost.ghost, orangeDead, vulkan.getAllocator(), orangeGhost.getGhostB());
//
//
//        if(rectCol(packmanB, orangeGhost.getGhostB()) && powerUpWorking && !orangeDead){
//            orangeDead = true;
//            sys.playSound(2);
//        }
//        if(rectCol(packmanB, blueGhost.getGhostB()) && powerUpWorking && !blueDead){
//            blueDead = true;
//            sys.playSound(2);
//        }
//        if(rectCol(packmanB, pinkGhost.getGhostB()) && powerUpWorking && !pinkDead){
//            pinkDead = true;
//            sys.playSound(2);
//        }
//        if(rectCol(packmanB, blinky.getGhostB()) && powerUpWorking && !blinkyDead){
//            blinkyDead = true;
//            sys.playSound(2);
//        }
////        if(pointVsRect(orangeGhost.getGhostB().pos, packmanB)){
////            std::cout<<"col"<<std::endl;// point vs rect works with ghosts and packman
////        }
//
//        for(int i = 0; i<turnBoxes.size(); i++){
//            if(rectCol(packmanB, turnBoxes[i]) && !skipPowerUps[i]){
//                powerUpWorking = true;
//                packmanPowerCounter = 0;
//                skipPowerUps[i] = true;
//                std::cout<<"power up start"<<std::endl;
//                break;
//            }
//        }
//
//
//        if(rectCol(packmanB, door)){
//            up = door.pos.y + packmanB.size.y + offsetY;
//            an = false;
//        }
//        if(rectCol(pinkGhost.getGhostB(), door)){
//            pinkGhost.pMoveY = door.pos.y + pinkGhost.getGhostB().size.y + pinkGhost.getOffsetY();
//        }
//        if(rectCol(orangeGhost.getGhostB(), door)){
//            orangeGhost.pMoveY = door.pos.y + orangeGhost.getGhostB().size.y + orangeGhost.getOffsetY();
//        }
//        if(rectCol(blueGhost.getGhostB(), door)){
//            blueGhost.pMoveY = door.pos.y + blueGhost.getGhostB().size.y + blueGhost.getOffsetY();
//        }
//        if(rectCol(blinky.getGhostB(), door)){
//            blinky.pMoveY = door.pos.y + blinky.getGhostB().size.y + blinky.getOffsetY();
//        }
//
//
////        dearGui.drawGuiNV(window.getWindow(), vulkan);// make frame render and frame present public and rerecord command buffer everyframe but only pass in render frame
////        vkWaitForFences(vulkan.getDevice(), 1, &vulkan.getInFlightFences()[vulkan.getCurrentFrame()], VK_TRUE, UINT64_MAX);// doesn't seem to be necessary, wrong without this check it will result jittering in full screen
////        dearGui.drawGuiSpecial();
////        if (vulkan.getImagesInFlight()[imageIndex] != VK_NULL_HANDLE) {
////            vkWaitForFences(vulkan.getDevice(), 1, &vulkan.getImagesInFlight()[imageIndex], VK_TRUE, UINT64_MAX);
////        }// idk draw frame already has those two checks so it might be sloving, but technically if fences were checked before then later they should be checked already
//
////        beginCommandBuffers(vulkan.getCommandBuffers(), vulkan.getRenderPass(), vulkan.getFrameBuffers(), vulkan.getInfo(), vulkan.getPipeline(), vulkan.getLayout(), stuff);// stuttering is caused by mac osx
//
//        vulkan.drawFrame(window,createGraphicsPipeline,createDescriptorSetLayout,imageIndex,vulkan.getCommandBuffers(),recreate);
//
//
//
//        if(recreate == VK_ERROR_OUT_OF_DATE_KHR){//fucked up
////            std::cout<<"after change"<<std::endl;
////            std::cout<<"x:"<<packmanB.pos.x<<" y:"<<packmanB.pos.y<<std::endl;
//            for(int i = 0; i<stuff.size(); i++){
//                stuff[i].destroyObjForRecreate(vulkan.getDevice(), vulkan.getAllocator());
//                stuff[i].createObjForRecreate(vulkan.getDevice(), vulkan.getPhysicalDevice(), 3, vulkan.getInfo(), vulkan.getRenderPass(), vulkan.getAllocator(), vulkan.getSetLayout(), vulkan.getLayout(), vulkan.getPipeline());
//            }
////            for(int i =0; i<vulkan.getCommandBuffers().size(); i++){
////                vkResetCommandBuffer(vulkan.getCommandBuffers()[i], NULL);
////            }
////            VkCommandBuffer cmd = vulkan.getCommandBuffers()[0];
////            std::cout<<"size:"<<sizeof(cmd)<<std::endl;
//            beginCommandBuffers(vulkan.getCommandBuffers(), vulkan.getRenderPass(), vulkan.getFrameBuffers(), vulkan.getInfo(), vulkan.getPipeline(), vulkan.getLayout(), stuff);
////            recreateObjects(stuff, vulkan);// forgot that i wrote this function
//            aspectRatio = float(vulkan.getInfo().extent.width)/vulkan.getInfo().extent.height;
//            perspectiveMatrixx = glm::ortho(-aspectRatio, aspectRatio, 1., -1., 1., 10.);//for some fucking reason shit is flipped so fuck i guess i have to find why it is flipped
////            std::cout<<vulkan.getInfo().extent.width<<" "<<vulkan.getInfo().extent.height<<std::endl;
////            int h;
////            int w;
//////            glfwGetWindowSize(window.getWindow(), &w, &h);
////            glfwGetFramebufferSize(window.getWindow(),&w, &h);
////            std::cout<<vulkan.getInfo().extent.width<<" vulkan "<<vulkan.getInfo().extent.height<<std::endl;
////            std::cout<<w<<" glfw "<<h<<std::endl;same
//        }
//        window.displayFps(fps, previousTime);
//
//        glfwPollEvents();
//        while (glfwGetTime() < lastTime + 1.0/frameLimit) {}
//        lastTime += 1.0/frameLimit;
//    }
//
//    vkQueueWaitIdle(vulkan.getPresentQueue());
//    destroyAtlas(vulkan.getDevice(), atlasImage, atlasImageMem, atlasImageSampler, atlasImageView);
//    destroyAtlas(vulkan.getDevice(), atlasImageG, atlasImageMemG, atlasImageSamplerG, atlasImageViewG);
//    destroyAtlas(vulkan.getDevice(), atlasScoreImage, atlasScoreImageMem, atlasScoreImageSampler, atlasScoreImageView);
//    for(int i =0; i<stuff.size(); i++){
//        stuff[i].destroyObj(vulkan.getDevice(), vulkan.getAllocator());
//    }
////    gameOver.destroyObj(vulkan.getDevice(), vulkan.getAllocator());
////    dearGui.cleanUpNV(vulkan);
////    dearGui.cleanUpSpecial(vulkan);
//    vulkan.destroy();
//}
//void packman::beginCommandBuffers(std::vector<VkCommandBuffer> &commandBuffers,VkRenderPass &renderPass,std::vector<VkFramebuffer> &frameBuffers,infoForViews &info,VkPipeline &pipeline,VkPipelineLayout &layout,std::vector<ObjT> &stuff){
//    for(int i = 0; i<commandBuffers.size(); i++){
//        VkCommandBufferBeginInfo beginInfo = {};
//        beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
//        if(vkBeginCommandBuffer(commandBuffers[i], &beginInfo) != VK_SUCCESS){
//            throw std::runtime_error("failed to begin command buffers");
//        }
//        recordCommands(renderPass, frameBuffers[i], info, commandBuffers[i]);
//        vkCmdBindPipeline(commandBuffers[i], VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline);
//
//        for(int j = 0; j<stuff.size(); j++){
//            recordObj(stuff[j].vertexBuffer, stuff[j].indexBuffer, stuff[j].descSets[i], stuff[j].layout, commandBuffers[i]);
//        }
//
//        vkCmdEndRenderPass(commandBuffers[i]);
//        if(vkEndCommandBuffer(commandBuffers[i]) != VK_SUCCESS){
//            throw std::runtime_error("failed to end command buffers");
//        }
//    }
//
//}
//void packman::createMapFromTmx(int map[21][21],infoForViews &info,std::vector<textureObj> &objectsTex,std::vector<ObjT> &returnObj,int *indices){//should change should be in helper function and here it should be specifically optimised for any game
//    glm::vec3 black(0.f,0.f,0.f);
//    glm::vec3 white(1.f,1.f,1.f);
//    glm::vec3 green(0.f,1.f,0.f);
//    glm::vec3 red(1.f,0.f,0.f);
//    glm::vec3 blue(0.f,0.f,1.f);
//
//    uint32_t WIDTH = info.extent.width;
//    uint32_t HEIGHT = info.extent.height;
//
//    bool continious = false;
//    bool firstVertex = false;
//    std::vector<Vertex> tmp(2);
//    tmp[0].color = white;
//    tmp[1].color = white;
//
//    std::vector<Vertex> tmp2(2);
//    tmp2[0].color = green;
//    tmp2[1].color = green;
//    float tileMapWidth = WIDTH/21.f;
//    float tileMapHeight = HEIGHT/21.f;
//    std::vector<ObjT> walls;// this has to be changed
//
//    int offset = 0;
//    int offset2 = 0;
//
//    std::vector<ObjT> food;
//    ObjT packman;
//
//    int xCord;
//    int yCord;
//
//
//    ObjT blinky;
//    ObjT orangeGhost;
//    ObjT pinkGhost;
//    ObjT blueGhost;
//
//    ObjT temporary; // one object at a time
//    temporary.vertices.resize(4);
//
//    int tempIndex = 0;
//    float depth = 0.f;
//
//    for(int i = 0; i<21; i++){
//        firstVertex = false;
//        for(int j = 0; j<21; j++){
//            if(map[i][j] == 1){
//                if(firstVertex == false || continious == false){
//                    tmp[0].pos.x = deviceToNormalized(j*tileMapWidth, WIDTH);
//                    tmp[0].pos.y = -deviceToNormalized((i-0.01)*tileMapHeight, HEIGHT);//remember float points aren't very accurate and you should always offset vertices, y1 of left top might not be equal to y2 of a top left below it
//                    firstVertex = true;
//                    continious = true;
//                }
//            }else if(map[i][j] != 1){
//                if(continious == true && firstVertex == true){
//                    continious = false;
//
//                    tmp[1].pos.x = deviceToNormalized((j)*tileMapWidth, WIDTH);
//                    tmp[1].pos.y = -deviceToNormalized((i+1.01)*tileMapHeight, HEIGHT);
//                    walls.resize(offset+1);
//                    walls[offset].copy({tmp[0],tmp[1]},"textures/walls1.png");
//                    texPosVert90deg(walls[offset].vertices, objectsTex[0]);
//                    offset++;
//
//                }
//            }
////            if(map[i][j] == 11){
////                tmp[0].pos.x = deviceToNormalized(j*tileMapWidth, WIDTH);
////                tmp[0].pos.y = -deviceToNormalized((i-0.01)*tileMapHeight, HEIGHT);
////
////
////                tmp[1].pos.x = deviceToNormalized((j+1)*tileMapWidth, WIDTH);
////                tmp[1].pos.y = -deviceToNormalized((i+1.01)*tileMapHeight, HEIGHT);
////
////                walls.resize(offset+1);
////                walls[offset].copy({tmp[0],tmp[1]},"textures/food.png");
////                texPosVert90degR(walls[offset].vertices, objectsTex[2]);
////                offset++;
////            }
////            if(map[i][j] == 12){
////                tmp[0].pos.x = deviceToNormalized(j*tileMapWidth, WIDTH);
////                tmp[0].pos.y = -deviceToNormalized((i-0.01)*tileMapHeight, HEIGHT);
////
////
////                tmp[1].pos.x = deviceToNormalized((j+1)*tileMapWidth, WIDTH);
////                tmp[1].pos.y = -deviceToNormalized((i+1.01)*tileMapHeight, HEIGHT);
////
////                walls.resize(offset+1);
////                walls[offset].copy({tmp[0],tmp[1]},"textures/food.png");
////                texPosVert(walls[offset].vertices, objectsTex[1]);
////                offset++;
////            }
////            if(map[i][j] == 13){
////                tmp[0].pos.x = deviceToNormalized(j*tileMapWidth, WIDTH);
////                tmp[0].pos.y = -deviceToNormalized((i-0.01)*tileMapHeight, HEIGHT);
////
////
////                tmp[1].pos.x = deviceToNormalized((j+1)*tileMapWidth, WIDTH);
////                tmp[1].pos.y = -deviceToNormalized((i+1.01)*tileMapHeight, HEIGHT);
////
////                walls.resize(offset+1);
////                walls[offset].copy({tmp[0],tmp[1]},"textures/food.png");
////
////                texPosVert180deg(walls[offset].vertices, objectsTex[1]);
////                offset++;
////            }
//            if(map[i][j] == 4 || map[i][j] == 10){
//                    tmp2[0].pos.x = deviceToNormalized(j*tileMapWidth, WIDTH);
//                    tmp2[0].pos.y = -deviceToNormalized(i*tileMapHeight, HEIGHT);
//
//
//                    tmp2[1].pos.x = deviceToNormalized((j+1)*tileMapWidth, WIDTH);
//                    tmp2[1].pos.y = -deviceToNormalized((i+1)*tileMapHeight, HEIGHT);
//
//                    food.resize(offset2+1);
//                    food[offset2].copy({tmp2[0],tmp2[1]},"textures/food.png");
//                    texPosVert(food[offset2].vertices, objectsTex[21]);
////                if(food.size() == 1){
////                    texPosVert(food[offset2].vertices, objectsTex[5]);
////                }
//                    offset2++;
//                if(map[i][j] == 10){
//                    turnBoxes.resize(turnBoxes.size() + 1);
//                    turnBoxes[turnBoxes.size()-1].pos.x = turnBoxes[turnBoxes.size()-1].pos.x + turnBoxes[turnBoxes.size()-1].size.x/2.5;
//                    turnBoxes[turnBoxes.size()-1].pos.y = turnBoxes[turnBoxes.size()-1].pos.y - turnBoxes[turnBoxes.size()-1].size.y/2.5;
//                    turnBoxes[turnBoxes.size()-1].size.x = turnBoxes[turnBoxes.size()-1].size.x/10;
//                    turnBoxes[turnBoxes.size()-1].size.y = turnBoxes[turnBoxes.size()-1].size.y/10;
//                    turnBoxes[turnBoxes.size()-1] = food[offset2-1].vertices;
//                    texPosVert(food[offset2-1].vertices, objectsTex[3]);
//                }
//            }
//
//            if(map[i][j] == 29){
//                xCord = i;
//                yCord = j;
//                float packManX = j*tileMapWidth;
//                float packManY = i*tileMapHeight;
//                packman.vertices.resize(4);
//                packman.vertices[0].pos = {deviceToNormalized(packManX,WIDTH),-deviceToNormalized(packManY,HEIGHT),depth};
//
//                packman.vertices[1].pos.x = deviceToNormalized(packManX + tileMapWidth/2,WIDTH);
//                packman.vertices[1].pos.y = -deviceToNormalized(packManY,HEIGHT);// top right
//
//                packman.vertices[2].pos.x = deviceToNormalized(packManX + tileMapWidth/2, WIDTH);
//                packman.vertices[2].pos.y = -deviceToNormalized(packManY + tileMapHeight/2, HEIGHT); // bottom right
//
//                packman.vertices[3].pos.x = deviceToNormalized(packManX,WIDTH);
//                packman.vertices[3].pos.y = -deviceToNormalized(packManY + tileMapHeight/2, HEIGHT); // bottom left
//
//                for(int i = 0; i<4; i++){
//                    packman.vertices[i].texCoord = objectsTex[28].cords[i];
//                }
//                packman.packman = true;
//            }
//            if(map[i][j] == 5){
//                blinky.vertices.resize(4);
//                float packManX = j*tileMapWidth;
//                float packManY = i*tileMapHeight;
//                blinky.vertices[0].pos = {deviceToNormalized(packManX,WIDTH),-deviceToNormalized(packManY,HEIGHT),depth};
//
//                blinky.vertices[1].pos.x = deviceToNormalized(packManX + tileMapWidth,WIDTH);
//                blinky.vertices[1].pos.y = -deviceToNormalized(packManY,HEIGHT);// top right
//
//                blinky.vertices[2].pos.x = deviceToNormalized(packManX + tileMapWidth, WIDTH);
//                blinky.vertices[2].pos.y = -deviceToNormalized(packManY +tileMapHeight, HEIGHT); // bottom right
//
//                blinky.vertices[3].pos.x = deviceToNormalized(packManX,WIDTH);
//                blinky.vertices[3].pos.y = -deviceToNormalized(packManY +tileMapHeight, HEIGHT); // bottom left
//
////                for(int i = 0; i<4; i++){
////                    returnObj[i].vertices[i].color = red;
////                }
//                texPosVert(blinky.vertices, objectsTex[4]);
//
//            }
//            if(map[i][j] == 23){
//                orangeGhost.vertices.resize(4);
//                float packManX = j*tileMapWidth;
//                float packManY = i*tileMapHeight;
//                orangeGhost.vertices[0].pos = {deviceToNormalized(packManX,WIDTH),-deviceToNormalized(packManY,HEIGHT),depth};
//
//                orangeGhost.vertices[1].pos.x = deviceToNormalized(packManX + tileMapWidth,WIDTH);
//                orangeGhost.vertices[1].pos.y = -deviceToNormalized(packManY,HEIGHT);// top right
//
//                orangeGhost.vertices[2].pos.x = deviceToNormalized(packManX + tileMapWidth, WIDTH);
//                orangeGhost.vertices[2].pos.y = -deviceToNormalized(packManY +tileMapHeight, HEIGHT); // bottom right
//
//                orangeGhost.vertices[3].pos.x = deviceToNormalized(packManX,WIDTH);
//                orangeGhost.vertices[3].pos.y = -deviceToNormalized(packManY +tileMapHeight, HEIGHT); // bottom left
//                texPosVert(orangeGhost.vertices, objectsTex[map[i][j]-1]);
//            }
//            if(map[i][j] == 11){
//                pinkGhost.vertices.resize(4);
//                float packManX = j*tileMapWidth;
//                float packManY = i*tileMapHeight;
//                pinkGhost.vertices[0].pos = {deviceToNormalized(packManX,WIDTH),-deviceToNormalized(packManY,HEIGHT),depth};
//
//                pinkGhost.vertices[1].pos.x = deviceToNormalized(packManX + tileMapWidth,WIDTH);
//                pinkGhost.vertices[1].pos.y = -deviceToNormalized(packManY,HEIGHT);// top right
//
//                pinkGhost.vertices[2].pos.x = deviceToNormalized(packManX + tileMapWidth, WIDTH);
//                pinkGhost.vertices[2].pos.y = -deviceToNormalized(packManY +tileMapHeight, HEIGHT); // bottom right
//
//                pinkGhost.vertices[3].pos.x = deviceToNormalized(packManX,WIDTH);
//                pinkGhost.vertices[3].pos.y = -deviceToNormalized(packManY+tileMapHeight, HEIGHT); // bottom left
//                texPosVert(pinkGhost.vertices, objectsTex[map[i][j]-1]);
//            }
//
//            if(map[i][j] == 17){
//                blueGhost.vertices.resize(4);
//                float packManX = j*tileMapWidth;
//                float packManY = i*tileMapHeight;
//                blueGhost.vertices[0].pos = {deviceToNormalized(packManX,WIDTH),-deviceToNormalized(packManY,HEIGHT),depth};
//
//                blueGhost.vertices[1].pos.x = deviceToNormalized(packManX + tileMapWidth,WIDTH);
//                blueGhost.vertices[1].pos.y = -deviceToNormalized(packManY,HEIGHT);// top right
//
//                blueGhost.vertices[2].pos.x = deviceToNormalized(packManX + tileMapWidth, WIDTH);
//                blueGhost.vertices[2].pos.y = -deviceToNormalized(packManY+tileMapHeight, HEIGHT); // bottom right
//
//                blueGhost.vertices[3].pos.x = deviceToNormalized(packManX,WIDTH);
//                blueGhost.vertices[3].pos.y = -deviceToNormalized(packManY+tileMapHeight, HEIGHT); // bottom left
//                texPosVert(blueGhost.vertices, objectsTex[map[i][j]-1]);
//            }
//
//            if(map[i][j] == 9){
//                float packManX = j*tileMapWidth;
//                float packManY = i*tileMapHeight;
//                temporary.vertices[0].pos = {deviceToNormalized(packManX,WIDTH),-deviceToNormalized(packManY,HEIGHT),depth};
//
//                temporary.vertices[1].pos.x = deviceToNormalized(packManX + tileMapWidth,WIDTH);
//                temporary.vertices[1].pos.y = -deviceToNormalized(packManY,HEIGHT);// top right
//
//                temporary.vertices[2].pos.x = deviceToNormalized(packManX + tileMapWidth, WIDTH);
//                temporary.vertices[2].pos.y = -deviceToNormalized(packManY+tileMapHeight, HEIGHT); // bottom right
//
//                temporary.vertices[3].pos.x = deviceToNormalized(packManX,WIDTH);
//                temporary.vertices[3].pos.y = -deviceToNormalized(packManY+tileMapHeight, HEIGHT); // bottom left
//                portals[tempIndex++] = temporary.vertices;
//            }
//            if(map[i][j] == -1){
//                float packManX = j*tileMapWidth;
//                float packManY = i*tileMapHeight;
//                temporary.vertices[0].pos = {deviceToNormalized(packManX,WIDTH),-deviceToNormalized(packManY,HEIGHT),depth};
//
//                temporary.vertices[1].pos.x = deviceToNormalized(packManX + tileMapWidth,WIDTH);
//                temporary.vertices[1].pos.y = -deviceToNormalized(packManY,HEIGHT);// top right
//
//                temporary.vertices[2].pos.x = deviceToNormalized(packManX + tileMapWidth, WIDTH);
//                temporary.vertices[2].pos.y = -deviceToNormalized(packManY+tileMapHeight, HEIGHT); // bottom right
//
//                temporary.vertices[3].pos.x = deviceToNormalized(packManX,WIDTH);
//                temporary.vertices[3].pos.y = -deviceToNormalized(packManY+tileMapHeight, HEIGHT); // bottom left
//                door = temporary.vertices;
//            }
//
//
//
//        }
//    }
//    returnObj.resize(walls.size() + food.size() +5);
//    int index = 0;
//    for(int i = 0; i<walls.size(); i++){
//        returnObj[index++] = walls[i];
//    }
//    indices[0] = walls.size();
//    for(int i = 0; i <food.size(); i++){
//        returnObj[index++] = food[i];
//    }
//    indices[1] = food.size();
//    returnObj[index++] = blinky;
//    indices[2] = 1;
//    returnObj[index++] = packman;
//    indices[3] = 1;
//    returnObj[index++] = orangeGhost;
//    indices[4] = 1;
//    returnObj[index++] = pinkGhost;
//    indices[5] = 1;
//    returnObj[index++] = blueGhost;
//    indices[6] = 1;
////    return returnObj;
//}
//
//void createGraphicsPipeline(initVulkanH *vulkan){
//    infoForViews info = vulkan->getInfo();
//    VkDevice &logicalDevice = vulkan->getDevice();
//    auto vertShader = readFile("shaders/vert.spv");
//    auto fragShader = readFile("shaders/frag.spv");
//
//    VkShaderModule vert;
//    VkShaderModuleCreateInfo vertInfo = {};
//    vertInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
//    vertInfo.codeSize = static_cast<uint32_t>(vertShader.size());
//    vertInfo.pCode = reinterpret_cast<uint32_t *>(vertShader.data());
//    if(vkCreateShaderModule(logicalDevice, &vertInfo, nullptr, &vert) != VK_SUCCESS){
//        throw std::runtime_error("failed to create shader module");
//    }
//
//    VkShaderModule frag;
//    VkShaderModuleCreateInfo fragInfo = {};
//    fragInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
//    fragInfo.codeSize = static_cast<uint32_t>(fragShader.size());
//    fragInfo.pCode = reinterpret_cast<uint32_t *>(fragShader.data());
//    if(vkCreateShaderModule(logicalDevice, &fragInfo, nullptr, &frag) != VK_SUCCESS){
//        throw std::runtime_error("failed to create shader module");
//    }
//
//
//    auto bindingDescription = Vertex2::getBindingDescription();
//    auto attributeDescriptions = Vertex2::getAttributeDescriptions();
//
//    VkPipelineVertexInputStateCreateInfo vertCreateInfo = {};
//    //createVertexInput(vertCreateInfo);
//    vertCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
//    vertCreateInfo.vertexBindingDescriptionCount = 1;
//    vertCreateInfo.vertexAttributeDescriptionCount = static_cast<uint32_t>(attributeDescriptions.size());
//    vertCreateInfo.pVertexBindingDescriptions = &bindingDescription;
//    vertCreateInfo.pVertexAttributeDescriptions = attributeDescriptions.data();
//
//    VkPipelineInputAssemblyStateCreateInfo assemblyCreateInfo = {};
//    //createAssemblyInput(assemblyCreateInfo);
//    assemblyCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
//    assemblyCreateInfo.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
//    assemblyCreateInfo.primitiveRestartEnable = VK_FALSE;
//
//    VkViewport viewPort = {};
//    VkRect2D scissor = {};
//    VkPipelineViewportStateCreateInfo viewPortCreateInfo = {};
//    //createViewPort(viewPortCreateInfo, info,viewPort,scissor);
//    viewPortCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
//    viewPort.x = 0;
//    viewPort.y = 0;
//    viewPort.width = info.extent.width;
//    viewPort.height = info.extent.height;
//    viewPort.minDepth = 0;
//    viewPort.maxDepth = 1;
//
//    scissor.offset = {0,0};
//    scissor.extent = info.extent;
//
//    viewPortCreateInfo.viewportCount = 1;
//    viewPortCreateInfo.pViewports = &viewPort;
//    viewPortCreateInfo.scissorCount = 1;
//    viewPortCreateInfo.pScissors = &scissor;
//
//
//    VkPipelineRasterizationStateCreateInfo rasterizationCreateInfo = {};
//    //createRasterization(rasterizationCreateInfo);
//    rasterizationCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
//    rasterizationCreateInfo.depthClampEnable = VK_FALSE;
//    rasterizationCreateInfo.rasterizerDiscardEnable = VK_FALSE;// only false if multi sampling on
//    rasterizationCreateInfo.polygonMode = VK_POLYGON_MODE_FILL;
//    rasterizationCreateInfo.cullMode = VK_CULL_MODE_BACK_BIT;
//    rasterizationCreateInfo.frontFace = VK_FRONT_FACE_CLOCKWISE;
//    rasterizationCreateInfo.depthBiasEnable = VK_FALSE;
//    rasterizationCreateInfo.lineWidth = 1.0f;
//
//    VkPipelineColorBlendStateCreateInfo colorBlendCreateInfo = {};
//    //createColorBlend(colorBlendCreateInfo);
//    colorBlendCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
//    colorBlendCreateInfo.logicOpEnable = VK_FALSE;
//    colorBlendCreateInfo.attachmentCount = 1;
//    colorBlendCreateInfo.logicOp = VK_LOGIC_OP_COPY;
//
//    VkPipelineColorBlendAttachmentState colorBlendAttachment{};
//       colorBlendAttachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT |  VK_COLOR_COMPONENT_A_BIT;
//       colorBlendAttachment.blendEnable = VK_TRUE;
//       colorBlendAttachment.srcColorBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA;
//       colorBlendAttachment.dstColorBlendFactor =  VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
//       colorBlendAttachment.colorBlendOp = VK_BLEND_OP_ADD;
//       colorBlendAttachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
//       colorBlendAttachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
//       colorBlendAttachment.alphaBlendOp = VK_BLEND_OP_ADD;
//
//    colorBlendCreateInfo.pAttachments = &colorBlendAttachment;
//
//  //  createPipelineLayout(layout,logicalDevice);
//    VkPipelineLayoutCreateInfo layoutCreateInfo = {};
//    layoutCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
//    layoutCreateInfo.setLayoutCount = 1;
//    layoutCreateInfo.pSetLayouts = &vulkan->getSetLayout();
//    if(vkCreatePipelineLayout(logicalDevice, &layoutCreateInfo, nullptr, &vulkan->getLayout()) != VK_SUCCESS){
//        throw std::runtime_error("failed to create pipline layout");
//    }
//
//    VkPipelineShaderStageCreateInfo vertStage = {};
//    vertStage.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
//    vertStage.stage = VK_SHADER_STAGE_VERTEX_BIT;
//    vertStage.module = vert;
//    vertStage.pName = "main";
//
//    VkPipelineShaderStageCreateInfo fragStage = {};
//    fragStage.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
//    fragStage.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
//    fragStage.module = frag;
//    fragStage.pName = "main";
//
//    VkPipelineShaderStageCreateInfo stages[2] = {vertStage,fragStage};
//
//            VkPipelineMultisampleStateCreateInfo multisampling{};
//            multisampling.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
//            multisampling.sampleShadingEnable = VK_FALSE;
//            multisampling.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
//
//    VkGraphicsPipelineCreateInfo createInfo = {};
//    createInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
//    createInfo.stageCount = 2;
//    createInfo.pStages = stages;
//    createInfo.pVertexInputState = &vertCreateInfo;
//    createInfo.pInputAssemblyState = &assemblyCreateInfo;
//    createInfo.pViewportState = &viewPortCreateInfo;
//    createInfo.pRasterizationState = &rasterizationCreateInfo;
//    createInfo.pColorBlendState = &colorBlendCreateInfo;
//    createInfo.pMultisampleState = &multisampling;
//    createInfo.layout = vulkan->getLayout();
//    createInfo.renderPass = vulkan->getRenderPass();
//    createInfo.subpass = 0;//.
//    createInfo.basePipelineHandle = VK_NULL_HANDLE;//,
//
//    if(vkCreateGraphicsPipelines(logicalDevice, nullptr, 1, &createInfo, nullptr, &vulkan->getPipeline()) != VK_SUCCESS){
//        throw std::runtime_error("failed to create graphics pipeline");
//    }
//    vkDestroyShaderModule(logicalDevice, vert, nullptr);
//    vkDestroyShaderModule(logicalDevice, frag, nullptr);
//}
//
//void createDescriptorSetLayout(initVulkanH *vulkan){
//    VkDescriptorSetLayoutBinding setLayout = {};
//    setLayout.binding = 0;
//    setLayout.descriptorCount = 1;
//    setLayout.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
//    setLayout.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;
//
//    VkDescriptorSetLayoutBinding samplerLayoutBinding{};
//    samplerLayoutBinding.binding = 1;
//    samplerLayoutBinding.descriptorCount = 1;
//    samplerLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
//    samplerLayoutBinding.pImmutableSamplers = nullptr;
//    samplerLayoutBinding.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;
//
//    std::vector<VkDescriptorSetLayoutBinding> bindings = {setLayout,samplerLayoutBinding};
//
//    VkDescriptorSetLayoutCreateInfo createInfo = {};
//    createInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
//    createInfo.bindingCount = static_cast<uint32_t>(bindings.size());
//    createInfo.pBindings = bindings.data();
//
//
//    if(vkCreateDescriptorSetLayout(vulkan->getDevice(), &createInfo, nullptr, &vulkan->getSetLayout()) != VK_SUCCESS){
//        throw std::runtime_error("failed to create descriptor set layout");
//    }
//}
//
//void updateUniformBufferT(VkDevice &logicalDevice, std::vector<VmaAllocation> &uniforBufferMem,infoForViews &info,uint32_t &currentImage,bool num,VmaAllocator &allocator){
//    glm::mat4 matrix(1);
//
//    UniformBufferObject ubo{};
//    ubo.model = glm::transpose(matrix);
//    ubo.view = viewMatrixx;
//    ubo.proj = perspectiveMatrixx;
//    if(num == 1){
//        ubo.proj[1][1] *= -1;
//    }
//        void* data;
////        vkMapMemory(logicalDevice, uniforBufferMem[currentImage], 0, sizeof(ubo), 0, &data);
//    vmaMapMemory(allocator, uniforBufferMem[currentImage], &data);
//            memcpy(data, &ubo, sizeof(ubo));
////        vkUnmapMemory(logicalDevice, uniforBufferMem[currentImage]);
//    vmaUnmapMemory(allocator, uniforBufferMem[currentImage]);
//}
//void updateUniformBuffer(VkDevice &logicalDevice, std::vector<VmaAllocation> &uniforBufferMem,infoForViews &info,uint32_t &currentImage,float up,double &deltaTime,float &left,ObjT &packman,bool change,VmaAllocator &allocator,Rect &rectangle){
//    static auto startTime = std::chrono::high_resolution_clock::now();
//
//    auto currentTime = std::chrono::high_resolution_clock::now();
//    float time = std::chrono::duration<float, std::chrono::seconds::period>(currentTime - startTime).count();
//
//   // cout<<up<<endl;
//
//    glm::mat4 model(1,0,0,left,
//                    0,1,0,up,
//                    0,0,1,0,
//                    0,0,0,1);
//
//    UniformBufferObject ubo{};
//    ubo.model = glm::transpose(model);
//    ubo.view = viewMatrixx;
//    ubo.proj = perspectiveMatrixx;
//    glm::vec4 vector4;
//    vector4 = ubo.model * glm::vec4(packman.vertices[0].pos,1);
//    rectangle.pos.x = vector4.x;
//    rectangle.pos.y = vector4.y;
//    if(change){
//        ubo.proj[1][1] *= -1;
//    }
//
//    void* data;
////    vkMapMemory(logicalDevice, uniforBufferMem[currentImage], 0, sizeof(ubo), 0, &data);
//    vmaMapMemory(allocator, uniforBufferMem[currentImage], &data);
//        memcpy(data, &ubo, sizeof(ubo));
////    vkUnmapMemory(logicalDevice, uniforBufferMem[currentImage]);
//    vmaUnmapMemory(allocator, uniforBufferMem[currentImage]);
//
//
////    if(change){
//        VkDeviceSize bufferSize = sizeof(packman.vertices[0]) * packman.vertices.size();
//        void *data2;
//        //vkMapMemory(logicalDevice, packman.vertexBufferMem, 0, bufferSize, 0, &data2);
//        vmaMapMemory(allocator, packman.vertexBufferMem, &data2);// forgot to change data to data2
////            memcpy(data2, packman.vertices.data(), (size_t) bufferSize);//
//    memcpy(data2, packman.vertices.data(), (size_t) bufferSize);
//        //vkUnmapMemory(logicalDevice, packman.vertexBufferMem);
//        vmaUnmapMemory(allocator, packman.vertexBufferMem);
////    }
//
//}
//void updateUniformBufferTA(VkDevice &logicalDevice, std::vector<VmaAllocation> &uniforBufferMem,infoForViews &info,uint32_t &currentImage,bool num,VmaAllocator &allocator,ObjT &object){
//    glm::mat4 matrix(1);
//
//    UniformBufferObject ubo{};
//    ubo.model = glm::transpose(matrix);
//    ubo.view = viewMatrixx;
//    ubo.proj = perspectiveMatrixx;
//    if(num == 1){
//        ubo.proj[1][1] *= -1;
//    }
//        void* data;
////        vkMapMemory(logicalDevice, uniforBufferMem[currentImage], 0, sizeof(ubo), 0, &data);
//    vmaMapMemory(allocator, uniforBufferMem[currentImage], &data);
//            memcpy(data, &ubo, sizeof(ubo));
////        vkUnmapMemory(logicalDevice, uniforBufferMem[currentImage]);
//    vmaUnmapMemory(allocator, uniforBufferMem[currentImage]);
//
//    VkDeviceSize bufferSize = sizeof(object.vertices[0]) * object.vertices.size();
//    void *data2;
//    vmaMapMemory(allocator, object.vertexBufferMem, &data2);// forgot to change data to data2
//    memcpy(data2, object.vertices.data(), (size_t) bufferSize);
//    vmaUnmapMemory(allocator, object.vertexBufferMem);
//}
//void animationP(bool notRight,bool right, bool down,bool notDown,textureObj &packmanClosed,textureObj &packmanOpen,ObjT &packman,double &prev){
//    float delay = 0.1;
//    static bool change = false;
//    float currentTime = glfwGetTime();
//        if(currentTime - prev >= delay && !change){
//            if(notRight){
//                for(int i = 0; i<4; i++){
//                    packman.vertices[i].texCoord = packmanClosed.cords[i];
//                }
//            }
//            if(right){
//                packman.vertices[0].texCoord = packmanClosed.cords[1];
//                packman.vertices[1].texCoord = packmanClosed.cords[0];
//                packman.vertices[2].texCoord = packmanClosed.cords[3];
//                packman.vertices[3].texCoord = packmanClosed.cords[2];
//            }
//            if(down){
//                packman.vertices[0].texCoord = packmanClosed.cords[1];
//                packman.vertices[1].texCoord = packmanClosed.cords[2];
//                packman.vertices[2].texCoord = packmanClosed.cords[3];
//                packman.vertices[3].texCoord = packmanClosed.cords[0];
//            }
//            if(notDown){
//                packman.vertices[0].texCoord = packmanClosed.cords[0];
//                packman.vertices[1].texCoord = packmanClosed.cords[3];
//                packman.vertices[2].texCoord = packmanClosed.cords[2];
//                packman.vertices[3].texCoord = packmanClosed.cords[1];
//            }
//           change = true;
//        }else if(currentTime - prev >= delay && change){
//            if(right){
//
//                packman.vertices[0].texCoord = packmanOpen.cords[1];
//                packman.vertices[1].texCoord = packmanOpen.cords[0];
//                packman.vertices[2].texCoord = packmanOpen.cords[3];
//                packman.vertices[3].texCoord = packmanOpen.cords[2];
//
//            }
//            if(notRight){
//                for(int i = 0; i<4; i++){
//                    packman.vertices[i].texCoord = packmanOpen.cords[i];
//                }
//            }
//            if(down){
//                packman.vertices[0].texCoord = packmanOpen.cords[1];
//                packman.vertices[1].texCoord = packmanOpen.cords[2];
//                packman.vertices[2].texCoord = packmanOpen.cords[3];
//                packman.vertices[3].texCoord = packmanOpen.cords[0];
//            }
//            if(notDown){
//                packman.vertices[0].texCoord = packmanOpen.cords[0];
//                packman.vertices[1].texCoord = packmanOpen.cords[3];
//                packman.vertices[2].texCoord = packmanOpen.cords[2];
//                packman.vertices[3].texCoord = packmanOpen.cords[1];
//            }
//            change = false;
//        }
//    if(currentTime - prev >= delay){
//        prev = currentTime;
//    }
//}
////void colResolve(Rect &wallRect, Rect &player,bool &notRight, bool &right, bool &down, bool &notDown,float &up, float &left,float offsetX, float offsetY){//bug when phasing through wall if no direction(don't know why it was caused)
////        //left and right should be changed they should affect movement but shouldn't be set when collision happens
////        //later today i will change that
////        if(notRight && (pointVsRect({player.pos.x-0.01,player.pos.y-0.01}, wallRect) || pointVsRect({player.pos.x-0.01,player.pos.y-player.size.y+0.01},wallRect))){//so far without phasing bug, wheter or not functions are with direction eg(notRight or right) logic doesn't change, but it direction will matter when top and bot will be introduced
////            left = wallRect.pos.x+offsetX + wallRect.size.x;
//////            notRight = false;
////            return;
////        }
////        if(right && (pointVsRect({player.pos.x+player.size.x+0.01,player.pos.y-0.01}, wallRect) || pointVsRect({player.pos.x+player.size.x+0.01,player.pos.y-player.size.y+0.01},wallRect))){//always update both xdir functions together
////            left = wallRect.pos.x+offsetX-player.size.x;
//////            right = false;
////            return;
////        }
////        if(down && (pointVsRect({player.pos.x+0.01,player.pos.y-player.size.y -0.01}, wallRect) || pointVsRect({player.pos.x+player.size.x-0.01,player.pos.y-player.size.y-0.01},wallRect))){//always update both xdir functions together
////            up = wallRect.pos.y+offsetY+player.size.y;
//////            down = false;
////            return;
////        }
////        if(notDown && (pointVsRect({player.pos.x+0.01,player.pos.y+0.01}, wallRect) || pointVsRect({player.pos.x+player.size.x-0.01,player.pos.y+0.01},wallRect))){//always update both xdir functions together
////            up = wallRect.pos.y+offsetY-wallRect.size.y;
//////            notDown = false;
////            return;
////        }
////}
//
//void packman::recreateObjects(std::vector<ObjT> &stuff,initVulkanH &vulkan){
//    for(int i = 0; i<stuff.size(); i++){
//        stuff[i].destroyObjForRecreate(vulkan.getDevice(), vulkan.getAllocator());
//        stuff[i].createObjForRecreate(vulkan.getDevice(), vulkan.getPhysicalDevice(), 3, vulkan.getInfo(), vulkan.getRenderPass(), vulkan.getAllocator(), vulkan.getSetLayout(), vulkan.getLayout(), vulkan.getPipeline());
//    }
//    beginCommandBuffers(vulkan.getCommandBuffers(), vulkan.getRenderPass(), vulkan.getFrameBuffers(), vulkan.getInfo(), vulkan.getPipeline(), vulkan.getLayout(), stuff);
////    int index = 0;
////    for(int i = 0; i<walls.size(); i++){
////        walls[i] = stuff[index++];
////    }
////    for(int i = 0; i<food.size(); i++){
////            food[i] = stuff[index++];
////    }
//}
////void animation(textureObj &frame1,ObjT &object){
////    for(int i = 0; i<object.vertices.size(); i++){
////        object.vertices[i].texCoord = frame1.cords[i];
////    }
////}
//void ghostUpdateUniformBuffer(std::vector<VmaAllocation> &uniforBufferMem,uint32_t &currentImage,float &up,float &left,ObjT &ghost,bool change,VmaAllocator &allocator,Rect &rectangle){
//    static auto startTime = std::chrono::high_resolution_clock::now();
//
//    auto currentTime = std::chrono::high_resolution_clock::now();
//    float time = std::chrono::duration<float, std::chrono::seconds::period>(currentTime - startTime).count();
//
//    glm::mat4 model(1,0,0,left,
//                    0,1,0,up,
//                    0,0,1,0,
//                    0,0,0,1);
//
//    UniformBufferObject ubo{};
//    ubo.model = glm::transpose(model);
//    ubo.view = viewMatrixx;
//    ubo.proj = perspectiveMatrixx;
//    glm::vec4 vector4;
//    vector4 = ubo.model * glm::vec4(ghost.vertices[0].pos,1);
//    rectangle.pos.x = vector4.x;
//    rectangle.pos.y = vector4.y;
//    if(change){
//        ubo.proj[1][1] *= -1;
//    }
//
//    void* data;
//    vmaMapMemory(allocator, uniforBufferMem[currentImage], &data);
//    memcpy(data, &ubo, sizeof(ubo));
//    vmaUnmapMemory(allocator, uniforBufferMem[currentImage]);
//
//    VkDeviceSize bufferSize = sizeof(ghost.vertices[0]) * ghost.vertices.size();
//    void *data2;
//    vmaMapMemory(allocator, ghost.vertexBufferMem, &data2);// forgot to change data to data2
//    memcpy(data2, ghost.vertices.data(), (size_t) bufferSize);
//    vmaUnmapMemory(allocator, ghost.vertexBufferMem);
//}
//void portalCol(Rect &player, Rect *portals,float &xMov,float xOffset){
//    if(rectCol(player, portals[0])){
//        xMov = portals[1].pos.x - portals[1].size.x + xOffset;
//        std::cout<<"portal left"<<std::endl;
//    }else if(rectCol(player, portals[1])){
//        xMov = portals[0].pos.x + portals[0].size.x + xOffset;
//        std::cout<<"portal right"<<std::endl;
//    }
//}
////ghostAi(BlinkyB, ghostDelay, bDown, bUp, bLeft, bRight, bMoveX, bMoveY, 60, bOffsetX, bOffsetY);
//void Ghost::ghostAi(int &delayInSec,int &oldBox,int frameLimit){
//    for(int i =0; i<turnBoxes.size(); i++){
//        if((rectCol(ghostB, turnBoxes[i]) && oldBox != i) || (delayInSec == frameLimit && oldBox == i)){
//        label:{
//            switch (rand() % 4) {
//                case 0:
//                    if(cantUP){
//                        goto label;
//                    }
//                    pUp = true;
//                    pDown = false;
//                    pLeft = false;
//                    pRight = false;
//                    break;
//                case 1:
//                    if(cantDown){
//                        goto label;
//                    }
//                    pDown = true;
//                    pUp = false;
//                    pLeft = false;
//                    pRight = false;
//                    break;
//                case 2:
//                    if(cantLeft){
//                        goto label;
//                    }
//                    pLeft = true;
//                    pDown = false;
//                    pUp = false;
//                    pRight = false;
//                    break;
//                case 3:
//                    if(cantRight){
//                        goto label;
//                    }
//                    pRight = true;
//                    pDown = false;
//                    pLeft = false;
//                    pUp = false;
//                    break;
//            }
//        }
//            if(oldBox != i){
////                if(pointVsRect({ghost.pos.x + ghost.size.x,ghost.pos.y}, turnBoxes[i]) && bRight){
////                    xMove = turnBoxes[i].pos.x+gOffsetX;
////                }
////                if(pointVsRect(ghost.pos, turnBoxes[i]) && bLeft){
////                    xMove = turnBoxes[i].pos.x+gOffsetX;
////                }
////                if(pointVsRect({ghost.pos.x,ghost.pos.y - ghost.size.y}, turnBoxes[i]) && bDown){
////                    yMove = turnBoxes[i].pos.y+gOffsetY;
////                }
////                if(pointVsRect(ghost.pos, turnBoxes[i]) && bUp){
////                    yMove = turnBoxes[i].pos.y+gOffsetY;
////                }
////                if(pointVsRect({ghostB.pos.x + ghostB.size.x/2, ghostB.pos.y - ghostB.size.y/2}, turnBoxes[i])){
//                    pMoveX = turnBoxes[i].pos.x+pOffsetX;
//                    pMoveY = turnBoxes[i].pos.y+pOffsetY;
////                }
//            }
//
//            oldBox = i;
//            delayInSec = 0;
//        }
//    }
//}
//
//void Ghost::initialize(ObjT &ghostI){
//    ghost = ghostI;
//    ghostB = ghostI.vertices;
//    pUp = false;
//    pDown = false;
//    pLeft = false;
//    pRight = false;
//    pMoveX = 0;
//    pMoveY = 0;
//    pOffsetX = -ghostB.pos.x;
//    pOffsetY = -ghostB.pos.y;
//    pOut = false;
//}
//void Ghost::update(float speedY, float speedX, double deltaTime){
//    if(pOut == false){
//        ghostGetOut(deltaTime);
//    }
//    if(pDown) {
//        pMoveY -=speedY * deltaTime;
//    }
//    if(pUp){
//        pMoveY +=speedY * deltaTime;
//    }
//    if(pLeft){
//        pMoveX -=speedX * deltaTime;
//    }
//    if(pRight){
//        pMoveX +=speedX * deltaTime;
//    }
//}
//Rect &Ghost::getGhostB(){
//    return ghostB;
//}
//float Ghost::getOffsetX(){
//    return pOffsetX;
//}
//float Ghost::getOffsetY(){
//    return pOffsetY;
//}
//void Ghost::ghostGetOut(double deltaTime){
//    if(!ghostInIntersection){
//        if(posOfLeave.pos.x < ghostB.pos.x){
//           pMoveX -=0.01 * deltaTime;
//        }else{
//           pMoveX +=0.01 * deltaTime;
//        }
//        if(rectCol(ghostB, posOfLeave)){
//            ghostInIntersection = true;
//            pMoveX = posOfLeave.pos.x + pOffsetX;
//            std::cout<<"ghost in intersection"<<std::endl;
//        }
//    }else{
//        pMoveY += 0.1 * deltaTime;
//        if(rectCol(ghostB, posOffinish)){
//            pOut = true;
//            pMoveY = posOffinish.pos.y + pOffsetY;
//            std::cout<<"ghost left"<<std::endl;
//        }
//    }
//}
//
//void Ghost::setGhostOutTrue(){
//    pOut = true;
//}
//
//void Ghost::setCantFalse(){
//    cantUP = false;
//    cantDown = false;
//    cantLeft = false;
//    cantRight = false;
//}
//
//void Ghost::decideCant(){
//    if(pLeft || pRight){
//        cantLeft = true;
//        cantRight = true;
//        cantUP = false;
//        cantDown = false;
//    }
//    if(pUp || pDown){
//        cantLeft = false;
//        cantRight = false;
//        cantUP = true;
//        cantDown = true;
//    }
//}
//
//void wallColGhosts(std::vector<ObjT> &walls, Ghost &blinky, Ghost &pinkGhost, Ghost &blueGhost, Ghost &orangeGhost){
//    for(int i = 0; i<walls.size(); i++){
//        Rect rectangle(walls[i].vertices);
//        if(rectCol(blinky.getGhostB(), rectangle)){
//            colResolve(rectangle, blinky.getGhostB(), blinky.pLeft, blinky.pRight, blinky.pDown, blinky.pUp, blinky.pMoveY, blinky.pMoveX, blinky.getOffsetX(), blinky.getOffsetY());
//            blinky.setCantFalse();
//            blinky.decideCant();
//        }
//        if(rectCol(pinkGhost.getGhostB(), rectangle)){
//            colResolve(rectangle, pinkGhost.getGhostB(), pinkGhost.pLeft, pinkGhost.pRight, pinkGhost.pDown, pinkGhost.pUp, pinkGhost.pMoveY, pinkGhost.pMoveX, pinkGhost.getOffsetX(), pinkGhost.getOffsetY());
//            pinkGhost.setCantFalse();
//            pinkGhost.decideCant();
//        }
//        if(rectCol(blueGhost.getGhostB(), rectangle)){
//            colResolve(rectangle, blueGhost.getGhostB(), blueGhost.pLeft, blueGhost.pRight, blueGhost.pDown, blueGhost.pUp, blueGhost.pMoveY, blueGhost.pMoveX, blueGhost.getOffsetX(), blueGhost.getOffsetY());
//            blueGhost.setCantFalse();
//            blueGhost.decideCant();
//        }
////            if(pointVsRect(orangeGhost.getGhostB().pos, rectangle)){
////                std::cout<<"col"<<std::endl;// point vs rect works
////            }
//        if(rectCol(orangeGhost.getGhostB(), rectangle)){
//            colResolve(rectangle, orangeGhost.getGhostB(), orangeGhost.pLeft, orangeGhost.pRight, orangeGhost.pDown, orangeGhost.pUp, orangeGhost.pMoveY, orangeGhost.pMoveX, orangeGhost.getOffsetX(), orangeGhost.getOffsetY());
//            orangeGhost.setCantFalse();
//            orangeGhost.decideCant();
//        }
//    }
//}
