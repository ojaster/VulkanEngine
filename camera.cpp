//
//  camera.cpp
//  FIrst_Project
//
//  Created by Данил on 16.10.2021.
//  Copyright © 2021 Daniil. All rights reserved.
//

#include "camera.hpp"

cameraH::cameraH(cameraMode m):mode(m){
    initialize();
}
cameraH::cameraH(){
    initialize();
}
void cameraH::adjustCamera(){
    glm::vec3 eye;
    eye.x = cameraPos.x;
    eye.y = cameraPos.y;
    eye.z = 0.f;
    switch (mode) {
        case CAMERA_SIDE_SCROLL:
            camera = glm::lookAt(cameraPos, eye, cameraUp);
            break;
        case CAMERA_STATIC:
            camera = glm::lookAt(cameraPos, cameraFront, cameraUp);
            break;
        case CAMERA_FOLLOW:
            camera = glm::lookAt(cameraPos, eye, cameraUp);
            break;
        default:
            break;
    }
}
void cameraH::initialize(){
    cameraPos   = glm::vec3(0.0f, 0.0f,  3.0f);
    cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
    cameraUp    = glm::vec3(0.0f, 1.0f,  0.0f);
    adjustCamera();
}
void cameraH::initializeCamera(glm::vec3 eye, glm::vec3 center, glm::vec3 up){
    if(mode == CAMERA_STATIC && oneTimeCheck){
        std::cout<<"can't initialize static camera twice"<<std::endl;
    }
    cameraPos = eye;
    cameraFront = center;
    cameraUp = up;
    adjustCamera();
    oneTimeCheck = true;
}
void cameraH::updateCamerPos(glm::vec3 newPos){
    switch (mode) {
        case CAMERA_STATIC:
            return;
            break;
            
        case CAMERA_SIDE_SCROLL:
            cameraPos.x = newPos.x;
            cameraPos.z = newPos.z;
            break;
        default:
            cameraPos = newPos;
            break;
    }
    adjustCamera();
}

glm::mat4 cameraH::getCamera(){
    return camera;
}
