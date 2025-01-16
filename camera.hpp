//
//  camera.hpp
//  FIrst_Project
//
//  Created by Данил on 16.10.2021.
//  Copyright © 2021 Daniil. All rights reserved.
//

#ifndef camera_hpp
#define camera_hpp
#include "helperFunctions.hpp"
#include <iostream>

enum cameraMode{
    CAMERA_SIDE_SCROLL = 1,
    CAMERA_STATIC = 2,
    CAMERA_FOLLOW = 3
};

class cameraH{
public:
    cameraH(cameraMode m);
    cameraH();
    void initializeCamera(glm::vec3 eye, glm::vec3 center, glm::vec3 up);
    void updateCamerPos(glm::vec3 newPos);
    glm::mat4 getCamera();
    
private:
    cameraMode mode;
    
    glm::vec3 cameraPos;
    glm::vec3 cameraFront;
    glm::vec3 cameraUp;
    
    glm::mat4 camera;
    
    bool oneTimeCheck = false;
    
    void initialize();
    void adjustCamera();
};


#endif /* camera_hpp */
