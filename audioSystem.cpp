//
//  audioSystem.cpp
//  FIrst_Project
//
//  Created by Данил on 19.9.2021.
//  Copyright © 2021 Daniil. All rights reserved.
//

#include "audioSystem.hpp"
audioSystem::audioSystem(){
    FMOD_RESULT res;
    
    res = FMOD::System_Create(&sys);
    if(res != FMOD_OK){
        throw std::runtime_error(FMOD_ErrorString(res));
    }
    
    res = sys->init(512, FMOD_INIT_NORMAL, 0);
    
    if(res != FMOD_OK){
        throw std::runtime_error(FMOD_ErrorString(res));
    }
}
audioSystem::~audioSystem(){
    channel->stop();
    FMOD_RESULT res;
    for(int i =0; i<sound.size(); i++){
        res = sound[i]->release();
        std::cout<<FMOD_ErrorString(res)<<std::endl;
    }
    res = sys->release();
    std::cout<<FMOD_ErrorString(res)<<std::endl;
}

void audioSystem::createSoundS(const char *path){
    FMOD_RESULT res;
    
    sound.resize(sound.size() +1);
    res = sys->createSound(path, FMOD_CREATESTREAM, nullptr, &sound[sound.size()-1]);
    
    if(res != FMOD_OK){
        throw std::runtime_error(FMOD_ErrorString(res));
    }
}
void audioSystem::playSound(int index){
    FMOD_RESULT res;
    if(index >= 0 && index < sound.size()){
        res = sys->playSound(sound[index], nullptr, false, &channel);
    }else{
        throw std::runtime_error("not valid index");
    }
//    std::cout<<FMOD_ErrorString(res)<<std::endl;
}
bool audioSystem::isPlaying(){
    bool temp;
    channel->isPlaying(&temp);
    return temp;
}
void audioSystem::createReverb(){
    FMOD_RESULT res;
    res = sys->createReverb3D(&reverb);
    if(res != FMOD_OK){
        throw std::runtime_error(FMOD_ErrorString(res));
    }
    FMOD_REVERB_PROPERTIES prop2 = FMOD_PRESET_CITY;
    reverb->setProperties(&prop2);
}
void audioSystem::setReverbAttributes(){
    FMOD_VECTOR pos = { 0.0f, 0.0f, 0.0f };
    float mindist = 0.0f;
    float maxdist = 2.0f;
    reverb->set3DAttributes(&pos, mindist, maxdist);
    FMOD_VECTOR  listenerpos  = { 10.0f, 10.0f, 10.0f };
    sys->set3DListenerAttributes(0, &listenerpos, 0, 0, 0);
}
