//
//  audioSystem.hpp
//  FIrst_Project
//
//  Created by Данил on 19.9.2021.
//  Copyright © 2021 Daniil. All rights reserved.
//

#ifndef audioSystem_hpp
#define audioSystem_hpp

#include <stdio.h>
#include <fmod.hpp>
#include <fmod_errors.h>
#include "loadFiles.hpp"
//#include <FMOD/api/studio/inc/fmod_studio.hpp>
class audioSystem{
public:
    audioSystem();
    ~audioSystem();
    void createSoundS(const char *path);
    void playSound(int index = 0);
    bool isPlaying();
    void createReverb();
    void setReverbAttributes();
private:
    FMOD::System *sys = nullptr;
    std::vector<FMOD::Sound *> sound;
//    int soundIndex = 0;
    FMOD::Channel *channel;
    FMOD::Reverb3D *reverb;
    
};

#endif /* audioSystem_hpp */
